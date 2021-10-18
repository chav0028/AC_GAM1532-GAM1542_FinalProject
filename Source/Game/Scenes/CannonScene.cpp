#include "CommonHeader.h"
#include <time.h>

using namespace std;

CannonScene::CannonScene() :
	m_score(0)
{
	//Generate random number seed
	srand((unsigned int)time(0));//Use time for random
}

CannonScene::~CannonScene()
{
}

void CannonScene::OnSurfaceChanged(unsigned int aWidth, unsigned int aHeight)
{
	Scene::OnSurfaceChanged(aWidth, aHeight);
}

void CannonScene::LoadContent(b2Vec2 aWorldGravity)
{
	//Save the current highscore
	char* file = LoadCompleteFile("Data/GameSaves/CannonSceneScore.txt", 0);//Load the file
	cJSON* jRoot = nullptr;

	if (file != 0)
	{
		jRoot = cJSON_Parse(file);//Parse the file
		if (jRoot != 0)
		{
			int jHighScore = cJSON_GetObjectItem(jRoot, "HighScore: ")->valueint;
			if (jHighScore != 0)
			{
				m_highScore = jHighScore;//Save the highscore value
			}
		}
	}

	SAFE_DELETE_ARRAY(file);
	cJSON_Delete(jRoot);


	// create cameras
	// Game camera
	m_pGameObjects[GAME_DEFAULT_CAMERA_NAME] = new CameraObject(this, GAME_DEFAULT_CAMERA_NAME, CANNON_SCENE_CAMERA_DEFAULT_OFFSET, Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);
	CameraObject*pGameCamera = (CameraObject*)m_pGameObjects[GAME_DEFAULT_CAMERA_NAME];
	pGameCamera->SetCameraOffset(CANNON_SCENE_CAMERA_DEFAULT_OFFSET);
	//AddInputHandler(m_pGameObjects[GAME_DEFAULT_CAMERA_NAME]);

	//HUD camera
	m_pGameObjects[HUD_DEFAULT_CAMERA_NAME] = new CameraObject(this, HUD_DEFAULT_CAMERA_NAME, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0, false);

	LoadResources(CANNON_SCENE_WORLD_GRAVITY);//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

	LoadGameObjects();//Load the game itself
}

bool CannonScene::HandleInput(InputEvent& aInputevent, double aDelta)
{


	if (aInputevent.state == InputEventState_Up)
	{
		//Save
		if (aInputevent.keycode == VK_F5)
		{
			cJSON* jRoot = cJSON_CreateObject();//Create  emtpy json object

			//cJSON* jScore = cJSON_CreateNumber(m_score);//Create a Json number object
			//cJSON_AddItemToObject(jRoot, "Score", jScore);//Add the object to the root

			//Add the score to the root.
			if (m_score > m_highScore)
			{
				cJSON_AddNumberToObject(jRoot, "HighScore: ", m_score);
			}
			else
			{
				cJSON_AddNumberToObject(jRoot, "HighScore: ", m_highScore);
			}

			vec3 cannonPosition = m_pGameObjects["Cannon"]->GetPosition();//Make local copy of values
			cannonPosition.x += 95;//Offset for testing

			cJSON* jCannonPos = cJSON_CreateFloatArray(&cannonPosition.x, 3);//Create an array
			cJSON_AddItemToObject(jRoot, "Cannon Position: ", jCannonPos);//Add the array of Json to the root

			//Output message, for testing only
			char* jsonString = cJSON_Print(jRoot);
			OutputMessage(jsonString);

			FILE* file = nullptr;
			errno_t error;

			if(error = fopen_s(&file, "CannonSceneScore.txt", "w")!=0)//Check if the file was opened
			{
				//FILE* file = fopen("CannonSceneScore.txt", "w");//Open the file
				fprintf(file, "%s", jsonString);//Write to the file
			}

			fclose(file);//Close the file

			cJSON_Delete(jRoot);//Delete the json object, it will also delete all of its children
		}


		//Load
		if (aInputevent.keycode == VK_F9)
		{
			char* file = LoadCompleteFile("CannonSceneScore.txt", 0);//Load the file
			cJSON* jRoot = nullptr;

			if (file != 0)
			{
				jRoot = cJSON_Parse(file);//Parse the file

				if (jRoot != 0)
				{
					int jHighScore = cJSON_GetObjectItem(jRoot, "HighScore: ")->valueint;
					if (jHighScore != 0)
					{
						m_score = jHighScore;//Save the highscore to be the furrent score
					}

					cJSON* jCannonPos = cJSON_GetObjectItem(jRoot, "Cannon Position: ");
					if (jCannonPos != 0)
					{
						vec3 playerPosition;

						//Get all the values in the array
						playerPosition.x = (float)cJSON_GetArrayItem(jCannonPos, 0)->valuedouble;
						playerPosition.y = (float)cJSON_GetArrayItem(jCannonPos, 1)->valuedouble;
						playerPosition.z = (float)cJSON_GetArrayItem(jCannonPos, 2)->valuedouble;

						m_pGameObjects["Cannon"]->SetPhysicsAndGameObjectPosition(playerPosition);//Set the position
						m_pGameObjects["CannonBase"]->SetPhysicsAndGameObjectPosition(playerPosition);
					}


				}
			}

			SAFE_DELETE_ARRAY( file);
			cJSON_Delete(jRoot);//Delete the json object, it will also delete all of its children
		}
	}

	Scene::HandleInput(aInputevent, aDelta);

    return false;
}

void CannonScene::LoadGameObjects()
{
	//Floor
	Vector3 gameCenterPosition;
	gameCenterPosition.x = (CANNON_SCENE_MAX_BOUNDARIES.x + CANNON_SCENE_MIN_BOUNDARIES.x) / 2;
	gameCenterPosition.y = (CANNON_SCENE_MAX_BOUNDARIES.y + CANNON_SCENE_MIN_BOUNDARIES.y) / 2;
	gameCenterPosition.z = (CANNON_SCENE_MAX_BOUNDARIES.z + CANNON_SCENE_MIN_BOUNDARIES.z) / 2;

	m_pGameObjects["Floor"] = new GameObject(this, "Floor", Vector3(gameCenterPosition.x, -45, gameCenterPosition.z - 1), Vector3(0, 0, 0), Vector3(1, 0, 1), g_pGame->GetMesh("Plane"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("White"));
	m_pGameObjects["Floor"]->SetColor(Vector4(0, 0, 0, 0));
	m_pGameObjects["Floor"]->CreatePhysicsBox(vec2(CANNON_SCENE_PLANE_SIZE.x, CANNON_SCENE_PLANE_SIZE.y), b2_staticBody);

	//Background
	m_pGameObjects["Background"] = new GameObject(this, "Background", Vector3(gameCenterPosition.x, gameCenterPosition.y, CANNON_SCENE_MAX_BOUNDARIES.z), Vector3(0, 0, 0), Vector3(150, 150, 1), g_pGame->GetMesh("Box"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("SkyBackground"));

	//Cannon
	m_pGameObjects["CannonBase"] = new GameObject(this, "CannonBase", Vector3(-50, -5, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);
	m_pGameObjects["CannonBase"]->CreatePhysicsBox(vec2(1, 1), b2_staticBody, vec2(0, 0), false, CannonSceneFilteringGroups_CannonAndProjectile);

	m_pGameObjects["Cannon"] = new Cannon(m_pGameObjects["CannonBase"]->GetPhysicsBody(), 35, this, "Cannon", Vector3(-50, -5, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), g_pGame->GetMesh("SubmarineOBJ"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("White"));
	AddInputHandler(m_pGameObjects["Cannon"]);

	//Target
	m_pGameObjects["Target"] = new ScoreSensor(this, "Target", Vector3(CANNON_SCENE_MAX_BOUNDARIES.x, 0, 0), Vector3(0, 0, 0), Vector3(5, 5, 5), g_pGame->GetMesh("Box"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Target"));

	//Obstacles, or moving walls
	vec3 obstaclePosition = vec3(-35, 0, 0);
	vec2 movingWallVelocity = vec2(0, 5);
	float minYPosition = 0 - 5;
	float maxYPositon = 0 + 10;
	float minXDistance = CANNON_SCENE_MIN_DISTANCE_BETWEEN_OBSTACLES + CANNON_SCENE_MOVING_WALLS_SIZE.x / 2;
	float maxXDistance = CANNON_SCENE_MAX_DISTANCE_BETWEEN_OBSTACLES + CANNON_SCENE_MOVING_WALLS_SIZE.x / 2;
	int directionVelocity = 0;
	double timeToChangeDirection = 0;

	for (unsigned int i = 0; i < CANNON_SCENE_NUMBER_OBSTACLES; i++)
	{
		//randomNumber=min + ( std::rand() % ( max - min + 1 ) )
		obstaclePosition.x += minXDistance + (rand() % (int)(maxXDistance - minXDistance + 1));
		obstaclePosition.y += minYPosition + (rand() % (int)(maxYPositon - minYPosition + 1));

		//See if object will go down or up
		directionVelocity = 0 + (rand() % (1 - 0 + 1));
		if (directionVelocity == 1)
		{
			movingWallVelocity *= -1;
		}

		timeToChangeDirection = CANNON_SCENE_MOVING_WALLS_MIN_TIME + (rand() % (int)(CANNON_SCENE_MOVING_WALLS_MAX_TIME - CANNON_SCENE_MOVING_WALLS_MIN_TIME + 1.0));

		m_pGameObjects["Obstacle" + to_string(i)] = new MovingWall(timeToChangeDirection, movingWallVelocity, this, "Obstacle" + to_string(i), obstaclePosition, Vector3(0, 0, 0), CANNON_SCENE_MOVING_WALLS_SIZE, g_pGame->GetMesh("Cube"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Blocks"));
	}

	//Score display
	m_pGameObjects["Score"] = new NumberFontSystem((unsigned int)m_score, this, "Score", Vector3(0, -4, 0.01f), Vector3(0, 0, 0), Vector3(5, 1, 1), g_pGame->GetMesh("Box"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("NumberFont"));
	NumberFontSystem* pScore = dynamic_cast<NumberFontSystem*>(m_pGameObjects["Score"]);

	if (pScore != nullptr)
	{
		pScore->LoadContent();
		pScore->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1));
	}

	//High Score
	m_pGameObjects["HighScore"] = new NumberFontSystem((unsigned int)m_score, this, "HighScore", Vector3(3, -4, 0.01f), Vector3(0, 0, 0), Vector3(5, 1, 1), g_pGame->GetMesh("Box"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("NumberFont"));
	NumberFontSystem* pHighScore = dynamic_cast<NumberFontSystem*>(m_pGameObjects["HighScore"]);

	if (pHighScore != nullptr)
	{
		pHighScore->LoadContent();
		pHighScore->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1));
	}
}

void CannonScene::Update(double aTimePassed)
{

	Scene::Update(aTimePassed);

	//Update the score
	NumberFontSystem* pScore = dynamic_cast<NumberFontSystem*>(m_pGameObjects["Score"]);
	if (pScore != nullptr)
	{
		pScore->SetNumber(m_score);
	}


	//Update the highScore
	NumberFontSystem* pHighScore = dynamic_cast<NumberFontSystem*>(m_pGameObjects["HighScore"]);
	if (pHighScore != nullptr)
	{
		pHighScore->SetNumber(m_highScore);
	}
}

void CannonScene::Draw()
{
	Scene::Draw();
}

void CannonScene::AddToScore(int aAmount)
{
	//Prevent negative values
	if (m_score + aAmount < 0)
	{
		m_score = 0;
	}
	else
	{
		m_score += aAmount;
	}
}

void CannonScene::LoadResources(b2Vec2 aWorldGravity)
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

void CannonScene::Reset()
{

	Scene::Reset();

	////Set the view in the debug draw
	//CameraObject* pCamera = dynamic_cast<CameraObject*>(GetGameObject(GAME_DEFAULT_CAMERA_NAME));//get the camera
	//if (pCamera != nullptr)
	//{
	//	pCamera->SetTargetPosition(m_pGameObjects["Player"]->GetPosition());
	//}
}
