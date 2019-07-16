#include "../../../stdafx.h"
#include "Enemy.h"

MEnemy::MEnemy():MLiving() {
	VisionSensor = NULL;
	BodySensor = NULL;
	pTarget = NULL;
	WallsCollided = 0;
	Health = 1;//if <= zero - removed
	VisionScale = 4;
	BodyScale = 1;
	FollowVelocity = 0.4;
	IdleVelocity = 0.2;
	StuckFixing = true;
	FollowDistance = 0.5;
}

bool MEnemy::Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts) {
	if(!MPhysicQuad::Set(vp, vs, tp, ts, b2_dynamicBody, OT_ENEMY, OT_WALL, false)) return false;
	
	StartParameters.AddFixtureData(glm::vec2(vs.x * 0.5f * VisionScale, vs.y * 0.5f * VisionScale), OT_REGION, OT_HERO, true);
	StartParameters.AddFixtureData(glm::vec2(vs.x * 0.5f * BodyScale, vs.y * 0.5f * BodyScale), OT_ENEMY, OT_HERO | OT_BULLET | OT_REGION, true);
	
	return true;
}

bool MEnemy::SetVisionScale(float inVisionScale) {
	if(inVisionScale <= 0) return false;
	VisionScale = inVisionScale;
	return true;
}

bool MEnemy::SetBodyScale(float inBodyScale) {
	if(inBodyScale <= 0) return false;
	BodyScale = inBodyScale;
	return true;
}

void MEnemy::OnBeginCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture) {	
	MPhysicQuad* pObject = (MPhysicQuad*)ObjectFixture->GetBody()->GetUserData();
	switch(MainFixture->GetFilterData().categoryBits | ObjectFixture->GetFilterData().categoryBits) {
		case CT_REGION_HERO:
			//set target
			cout<<"Find target"<<endl;
			pTarget = (MLiving*)pObject;
			break;
		case CT_ENEMY_HERO:
			//start damage timer
			break;
	}
}

void MEnemy::OnEndCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture) {
	switch(MainFixture->GetFilterData().categoryBits | ObjectFixture->GetFilterData().categoryBits) {
		case CT_REGION_HERO:
			//remove target
			cout<<"Lose target"<<endl;
			pTarget = NULL;
			break;
		case CT_ENEMY_HERO:
			//stop damage timer
			break;
	}
}

void MEnemy::OnBeginWallCollide() {
	if(!WallsCollided) {
		//inverse velocity
		SetVelocity(b2Vec2(-Body->GetLinearVelocity().x, -Body->GetLinearVelocity().y));
	}
	WallsCollided ++;
}

void MEnemy::OnEndWallCollide() {
	WallsCollided --;
}

void MEnemy::Move() {
	if(pTarget) {
		b2Vec2 Diff = pTarget->GetPosition() - Body->GetPosition();
		glm::vec2 Direction;
		if(glm::length(glm::vec2(Diff.x, Diff.y)) <= FollowDistance) {
			SetVelocity(b2Vec2(0, 0));
			return;
		}
		
		Direction = glm::normalize(glm::vec2(Diff.x, Diff.y));
		SetVelocity(b2Vec2(FollowVelocity * Direction.x, FollowVelocity * Direction.y));
		return;
	}
	if(StuckFixing) {
		//if prev and current position are the same - enemy is stuck
		if(PrevPosition == Body->GetPosition()) {
			SetVelocity(b2Vec2(IdleVelocity * ((rand() % 2) * 2 - 1), IdleVelocity * ((rand() % 2) * 2 - 1)));
			cout<<(int)this<<" stuck!"<<endl;
		}
		//chance to change direction without wall collision
		if(!WallsCollided && (rand() % 50 > 48)) SetVelocity(b2Vec2(IdleVelocity * ((rand() % 2) * 2 - 1), IdleVelocity * ((rand() % 2) * 2 - 1)));
		PrevPosition = Body->GetPosition();
	}
	return;
}

void MEnemy::OnUpdate() {
	//some future actions
	Move();
	MLiving::OnUpdate();
}
