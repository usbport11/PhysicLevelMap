#ifndef enemyH
#define enemyH

#include "Living.h"

class MEnemy: public MLiving {
private:
	MLiving* pTarget;
	//additional fixtures
	float VisionScale;
	b2Fixture* VisionSensor;
	float BodyScale;
	b2Fixture* BodySensor;
	//stuck control
	int WallsCollided;
	b2Vec2 PrevPosition;
	bool StuckFixing;
	//velocities
	float FollowVelocity;
	float IdleVelocity;
	//distances
	float FollowDistance;
protected:
	void OnBeginCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture);
	void OnEndCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture);
	void OnBeginWallCollide();
	void OnEndWallCollide();
public:
	MEnemy();
	bool Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts);
	bool SetVisionScale(float inVisionScale);
	bool SetBodyScale(float inBodyScale);
	void Close();
	void Move();
	void OnUpdate();
};

#endif
