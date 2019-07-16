#include "../../stdafx.h"
#include "ContactListener.h"
#include "PhysicQuad.h"

void MContactListener::BeginContact(b2Contact* pContact)
{
	b2Fixture* fixtureA = pContact->GetFixtureA();
	b2Fixture* fixtureB = pContact->GetFixtureB();
	MPhysicQuad* pObjectA = (MPhysicQuad*)fixtureA->GetBody()->GetUserData();
	MPhysicQuad* pObjectB = (MPhysicQuad*)fixtureB->GetBody()->GetUserData();
	if(fixtureA->GetFilterData().categoryBits == OT_WALL || fixtureB->GetFilterData().categoryBits == OT_WALL) {
		if(pObjectA) pObjectA->OnBeginWallCollide();
		if(pObjectB) pObjectB->OnBeginWallCollide();
	}
	if(pObjectA && pObjectB)
	{
		pObjectA->OnBeginCollide(fixtureA, fixtureB);
		pObjectB->OnBeginCollide(fixtureB, fixtureA);
	}
}

void MContactListener::EndContact(b2Contact* pContact)
{
	if(!pContact) return;
	b2Fixture* fixtureA = pContact->GetFixtureA();
	b2Fixture* fixtureB = pContact->GetFixtureB();
	MPhysicQuad* pObjectA = (MPhysicQuad*)fixtureA->GetBody()->GetUserData();
	MPhysicQuad* pObjectB = (MPhysicQuad*)fixtureB->GetBody()->GetUserData();
	if(fixtureA->GetFilterData().categoryBits == OT_WALL || fixtureB->GetFilterData().categoryBits == OT_WALL) {
		if(pObjectA) pObjectA->OnEndWallCollide();
		if(pObjectB) pObjectB->OnEndWallCollide();
	}
	if(pObjectA && pObjectB) 
	{
		pObjectA->OnEndCollide(fixtureA, fixtureB);
		pObjectB->OnEndCollide(fixtureB, fixtureA);
	}
}
