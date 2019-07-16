#include "Timer4.h"

MTimer4::MTimer4() {
	PreviousTime = 0.0f;
	CurrentTime = 0.0f;
	TimeElapsed = 0.0f;
	TimeLimit = 0.0f;
	Enabled = false;
}

bool MTimer4::SetTimeLimit(double inTimeLimit) {
	if(inTimeLimit <= 0) {
		return false;
	}
	TimeLimit = inTimeLimit;
	
	return true;
}

bool MTimer4::Start() {
	if(TimeLimit <= 0) {
		return false;
	}
	PreviousTime = 0.0f;
	CurrentTime = 0.0f;
	TimeElapsed = 0.0f;
	Enabled = true;
	
	return true;
}

bool MTimer4::Start(double inTimeLimit) {
	TimeLimit = inTimeLimit;
	return Start();
}

void MTimer4::Stop() {
	Enabled = false;
}

double MTimer4::GetTimeEllapsed() {
	if(!Enabled) return 0;
	CurrentTime = glfwGetTime();
	if(PreviousTime <= 0) TimeElapsed = 0;
	else TimeElapsed += CurrentTime - PreviousTime;
	PreviousTime = CurrentTime;
	
	return TimeElapsed;
}

bool MTimer4::Time() {
	if(!Enabled) return false;
	if(GetTimeEllapsed() >= TimeLimit) {
		Enabled = false;
		return true;
	}
	
	return false;
}
