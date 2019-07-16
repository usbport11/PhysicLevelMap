#ifndef physicworldH
#define physicworldH

#include "../buffers/TextureQuadBuffer.h"
#include "PhysicQuad.h"
#include "ContactListener.h"

class MPhysicWorld {
private:
	MTextureQuadBuffer QuadBuffer;
	
	float Scale;
	b2World* World;
	float Gravity;
	float TimeStep;
	int VelocityIterations;
	int PositionIterations;
	MContactListener ContactListener;
	b2Body* LevelBody;
	
	b2Body* CreateBody(b2Vec2 Center, b2BodyType Type);
	b2Fixture* CreateFixture(b2Body* pBody, b2Vec2 HalfSize, bool Sensor, uint16 Category, uint16 Mask);
public:
	MPhysicWorld();
	bool Initialize(float inGravity, float inTimeStep, int inVelocityIterations, int inPositionIterations, float inScale, unsigned int Id);
	bool CreateLevelBody();
	void Step();
	bool AddWall(b2Vec2 A, b2Vec2 B);
	bool AddPhysicQuad(MPhysicQuad* pObject);
	void UpdateObjects();
	void FillQuadBuffer();
	void DrawQuadBuffer();
	void Clear();
	void Close();
};

#endif
