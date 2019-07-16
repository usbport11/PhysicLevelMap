#ifndef texturequadbufferH
#define texturequadbufferH

#include "../physic/TextureQuad.h"

struct stQuadLink {
	stTextureQuad* pTextureQuad;
	unsigned int Shift;
	stQuadLink() {
		pTextureQuad = NULL;
		Shift = 0;
	}
	stQuadLink(stTextureQuad* inpTextureQuad, unsigned int inShift) {
		pTextureQuad = inpTextureQuad;
		Shift = inShift;
	}
};

struct stFindQuadLink {
	stQuadLink QuadLink;
	stFindQuadLink(stTextureQuad* inTextureQuad) {
		QuadLink.pTextureQuad = inTextureQuad;
	}
	bool operator () (stQuadLink inQuadLink) {
		return QuadLink.pTextureQuad == inQuadLink.pTextureQuad;
	}
};

class MTextureQuadBuffer {
private:
	unsigned int BindNumber;
	unsigned int Id;
	GLuint VerticesId;
	GLuint UVsId;
	GLenum Type;
	vector<glm::vec2> Vertices;
	vector<glm::vec2> UVs;
	vector<stQuadLink> Quads;
public:
	MTextureQuadBuffer();
	MTextureQuadBuffer(GLenum inType, unsigned int inId);
	bool AddQuad(stTextureQuad* Quad);
	bool RemoveQuad(stTextureQuad* Quad);
	bool UpdateQuad(stTextureQuad* Quad);
	void UpdateAll();
	void Relocate();
	void Begin();
	void End();
	void DrawAll();
	void DrawQuad(stTextureQuad* Quad);
	void Close();
	void Clear();
	bool IsReady();
};

#endif
