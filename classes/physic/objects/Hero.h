#ifndef heroH
#define heroH

#include "Living.h"
#include "Chest.h"
#include "Exit.h"

class MHero: public MLiving {
private:
	MChest* pChest;
	MExit* pExit;
protected:
	void OnBeginCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture, MPhysicQuad* pObject);
	void OnEndCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture, MPhysicQuad* pObject);
	void OnBeginWallCollide();
	void OnEndWallCollide();
public:
	MHero();
	bool Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts);
	void Update();
	void OpenChest();
	unsigned char GetExitType();
};

#endif
