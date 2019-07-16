#ifndef tilemapH
#define tilemapH

#include <string.h>
#include "N2.h"

#define TT_NONE 0
#define TT_FLOOR 1
#define TT_WALL_FULL 2
#define TT_WALL_PART 3 

class MTileMap {
private:
	char** Map;
	NVector2 Size;
public:
	MTileMap();
	MTileMap(int SizeX, int SizeY);
	MTileMap(NVector2 inSize);
	void SetVector(int x, int y, char Value);
	void SetVector(NVector2 Position, char Value);
	void SetRectangle(int x, int y, int w, int h, char Value);
	void SetRectangle(NRectangle2 Rectangle, char Value);
	char GetValue(int x, int y);
	char GetValue(NVector2 Position);
	bool CreateWalls();
	bool CreateFloor();
	void Clear();
	void Close();
};

#endif
