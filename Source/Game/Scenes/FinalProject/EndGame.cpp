#include "CommonHeader.h"

EndGame::EndGame(int score, FGEndStates aEndState)
{
    m_finalScore = score;
    m_endState = aEndState;
}

EndGame::~EndGame()
{
}

void EndGame::LoadContent(b2Vec2 worldGravity)
{
    // create cameras
    // Game camera
    m_pGameObjects[HUD_DEFAULT_CAMERA_NAME] = new CameraObject(this, HUD_DEFAULT_CAMERA_NAME, SPLASH_SCREEN_CAMERA_DEFAULT_OFFSET, Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0,0);
    CameraObject*pGameCamera = (CameraObject*)m_pGameObjects[HUD_DEFAULT_CAMERA_NAME];
    pGameCamera->SetCameraOffset(MAIN_MENU_SCREEN_CAMERA_DEFAULT_OFFSET);
    pGameCamera->SetUseLookAt(false);

    LoadResources(worldGravity);//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

    LoadGameObjects();//Load the game itself
}
void EndGame::LoadResources(b2Vec2 aWorldGravity)
{
	Scene::LoadContent(aWorldGravity);

	// create our shaders.
	{
		g_pGame->AddShader("Texture", new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag"));
        g_pGame->AddShader("TextureColor", new ShaderProgram("Data/Shaders/TextureColor.vert", "Data/Shaders/TextureColor.frag"));
	}

	// Create meshes.
	{
		g_pGame->AddMesh("Box", Mesh::CreateBox(vec2(1, 1)));
        g_pGame->AddMesh("Button", Mesh::CreateBox(vec2(1, 1)));
        g_pGame->AddMesh("ScreenSprite", Mesh::CreateBox(vec2(20, 20)));

	}

	// load our textures.
	{
        g_pGame->AddTexture("ButtonResetSelected", LoadTexture("Data/Textures/FGButtonResetSelected.png"));
        g_pGame->AddTexture("ButtonResetUnselected", LoadTexture("Data/Textures/FGButtonResetUnselected.png"));
        g_pGame->AddTexture("ButtonMainMenuSelected", LoadTexture("Data/Textures/FGButtonMainMenuSelected.png"));
        g_pGame->AddTexture("ButtonMainMenuUnselected", LoadTexture("Data/Textures/FGButtonMainMenuUnselected.png"));
        g_pGame->AddTexture("EndScreenDead", LoadTexture("Data/Textures/FGEndScreenDead.png"));
        g_pGame->AddTexture("EndScreenTime", LoadTexture("Data/Textures/FGEndScreenTime.png"));
        g_pGame->AddTexture("EndScreenComplete", LoadTexture("Data/Textures/FGEndScreenComplete.png"));
        g_pGame->AddTexture("NumberFont", LoadTexture("Data/Textures/NumberFont.png"));
	}

    Scene::LoadContent(aWorldGravity);
}

void EndGame::LoadGameObjects()
{
    GLint backgroundTexture = 0;
    Vector4 fontColor = Vector4(0, 0, 0, 0);

    //Choose which texture to use
    switch (m_endState)
    {
    case FGEndStates_Dead:
        backgroundTexture = g_pGame->GetTexture("EndScreenDead");
        fontColor = Vector4(244, 0, 0, 255);
        break;
    case FGEndStates_TimeOver:
        backgroundTexture = g_pGame->GetTexture("EndScreenTime");
        fontColor = Vector4(255, 255, 255, 255);
        break;
    case FGEndStates_Completed:
        backgroundTexture = g_pGame->GetTexture("EndScreenComplete");
        fontColor = Vector4(0, 0, 230, 255);
        break;
    default:
        break;
    }


    GameObject* background = AddGameObject("EndBackground", new GameObject(this, "EndBackground", Vector3(0, 0, 2), vec3(0, 0, 0), vec3(1, 1, 1), g_pGame->GetMesh("ScreenSprite"), g_pGame->GetShader("Texture"), backgroundTexture));
    background->SetElementOfHUD(true);
    background->SetCameraDrawerName(HUD_DEFAULT_CAMERA_NAME);
    background->SetRenderOrder(0);

    m_pGameObjects["FinalScore"] = new NumberFontSystem(m_finalScore, this, "FinalScore", Vector3(0.0, -1.0, 0.0), Vector3(0, 0, 0), Vector3(2, 1, 1), g_pGame->GetMesh("Box"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("NumberFont"));
    NumberFontSystem* score = dynamic_cast<NumberFontSystem*>(m_pGameObjects["FinalScore"]);

    if (score != nullptr)
    {
        score->LoadContent();
        score->SetColor(fontColor);
        score->SetElementOfHUD(true);
    }

    GameObject* mainMenuButton = AddGameObject("MainMenuButton", new MainMenuButton(this, "MainMenuButton", Vector3(3.5, -8, 0), g_pGame->GetMesh("Button"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ButtonMainMenuUnselected"), g_pGame->GetTexture("ButtonMainMenuUnselected")));
    AddInputHandler(mainMenuButton);
    mainMenuButton->SetRenderOrder(3);

    GameObject* resetButton = AddGameObject("ResetButton", new ResetButton(this, "ResetButton", Vector3(-3.5, -8, 0), g_pGame->GetMesh("Button"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ButtonResetUnselected"), g_pGame->GetTexture("ButtonResetSelected")));
    AddInputHandler(resetButton);
    resetButton->SetRenderOrder(3);

    //Set tweens , different start time
    mainMenuButton->SetTweenYPosition(PAUSE_SCREEN_BUTTONS_FINAL_Y, PAUSE_SCREEN_BUTTONS_ANIMATION_TIME, TweeningType_BounceEaseout, 0.8);
    resetButton->SetTweenYPosition(PAUSE_SCREEN_BUTTONS_FINAL_Y, PAUSE_SCREEN_BUTTONS_ANIMATION_TIME, TweeningType_BounceEaseout, 1.2);
    

}

bool EndGame::HandleInput(InputEvent& aInputevent, double aDelta)
{
    return Scene::HandleInput(aInputevent, aDelta);
}

void EndGame::Update(double TimePassed)
{
    Scene::Update(TimePassed);
}