#include "CommonHeader.h"

ControlsScene::ControlsScene()
{
}

ControlsScene::~ControlsScene()
{
}

void ControlsScene::LoadContent(b2Vec2 worldGravity)
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
void ControlsScene::LoadResources(b2Vec2 aWorldGravity)
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
        g_pGame->AddTexture("ButtonBackSelected", LoadTexture("Data/Textures/FGButtonBackSelected.png"));
        g_pGame->AddTexture("ButtonBackUnselected", LoadTexture("Data/Textures/FGButtonBackUnselected.png"));
        g_pGame->AddTexture("ControlsBackground", LoadTexture("Data/Textures/FGControslScreen.png"));

    }

    Scene::LoadContent(aWorldGravity);
}

void ControlsScene::LoadGameObjects()
{
    GameObject* background = AddGameObject("ControlsBackground", new GameObject(this, "ControlsBackground", Vector3(0, 0, 2), vec3(0, 0, 0), vec3(1, 1, 1), g_pGame->GetMesh("ScreenSprite"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ControlsBackground")));
    background->SetElementOfHUD(true);
    background->SetCameraDrawerName(HUD_DEFAULT_CAMERA_NAME);
    background->SetRenderOrder(0);


    GameObject* backButton = AddGameObject("BackButton", new BackButton(this, "BackButton", Vector3(-7, -4, 0), g_pGame->GetMesh("Button"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ButtonBackUnselected"), g_pGame->GetTexture("ButtonBackSelected")));
    AddInputHandler(backButton);
    backButton->SetRenderOrder(3);

    //Set tweens , different start time
    backButton->SetTweenXPosition(-3.0f, 2.0, TweeningType_ElasticEaseInOut, 0.1);
}

bool ControlsScene::HandleInput(InputEvent& aInputevent, double aDelta)
{
    return Scene::HandleInput(aInputevent, aDelta);
}

void ControlsScene::Update(double TimePassed)
{
    Scene::Update(TimePassed);
}