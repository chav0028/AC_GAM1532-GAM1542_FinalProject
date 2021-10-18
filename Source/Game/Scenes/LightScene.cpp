#include "CommonHeader.h"
#include <time.h>

using namespace std;

LightScene::LightScene()
{
	//Generate random number seed
	srand((unsigned int)time(0));//Use time for random
}

LightScene::~LightScene()
{
	SAFE_DELETE(g_pQuickWatchManager);
}

void LightScene::OnSurfaceChanged(unsigned int aWidth, unsigned int aHeight)
{
	Scene::OnSurfaceChanged(aWidth, aHeight);
}

void LightScene::LoadContent(b2Vec2 aWorldGravity)
{
	// create cameras
	// Game camera
	m_pGameObjects[GAME_DEFAULT_CAMERA_NAME] = new CameraObject(this, GAME_DEFAULT_CAMERA_NAME,LIGHT_SCENE_CAMERA_DEFAULT_OFFSET, Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);
	CameraObject*pGameCamera = (CameraObject*)m_pGameObjects[GAME_DEFAULT_CAMERA_NAME];
    pGameCamera->SetCameraOffset(LIGHT_SCENE_CAMERA_DEFAULT_OFFSET);
    pGameCamera->SetUseLookAt(false);
	AddInputHandler(m_pGameObjects[GAME_DEFAULT_CAMERA_NAME]);

	LoadResources(LIGHT_SCENE_WORLD_GRAVITY);//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

	LoadGameObjects();//Load the game itself
}

bool LightScene::HandleInput(InputEvent& aInputevent, double aDelta)
{
	Scene::HandleInput(aInputevent, aDelta);

    if (aInputevent.state == InputEventState_Held || aInputevent.state == InputEventState_Down)
    {
        float movementSpeed = 20*(float)aDelta;

        if (aInputevent.keycode == 'A')
        {
            GameObject* TestObject = m_pGameObjects["TestObject"];

            if (TestObject != nullptr)
            {
                TestObject->SetPositionX(TestObject->GetPosition().x - movementSpeed);
            }
        }
        
        if (aInputevent.keycode == 'D')
        {
            GameObject* TestObject = m_pGameObjects["TestObject"];

            if (TestObject != nullptr)
            {
                TestObject->SetPositionX(TestObject->GetPosition().x + movementSpeed);
            }
        }

        if (aInputevent.keycode == 'W')
        {
            GameObject* TestObject = m_pGameObjects["TestObject"];

            if (TestObject != nullptr)
            {
                TestObject->SetPositionY(TestObject->GetPosition().y + movementSpeed);
            }
        }

        if (aInputevent.keycode == 'S')
        {
            GameObject* TestObject = m_pGameObjects["TestObject"];

            if (TestObject != nullptr)
            {
                TestObject->SetPositionY(TestObject->GetPosition().y - movementSpeed);
            }
        }

    }

	if (aInputevent.state == InputEventState_Up)
	{
		if (aInputevent.keycode == VK_SPACE)
		{
				m_testEmitter->FireParticles(10000, Vector3(0, 10, 0), 0.8f);
		}
	}

    return true;
}

void LightScene::LoadGameObjects()
{
    GameObject* parentObject = AddGameObject("TestObject", new GameObject(this, "parent", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0.5f, 0.5f, 0.5f), g_pGame->GetMesh("SphereOBJ"), g_pGame->GetShader("Light"), g_pGame->GetTexture("SkyBoxAmbient")));
    GameObject* childObject = AddGameObject("child", new GameObject(this, "child", Vector3(2, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), g_pGame->GetMesh("Cube"), g_pGame->GetShader("Light"), g_pGame->GetTexture("White")));
    //parentObject->SetSecondaryTexture(g_pGame->GetTexture("SkyBoxAmbient"));
    //parentObject->SetSecondaryTexture(g_pGame->GetTexture("TestCubeMap"));
       
    GameObject* skyBox = new GameObject(this, "SkyBox", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), g_pGame->GetMesh("InversedCube"), g_pGame->GetShader("skyBox"), g_pGame->GetTexture("SkyBoxAmbient"));
    SetSkyBox(skyBox);

	//Background object to see light
	//AddGameObject("Plane", new GameObject(this, "Plane", Vector3(0, 0, 15), Vector3(0, 0, 0), Vector3(15, 15, 15), g_pGame->GetMesh("Cube"), g_pGame->GetShader("Light"), g_pGame->GetTexture("White")));

    //Lights
    m_pGameObjects["TestLight"] = new LightObject(this, "Light", Vector3(2, 0, -5), Vector4(1, 0, 0, 1));

    LightObject* light = dynamic_cast<LightObject*>(m_pGameObjects["TestLight"]);
    if (light != nullptr)
    {
        AddLight(light);
    }

    m_pGameObjects["TestLight2"] = new LightObject(this, "Light2", Vector3(-2, 0, -5), Vector4(0, 1, 0, 1));
    LightObject* light2 = dynamic_cast<LightObject*>(m_pGameObjects["TestLight2"]);
    if (light2 != nullptr)
    {
        //AddLight(light2);
    }

	//create player emitter
	ParticleEmitter* playerEmitter = nullptr;
	ParticleSystem* particleSystem = new ParticleSystem(this,g_pGame->GetShader("Light"), g_pGame->GetTexture("Megaman"));
	AddParticleSystem(particleSystem);

	//Set the emitter properties
	ParticleEmitter* tempEmitter = nullptr;
	particleSystem->CreateAddParticleEmitter(20000);
	tempEmitter = particleSystem->GetParticleEmitter(0);
	tempEmitter->SetUseDirectionRange(true);
	tempEmitter->SetUseRandomEmitterPosition(false);
	tempEmitter->SetUseSpeedRange(false);
    tempEmitter->SetFiringAngleRange(vec3(-20, -5, -10), vec3(20, 10, 10));
	tempEmitter->SetSpeedRange(5);
    tempEmitter->SetAllParticles(Vector4(255, 255, 255, 255), Vector3(0.10f, 0.10f, 0.10f), 5, Vector3(0, 2.8f, 0));
	tempEmitter->SetSize(vec3(1,1,1));
	m_testEmitter = tempEmitter;

    //Add the cube as a child of the test object
    tempEmitter->SetParentObject(parentObject);
    childObject->SetParentObject(parentObject);

	g_pQuickWatchManager = new QuickWatchManager();
	light->SetLightType(LightType_PointLight);
	light->AddPositionToQuickWatch(g_pQuickWatchManager, -25, 25);
	light->AddRotationToQuickWatch(g_pQuickWatchManager,0.0f,360.0f,false,true,true);
	light->AddLightColorToQuickWatch(g_pQuickWatchManager);
	light->AddSpecularColorToQuickWatch(g_pQuickWatchManager);
	light->AddAttenuationToQuickWatch(g_pQuickWatchManager, -3, 3);
	//light->AddAngleRangeToQuickWatch(g_pQuickWatchManager);
	//light->AddLightTypeToQuickWatch(g_pQuickWatchManager);

	//g_pQuickWatchManager->AddSpace();
	//light2->AddPositionToQuickWatch(g_pQuickWatchManager, -25, 25);
	//ligh2t->AddRotationToQuickWatch(g_pQuickWatchManager, 0.0f, 360.0f, false, true, true);
	//light2->AddLightColorToQuickWatch(g_pQuickWatchManager);
	//light2->AddSpecularColorToQuickWatch(g_pQuickWatchManager);
	//light2->AddAttenuationToQuickWatch(g_pQuickWatchManager, -3, 3);
	//light2->AddAngleRangeToQuickWatch(g_pQuickWatchManager);
	//light2->AddLightTypeToQuickWatch(g_pQuickWatchManager);

	g_pQuickWatchManager->AddSpace();
	parentObject->AddReflectivityToQuickWatch(g_pQuickWatchManager);
	childObject->AddReflectivityToQuickWatch(g_pQuickWatchManager);

    extern HWND hWnd;
    SetFocus(hWnd);

	//parentObject->SetTweenXPosition(1, 2, TweeningType_ElasticEaseIn);
}

void LightScene::Update(double aTimePassed)
{
	Scene::Update(aTimePassed);

    //Rotation parent object
    //if (m_pGameObjects["TestObject"] != nullptr)
    //{
    //    float rotationSpeed = 100 * (float)aTimePassed;//Get the rotation speed
    //    Vector3 currentRotation = m_pGameObjects["TestObject"]->GetRotation();//Get the die rotatio
    //   //m_pGameObjects["TestObject"]->SetRotation(Vector3(currentRotation.x , currentRotation.y+rotationSpeed, currentRotation.z ));//Make the die rotate
    //
    //}

}

void LightScene::Draw()
{
	Scene::Draw();
}

void LightScene::LoadResources(b2Vec2 aWorldGravity)
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
        g_pGame->AddShader("skyBox", new ShaderProgram("Data/Shaders/SkyBox.vert", "Data/Shaders/SkyBox.frag"));
        g_pGame->AddShader("Reflective", new ShaderProgram("Data/Shaders/LightSkyBoxReflective.vert", "Data/Shaders/LightSkyBoxReflective.frag"));
        g_pGame->AddShader("PureReflective", new ShaderProgram("Data/Shaders/PureSkyBoxReflective.vert", "Data/Shaders/PureSkyBoxReflective.frag"));
	}

	// Create meshes.
	{
		g_pGame->AddMesh("Box", Mesh::CreateBox(vec2(1, 1)));

		g_pGame->AddMesh("Circle", Mesh::CreateCircle(0.5f));
		g_pGame->AddMesh("Cube", Mesh::CreateCube(vec3(1, 1, 1)));
        g_pGame->AddMesh("InversedCube", Mesh::CreateInversedCube(vec3(1, 1, 1)));
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

        const char* testCubeTexture[] =
        {
            "Data/Textures/TestCubeMap/posx.png",//Positive X
            "Data/Textures/TestCubeMap/negx.png",//Negative X
            "Data/Textures/TestCubeMap/posy.png",//Positive Y
            "Data/Textures/TestCubeMap/negy.png",//Negative Y
            "Data/Textures/TestCubeMap/posz.png",//Positive Z
            "Data/Textures/TestCubeMap/negz.png",//Negative Z
        };
        g_pGame->AddTexture("TestCubeMap",g_pGame->LoadTextureCubemap(testCubeTexture));

        const char* skyBox[] =
        {
            "Data/Textures/Yokohama2/posx.png",//Positive X
            "Data/Textures/Yokohama2/negx.png",//Negative X
            "Data/Textures/Yokohama2/posy.png",//Positive Y
            "Data/Textures/Yokohama2/negy.png",//Negative Y
            "Data/Textures/Yokohama2/posz.png",//Positive Z
            "Data/Textures/Yokohama2/negz.png",//Negative Z
        };
        g_pGame->AddTexture("SkyBoxAmbient", g_pGame->LoadTextureCubemap(skyBox));

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

void LightScene::Reset()
{
	Scene::Reset();
}
