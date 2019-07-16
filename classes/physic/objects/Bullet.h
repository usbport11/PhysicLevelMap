#ifndef bulletH
#define bulletH

#include "Living.h"

class MBullet: public MPhysicQuad {
private:
	int Damage;
	int PenetrateLimit;
	int PenetrateCount;
protected:
	void OnBeginCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture);
	void OnEndCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture);
	void OnBeginWallCollide();
	void OnEndWallCollide();
public:
	MBullet();
	bool Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts, uint16 Mask);
	void SetDamage(int Value);
	void SetPenetrating(int inPenetrateLimit);
	void OnUpdate();
};

#endif
