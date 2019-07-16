#include "../../stdafx.h"
#include "PhysicWorld.h"

MPhysicWorld::MPhysicWorld() {
	World = NULL;
	Gravity = 0;
	TimeStep = 1/60;
	VelocityIterations = 8;
	PositionIterations = 3;
	Scale = 1;
	LevelBody = NULL;
}

bool MPhysicWorld::Initialize(float inGravity, float inTimeStep, int inVelocityIterations, int inPositionIterations, float inScale, unsigned int Id) {
	if(inVelocityIterations <= 0 || inPositionIterations <= 0 || inVelocityIterations > 10 || inPositionIterations > 10) {
		LogFile<<"PhysicWorld: Wrong iterations values"<<endl;
		return false;
	}
	if(inScale < 0 || inScale > 1) {
		LogFile<<"PhysicWorld: Wrong scale value"<<endl;
		return false;
	}
	if(inTimeStep > (float)1/30) {
		LogFile<<"PhysicWorld: Wrong time step"<<endl;
		return false;
	}
	
	Gravity = inGravity;
	TimeStep = inTimeStep;
	VelocityIterations = inVelocityIterations;
	PositionIterations = inPositionIterations;
	Scale = inScale;
	
	World = new b2World(b2Vec2(0, Gravity)); //IMPORTANT! do not use in constructor
	World->SetContactListener(&ContactListener);
	
	QuadBuffer = MTextureQuadBuffer(GL_STREAM_DRAW, Id);
	
	return true;
}

bool MPhysicWorld::CreateLevelBody() {
	if(!World) {
		LogFile<<"PhysicWorld: Empty world"<<endl;
		return false;
	}
	
	b2BodyDef LevelBodyDef;
	LevelBodyDef.position.Set(0.0f, 0.0f);
	LevelBodyDef.type = b2_staticBody;
	LevelBody = World->CreateBody(&LevelBodyDef);
	
	return true;
}

void MPhysicWorld::Step() {
	World->Step(TimeStep, VelocityIterations, PositionIterations);
}

bool MPhysicWorld::AddWall(b2Vec2 A, b2Vec2 B) {
	if(!LevelBody) {
		LogFile<<"PhysicWorld: Can not create wall. Empty level body"<<endl;
		return false;
	}
	
	A = b2Vec2(A.x * Scale, A.y * Scale);
	B = b2Vec2(B.x * Scale, B.y * Scale);
	
	b2EdgeShape EdgeShape;
	b2FixtureDef FixtureDef;
	
	EdgeShape.Set(A, B);
	FixtureDef.shape = &EdgeShape;
	LevelBody->CreateFixture(&FixtureDef);
	
	return true;
}

b2Body* MPhysicWorld::CreateBody(b2Vec2 Center, b2BodyType Type) {
	b2Body* Body = NULL;
	
	b2BodyDef BodyDef;
	b2PolygonShape Box;
	BodyDef.type = Type;
	BodyDef.fixedRotation = true;
	BodyDef.position.Set(Center.x * Scale, Center.y * Scale);
	Body = World->CreateBody(&BodyDef);
	
	return Body;
}

b2Fixture* MPhysicWorld::CreateFixture(b2Body* pBody, b2Vec2 HalfSize, bool Sensor, uint16 Category, uint16 Mask) {
	b2Fixture* Fixture = NULL;
	if(!pBody || HalfSize.x <= 0 || HalfSize.y <= 0) return Fixture;
	
	b2PolygonShape Box;
	Box.SetAsBox(HalfSize.x * Scale, HalfSize.y * Scale);
	
	b2FixtureDef fixtureDef;
	fixtureDef.filter.categoryBits = Category;
	fixtureDef.filter.maskBits = Mask;
	fixtureDef.isSensor = Sensor;
	fixtureDef.shape = &Box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	
	Fixture = pBody->CreateFixture(&fixtureDef);
	
	return Fixture;
}

bool MPhysicWorld::AddPhysicQuad(MPhysicQuad* pObject) {
	if(!pObject) return false;
	
	stPhysicQuadParameters SP = pObject->GetStartParameters();
	if(SP.FixturesData.empty()) return false;
	
	//fill physic part
	b2Vec2 Center = b2Vec2(SP.vp.x + SP.vs.x * 0.5f, SP.vp.y + SP.vs.y * 0.5f);
	b2Body* Body = CreateBody(Center, SP.BodyType);
	Body->SetUserData((void*)pObject); //IMPORTANT!
	pObject->SetBody(Body);
	b2Fixture* Fixture = CreateFixture(Body, SP.FixturesData[0].HalfSize, SP.FixturesData[0].Sensor, SP.FixturesData[0].Category, SP.FixturesData[0].Mask);
	pObject->SetShape((b2PolygonShape*)Fixture->GetShape());
	for(int i=1; i<SP.FixturesData.size(); i++) {
		CreateFixture(Body, SP.FixturesData[i].HalfSize, SP.FixturesData[i].Sensor, SP.FixturesData[i].Category, SP.FixturesData[i].Mask);
	}
	pObject->SetScale(&Scale);
	
	//fill graphic part
	if(!QuadBuffer.AddQuad(pObject->GetTextureQuad())) return false;
	
	return true;
}

void MPhysicWorld::UpdateObjects() {
	MPhysicQuad* pObject;
	MPhysicQuad** Address;
	for(b2Body* pBody = World->GetBodyList(); pBody; pBody = pBody->GetNext()) {
		if(pBody == LevelBody) continue;
		pObject = (MPhysicQuad*)pBody->GetUserData();
		if(!pObject) continue;
		if(pObject->GetNeedRemove()) {
			QuadBuffer.RemoveQuad(pObject->GetTextureQuad());
			Address = pObject->GetAddress();
			if(Address) {
				delete *Address;
				*Address = NULL;
			}
			World->DestroyBody(pBody);
		}
		else {
			pObject->Update();
		}
	}
	
	QuadBuffer.UpdateAll();
}

void MPhysicWorld::FillQuadBuffer() {
	QuadBuffer.Relocate();
}

void MPhysicWorld::DrawQuadBuffer() {
	QuadBuffer.Begin();
	QuadBuffer.DrawAll();
	QuadBuffer.End();
}

void MPhysicWorld::Clear() {
	QuadBuffer.Clear();
	if(World) {
		for(b2Body* pBody = World->GetBodyList(); pBody; pBody = pBody->GetNext()) {
			World->DestroyBody(pBody);
		}
	}
	LevelBody = NULL;
}

void MPhysicWorld::Close() {
	QuadBuffer.Close();
	if(World) {
		MPhysicQuad* pObject;
		MPhysicQuad** Address;
		for(b2Body* pBody = World->GetBodyList(); pBody; pBody = pBody->GetNext()) {
			pObject = (MPhysicQuad*)pBody->GetUserData();
			if(!pObject) continue;
			Address = pObject->GetAddress();
			if(Address) {
				delete *Address;
				*Address = NULL;
			}
			World->DestroyBody(pBody);
		}
		delete World;
	}
}
