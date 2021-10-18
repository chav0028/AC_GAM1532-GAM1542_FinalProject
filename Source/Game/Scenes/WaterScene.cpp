#include "CommonHeader.h"

WaterScene::WaterScene()
{
}

WaterScene::~WaterScene()
{
}

void WaterScene::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    Scene::OnSurfaceChanged( width, height );
}

void WaterScene::LoadContent(b2Vec2 aWorldGravity)
{
     
	// create cameras
	// Game camera
	// Camera         
	m_pGameObjects[GAME_DEFAULT_CAMERA_NAME] = new CameraObject(this, GAME_DEFAULT_CAMERA_NAME, Vector3(0, 5, -20), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);
	CameraObject* pCamera = dynamic_cast<CameraObject*>(GetGameObject(GAME_DEFAULT_CAMERA_NAME));//get the camera
	pCamera->SetCameraOffset(Vector3(0, 5, -20));//Offset
	//AddInputHandler(m_pGameObjects[GAME_DEFAULT_CAMERA_NAME]);//FOR TESTING ONLY

	LoadResources();//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

	LoadGameObjects();//Load the game itself

}

void WaterScene::LoadResources(b2Vec2 aWorldGravity)
{
	// create our shaders.
	{
		g_pGame->AddShader("Texture", new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag"));
		g_pGame->AddShader("Water", new ShaderProgram("Data/Shaders/Water.vert", "Data/Shaders/Water.frag"));
	}

	// Create meshes.
	{
		g_pGame->AddMesh("Box", Mesh::CreateBox(vec2(1, 1)));
		g_pGame->AddMesh("Cube", Mesh::CreateCube(vec3(1, 1, 1)));
		g_pGame->AddMesh("Plane", Mesh::CreatePlane(vec2(100, 100), Vector2Int(500, 500)));
		g_pGame->AddMesh("CubeOBJ", Mesh::LoadObj("OBJs/Cube.obj", vec3(1, 1, 1)));

	}

	// load our textures.
	{
		g_pGame->AddTexture("MegamanCube", LoadTexture("Data/Textures/MegamanCube.png"));
		g_pGame->AddTexture("Water", LoadTexture("Data/Textures/Water.png"));
		g_pGame->AddTexture("Steel", LoadTexture("Data/Textures/Blocks.png"));
		g_pGame->AddTexture("Meteor", LoadTexture("Data/Textures/Enemy.png"));
		g_pGame->AddTexture("White", LoadTexture("Data/Textures/White.png"));
	}

		Scene::LoadContent(aWorldGravity);//Create Box2D World

		////Set the view in the debug draw
		CameraObject* pCamera = dynamic_cast<CameraObject*>(GetGameObject(GAME_DEFAULT_CAMERA_NAME));//get the camera
		pCamera->SetCameraOffset(Vector3(0, 5, -20));

		if (m_pSceneWorld != nullptr)
		{

			if (pCamera != nullptr)
			{
				m_pSceneWorld->SetDebugDrawViewMatrix(pCamera->GetViewMatrix());//Pass the debug draw the view matrix of the camera
			}

			m_pSceneWorld->SetDebugDrawShader(g_pGame->GetShader("Color"));//Set which sahder the debug draw will use
		}
}

void WaterScene::LoadGameObjects()
{
	// Water
	m_pGameObjects["Water"] = new GameObject(this, "Water", Vector3(0, 0, 50), Vector3(0), Vector3(1),
		g_pGame->GetMesh("Plane"), g_pGame->GetShader("Water"), g_pGame->GetTexture("Water"));
	m_pGameObjects["Water"]->SetColor(Vector4(0, 240, 240, 255));
    m_pGameObjects["Water"]->SetRenderOrder(2);

	// Player
	m_pGameObjects["Player"] = new FloatingPlayerObject(this, "Player", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1),
		g_pGame->GetMesh("Cube"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("MegamanCube"));
	AddInputHandler(m_pGameObjects["Player"]);//Set player for input
    m_pGameObjects["Player"]->SetRenderOrder(3);

	// Mines
	for (int i = 0; i<WATER_SCENE_NUMBER_MINE_OBJECT; i++)
	{
		std::string minename = "Mine" + std::to_string(i);
		Vector3 pos = Vector3((float)sin(i)*10.0f, 0.0f, 3.0f + i*3.0f);
		m_pGameObjects[minename] = new FloatingMineObject(this, minename, pos, Vector3(0, 0, 0), Vector3(1),
			g_pGame->GetMesh("Box"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Meteor"));
        m_pGameObjects[minename]->SetRenderOrder(3);
	}

}

void WaterScene::Update(double aTimePassed)
{
    CameraObject* pCamera = dynamic_cast<CameraObject*>(m_pGameObjects[GAME_DEFAULT_CAMERA_NAME]);//get the camera

    if (pCamera != nullptr)
    {
        //Update the camera
        //pCamera->SetTargetPosition(vec3(0, 0, 0));
        pCamera->SetTargetPosition(m_pGameObjects["Player"]->GetPosition());//Set camera target as the player position
        pCamera->FollowObject(m_pGameObjects["Player"], aTimePassed);
        pCamera->Update(aTimePassed);
    }

    Scene::Update( aTimePassed );
}

void WaterScene::Draw()
{
    Scene::Draw();
}
