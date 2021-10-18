#include "CommonHeader.h"

MainMenu::MainMenu() :m_levelSphere(nullptr)
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::LoadContent(b2Vec2 worldGravity)
{
    // create cameras
    // HUD camera
    m_pGameObjects[HUD_DEFAULT_CAMERA_NAME] = new CameraObject(this, HUD_DEFAULT_CAMERA_NAME, MAIN_MENU_SCREEN_CAMERA_DEFAULT_OFFSET, Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0,0);
    CameraObject*pHUDCamera = (CameraObject*)m_pGameObjects[HUD_DEFAULT_CAMERA_NAME];
    pHUDCamera->SetCameraOffset(MAIN_MENU_SCREEN_CAMERA_DEFAULT_OFFSET);
    pHUDCamera->SetUseLookAt(false);

    // Game camera
    m_pGameObjects[GAME_DEFAULT_CAMERA_NAME] = new CameraObject(this, GAME_DEFAULT_CAMERA_NAME, Vector3(0,0,-5), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);
    CameraObject*pGameCamera = (CameraObject*)m_pGameObjects[GAME_DEFAULT_CAMERA_NAME];
    pGameCamera->SetUseLookAt(false);
    AddInputHandler(m_pGameObjects[GAME_DEFAULT_CAMERA_NAME]);


    LoadResources(worldGravity);//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

    LoadGameObjects();//Load the game itself
}
void MainMenu::LoadResources(b2Vec2 aWorldGravity)
{
	Scene::LoadContent(aWorldGravity);

	// create our shaders.
	{
		g_pGame->AddShader("Texture", new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag"));
        g_pGame->AddShader("PureReflective", new ShaderProgram("Data/Shaders/PureSkyBoxReflective.vert", "Data/Shaders/PureSkyBoxReflective.frag"));
	}

	// Create meshes.
	{
		g_pGame->AddMesh("Box", Mesh::CreateBox(vec2(1, 1)));
        g_pGame->AddMesh("Button", Mesh::CreateBox(vec2(3.0f, 1.5f)));
        g_pGame->AddMesh("ScreenSprite", Mesh::CreateBox(vec2(10, 10)));
        g_pGame->AddMesh("SphereOBJ", Mesh::LoadObj("OBJs/Sphere.obj", vec3(1, 1, 1)));
	}

	// load our textures.
	{
		g_pGame->AddTexture("ButtonPlaySelected", LoadTexture("Data/Textures/FGButtonPlaySelected.png"));
        g_pGame->AddTexture("ButtonPlaySelected", LoadTexture("Data/Textures/FGButtonPlaySelected.png"));
		g_pGame->AddTexture("ButtonPlayUnselected", LoadTexture("Data/Textures/FGButtonPlayUnselected.png"));
        g_pGame->AddTexture("ButtonBackSelected", LoadTexture("Data/Textures/FGButtonBackSelected.png"));
        g_pGame->AddTexture("ButtonBackUnselected", LoadTexture("Data/Textures/FGButtonBackUnselected.png"));
        g_pGame->AddTexture("ButtonQuitSelected", LoadTexture("Data/Textures/FGButtonQuitSelected.png"));
        g_pGame->AddTexture("ButtonQuitUnselected", LoadTexture("Data/Textures/FGButtonQuitUnselected.png"));
        g_pGame->AddTexture("ButtonControlsSelected", LoadTexture("Data/Textures/FGButtonControlsSelected.png"));
        g_pGame->AddTexture("ButtonControlsUnselected", LoadTexture("Data/Textures/FGButtonControlsUnselected.png"));
        g_pGame->AddTexture("FGTitle", LoadTexture("Data/Textures/FGTitle.png"));
        g_pGame->AddTexture("MainMenuBackground", LoadTexture("Data/Textures/FGMainMenuScreen.png"));

        const char* skyBox[] =
        {
            "Data/Textures/FGSkyBox/PosX.png",//Positive X
            "Data/Textures/FGSkyBox/NegX.png",//Negative X
            "Data/Textures/FGSkyBox/PosY.png",//Positive Y
            "Data/Textures/FGSkyBox/NegY.png",//Negative Y
            "Data/Textures/FGSkyBox/PosZ.png",//Positive Z
            "Data/Textures/FGSkyBox/NegZ.png",//Negative Z
        };

        g_pGame->AddTexture("FGSkyBox", g_pGame->LoadTextureCubemap(skyBox));
	}

    {
        AudioCue* tempCue = nullptr;
        Mix_Chunk* pSound = Mix_LoadWAV("Data/Audio/FGInputSound.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlaySingleSound);
        tempCue->SetVolume(64);
        g_pGame->AddAudio("InputSound", tempCue);
    }

    Scene::LoadContent(aWorldGravity);
}

void MainMenu::LoadGameObjects()
{
    GameObject* background = AddGameObject("MainMenuBackground", new GameObject(this, "MainMenuBackground", Vector3(0, 0, 2), vec3(0, 0, 0), vec3(1, 1, 1), g_pGame->GetMesh("ScreenSprite"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("MainMenuBackground")));
    background->SetElementOfHUD(true);
    background->SetCameraDrawerName(HUD_DEFAULT_CAMERA_NAME);
    background->SetRenderOrder(0);

    GameObject* playButton = AddGameObject("PlayGameButton", new PlayGameButton(this, "PlayGameButton", Vector3(-10, 0, 0), g_pGame->GetMesh("Button"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ButtonPlayUnselected"), g_pGame->GetTexture("ButtonPlaySelected")));
    AddInputHandler(playButton);
    playButton->SetRenderOrder(3);

    GameObject* controlsButton = AddGameObject("ControlsButton", new ControlsButton(this, "ControlsButton", Vector3(-10, -2, 0), g_pGame->GetMesh("Button"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ButtonControlsUnselected"), g_pGame->GetTexture("ButtonControlsSelected")));
    AddInputHandler(controlsButton);
    controlsButton->SetRenderOrder(3);

    GameObject* quitButton = AddGameObject("QuitButton", new QuitButton(this, "QuitButton", Vector3(-10, -4, 0), g_pGame->GetMesh("Button"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ButtonQuitUnselected"), g_pGame->GetTexture("ButtonQuitSelected")));
    AddInputHandler(quitButton);
    quitButton->SetRenderOrder(3);

    GameObject* title = AddGameObject("GameTitle", new GameObject(this, "GameTitle", Vector3(3, 10, 0), vec3(0, 0, 0), vec3(3, 3, 3), g_pGame->GetMesh("Box"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("FGTitle")));
    title->SetCameraDrawerName(HUD_DEFAULT_CAMERA_NAME);
    title->SetElementOfHUD(true);
    title->SetRenderOrder(3);

    m_levelSphere = AddGameObject("LevelSphere", new GameObject(this, "LevelSphere", Vector3(2, -1, 3), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f), g_pGame->GetMesh("SphereOBJ"), g_pGame->GetShader("PureReflective"), g_pGame->GetTexture("FGSkyBox")));
    m_levelSphere->SetReflectivity(1.0f);
    m_levelSphere->SetRenderOrder(3);

    //Set tweens , different start time
    playButton->SetTweenXPosition(MAIN_MENU_BUTTONS_FINAL_X, MAIN_MENU_BUTTONS_ANIMATION_TIME, TweeningType_SineEaseout, 0.2);
    controlsButton->SetTweenXPosition(MAIN_MENU_BUTTONS_FINAL_X, MAIN_MENU_BUTTONS_ANIMATION_TIME, TweeningType_SineEaseout, 1.8);
    quitButton->SetTweenXPosition(MAIN_MENU_BUTTONS_FINAL_X, MAIN_MENU_BUTTONS_ANIMATION_TIME, TweeningType_SineEaseout,2.3);
    title->SetTweenYPosition(MAIN_MENU_TITLE_FINAL_Y, MAIN_MENU_TITLE_ANIMATION_TIME, TweeningType_SineEaseout,0.2);
}

bool MainMenu::HandleInput(InputEvent& aInputevent, double aDelta)
{
    return Scene::HandleInput(aInputevent, aDelta);
}

void MainMenu::Update(double TimePassed)
{
    //Rotation
    if (m_levelSphere != nullptr)
    {
        float rotationSpeed = 50.0f * (float)TimePassed;//Get the rotation speed
        Vector3 currentRotation = m_levelSphere->GetRotation();//Get the die rotatio
        m_levelSphere->SetRotation(Vector3(currentRotation.x, currentRotation.y + rotationSpeed, currentRotation.z));//Rotate the sphere
    
    }

    Scene::Update(TimePassed);
}
