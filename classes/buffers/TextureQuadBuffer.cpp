#include "../../stdafx.h"
#include "TextureQuadBuffer.h"

MTextureQuadBuffer::MTextureQuadBuffer() {
	VerticesId = 0;
	UVsId = 0;
	BindNumber = 0;
}

MTextureQuadBuffer::MTextureQuadBuffer(GLenum inType, unsigned int inId) {
	Type = inType;
	glGenBuffers(1, &VerticesId);
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec2), NULL, Type);
	glGenBuffers(1, &UVsId);
	glBindBuffer(GL_ARRAY_BUFFER, UVsId);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), NULL, Type);
	Id = inId;
}

bool MTextureQuadBuffer::AddQuad(stTextureQuad* Quad) {
	if(!Quad) return false;
	vector<stQuadLink>::iterator it = find_if(Quads.begin(), Quads.end(), stFindQuadLink(Quad));
	if(it != Quads.end()) return false;
	if(it != Quads.end()) {
		UpdateQuad(Quad);
		return true;
	}
	Quads.push_back(stQuadLink(Quad, Vertices.size()));
	Vertices.push_back(Quad->v[0]);
	Vertices.push_back(Quad->v[1]);
	Vertices.push_back(Quad->v[2]);
	Vertices.push_back(Quad->v[3]);
	UVs.push_back(Quad->t[0]);
	UVs.push_back(Quad->t[1]);
	UVs.push_back(Quad->t[2]);
	UVs.push_back(Quad->t[3]);
	return true;
}

bool MTextureQuadBuffer::RemoveQuad(stTextureQuad* Quad) {
	if(!Quad) return false;
	vector<stQuadLink>::iterator it = find_if(Quads.begin(), Quads.end(), stFindQuadLink(Quad));
	if(it != Quads.end()) {
		vector<stQuadLink>::iterator itSave = it;
		for(itSave++; itSave != Quads.end(); itSave++) {
			itSave->Shift -= 4;
		}
		Vertices.erase(Vertices.begin() + it->Shift, Vertices.begin() + it->Shift + 4);
		UVs.erase(UVs.begin() + it->Shift, UVs.begin() + it->Shift + 4);
		Quads.erase(it);
	}
	return true;
}

bool MTextureQuadBuffer::UpdateQuad(stTextureQuad* Quad) {
	if(!Quad) return false;
	vector<stQuadLink>::iterator it = find_if(Quads.begin(), Quads.end(), stFindQuadLink(Quad));
	if(it == Quads.end()) return false;
	Vertices[it->Shift] = Quad->v[0];
	Vertices[it->Shift + 1] = Quad->v[1];
	Vertices[it->Shift + 2] = Quad->v[2];
	Vertices[it->Shift + 3] = Quad->v[3];
	UVs[it->Shift] = Quad->t[0];
	UVs[it->Shift + 1] = Quad->t[1];
	UVs[it->Shift + 2] = Quad->t[2];
	UVs[it->Shift + 3] = Quad->t[3];
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glBufferSubData(GL_ARRAY_BUFFER, it->Shift * sizeof(glm::vec2), sizeof(glm::vec2) * 4, &Vertices[it->Shift][0]);
	glBindBuffer(GL_ARRAY_BUFFER, UVsId);
	glBufferSubData(GL_ARRAY_BUFFER, it->Shift * sizeof(glm::vec2), sizeof(glm::vec2) * 4, &UVs[it->Shift][0]);
	return true;
}

void MTextureQuadBuffer::UpdateAll() {
	for(int i=0; i < Quads.size(); i ++) {
		Vertices[Quads[i].Shift] = Quads[i].pTextureQuad->v[0];
		Vertices[Quads[i].Shift + 1] = Quads[i].pTextureQuad->v[1];
		Vertices[Quads[i].Shift + 2] = Quads[i].pTextureQuad->v[2];
		Vertices[Quads[i].Shift + 3] = Quads[i].pTextureQuad->v[3];
		UVs[Quads[i].Shift] = Quads[i].pTextureQuad->t[0];
		UVs[Quads[i].Shift + 1] = Quads[i].pTextureQuad->t[1];
		UVs[Quads[i].Shift + 2] = Quads[i].pTextureQuad->t[2];
		UVs[Quads[i].Shift + 3] = Quads[i].pTextureQuad->t[3];
	}
	Relocate();
}

void MTextureQuadBuffer::Relocate() {
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec2), NULL, Type);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec2), &Vertices[0][0], Type);
	glBindBuffer(GL_ARRAY_BUFFER, UVsId);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), NULL, Type);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0][0], Type);
}

void MTextureQuadBuffer::Begin() {
	glEnableVertexAttribArray(SHR_LAYOUT_VERTEX);
	glEnableVertexAttribArray(SHR_LAYOUT_UV);
}

void MTextureQuadBuffer::End() {
	glDisableVertexAttribArray(SHR_LAYOUT_VERTEX);
	glDisableVertexAttribArray(SHR_LAYOUT_UV);
}

void MTextureQuadBuffer::DrawAll() {
	glActiveTexture(GL_TEXTURE0 + BindNumber);
	glBindTexture(GL_TEXTURE_2D, Id);
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glVertexAttribPointer(SHR_LAYOUT_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, UVsId);
	glVertexAttribPointer(SHR_LAYOUT_UV, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_QUADS, 0, Vertices.size());
}

void MTextureQuadBuffer::DrawQuad(stTextureQuad* Quad) {
	if(!Quad) return;
	vector<stQuadLink>::iterator it = find_if(Quads.begin(), Quads.end(), stFindQuadLink(Quad));
	if(it == Quads.end()) return;
	
	glActiveTexture(GL_TEXTURE0 + BindNumber);
	glBindTexture(GL_TEXTURE_2D, Id);
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glVertexAttribPointer(SHR_LAYOUT_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, UVsId);
	glVertexAttribPointer(SHR_LAYOUT_UV, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_QUADS, it->Shift, 4);
}

void MTextureQuadBuffer::Clear() {
	if(!IsReady()) return;
	Vertices.clear();
	UVs.clear();
	Quads.clear();
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec2), NULL, Type);
	glBindBuffer(GL_ARRAY_BUFFER, UVsId);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), NULL, Type);
}

void MTextureQuadBuffer::Close() {
	Clear();
	glDeleteBuffers(1, &VerticesId);
	glDeleteBuffers(1, &UVsId);
}

bool MTextureQuadBuffer::IsReady() {
	return (VerticesId | UVsId);
}
