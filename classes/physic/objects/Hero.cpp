#include "../../../stdafx.h"
#include "Hero.h"

MHero::MHero():MLiving() {
	pChest = NULL;
	pExit = NULL;
	Health = 1;
}

bool MHero::Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts) {
	return MPhysicQuad::Set(vp, vs, tp, ts, b2_dynamicBody, OT_HERO, OT_WALL | OT_REGION | OT_BULLET | OT_ENEMY | OT_ITEM | OT_OBJECT, false);
}

void MHero::OnBeginCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture, MPhysicQuad* pObject) {
	switch(MainFixture->GetFilterData().categoryBits | ObjectFixture->GetFilterData().categoryBits) {
		case CT_OBJECT_HERO:
			MLevelObject* LO = (MLevelObject*)pObject;
			switch(LO->GetType()) {
				case LOT_EXIT:
					pExit = (MExit*)pObject;
					break;
				case LOT_CHEST:
					pChest = (MChest*)pObject;
					break;
			}
			break;
	};
}

void MHero::OnEndCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture, MPhysicQuad* pObject) {
	switch(MainFixture->GetFilterData().categoryBits | ObjectFixture->GetFilterData().categoryBits) {
		case CT_OBJECT_HERO:
			MLevelObject* LO = (MLevelObject*)pObject;
			switch(LO->GetType()) {
				case LOT_EXIT:
					pExit = NULL;
					break;
				case LOT_CHEST:
					pChest = NULL;
					break;
			}
			break;
	};
}

void MHero::OnBeginWallCollide(){}
void MHero::OnEndWallCollide(){}

void MHero::Update() {
	MLiving::LevelUp();
	MPhysicQuad::Update();
}

void MHero::OpenChest() {
	if(pChest) {
		if(pChest->GetClosed()) {
			pChest->SetClosed(false);
			cout<<"Chest opened!"<<endl;
		}
		else cout<<"Chest alredy opened!"<<endl;
	}
}

unsigned char MHero::GetExitType() {
	if(!pExit) return LET_NONE;
	return pExit->GetExitType();
}
