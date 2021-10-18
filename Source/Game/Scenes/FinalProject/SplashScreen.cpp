#include "CommonHeader.h"

SplashScreen::SplashScreen()
{
    m_gameTitle = nullptr;
    m_gameTitleInPosition = false;
    m_mainMenuLoaded = false;

}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::LoadGameObjects()
{
	m_gameTitle = AddGameObject("GameTitle", new GameObject(this, "GameTitle", Vector3(-10, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), g_pGame->GetMesh("Box"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("FGTitle")));
	m_gameTitle->SetCameraDrawerName(HUD_DEFAULT_CAMERA_NAME);
	m_gameTitle->SetElementOfHUD(true);

	//Set tween for game title
	m_gameTitle->SetTweenXPosition(0, 3.0, TweeningType_ElasticEaseout);
}

void SplashScreen::LoadContent(b2Vec2 worldGravity)
{
    // create cameras
    // Game camera
    m_pGameObjects[HUD_DEFAULT_CAMERA_NAME] = new CameraObject(this, HUD_DEFAULT_CAMERA_NAME, SPLASH_SCREEN_CAMERA_DEFAULT_OFFSET, Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0,0);
    CameraObject*pGameCamera = (CameraObject*)m_pGameObjects[HUD_DEFAULT_CAMERA_NAME];
    pGameCamera->SetCameraOffset(SPLASH_SCREEN_CAMERA_DEFAULT_OFFSET);
    pGameCamera->SetUseLookAt(false);

    LoadResources(worldGravity);//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

    LoadGameObjects();//Load the game itself
}
void SplashScreen::LoadResources(b2Vec2 worldGravity)
{
    Scene::LoadContent(worldGravity);

    // create our shaders.
    {
        g_pGame->AddShader("Texture", new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag"));
		g_pGame->AddShader("PostEffectBW", new ShaderProgram("Data/Shaders/PostEffectTextureBlackAndWhite.vert", "Data/Shaders/PostEffectTextureBlackAndWhite.frag"));
        g_pGame->AddShader("PostEffectSepia", new ShaderProgram("Data/Shaders/PostEffectTextureSepia.vert", "Data/Shaders/PostEffectTextureSepia.frag"));
        g_pGame->AddShader("PostEffectNegative", new ShaderProgram("Data/Shaders/PostEffectTextureNegative.vert", "Data/Shaders/PostEffectTextureNegative.frag"));
        //g_pGame->AddShader("PostEffectVignette", new ShaderProgram("Data/Shaders/PostEffectTextureVignette.vert", "Data/Shaders/PostEffectTextureVignette.frag"));
    }

    // Create meshes.
    {
        g_pGame->AddMesh("Box", Mesh::CreateBox(vec2(1, 1))); 
    }

    // load our textures.
    {
        g_pGame->AddTexture("MegamanX", LoadTexture("Data/Textures/MegamanX.png"));
        g_pGame->AddTexture("White", LoadTexture("Data/Textures/White.png"));
        g_pGame->AddTexture("FGTitle", LoadTexture("Data/Textures/FGTitle.png"));
    }
	
    g_pGame->GetSceneManager()->SetMainFBO(1024, 1024);
    g_pGame->GetSceneManager()->SetSecondaryFBO(1024, 1024);

    AddPostProcessingShader(g_pGame->GetShader("PostEffectNegative"));
    m_usePostProcessEffects = true;
}

bool SplashScreen::HandleInput(InputEvent& aInputevent, double aDelta)
{
    Scene::HandleInput(aInputevent, aDelta);

    if (aInputevent.state == InputEventState_Held || aInputevent.state == InputEventState_Down)
    {
            if (aInputevent.keycode == VK_SPACE)
            {
                GoToMainMenu();

                return true;
            }
    }

    return false;
}

void SplashScreen::Update(double TimePassed)
{
    Scene::Update(TimePassed);

    if (m_gameTitle != nullptr)
    {
        //Check if the position tween is over
        if (m_gameTitleInPosition == false)
        {
            if (m_gameTitle->GetFloatTweenerOver(GAMEOBJECT_X_POSITION_TWEEN_NAME) == true)
            {
                //Set the scale tween
                m_gameTitle->SetTweenScale(Vector3(8, 8, 8), 4, TweeningType_BounceEaseInOut);
                m_gameTitleInPosition = true;


                m_usePostProcessEffects = false;
            }
        }
        else
        {
            //If the scale tween is over
            if (m_gameTitle->GetVec3TweenerOver(GAMEOBJECT_SCALE_TWEEN_NAME)==true)
            {
                GoToMainMenu();//Go to main menu
            }
            
        }
    }
}

void SplashScreen::GoToMainMenu()
{
    SceneManager* sceneManager = g_pGame->GetSceneManager();
    
    //Check if the menu hasn't been already loaded, this to prevent issues with high framerate
    if (m_mainMenuLoaded == true)
    {
        return;
    }

    if (sceneManager != nullptr)
    {
        //Disable the update and draw in this scene
        m_drawable = false;
        m_updateable = false;
        m_mainMenuLoaded = true;

        //Push the main menu
        sceneManager->PushAndLoadScene(new MainMenu());
    }
}