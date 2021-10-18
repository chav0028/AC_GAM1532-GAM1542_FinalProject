#include "CommonHeader.h"

PauseScene::PauseScene() :m_resumeButton(nullptr)
{
    m_sceneIsOpaque = false;
}

PauseScene::~PauseScene()
{
}

void PauseScene::LoadContent(b2Vec2 worldGravity)
{
    // create cameras
    // Game camera
    m_pGameObjects[HUD_DEFAULT_CAMERA_NAME] = new CameraObject(this, HUD_DEFAULT_CAMERA_NAME, CONTROLS_SCENE_SCREEN_CAMERA_DEFAULT_OFFSET, Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);
    CameraObject*pGameCamera = (CameraObject*)m_pGameObjects[HUD_DEFAULT_CAMERA_NAME];
    pGameCamera->SetCameraOffset(MAIN_MENU_SCREEN_CAMERA_DEFAULT_OFFSET);
    pGameCamera->SetUseLookAt(false);

    LoadResources(worldGravity);//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

    LoadGameObjects();//Load the game itself
}
void PauseScene::LoadResources(b2Vec2 aWorldGravity)
{
    Scene::LoadContent(aWorldGravity);

    // create our shaders.
    {
        g_pGame->AddShader("Texture", new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag"));
    }

    // Create meshes.
    {
        g_pGame->AddMesh("Box", Mesh::CreateBox(vec2(1, 1)));
        g_pGame->AddMesh("Button", Mesh::CreateBox(vec2(3.0f, 1.5f)));
        g_pGame->AddMesh("ScreenSprite", Mesh::CreateBox(vec2(10, 10)));
    }

    // load our textures.
    {
        g_pGame->AddTexture("ButtonResumeSelected", LoadTexture("Data/Textures/FGButtonResumeSelected.png"));
        g_pGame->AddTexture("ButtonResumeUnselected", LoadTexture("Data/Textures/FGButtonResumeUnselected.png"));
        g_pGame->AddTexture("ButtonResetSelected", LoadTexture("Data/Textures/FGButtonResetSelected.png"));
        g_pGame->AddTexture("ButtonResetUnselected", LoadTexture("Data/Textures/FGButtonResetUnselected.png"));
        g_pGame->AddTexture("ButtonMainMenuSelected", LoadTexture("Data/Textures/FGButtonMainMenuSelected.png"));
        g_pGame->AddTexture("ButtonMainMenuUnselected", LoadTexture("Data/Textures/FGButtonMainMenuUnselected.png"));
        g_pGame->AddTexture("PauseScreen", LoadTexture("Data/Textures/FGPauseScreen.png"));

    }
}

void PauseScene::LoadGameObjects()
{
    GameObject* background = AddGameObject("PauseScreen", new GameObject(this, "PauseScreen", Vector3(0, 0, 2), vec3(0, 0, 0), vec3(1, 1, 1), g_pGame->GetMesh("ScreenSprite"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("PauseScreen")));
    background->SetElementOfHUD(true);
    background->SetCameraDrawerName(HUD_DEFAULT_CAMERA_NAME);
    background->SetRenderOrder(0);

    m_resumeButton = (BackButton*)AddGameObject("ResumeButton", new BackButton(this, "ResumeButton", Vector3(3.5, -8, 0), g_pGame->GetMesh("Button"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ButtonResumeUnselected"), g_pGame->GetTexture("ButtonResumeSelected")));
    AddInputHandler(m_resumeButton);
    m_resumeButton->SetRenderOrder(3);

    GameObject* mainMenuButton = AddGameObject("MainMenuButton", new MainMenuButton(this, "MainMenuButton", Vector3(0, -8, 0), g_pGame->GetMesh("Button"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ButtonMainMenuUnselected"), g_pGame->GetTexture("ButtonMainMenuSelected")));
    AddInputHandler(mainMenuButton);
    mainMenuButton->SetRenderOrder(3);

    GameObject* resetButton = AddGameObject("ResetButton", new ResetButton(this, "ResetButton", Vector3(-3.5, -8, 0), g_pGame->GetMesh("Button"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ButtonResetUnselected"), g_pGame->GetTexture("ButtonResetSelected")));
    AddInputHandler(resetButton);
    resetButton->SetRenderOrder(3);

    //Set tweens , different start time
    m_resumeButton->SetTweenYPosition(PAUSE_SCREEN_BUTTONS_FINAL_Y,PAUSE_SCREEN_BUTTONS_ANIMATION_TIME, TweeningType_BounceEaseout, 0.4);
    mainMenuButton->SetTweenYPosition(PAUSE_SCREEN_BUTTONS_FINAL_Y, PAUSE_SCREEN_BUTTONS_ANIMATION_TIME, TweeningType_BounceEaseout, 0.8);
    resetButton->SetTweenYPosition(PAUSE_SCREEN_BUTTONS_FINAL_Y, PAUSE_SCREEN_BUTTONS_ANIMATION_TIME, TweeningType_BounceEaseout, 1.2);
}

bool PauseScene::HandleInput(InputEvent& aInputevent, double aDelta)
{
    Scene::HandleInput(aInputevent, aDelta);

    //Check if P is pressed
    if (m_resumeButton != nullptr)
    {
        if (aInputevent.type == InputEventType_Key)
        {
            if (aInputevent.state == InputEventState_Up)
            {
                if (aInputevent.keycode == 'P')
                {
                    m_resumeButton->OnButtonPressed();
                }
            }
        }
    }

    return true;
}

void PauseScene::Update(double TimePassed)
{
    Scene::Update(TimePassed);
}