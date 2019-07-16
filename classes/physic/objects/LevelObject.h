#ifndef levelobjectH
#define levelobjectH

#include "../PhysicQuad.h"

#define LOT_NONE 0
#define LOT_EXIT 1
#define LOT_CHEST 2
//door, switch, teport, bridge, etc...

class MLevelObject: public MPhysicQuad {
protected:
	void OnBeginCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture);
	void OnEndCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture);
	void OnBeginWallCollide();
	void OnEndWallCollide();
	unsigned char Type;
public:
	MLevelObject();
	bool Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts, bool Sensor);
	unsigned char GetType();
};

#endif
