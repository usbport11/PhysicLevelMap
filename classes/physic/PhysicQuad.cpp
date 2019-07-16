#include "../../stdafx.h"
#include "PhysicQuad.h"

MPhysicQuad::MPhysicQuad() {
	Body = NULL;
	TextureQuad = stTextureQuad();
	NeedRemove = false;
	Shape = NULL;
	Address = NULL;
}

bool MPhysicQuad::Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts, b2BodyType BodyType, uint16 Category, uint16 Mask, bool Sensor) {
	if(vs.x <= 0 || vs.y <= 0) return false;
	if(ts.x <= 0 || ts.y <= 0) return false;
	
	TextureQuad = stTextureQuad(vp, vs, tp, ts);
	
	StartParameters.SetMainData(vp, vs, tp, ts, BodyType);
	StartParameters.AddFixtureData(glm::vec2(vs.x * 0.5f, vs.y * 0.5f), Category, Mask, Sensor);
	
	return true;
}

void MPhysicQuad::SetVelocity(b2Vec2 Velocity) {
	Body->SetLinearVelocity(Velocity);
}

void MPhysicQuad::Update() {
	OnUpdate();
	
	//int convertion need to prevent twitching (while not rotating)
	//but what to do with rotation?
	for(int i=0; i<4; i++) {
		//TextureQuad.v[i].x = (int)(Body->GetWorldPoint(Shape->GetVertex(i)).x / *pScale);
		//TextureQuad.v[i].y = (int)(Body->GetWorldPoint(Shape->GetVertex(i)).y / *pScale);
		TextureQuad.v[i].x = Body->GetWorldPoint(Shape->GetVertex(i)).x / *pScale;
		TextureQuad.v[i].y = Body->GetWorldPoint(Shape->GetVertex(i)).y / *pScale;
	}
}

void MPhysicQuad::SetBody(b2Body* inBody) {
	Body = inBody;
}

void MPhysicQuad::SetShape(b2PolygonShape* inShape) {
	Shape = inShape;
}

void MPhysicQuad::SetScale(float* inpScale) {
	pScale = inpScale;
}

bool MPhysicQuad::GetNeedRemove() {
	return NeedRemove;
}

stTextureQuad* MPhysicQuad::GetTextureQuad() {
	return &TextureQuad;
}

stPhysicQuadParameters MPhysicQuad::GetStartParameters() {
	return StartParameters;
}

glm::vec2 MPhysicQuad::GetCenter() {
	return glm::vec2(Body->GetPosition().x / *pScale, Body->GetPosition().y / *pScale);
}

b2Vec2 MPhysicQuad::GetPosition() {
	return Body->GetPosition();
}

void MPhysicQuad::SetRotationAngle(float Angle) {
	Body->SetTransform(Body->GetPosition(), Angle);
}

void MPhysicQuad::SetAddress(MPhysicQuad** inAddress) {
	Address = inAddress;
}

MPhysicQuad** MPhysicQuad::GetAddress() {
	return Address;
}

void MPhysicQuad::OnUpdate(){}
void MPhysicQuad::OnBeginCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture) {}
void MPhysicQuad::OnEndCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture) {}
void MPhysicQuad::OnBeginWallCollide() {}
void MPhysicQuad::OnEndWallCollide() {}

