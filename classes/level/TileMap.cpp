#include "TileMap.h"

MTileMap::MTileMap() {
	Map = NULL;
	Size.x = Size.y = 0;
}
MTileMap::MTileMap(int SizeX, int SizeY) {
	if(SizeX <= 0 || SizeY <= 0) return;
	Size.x = SizeX;
	Size.y = SizeY;
	Map = new char* [Size.x];
	for(int i=0; i<Size.x; i++) {
		Map[i] = new char[Size.y];
		memset(Map[i], 0, Size.y);
	}
}

MTileMap::MTileMap(NVector2 inSize) {
	if(inSize.x <= 0 || inSize.y <= 0) return;
	Size = inSize;
	Map = new char* [Size.x];
	for(int i=0; i<Size.x; i++) {
		Map[i] = new char[Size.y];
		memset(Map[i], 0, Size.y);
	}
}

void MTileMap::SetVector(int x, int y, char Value) {
	Map[x][y] = Value;
}

void MTileMap::SetVector(NVector2 Position, char Value) {
	Map[Position.x][Position.y] = Value;
}

void MTileMap::SetRectangle(int x, int y, int w, int h, char Value) {
	for(int i=x; i < x + w; i++) {
		for(int j=y; j < y + h; j++) {
			Map[i][j] = Value;
		}
	}
}

void MTileMap::SetRectangle(NRectangle2 Rectangle, char Value) {
	for(int i=Rectangle.Position.x; i < Rectangle.Position.x + Rectangle.Size.x; i++) {
		for(int j=Rectangle.Position.y; j < Rectangle.Position.y + Rectangle.Size.y; j++) {
			Map[i][j] = Value;
		}
	}
}

char MTileMap::GetValue(int x, int y) {
	return Map[x][y];
}

char MTileMap::GetValue(NVector2 Position) {
	return Map[Position.x][Position.y];
}

bool MTileMap::CreateWalls() {
	if(!Map) return false;
	if(Size.x <= 0 || Size.y <= 0) return false;
	
	for(int i=0; i<Size.x - 1; i++) {
		for(int j=0; j<Size.y - 1; j++) {
			if(Map[i][j] == TT_NONE) continue;
			//left wall
			if(i > 0) {
				if(Map[i][j] == TT_FLOOR && Map[i-1][j] == TT_NONE) Map[i-1][j] = TT_WALL_FULL;
			}
			//top wall
			if(Map[i][j] == TT_FLOOR && Map[i][j+1] == TT_NONE) Map[i][j+1] = TT_WALL_PART;
		}
	}
	
	for(int i=0; i<Size.x - 1; i++) {
		for(int j=0; j<Size.y - 1; j++) {
			if(Map[i][j] == TT_NONE) continue;
			//right walls
			if(Map[i][j] == TT_FLOOR && Map[i+1][j] == TT_NONE) Map[i+1][j] = TT_WALL_FULL;
			//bottom wall
			if(j > 0) {
				if(Map[i][j] == TT_FLOOR && Map[i][j-1] == TT_NONE) Map[i][j-1] = TT_WALL_PART;
			}
		}
	}
	
	//bottom angles
	for(int i=0; i<Size.x; i++) {
		for(int j=0; j<Size.y; j++) {
			if(Map[i][j] != TT_WALL_FULL) continue;
			if(j > 0 && Map[i][j-1] == TT_NONE) {
				Map[i][j-1] = TT_WALL_PART;
			}
		}
	}
	
	//top angels
	for(int i=0; i<Size.x-1; i++) {
		for(int j=0; j<Size.y-1; j++) {
			if(Map[i][j] != TT_NONE) continue;
			if(j > 0) {
				if(Map[i][j-1] == TT_WALL_FULL && Map[i+1][j] == TT_WALL_PART && Map[i+1][j-1] == TT_FLOOR) Map[i][j] = TT_WALL_FULL;
				if(Map[i][j-1] == TT_WALL_PART && Map[i+1][j] == TT_WALL_PART && Map[i+1][j-1] == TT_FLOOR) Map[i][j] = TT_WALL_FULL;
			}
			if(i > 0) {
				if(Map[i][j-1] == TT_WALL_FULL && Map[i-1][j] == TT_WALL_PART && Map[i-1][j-1] == TT_FLOOR) Map[i][j] = TT_WALL_FULL;
				if(Map[i][j-1] == TT_WALL_PART && Map[i-1][j] == TT_WALL_PART && Map[i-1][j-1] == TT_FLOOR) Map[i][j] = TT_WALL_FULL;
			}
		}
	}
	
	//fix top (TT_WALL_FULL - TT_WALL_PART)
	for(int i=0; i<Size.x-1; i++) {
		for(int j=0; j<Size.y-1; j++) {
			if(Map[i][j] != TT_WALL_FULL) continue;
			if(Map[i][j+1] == TT_WALL_PART) Map[i][j+1] = TT_WALL_FULL;
		}
	}
	
	return true;
}

bool MTileMap::CreateFloor() {
	//may be multi texturing or other
	return true;
}

void MTileMap::Clear() {
	for(int i=0; i<Size.x; i++) {
		memset(Map[i], 0, Size.y);
	}
}

void MTileMap::Close() {
	if(!Map) return;
	for(int i=0; i<Size.x; i++) {
		if(Map[i]) delete [] Map[i];
	}
	delete [] Map;
}
