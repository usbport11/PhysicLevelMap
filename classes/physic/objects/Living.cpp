#include "../../../stdafx.h"
#include "Living.h"

MLiving::MLiving():MPhysicQuad() {
	Health = 0;
	Expirience = 0;
	Level = 0;
	NextLevelExpirience = glm::exp(Level) * LEVEL_EXP_VALUE;
	Status = LS_NONE;
}

void MLiving::ReciveDamage(int Value) {
	if(Status & LS_GOD == LS_GOD) return;
	cout<<(int)this<<" Recieve damage: "<<Value<<endl;
	Health -= Value;
}

void MLiving::ReciveHealing(int Value) {
	cout<<(int)this<<" Recieve healing: "<<Value<<endl;
	Health += Value;
}

void MLiving::SetHealth(int Value) {
	Health = Value;
}

int MLiving::GetHealth() {
	return Health;
}

void MLiving::SetExpirience(int Value) {
	Expirience = Value;
}

int MLiving::GetExpirience() {
	return Expirience;
}

void MLiving::AddExpirience(int Value) {
	Expirience += Value;
}

void MLiving::LevelUp() {
	if(Expirience > NextLevelExpirience) {
		Level ++;
		Expirience -= NextLevelExpirience;
		NextLevelExpirience = glm::exp(Level) * LEVEL_EXP_VALUE;
		cout<<"Level up! To next: "<<NextLevelExpirience<<endl;
	}
}

void MLiving::OnUpdate() {
	if(Health <= 0) {
		NeedRemove = true;
	}
}
