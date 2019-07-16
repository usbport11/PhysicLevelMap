#include "stdafx.h"
#include "classes/system/Shader.h"
#include "classes/system/Scene.h"
#include "classes/system/FPSController.h"

#include "classes/physic/PhysicWorld.h"
#include "classes/physic/objects/Hero.h"
#include "classes/physic/objects/Enemy.h"
#include "classes/physic/objects/Bullet.h"

#include "classes/level/Leaf.h"
#include "classes/delaunay/delaunay.h"
#include "classes/level/TileMap.h"
#include "classes/buffers/AtlasBuffer.h"

#include "classes/buffers/StaticBuffer.h"

#include "classes/image/TextureLoader.h"

bool Pause;
bool keys[1024] = {0};
int WindowWidth = 800, WindowHeight = 600;
bool EnableVsync = 1;
GLFWwindow* window;
stFPSController FPSController;

MShader Shader;
MScene Scene;
MShader WallsShader;
MStaticBuffer StaticBuffer;

MTextureLoader TextureLoader;
stTexture* txOne;
unsigned int txOne_cnt;

MPhysicWorld PhysicWorld;
MHero* Hero = NULL;
MEnemy* Enemy = NULL;
MBullet* Bullet = NULL;
//velocity will be multiply by frame time value (1/60 = 0.016)
//example {80, 80} * {0.016, 0.016} = {0.128, 0.128}
glm::vec2 HeroVelocity = glm::vec2(80.0, 80.0);//2,2
glm::vec2 BulletVelocity = glm::vec2(4.0, 4.0);
glm::vec2 HeroSize = glm::vec2(32, 32);

//mouse data
bool UseCamera = false;
glm::vec2 MouseSceneCoord;
double mx, my;
glm::vec2 ObjectDirection;
float ObjectRotation;

MAtlasBuffer AtlasBuffer;
MTileMap TileMap;
int TilesCount[2] = {30, 30};
glm::vec2 TileSize(64, 64);//40,40
glm::vec2 Room0Center;
glm::vec2 CameraPosition;
glm::vec2 OldPosition;

bool GenerateLevel();

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void mousepos_callback(GLFWwindow* window, double x, double y) {
	//mouse coords in play mode must be recalculated each frame, because we can not move by mouse and not update view position
	//MouseSceneCoord = Scene.WindowPosToWorldPos(x, y);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	/*
	if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {//IMPORTANT!!! fix mouse double click 
		Bullet = new MBullet;
    	Bullet->Set(glm::vec2(Hero->GetCenter().x - 10, Hero->GetCenter().y - 10), glm::vec2(20, 20), glm::vec2(0, 0), glm::vec2(1, 1), OT_ENEMY);
    	if(!PhysicWorld.AddPhysicQuad((MPhysicQuad*)Bullet)) return;
    	glm::vec2 BulletDirection = glm::normalize(MouseSceneCoord - Hero->GetCenter());
    	float BulletRotation = atan2(BulletDirection.y, BulletDirection.x);
    	Bullet->SetRotationAngle(BulletRotation);
    	Bullet->SetVelocity(b2Vec2(BulletDirection.x * BulletVelocity.x, BulletDirection.y * BulletVelocity.y));
    	PhysicWorld.FillQuadBuffer();
	}
	*/
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}
	//regenerate level
	if(key == 'R' && action == GLFW_PRESS) {
		if(!GenerateLevel()) glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}
	if(key == 'C' && action == GLFW_PRESS) {
		UseCamera = !UseCamera;
		CameraPosition.x = (int)Hero->GetCenter().x;
		CameraPosition.y = (int)Hero->GetCenter().y;
		OldPosition = Hero->GetCenter();
    	Scene.ViewAt(CameraPosition);//start view center
    	return;
	}
	
	if(action == GLFW_PRESS)
    	keys[key] = true;
    else if (action == GLFW_RELEASE)
    	keys[key] = false;
}

void MoveKeysPressed() {
	b2Vec2 CurrentVelocity = b2Vec2(0.0f, 0.0f);
	if(keys['A'] || keys['D'] || keys['W'] || keys['S']) {
		if(keys['A']) {
			CurrentVelocity.x = -HeroVelocity.x;
		}
		if(keys['D']) {
			CurrentVelocity.x = HeroVelocity.x ;
		}
		if(keys['W']) {
			CurrentVelocity.y = HeroVelocity.y;
		}
		if(keys['S']) {
			CurrentVelocity.y = -HeroVelocity.y;
		}
	}
	CurrentVelocity.x *= FPSController.DeltaFrameTime;
	CurrentVelocity.y *= FPSController.DeltaFrameTime;
	if(Hero) Hero->SetVelocity(CurrentVelocity); 
}

bool CreatePhysicWalls() {
	vector<NLine2> Lines;
	
	NLine2 Line1, Line2;
	//get horizontal walls
	for(int i=0; i<TilesCount[0]; i++) {
		for(int j=0; j<TilesCount[1] - 1; j++) {
			//top walls
			if(TileMap.GetValue(i, j) != TT_FLOOR && TileMap.GetValue(i, j+1) == TT_FLOOR) {
				if(!Line1.a.x) {
					Line1.a.x = i;
					Line1.b.x = i;
					Line1.a.y = j+1;
					Line1.b.y = j+1;
				}
				else Line1.b.x ++;
			}
			else {
				if(Line1.a.x) {
					Line1.b.x ++;
					Lines.push_back(Line1);
					Line1.a.x = 0;
				}
			}
			//bottom walls
			if(TileMap.GetValue(i, j) == TT_FLOOR && TileMap.GetValue(i, j+1) != TT_FLOOR) {
				if(!Line2.a.x ) {
					Line2.a.x = i;
					Line2.b.x = i;
					Line2.a.y = j+1;
					Line2.b.y = j+1;
				}
				else Line2.b.x ++;
			}
			else {
				if(Line2.a.x) {
					Line2.b.x ++;
					Lines.push_back(Line2);
					Line2.a.x = 0;
				}
			}
		}
	}

	//get vertical walls
	for(int i=0; i<TilesCount[0] - 1; i++) {
		for(int j=0; j<TilesCount[1]; j++) {
			//left walls
			if(TileMap.GetValue(i, j) != TT_FLOOR && TileMap.GetValue(i+1, j) == TT_FLOOR) {
				if(!Line1.a.y) {
					Line1.a.y = j;
					Line1.b.y = j;
					Line1.a.x = i+1;
					Line1.b.x = i+1;
				}
				else Line1.b.y ++;
			}
			else {
				if(Line1.a.y) {
					Line1.b.y ++;
					Lines.push_back(Line1);
					Line1.a.y = 0;
				}
			}
			//right walls
			if(TileMap.GetValue(i, j) == TT_FLOOR && TileMap.GetValue(i+1, j) != TT_FLOOR) {
				if(!Line2.a.y ) {
					Line2.a.y = j;
					Line2.b.y = j;
					Line2.a.x = i+1;
					Line2.b.x = i+1;
				}
				else Line2.b.y ++;
			}
			else {
				if(Line2.a.y) {
					Line2.b.y ++;
					Lines.push_back(Line2);
					Line2.a.y = 0;
				}
			}
		}
	}
	
	for(int i=0; i<Lines.size(); i++) {
		if(Lines[i].a.x) {
			if(!PhysicWorld.AddWall(b2Vec2(Lines[i].a.x * TileSize.x, Lines[i].a.y * TileSize.y), b2Vec2(Lines[i].b.x * TileSize.x, Lines[i].b.y * TileSize.y))) return false;
			StaticBuffer.AddVertex(glm::vec2(Lines[i].a.x * TileSize.x, Lines[i].a.y * TileSize.y), glm::vec3(1, 1, 1));
    		StaticBuffer.AddVertex(glm::vec2(Lines[i].b.x * TileSize.x, Lines[i].b.y * TileSize.y), glm::vec3(1, 1, 1));
		}
	}
	StaticBuffer.Dispose();
	
	Lines.clear();
	
	return true;
}

bool FillTileBuffer() {
	unsigned int AtasPos[2];
	for(int i=0; i<TilesCount[0] - 1; i++) {
		for(int j=0; j<TilesCount[1] - 1; j++) {
			if(TileMap.GetValue(i, j) == TT_NONE) continue;
			if(TileMap.GetValue(i, j) == TT_FLOOR) {
				AtasPos[0] = 0;
				AtasPos[1] = 1;
			}
			if(TileMap.GetValue(i, j) == TT_WALL_FULL) {
				AtasPos[0] = 1;
				AtasPos[1] = 0;
			}
			if(TileMap.GetValue(i, j) == TT_WALL_PART) {
				AtasPos[0] = 0;
				AtasPos[1] = 0;
			}
			if(!AtlasBuffer.AddData(glm::vec2(i * TileSize.x, j * TileSize.y), glm::vec2((i + 1) * TileSize.x, (j + 1) * TileSize.y), AtasPos[0], AtasPos[1], 0, true)) return false;
		}
	}
	return true;
}

bool GenerateLevel() {
	//clear data before fill new
	AtlasBuffer.Clear();
	TileMap.Clear();
	PhysicWorld.Clear();
	StaticBuffer.Clear();
	
	list<TNode<stLeaf>* > Tree;
	int MinLeafSize = 6;
	int MaxLeafSize = 20;
	int MinRoomSize = 3;
	
	//create tree
	if(MinRoomSize >= MinLeafSize || MinLeafSize >= MaxLeafSize) {
		cout<<"Wrong settings"<<endl;
		return 0;
	}
	if(!SplitTree(&Tree, TilesCount[0], TilesCount[1], MinLeafSize, MaxLeafSize)) return false;
	
	//create rooms and fill centers map
	glm::vec3 Color = glm::vec3(1, 1, 1);
	TNode<NRectangle2>* pRoomNode;
	map<glm::vec2, TNode<NRectangle2>*, stVec2Compare> NodesCenters;
	glm::vec2 Center;
	NRectangle2* pRoom;
	list<TNode<stLeaf>* >::iterator itl;
	int RoomsNumber = 0;
	for(itl = Tree.begin(); itl != Tree.end(); itl++) {
		pRoomNode = CreateRoomInLeaf(*itl, MinRoomSize);
		if(!pRoomNode) continue;
		pRoom = pRoomNode->GetValueP();
		if(!pRoom) continue;
		//add in map
		Center.x = (pRoom->Position.x + pRoom->Size.x * 0.5) * TileSize.x;
		Center.y = (pRoom->Position.y + pRoom->Size.y * 0.5) * TileSize.y;
		NodesCenters.insert(pair<glm::vec2, TNode<NRectangle2>* >(Center, pRoomNode));
		//add in buffer
		RoomsNumber ++;
		TileMap.SetRectangle(*pRoom, 1);
	}
	if(RoomsNumber < 2) {
		cout<<"Too few rooms: "<<RoomsNumber<<endl;
		return false;
	}
	Room0Center = NodesCenters.begin()->first;
	
	//copy centers for triangulation
	map<glm::vec2, TNode<NRectangle2>*, stVec2Compare>::iterator itm;
	vector<glm::vec2> CentersPoints;
	for(itm = NodesCenters.begin(); itm != NodesCenters.end(); itm++) {
		CentersPoints.push_back(itm->first);
	}
	
	//triangulate by delaunay and get mst
	MDelaunay Triangulation;
	vector<MTriangle> Triangles = Triangulation.Triangulate(CentersPoints);
	vector<MEdge> Edges = Triangulation.GetEdges();
	vector<MEdge> MST = Triangulation.CreateMSTEdges();
	
	//create halls
	vector<NRectangle2> Halls;
	TNode<NRectangle2>* pNode0;
	TNode<NRectangle2>* pNode1;
	for(int i=0; i<MST.size(); i++) {
		pNode0 = NodesCenters[MST[i].p1];
		pNode1 = NodesCenters[MST[i].p2];
		Halls = CreateHalls2(pNode0->GetValueP(), pNode1->GetValueP());
		for(int k=0; k<Halls.size(); k++) {
			TileMap.SetRectangle(Halls[k], 1);
		}
	}
	Halls.clear();
	
	MST.clear();
	Triangulation.Clear();
	Triangles.clear();
	Edges.clear();
	CentersPoints.clear();

	NodesCenters.clear();
	
	ClearTree(&Tree);
	
	//later create floor
	TileMap.CreateWalls();
	//fill buffer based on 
	FillTileBuffer();
	//dispose buffer
	if(!AtlasBuffer.Dispose()) return false;
	
	//create physic walls
	if(!PhysicWorld.CreateLevelBody()) return false;
	if(!CreatePhysicWalls()) return false;
	//create main object
    if(!Hero) {
    	Hero = new MHero;
    	Hero->SetAddress((MPhysicQuad**)&Hero);
	}
    if(!Hero->Set(Room0Center, HeroSize, glm::vec2(0.5, 0.5), glm::vec2(0.5, 0.5))) return false;
    if(!PhysicWorld.AddPhysicQuad((MPhysicQuad*)Hero)) return false;
    PhysicWorld.FillQuadBuffer();
    //camera
    //CameraPosition = Hero->GetCenter();
    CameraPosition.x = (int)Hero->GetCenter().x;
	CameraPosition.y = (int)Hero->GetCenter().y;
    OldPosition = Hero->GetCenter();
    Scene.ViewAt(CameraPosition);//start view center
	
	return true;
}

bool InitApp() {
	LogFile<<"Starting application"<<endl;    
    glfwSetErrorCallback(error_callback);
    
    if(!glfwInit()) return false;
    window = glfwCreateWindow(WindowWidth, WindowHeight, "TestApp", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return false;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mousepos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwMakeContextCurrent(window);
    if(glfwExtensionSupported("WGL_EXT_swap_control")) {
    	LogFile<<"Window: V-Sync supported. V-Sync: "<<EnableVsync<<endl;
		glfwSwapInterval(EnableVsync);//0 - disable, 1 - enable
	}
	else LogFile<<"Window: V-Sync not supported"<<endl;
    LogFile<<"Window created: width: "<<WindowWidth<<" height: "<<WindowHeight<<endl;

	//glew
	GLenum Error = glewInit();
	if(GLEW_OK != Error) {
		LogFile<<"Window: GLEW Loader error: "<<glewGetErrorString(Error)<<endl;
		return false;
	}
	LogFile<<"GLEW initialized"<<endl;
	
	if(!CheckOpenglSupport()) return false;

	//shaders
	if(!Shader.CreateShaderProgram("shaders/main.vertexshader.glsl", "shaders/main.fragmentshader.glsl")) return false;
	if(!Shader.AddUnifrom("MVP", "MVP")) return false;
	if(!WallsShader.CreateShaderProgram("shaders/walls.vertexshader.glsl", "shaders/walls.fragmentshader.glsl")) return false;
	if(!WallsShader.AddUnifrom("MVP", "MVP")) return false;
	LogFile<<"Shaders loaded"<<endl;

	//scene
	if(!Scene.Initialize(&WindowWidth, &WindowHeight)) return false;
	LogFile<<"Scene initialized"<<endl;

	//randomize
    srand(time(NULL));
    LogFile<<"Randomized"<<endl;
    
    //other initializations
    //load texture
    txOne = TextureLoader.LoadTexture("textures/tex04.png", 1, 1, 0, txOne_cnt, GL_NEAREST, GL_REPEAT);
    if(!txOne) return false;
    
    //prepare level data
	if(!AtlasBuffer.Initialize(txOne, 32, 32, 2, 2)) return false;
    TileMap = MTileMap(TilesCount[0], TilesCount[1]);
    StaticBuffer.SetPrimitiveType(GL_LINES);
    if(!StaticBuffer.Initialize()) return false;
    if(!PhysicWorld.Initialize(0.0f, (float)1/60, 8, 3, 0.01f, 1)) return false;
    
    //generate level (first time)
	if(!GenerateLevel()) return false;
	
	//turn off pause
	Pause = false;
    
    return true;
}

void PreRenderStep() {
	if(Pause) return;
	
	//direction move
	//having some twiching whili use with camera
	glfwGetCursorPos(window, &mx, &my);
	MouseSceneCoord = Scene.WindowPosToWorldPos(mx, my);
	ObjectDirection = glm::normalize(MouseSceneCoord - Hero->GetCenter());
	ObjectRotation = atan2(ObjectDirection.y, ObjectDirection.x);
	Hero->SetRotationAngle(ObjectRotation);
	
	MoveKeysPressed();
	
	PhysicWorld.Step();
	PhysicWorld.UpdateObjects();
	
	//update camera only in case that hero position is changed
	//if camera use float point while moving textures may glitch
	if(UseCamera) {
		if(OldPosition != Hero->GetCenter()) {
			//CameraPosition.x = round(Hero->GetCenter().x);
			//CameraPosition.y = round(Hero->GetCenter().y);
			//CameraPosition.x = (int)Hero->GetCenter().x;
			//CameraPosition.y = (int)Hero->GetCenter().y;
			CameraPosition.x = Hero->GetCenter().x;
			CameraPosition.y = Hero->GetCenter().y;
			Scene.ViewAt(CameraPosition);
	    	//CameraPosition = Hero->GetCenter();
	    	OldPosition = Hero->GetCenter();
	    	//cout<<(int)Hero->GetCenter().x<<", "<<(int)Hero->GetCenter().y<<" | "<<Hero->GetCenter().x<<", "<<Hero->GetCenter().y<<endl;
		}
	}
}

void RenderStep() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2, 0.2, 0.2, 0);//grey background
    glUseProgram(Shader.ProgramId);
	glUniformMatrix4fv(Shader.Uniforms["MVP"], 1, GL_FALSE, Scene.GetDynamicMVP());
	
	//draw functions
	AtlasBuffer.Begin();
	AtlasBuffer.Draw();
	AtlasBuffer.End();
	PhysicWorld.DrawQuadBuffer();
	
	//draw walls
	//glUseProgram(WallsShader.ProgramId);
	//glUniformMatrix4fv(WallsShader.Uniforms["MVP"], 1, GL_FALSE, Scene.GetDynamicMVP());
	//StaticBuffer.Begin();
	//StaticBuffer.Draw();
	//StaticBuffer.End();
}

void ClearApp() {
	//clear funstions
	StaticBuffer.Close();
	PhysicWorld.Close();
	AtlasBuffer.Close();
	TileMap.Close();
	TextureLoader.DeleteTexture(txOne, txOne_cnt);
	TextureLoader.Close();
	
	memset(keys, 0, 1024);
	Shader.Close();
	WallsShader.Close();
	LogFile<<"Application: closed"<<endl;
}

int main(int argc, char** argv) {
	LogFile<<"Application: started"<<endl;
	if(!InitApp()) {
		ClearApp();
		glfwTerminate();
		LogFile.close();
		return 0;
	}
	FPSController.Initialize(glfwGetTime());
	while(!glfwWindowShouldClose(window)) {
		FPSController.FrameStep(glfwGetTime());
    	FPSController.FrameCheck();
    	PreRenderStep();
		RenderStep();
        glfwSwapBuffers(window);
        glfwPollEvents();
        //cout<<FPSController.DeltaFrameTime<<endl;
	}
	ClearApp();
    glfwTerminate();
    LogFile.close();
}
