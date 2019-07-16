#ifndef physicquadH
#define physicquadH

#include "TextureQuad.h"

#define OT_WALL 0x0001
#define OT_HERO 0x0002
#define OT_ENEMY 0x0004
#define OT_REGION 0x0008
#define OT_BULLET 0x0004
#define OT_OBJECT 0x0008
#define OT_ITEM 0x0010

#define CT_REGION_HERO OT_REGION | OT_HERO
#define CT_REGION_ENEMY OT_REGION | OT_ENEMY
#define CT_ENEMY_HERO OT_ENEMY | OT_HERO
#define CT_BULLET_HERO OT_BULLET | OT_HERO
#define CT_BULLET_ENEMY OT_BULLET | OT_ENEMY
#define CT_OBJECT_HERO OT_OBJECT | OT_HERO
#define CT_OBJECT_ENEMY OT_OBJECT | OT_ENEMY

struct stFixtureData {
	b2Vec2 HalfSize;
	uint16 Category;
	uint16 Mask;
	bool Sensor;
	stFixtureData(glm::vec2 inHalfSize, uint16 inCategory, uint16 inMask, bool inSensor) {
		HalfSize = b2Vec2(inHalfSize.x, inHalfSize.y);
		Category = inCategory;
		Mask = inMask;
		Sensor = inSensor;
	}
};

struct stPhysicQuadParameters {
	glm::vec2 vp;
	glm::vec2 vs;
	glm::vec2 tp;
	glm::vec2 ts;
	b2BodyType BodyType;
	vector<stFixtureData> FixturesData;
	void SetMainData(glm::vec2 invp, glm::vec2 invs, glm::vec2 intp, glm::vec2 ints, b2BodyType inBodyType) {
		vp = invp;
		vs = invs;
		tp = intp;
		ts = ints;
		BodyType = inBodyType;
	}
	void AddFixtureData(glm::vec2 inHalfSize, uint16 inCategory, uint16 inMask, bool inSensor) {
		FixturesData.push_back(stFixtureData(inHalfSize, inCategory, inMask, inSensor));
	}
};

class MPhysicQuad {
private:
	stTextureQuad TextureQuad;
	MPhysicQuad** Address;
protected:
	float* pScale;
	b2Body* Body;
	b2PolygonShape* Shape;
	bool NeedRemove;
	stPhysicQuadParameters StartParameters;
public:
	MPhysicQuad();
	bool Set(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts, b2BodyType BodyType, uint16 Category, uint16 Mask, bool Sensor);
	void SetVelocity(b2Vec2 Velocity);
	void Update();
	void SetBody(b2Body* inBody);
	void SetShape(b2PolygonShape* inShape);
	void SetScale(float* inpScale);
	bool GetNeedRemove();
	stTextureQuad* GetTextureQuad();
	stPhysicQuadParameters GetStartParameters();
	glm::vec2 GetCenter();
	b2Vec2 GetPosition();
	void SetRotationAngle(float Angle);
	
	void SetAddress(MPhysicQuad** inAddress);
	MPhysicQuad** GetAddress();
	
	virtual void OnUpdate();
	virtual void OnBeginCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture);
	virtual void OnEndCollide(b2Fixture* MainFixture, b2Fixture* ObjectFixture);
	virtual void OnBeginWallCollide();
	virtual void OnEndWallCollide();
};

#endif

