#include "../../../stdafx.h"
#include "Exit.h"

MExit::MExit():MLevelObject() {
	Type = LOT_EXIT;
	ExitType = LET_NONE;
}

bool MExit::Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts, unsigned char inExitType) {
	ExitType = inExitType;
	return MLevelObject::Set(vp, vs, tp ,ts, true);
}

unsigned char MExit::GetExitType() {
	return ExitType;
}
