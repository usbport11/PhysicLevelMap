#include "../../../stdafx.h"
#include "LevelObject.h"

MLevelObject::MLevelObject():MPhysicQuad() {
	Type = LOT_NONE;
}

bool MLevelObject::Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts, bool Sensor) {
	return MPhysicQuad::Set(vp, vs, tp, ts, b2_kinematicBody, OT_OBJECT, OT_HERO, Sensor);
}

void MLevelObject::OnBeginCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture) {}
void MLevelObject::OnEndCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture) {}
void MLevelObject::OnBeginWallCollide() {}
void MLevelObject::OnEndWallCollide() {}

unsigned char MLevelObject::GetType() {
	return Type;
}
