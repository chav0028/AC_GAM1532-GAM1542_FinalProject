#include "CommonHeader.h"
#include <time.h>

using namespace std;

LoadingSampleScene::LoadingSampleScene()
{
	//Generate random number seed
	srand((unsigned int)time(0));//Use time for random
}

LoadingSampleScene::~LoadingSampleScene()
{
}

void LoadingSampleScene::OnSurfaceChanged(unsigned int aWidth, unsigned int aHeight)
{
	Scene::OnSurfaceChanged(aWidth, aHeight);
}

void LoadingSampleScene::LoadContent(b2Vec2 aWorldGravity)
{
	// create cameras
	// Game camera
	LoadResources(LIGHT_SCENE_WORLD_GRAVITY);//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

    //Load the scene file
    char* file = LoadCompleteFile("Data/Scenes/test.box2dscene", 0);//Load the file
	cJSON* jRoot = nullptr;

	//If fie is valid
	if (file != 0)
	{
		jRoot = cJSON_Parse(file);//Parse the file
		if (jRoot != 0)
		{
			LoadGameObjectsScene(jRoot);//Load the game itself
		}
	}

    SAFE_DELETE_ARRAY(file);
    cJSON_Delete(jRoot);
}

bool LoadingSampleScene::HandleInput(InputEvent& aInputevent, double aDelta)
{
	Scene::HandleInput(aInputevent, aDelta);

    return false;
}


void LoadingSampleScene::Update(double aTimePassed)
{

	Scene::Update(aTimePassed);

}

void LoadingSampleScene::Draw()
{
	Scene::Draw();
}

void LoadingSampleScene::LoadResources(b2Vec2 aWorldGravity)
{
	Scene::LoadContent(aWorldGravity);

	// create our shaders.
	{
		g_pGame->AddShader("Texture", new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag"));
		g_pGame->AddShader("Color", new ShaderProgram("Data/Shaders/Color.vert", "Data/Shaders/Color.frag"));
		g_pGame->AddShader("TextureColor", new ShaderProgram("Data/Shaders/TextureColor.vert", "Data/Shaders/TextureColor.frag"));
		g_pGame->AddShader("MovingWater", new ShaderProgram("Data/Shaders/MovingWater.vert", "Data/Shaders/MovingWater.frag"));
		g_pGame->AddShader("WaterRipples", new ShaderProgram("Data/Shaders/WaterRipples.vert", "Data/Shaders/WaterRipples.frag"));
		g_pGame->AddShader("SinTextureColor", new ShaderProgram("Data/Shaders/SinTextureColor.vert", "Data/Shaders/SinTextureColor.frag"));
		g_pGame->AddShader("DebugDraw", new ShaderProgram("Data/Shaders/DebugDraw.vert", "Data/Shaders/DebugDraw.frag"));
        g_pGame->AddShader("Light", new ShaderProgram("Data/Shaders/Light.vert", "Data/Shaders/Light.frag"));
	}

	// Create meshes.
	{
		g_pGame->AddMesh("Box", Mesh::CreateBox(vec2(1, 1)));

		g_pGame->AddMesh("Circle", Mesh::CreateCircle(0.5f));
		g_pGame->AddMesh("Cube", Mesh::CreateCube(vec3(1, 1, 1)));
		g_pGame->AddMesh("OffsetCube", Mesh::CreateCube(vec3(1, 1, 1), vec3(0, 0, 1.5)));
		g_pGame->AddMesh("Plane", Mesh::CreatePlane(vec2(PLANE_SIZE.x, PLANE_SIZE.z), Vector2Int(PLANE_NUMBER_COLUMNS_ROWS.x, PLANE_NUMBER_COLUMNS_ROWS.y)));

		g_pGame->AddMesh("SphereOBJ", Mesh::LoadObj("OBJs/Sphere.obj", vec3(1, 1, 1)));
		g_pGame->AddMesh("CubeOBJ", Mesh::LoadObj("OBJs/Cube.obj", vec3(1, 1, 1)));
		g_pGame->AddMesh("SubmarineOBJ", Mesh::LoadObj("OBJs/Submarine.obj"));
	}

	// load our textures.
	{
		g_pGame->AddTexture("Megaman", LoadTexture("Data/Textures/Megaman.png"));
		g_pGame->AddTexture("MegamanCube", LoadTexture("Data/Textures/MegamanCube.png"));
		g_pGame->AddTexture("MegamanCubeRed", LoadTexture("Data/Textures/MegamanCubeRed.png"));
		g_pGame->AddTexture("BoulderCube", LoadTexture("Data/Textures/Boulder.png"));
		g_pGame->AddTexture("MegamanX", LoadTexture("Data/Textures/MegamanX.png"));
		g_pGame->AddTexture("Blocks", LoadTexture("Data/Textures/Blocks.png"));
		g_pGame->AddTexture("Die", LoadTexture("Data/Textures/Die.png"));
		g_pGame->AddTexture("BlueBackground", LoadTexture("Data/Textures/BlueBackground.png"));
		g_pGame->AddTexture("Enemy", LoadTexture("Data/Textures/Enemy.png"));
		g_pGame->AddTexture("Enemy2", LoadTexture("Data/Textures/Enemy2.png"));
		g_pGame->AddTexture("Sand", LoadTexture("Data/Textures/Sand.png"));
		g_pGame->AddTexture("White", LoadTexture("Data/Textures/White.png"));
		g_pGame->AddTexture("SkyBackground", LoadTexture("Data/Textures/SkyBackgorund.png"));
		g_pGame->AddTexture("NumberFont", LoadTexture("Data/Textures/NumberFont.png"));
		g_pGame->AddTexture("Target", LoadTexture("Data/Textures/Target.png"));
	}

	////Set the view in the debug draw
	CameraObject* pCamera = dynamic_cast<CameraObject*>(GetGameObject(GAME_DEFAULT_CAMERA_NAME));//get the camera

	if (m_pSceneWorld != nullptr)
	{

		if (pCamera != nullptr)
		{
			m_pSceneWorld->SetDebugDrawViewMatrix(pCamera->GetViewMatrix());//Pass the debug draw the view matrix of the camera
		}

		m_pSceneWorld->SetDebugDrawShader(g_pGame->GetShader("DebugDraw"));//Set which sahder the debug draw will use
	}
}

void LoadingSampleScene::Reset()
{
	Scene::Reset();
}
