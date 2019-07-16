#ifndef chestH
#define chestH

#include "LevelObject.h"

class MChest: public MLevelObject {
private:
	bool Closed;
public:
	MChest();
	bool GetClosed();
	void SetClosed(bool Value);
	unsigned char GetType();
};

#endif
