#ifndef timer4
#define timer4

#include <GLFW/glfw3.h>

class MTimer4
{
private:
	double PreviousTime;
	double CurrentTime;
	double TimeElapsed;
	double TimeLimit;
	bool Enabled;
public:
	MTimer4();
	double GetTimeEllapsed();
	bool SetTimeLimit(double inTimeLimit);
	bool Start();
	bool Start(double inTimeLimit);
	void Stop();
	bool Time();
};

#endif
