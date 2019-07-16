#ifndef livingH
#define livingH

#include "../PhysicQuad.h"

#define LEVEL_EXP_VALUE 50

//living statuses (combine by mask)
#define LS_NONE 0
#define LS_ACTIVE 1
#define LS_GOD 2

class MLiving: public MPhysicQuad {
protected:
	int Level;
	int Health;
	int Expirience;
	int NextLevelExpirience;
	unsigned int Status; //LS_*
public:
	MLiving();
	void ReciveDamage(int Value);
	void ReciveHealing(int Value);
	void SetHealth(int Value);
	int GetHealth();
	void SetExpirience(int Value);
	int GetExpirience();
	void AddExpirience(int Value);
	void LevelUp();
	void OnUpdate();
};

#endif
