#ifndef exitH
#define exitH

#include "LevelObject.h"

#define LET_NONE 0
#define LET_PREVIOUS 1
#define LET_NEXT 2
#define LET_SECRET 3

class MExit: public MLevelObject {
private:
	unsigned char ExitType;
public:
	MExit();
	bool Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts, unsigned char inExitType);
	unsigned char GetExitType();
};

#endif
