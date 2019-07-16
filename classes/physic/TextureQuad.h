#ifndef texturequadH
#define texturequadH

struct stTextureQuad {
	glm::vec2 v[4];
	glm::vec2 t[4];
	stTextureQuad() {
		v[0] = v[1] = v[2] = v[3] = glm::vec2(0, 0);
		t[0] = t[1] = t[2] = t[3] = glm::vec2(0, 0);
	}
	stTextureQuad(glm::vec2 vp, glm::vec2 vs, glm::vec2 tp, glm::vec2 ts) {
		v[0] = glm::vec2(vp.x, vp.y);
		v[1] = glm::vec2(vp.x + vs.x, vp.y);
		v[2] = glm::vec2(vp.x + vs.x, vp.y + vs.y);
		v[3] = glm::vec2(vp.x, vp.y + vs.y);
		
		t[0] = glm::vec2(tp.x, tp.y);
		t[1] = glm::vec2(tp.x + ts.x, tp.y);
		t[2] = glm::vec2(tp.x + ts.x, tp.y + ts.y);
		t[3] = glm::vec2(tp.x, tp.y + ts.y);
	}
};

#endif
