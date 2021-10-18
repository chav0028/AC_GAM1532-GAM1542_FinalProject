#include "CommonHeader.h"
#include <time.h>

using namespace std;

FinalGameScene::FinalGameScene() :
m_pGameCamera(nullptr),
m_pPlayer(nullptr),
m_pPlayerHealth(nullptr),
m_pScoreTimer(nullptr),
m_playerLight(nullptr),
m_playerPointLight(nullptr)
{

    m_gameScoreTimer = FG_GAME_STARTING_TIME;

    //Generate random number seed
    srand((unsigned int)time(0));//Use time for random
}

FinalGameScene::~FinalGameScene()
{
    SAFE_DELETE(g_pQuickWatchManager);
}

void FinalGameScene::OnSurfaceChanged(unsigned int aWidth, unsigned int aHeight)
{
    Scene::OnSurfaceChanged(aWidth, aHeight);
}

void FinalGameScene::LoadContent(b2Vec2 aWorldGravity)
{
    LoadResources(FINAL_GAME_SCENE_DEFAULT_WORLD_GRAVITY);//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

    //Load the scene file
    string sceneName(DEFAULT_FILE_PATH_SCENES);
    sceneName.append("FinalGame.box2dscene");
    char* file = LoadCompleteFile(sceneName.c_str(), 0);//Load the file
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

    //Set debug draw view matrix
    if (m_pSceneWorld != nullptr)
    {

        if (m_pGameCamera != nullptr)
        {

            m_pSceneWorld->SetDebugDrawViewMatrix(m_pGameCamera->GetViewMatrix());//Pass the debug draw the view matrix of the camera
        }

        m_pSceneWorld->SetDebugDrawShader(g_pGame->GetShader("DebugDraw"));//Set which sahder the debug draw will use
    }

}

bool FinalGameScene::HandleInput(InputEvent& aInputevent, double aDelta)
{
    if (aInputevent.type == InputEventType_Key)
    {

        if (aInputevent.state == InputEventState_Up)
        {
            if (aInputevent.keycode == 'P')
            {
                PauseGame();
            }

        }

    }

    Scene::HandleInput(aInputevent, aDelta);

    return false;
}


void FinalGameScene::Update(double aTimePassed)
{
	if (m_pGameCamera != nullptr)
	{

		if (m_pPlayer != nullptr)
		{

			//Update the camera
			m_pGameCamera->SetTargetPosition(vec3(m_pPlayer->GetPosition()));//Set camera target as the player position

																	 //Calculate the offset of the camera "LookAt"using player velocity
			vec2 playerVelocity = m_pPlayer->GetPhysicsBodyVelocity();
			m_pGameCamera->CalculateLookAtOffset(Vector3(playerVelocity.x, playerVelocity.y, 0), aTimePassed);//The position of what the player is tracking

			//Make the camera follow the player, this doesn't just snap the position. But it increases it over time until it matches target posoiton
			m_pGameCamera->FollowObjectXY(m_pPlayer, aTimePassed);//Actually move the camera

            //Update player health display
            UpdatePlayerHealthDisplay();

            //Make lights match the player
            if (m_playerLight != nullptr)
            {
                m_playerLight->SetPosition(m_pPlayer->GetPosition() + vec3(2, 0, 0));
                m_playerLight->SetRotation(m_pPlayer->GetRotation());
                m_playerLight->SetRotationZ(m_playerLight->GetRotationZ() + 180);

            }

            if (m_playerPointLight != nullptr && m_pPlayerHealth != nullptr)
            {
                if (m_pPlayer->GetGravityDown() == true)
                {
                    m_playerPointLight->SetPosition(m_pPlayer->GetPosition() + vec3(0, 2, 0));
                }
                else
                {
                    m_playerPointLight->SetPosition(m_pPlayer->GetPosition() + vec3(0, -2, 0));

                }
            }

            if (m_pPlayer->GetIsBoosted() == true)
            {
               //m_usePostProcessEffects = true;
               //AddPostProcessingShader(g_pGame->GetShader("PostEffectVignette"));//Doesnt look good
            }
            else if(m_pPlayer->GetIsInvincible() == true)
            {
                m_usePostProcessEffects = true;
                AddPostProcessingShader(g_pGame->GetShader("PostEffectHorizontalBlur"));
            }
            else
            {
               m_usePostProcessEffects = false;
               m_pLinkedPostProcessingShaders.clear();
            }

        }
	}

    //Update score (timer)
    if (m_pScoreTimer != nullptr)
    {
        m_gameScoreTimer -= aTimePassed;

        //If time is over
        if (m_gameScoreTimer <= 0)
        {
            m_gameScoreTimer = 0;//Check for negative values

            EndLevel(FGEndStates_TimeOver);
        }

        m_pScoreTimer->SetNumber((unsigned int)m_gameScoreTimer);

    }

    Scene::Update(aTimePassed);

    GameBoundCheck();

}

void FinalGameScene::Draw()
{
    Scene::Draw();
}

void FinalGameScene::LoadResources(b2Vec2 aWorldGravity)
{
    Scene::LoadContent(aWorldGravity);

    // create our shaders.
    {
        g_pGame->AddShader("DebugDraw", new ShaderProgram("Data/Shaders/DebugDraw.vert", "Data/Shaders/DebugDraw.frag"));
        g_pGame->AddShader("SkyBox", new ShaderProgram("Data/Shaders/SkyBox.vert", "Data/Shaders/SkyBox.frag"));
        g_pGame->AddShader("TextureColor", new ShaderProgram("Data/Shaders/TextureColor.vert", "Data/Shaders/TextureColor.frag"));
        g_pGame->AddShader("PostEffectBW", new ShaderProgram("Data/Shaders/PostEffectTextureBlackAndWhite.vert", "Data/Shaders/PostEffectTextureBlackAndWhite.frag"));
        g_pGame->AddShader("PostEffectVignette", new ShaderProgram("Data/Shaders/PostEffectTextureVignette.vert", "Data/Shaders/PostEffectTextureVignette.frag"));//NOT TESTED
        g_pGame->AddShader("PostEffectSepia", new ShaderProgram("Data/Shaders/PostEffectTextureSepia.vert", "Data/Shaders/PostEffectTextureSepia.frag"));
        g_pGame->AddShader("PostEffectNegative", new ShaderProgram("Data/Shaders/PostEffectTextureNegative.vert", "Data/Shaders/PostEffectTextureNegative.frag"));
        g_pGame->AddShader("PostEffectHorizontalBlur", new ShaderProgram("Data/Shaders/PostEffectTextureHorizontalBlur.vert", "Data/Shaders/PostEffectTextureHorizontalBlur.frag"));//NOT TESTED
    }

    // Create meshes.
    {
        g_pGame->AddMesh("Box", Mesh::CreateBox(vec2(1, 1)));
        g_pGame->AddMesh("Circle", Mesh::CreateCircle(0.5f));
        g_pGame->AddMesh("InversedCube", Mesh::CreateInversedCube(vec3(1, 1, 1)));
    }

    // load our textures.
    {      
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

        g_pGame->AddTexture("NumberFont", LoadTexture("Data/Textures/NumberFont.png"));
        g_pGame->AddTexture("ParticleSparks", LoadTexture("Data/Textures/FGParticlesSpark.png"));
    }
    //Load Sounds
    {
        AudioCue* tempCue = nullptr;
        Mix_Chunk* pSound = Mix_LoadWAV("Data/Audio/FGEnemyExplosion1.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlayRandomly);
        pSound = Mix_LoadWAV("Data/Audio/FGEnemyExplosion2.wav");
        tempCue->AddAudio(pSound);
        g_pGame->AddAudio("EnemyExplosion", tempCue);

        pSound = Mix_LoadWAV("Data/Audio/FGGravityChange.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlaySingleSound);
        g_pGame->AddAudio("GravityChange", tempCue);

        pSound = Mix_LoadWAV("Data/Audio/FGInvincibilityPickUp.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlaySingleSound);
        g_pGame->AddAudio("InvincibilityPickUp", tempCue);

        pSound = Mix_LoadWAV("Data/Audio/FGHealthPickUp.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlaySingleSound);
        g_pGame->AddAudio("HealthPickUp", tempCue);

        pSound = Mix_LoadWAV("Data/Audio/FGSpeedPickUp.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlaySingleSound);
        g_pGame->AddAudio("SpeedPickUp", tempCue);

        pSound = Mix_LoadWAV("Data/Audio/FGPlayerJump1.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlayRandomly);
        pSound = Mix_LoadWAV("Data/Audio/FGPlayerJump2.wav");
        tempCue->AddAudio(pSound);
        pSound = Mix_LoadWAV("Data/Audio/FGPlayerJump3.wav");
        tempCue->AddAudio(pSound);
        g_pGame->AddAudio("PlayerJump", tempCue);

        pSound = Mix_LoadWAV("Data/Audio/FGPlayerDeath.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlaySingleSound);
        g_pGame->AddAudio("PlayerDeath", tempCue);

        pSound = Mix_LoadWAV("Data/Audio/FGPlayerHurt1.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlayRandomly);
        pSound = Mix_LoadWAV("Data/Audio/FGPlayerHurt2.wav");
        tempCue->AddAudio(pSound);
        pSound = Mix_LoadWAV("Data/Audio/FGPlayerHurt3.wav");
        tempCue->AddAudio(pSound);
        pSound = Mix_LoadWAV("Data/Audio/FGPlayerHurt4.wav");
        tempCue->AddAudio(pSound);
        g_pGame->AddAudio("PlayerHurt", tempCue);

        pSound = Mix_LoadWAV("Data/Audio/FGPlayerHit1.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlayRandomly);
        pSound = Mix_LoadWAV("Data/Audio/FGPlayerHit2.wav");
        tempCue->AddAudio(pSound);
        pSound = Mix_LoadWAV("Data/Audio/FGPlayerHit3.wav");
        tempCue->AddAudio(pSound);
        pSound = Mix_LoadWAV("Data/Audio/FGPlayerHit4.wav");
        tempCue->AddAudio(pSound);
        pSound = Mix_LoadWAV("Data/Audio/FGPlayerHit5.wav");
        tempCue->AddAudio(pSound);
        tempCue->SetVolume(64);
        g_pGame->AddAudio("PlayerHit", tempCue);

        pSound = Mix_LoadWAV("Data/Audio/World-of-Automatons_Looping.wav");
        tempCue = new AudioCue(pSound, AudioCueType_PlaySingleSound,AudioGroups_Music);
        tempCue->SetVolume(64);
        g_pGame->AddAudio("FGMusic", tempCue);

        //Play background music
        AudioCue* music = g_pGame->GetAudioCue("FGMusic");
        if (music != nullptr)
        {
            //Initialize it (SDL needs to "play" a sound first to save channels, etc.)
            music->PlayAudio(PlayingStyle_Looping, -1, -1);

            //Play it
            music->PlayAudio(PlayingStyle_Looping, -1, -1);
        }
    }

}

void FinalGameScene::Reset()
{
    m_gameScoreTimer = FG_GAME_STARTING_TIME;
    
    AudioCue* music = g_pGame->GetAudioCue("FGMusic");
    if (music != nullptr)
    {
        //Initialize it (SDL needs to "play" a sound first to save channels, etc.)
        music->PlayAudio(PlayingStyle_Looping, -1, -1);

        //Play it
        music->PlayAudio(PlayingStyle_Looping, -1, -1);
    }

    Scene::Reset();
}

//Checks a JSON file corresponding to a Box2D scene with all its game objects and creates the game objects accordingly. It creates all the object as the simple, base, game objects.
void FinalGameScene::LoadGameObjectsScene(cJSON* ajRoot)
{
    //Set the sky box
    SetSkyBox(new GameObject(this, "SkyBox", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), g_pGame->GetMesh("InversedCube"), g_pGame->GetShader("SkyBox"), g_pGame->GetTexture("FGSkyBox")));

    vector<FGPlayerWheel*> playerWheels;
    signed short maskBit;
    GameObject* tempObject = nullptr;

    //Read from Box2DScene file
    if (ajRoot != nullptr)
    {
        //Get all the game objects
        cJSON* jGameObjects = cJSON_GetObjectItem(ajRoot, "GameObjects");

        if (jGameObjects != 0)
        {

            cJSON* jCurrentObject = nullptr;
            int numberGameObjects = cJSON_GetArraySize(jGameObjects);


            //Load all the game objects
            for (int i = 0; i < numberGameObjects; i++)
            {
                jCurrentObject = cJSON_GetArrayItem(jGameObjects, i);

                //Check the type of the object
                cJSON* jFlags = cJSON_GetObjectItem(jCurrentObject, "Flags");
                cJSON* jCurrentFlag = nullptr;
                string stringCurrentFlag;

                if (jFlags != 0)
                {
                    //If there are no flags, just create a gameobject
                    if (cJSON_GetArraySize(jFlags) == 0)
                    {
                        tempObject = LoadGameObject(jCurrentObject);
                        maskBit = FGCollisionFilteringCategory_Enemy | FGCollisionFilteringCategory_Player;
                        tempObject->SetFixtureCollisionsFiltering(0, 0, FGCollisionFilteringCategory_EnviromentStatic, maskBit);
                    }
                    //Check all the flags the object
                    for (int i = 0; i < cJSON_GetArraySize(jFlags); i++)
                    {
                        jCurrentFlag = cJSON_GetArrayItem(jFlags, i);
                        {
                            if (jCurrentFlag != 0)
                            {
                                stringCurrentFlag = jCurrentFlag->valuestring;

                                //If the object is a GAME camera
                                if (stringCurrentFlag == "Camera-Main")
                                {
                                    //Set properties of game camera
                                    m_pGameCamera = (CameraObject*)LoadGameObject(jCurrentObject, new CameraObject(GAME_DEFAULT_CAMERA_NAME),false);
                                    AddInputHandler(m_pGameCamera);
                                    
                                    //Customize camera
                                    m_pGameCamera->SetUseLookAt(true);
                                    m_pGameCamera->SetName(GAME_DEFAULT_CAMERA_NAME);
                                    m_pGameCamera->SetCameraOffset(FG_GAME_CAMERA_DEFAULT_OFFSET);
                                    m_pGameCamera->SetMaxVelocityThreshold(vec3(5, 5, 5));
                                    m_pGameCamera->SetMinVelocityThreshold(vec3(-5, -5, -5));
                                    m_pGameCamera->SetLimitYMovement(true);//Limit Y movement
                                    m_pGameCamera->SetMinPositionsLimit(vec3(0, -5, 0));
                                    m_pGameCamera->SetMaxPositionsLimit(vec3(0, 30, 0));
                                    AddGameObject(GAME_DEFAULT_CAMERA_NAME, m_pGameCamera);
                                }
                                //If the object is a HUD camera
                                else if (stringCurrentFlag == "Camera-HUD")
                                {
                                    //Set properties of HUD camera
                                    CameraObject* hudCamera = (CameraObject*)LoadGameObject(jCurrentObject, new CameraObject(HUD_DEFAULT_CAMERA_NAME), false);
                                    hudCamera->SetName(HUD_DEFAULT_CAMERA_NAME);
                                    hudCamera->SetUseLookAt(false);
                                    AddGameObject(HUD_DEFAULT_CAMERA_NAME, hudCamera);
                                }
                                else if (stringCurrentFlag == "Player")//Load player wheel
                                {
                                    m_pPlayer = (FGPlayer*)LoadGameObject(jCurrentObject, new FGPlayer());

                                    maskBit = FG_PLAYER_DEFAULT_COLLISION_MASK;
                                    m_pPlayer->SetFixtureCollisionsFiltering(-1, FGCollisionFilteringGroup_PlayerAndWheels, FGCollisionFilteringCategory_Player, maskBit);
                                    m_pPlayer->SetReflectivity(1.0f);

                                    b2Body* playerPhysicsBody = m_pPlayer->GetPhysicsBody();
                                    if (playerPhysicsBody != nullptr)
                                    {
                                        playerPhysicsBody->SetSleepingAllowed(false);
                                        playerPhysicsBody->SetActive(true);
                                        //playerPhysicsBody->SetFixedRotation(true);
                                    }

                                    AddInputHandler(m_pPlayer);
                                }
                                else if (stringCurrentFlag == "PlayerWheel")//Load player wheel
                                {
                                    playerWheels.push_back((FGPlayerWheel*)LoadGameObject(jCurrentObject, new FGPlayerWheel()));
                                    playerWheels.back()->SetReflectivity(0.0f);

                                    maskBit = FG_PLAYER_DEFAULT_COLLISION_MASK;
                                                              
                                    playerWheels.back()->SetFixtureCollisionsFiltering(-1, FGCollisionFilteringGroup_PlayerAndWheels, FGCollisionFilteringCategory_Player, maskBit);

                                    b2Body* wheelPhysicsBody = playerWheels.back()->GetPhysicsBody();
                                    if (wheelPhysicsBody != nullptr)
                                    {
                                        wheelPhysicsBody->SetSleepingAllowed(false);
                                        wheelPhysicsBody->SetActive(true);
                                        wheelPhysicsBody->SetAngularDamping(0.5f);
                                    }
                                }
                                else if (stringCurrentFlag == "PickUpSpeed")
                                {
                                    maskBit = FGCollisionFilteringCategory_Player | FGCollisionFilteringCategory_EnviromentStatic;

                                    FGSpeedPickUp* speedPickUp = (FGSpeedPickUp*)LoadGameObject(jCurrentObject, new FGSpeedPickUp(5,5.0));
                                    speedPickUp->SetFixtureCollisionsFiltering(-1, FGCollisionFilteringGroup_PickUpsAndEnemies, FGCollisionFilteringCategory_PickUp, maskBit);
                                }
                                else if (stringCurrentFlag == "PickUpHealth")
                                {
                                    maskBit = FGCollisionFilteringCategory_Player|FGCollisionFilteringCategory_EnviromentStatic;

                                    FGHealthPickUp* health = (FGHealthPickUp*)LoadGameObject(jCurrentObject, new FGHealthPickUp(20));
                                    health->SetFixtureCollisionsFiltering(-1, FGCollisionFilteringGroup_PickUpsAndEnemies, FGCollisionFilteringCategory_PickUp, maskBit);
                                }
                                else if (stringCurrentFlag == "InvincibilityPickUp")
                                {
                                    maskBit = FGCollisionFilteringCategory_Player | FGCollisionFilteringCategory_EnviromentStatic;

                                    FGInvincibilityPickUp* invincibilityPickUp = (FGInvincibilityPickUp*)LoadGameObject(jCurrentObject, new FGInvincibilityPickUp(3.0));
                                    invincibilityPickUp->SetFixtureCollisionsFiltering(-1, FGCollisionFilteringGroup_PickUpsAndEnemies, FGCollisionFilteringCategory_PickUp, maskBit);
                                }
                                else if (stringCurrentFlag == "Spikes")
                                {
                                    maskBit = FGCollisionFilteringCategory_Player | FGCollisionFilteringCategory_EnviromentStatic;
                                    FGPSpikes* spikes = (FGPSpikes*)LoadGameObject(jCurrentObject, new FGPSpikes(20,true));
                                    spikes->SetFixtureCollisionsFiltering(-1, 0, FGCollisionFilteringCategory_EnviromentStatic, maskBit);
                                }
                                else if (stringCurrentFlag == "InvisibleEnemy")
                                {
                                    maskBit = FGCollisionFilteringCategory_Player | FGCollisionFilteringCategory_EnviromentStatic|FGCollisionFilteringCategory_EnviromentDynamic;

                                    FGInvisibleEnemy* enemy = (FGInvisibleEnemy*)LoadGameObject(jCurrentObject, new FGInvisibleEnemy(30,5));
                                    enemy->InitializeEnemy(g_pGame->GetTexture("FGSkyBox"), enemy->GetTexture());
                                    enemy->SetFixtureCollisionsFiltering(-1, FGCollisionFilteringGroup_PickUpsAndEnemies, FGCollisionFilteringCategory_Enemy, maskBit);
                                }
                                else if (stringCurrentFlag == "Target")
                                {
                                    maskBit = FGCollisionFilteringCategory_Player;

                                    FGEndLevelSensor* endLevel = (FGEndLevelSensor*)LoadGameObject(jCurrentObject, new FGEndLevelSensor());
                                    endLevel->SetFixtureCollisionsFiltering(-1, 0, FGCollisionFilteringCategory_EndLevel, maskBit);
                                }
                                else
                                {
                                    tempObject = LoadGameObject(jCurrentObject);
                                    maskBit = FGCollisionFilteringCategory_Enemy | FGCollisionFilteringCategory_Player;
                                    tempObject->SetFixtureCollisionsFiltering(0, 0, FGCollisionFilteringCategory_EnviromentStatic, maskBit);
                                    tempObject->SetIsCheckingCollisionDepth(true);
                                }
                            }
                        }
                    }
                }
            }

        }

        //Load the joints, after the game objects have created
        LoadGameObjectsJointsScene(ajRoot);

        //Look through all the joints for the ones belonging to the player
        for (unsigned int i = 0; i < m_pSceneJoints.size(); i++)
        {
            //Check if it a revolute joint
            if (m_pSceneJoints.at(i).m_pJointType == e_revoluteJoint)
            {
                //Check if the joint is linked to the player
                if (m_pSceneJoints.at(i).m_pGameObjectA == m_pPlayer)
                {
                    FGPlayerWheel* wheel = dynamic_cast<FGPlayerWheel*>(m_pSceneJoints.at(i).m_pGameObjectB);//Check if the other object is a wheel
                    if (wheel != nullptr)
                    {
                        //Set the wheel to the player
                        m_pPlayer->AddWheel(wheel, (b2RevoluteJoint*)m_pSceneJoints.at(i).m_pJoint);
                    }
                }
                else if (m_pSceneJoints.at(i).m_pGameObjectB == m_pPlayer)
                {
                    FGPlayerWheel* wheel = dynamic_cast<FGPlayerWheel*>(m_pSceneJoints.at(i).m_pGameObjectB);//Check if the other object is a wheel
                    if (wheel != nullptr)
                    {
                        //Set the wheel to the player
                        m_pPlayer->AddWheel(wheel, (b2RevoluteJoint*)m_pSceneJoints.at(i).m_pJoint);
                    }
                }
            }
        }
    }

    //create player emitter
    ParticleEmitter* playerEmitter = nullptr;
    ParticleSystem* particleSystem = new ParticleSystem(this, g_pGame->GetShader("Texture"), g_pGame->GetTexture("ParticleSparks"));
    AddParticleSystem(particleSystem);

    //Set the emitter properties
    ParticleEmitter* tempEmitter = nullptr;
    particleSystem->CreateAddParticleEmitter(2000);
    tempEmitter = particleSystem->GetParticleEmitter(0);
    //tempEmitter->SetUseDirectionRange(true);
    //tempEmitter->SetUseRandomEmitterPosition(false);
    tempEmitter->SetUseSpeedRange(true);
    //tempEmitter->SetFiringAngleRange(vec3(-10, -5, -10), vec3(10, 10, 10));
    tempEmitter->SetSpeedRange(5);
    tempEmitter->SetAllParticles(Vector4(255, 255, 255, 255), Vector3(0.001f, 0.001f, 00.01f), 1, Vector3(0, 9.8f, 0));
  
    tempEmitter->SetParentObject(m_pPlayer);
    m_pPlayer->SetEmitter(tempEmitter);

    LoadHUD();
    LoadLights();

}

void FinalGameScene::LoadHUD()
{
    //Score display
    m_pGameObjects["Score"] = new NumberFontSystem((unsigned int)m_gameScoreTimer, this, "Score", Vector3(-0.075f, 0.06f, 0.2f), Vector3(0, 0, 0), Vector3(0.08f, 0.04f, 0.04f), g_pGame->GetMesh("Box"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("NumberFont"));
    m_pScoreTimer = dynamic_cast<NumberFontSystem*>(m_pGameObjects["Score"]);

    if (m_pScoreTimer != nullptr)
    {
        m_pScoreTimer->LoadContent();
        m_pScoreTimer->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1));
        m_pScoreTimer->SetElementOfHUD(true);
    }

    //Player Health display
    m_pGameObjects["PlayerHealth"] = new NumberFontSystem(100, this, "PlayerHealth", Vector3(0.05f, -0.06f, 0.2f), Vector3(0, 0, 0), Vector3(0.08f, 0.04f, 0.04f), g_pGame->GetMesh("Box"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("NumberFont"));
    m_pPlayerHealth = dynamic_cast<NumberFontSystem*>(m_pGameObjects["PlayerHealth"]);

    if (m_pPlayerHealth != nullptr)
    {
        m_pPlayerHealth->LoadContent();
        m_pPlayerHealth->SetColor(PLAYER_COLOR_HEALTH_HIGH);
        m_pPlayerHealth->SetElementOfHUD(true);
    }
}

void FinalGameScene::LoadLights()
{
    //Lights
    //g_pQuickWatchManager = new QuickWatchManager();
   
    //Lights
    m_pGameObjects["StartLight"] = new LightObject(this, "StartLight", Vector3(25, -8, 2), Vector4(0.55f, 0.27f, 0.57f, 1), Vector3(0.0f, 0.0f, 0.1f),LightType_SpotLight,Vector3(0.0f,266.4f,180.0f));
    LightObject* light = dynamic_cast<LightObject*>(m_pGameObjects["StartLight"]);
    if (light != nullptr)
    {
        light->SetAngleRange(86.40f);
       AddLight(light);

    }
    
  //light->SetDefaultQuickWatch(g_pQuickWatchManager, -50, 50, true, true, true);

    //m_pGameObjects["PlayerLight"] = new LightObject(this, "PlayerLight", Vector3(25, -8, 2), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.2f, 0.2f), LightType_SpotLight, Vector3(0.0f, 266.4f, 180.0f));
    m_pGameObjects["PlayerLight"] = new LightObject(this, "PlayerLight", Vector3(23, -8, 1), Vector4(0.01f, 0.2f, 0.2f, 1.0f), Vector3(0.0f, 0.2f, 0.4f), LightType_SpotLight, Vector3(0.0f, 360.0f, 180.0f));
    m_playerLight = dynamic_cast<LightObject*>(m_pGameObjects["PlayerLight"]);
    if (m_playerLight != nullptr)
    {
        AddLight(m_playerLight);
        m_playerLight->SetRotation(vec3(0.0f, 360.0f, 180.0f));
        m_playerLight->SetAngleRange(32.40f);
    }
    //m_playerLight->SetDefaultQuickWatch(g_pQuickWatchManager, 0, 1300, true, true, true);
    
    m_pGameObjects["PlayerPointLight"] = new LightObject(this, "PlayerPointLight", Vector3(25, -8, 2), Vector4(0.62f, 0.6f, 0.0f, 0.0f), Vector3(0.0f, 0.1f, 0.0f), LightType_PointLight, Vector3(0.0f, 266.4f, 180.0f));
    m_playerPointLight = dynamic_cast<LightObject*>(m_pGameObjects["PlayerPointLight"]);
    if (m_playerPointLight != nullptr)
    {
        m_playerPointLight->SetSpecularColor(Vector4(0.1f, 0.08f, 0.09f, 1.0f));
        AddLight(m_playerPointLight);
    }
    //m_playerPointLight->SetDefaultQuickWatch(g_pQuickWatchManager, 0, 1300, true, true, true);


    //Lights
    m_pGameObjects["EndLight"] = new LightObject(this, "EndLight", Vector3(1300, -8, 2), Vector4(0.55f, 0.27f, 0.57f, 1), Vector3(0.0f, 0.0f, 0.1f), LightType_SpotLight, Vector3(0.0f, 266.4f, 180.0f));
    light = dynamic_cast<LightObject*>(m_pGameObjects["EndLight"]);
    if (light != nullptr)
    {
        light->SetAngleRange(86.40f);
        AddLight(light);

    }
}


//Check game bounds for all objects in the scene
void FinalGameScene::GameBoundCheck()
{
    for each(auto iterator in m_pGameObjects)
    {
        if (iterator.second != nullptr)
        {
            CameraObject* pCamera = dynamic_cast<CameraObject*>(iterator.second);//Dynamic cast it to check if object is a camera
            LightObject* pLight = dynamic_cast<LightObject*>(iterator.second);//Dynamic cast it to check if object is a LIGHT
            

            //If the object is not a camera
            if (pCamera == nullptr && pLight==nullptr)
            {
                if (iterator.second!=nullptr)
                {
                    b2Body* physicsBody = iterator.second->GetPhysicsBody();
                    if (physicsBody != nullptr)
                    {
                        if (physicsBody->GetType() == b2_dynamicBody)
                        {
                            ObjectBoundCheck(iterator.second);// bound check normally
                        }
                    }
                }
            }
        }
    }
}

//Function that prevents the object grom going outside the bounds of the game.
void FinalGameScene::ObjectBoundCheck(GameObject* aObject, Vector3 aOffset)
{
    if (aObject != nullptr)
    {
        //Get object half size    
        b2Body* ObjectBody = aObject->GetPhysicsBody();

        vec3 objectHalfSize = aObject->GetSizes();//Get the full sizes


        //If object body is not null
        if (ObjectBody != nullptr)
        {
            //Use the vertices in the box2d body. This will help account for the actual collision box of the object, and
            //its rotation in the Z axis.
            objectHalfSize.x = abs(aObject->GetMaxFixtureXY().x - aObject->GetMinFixtureXY().x);
            objectHalfSize.y = abs(aObject->GetMaxFixtureXY().y - aObject->GetMinFixtureXY().y);
        }

        objectHalfSize /= 2;//Divided the sizes by 2.

        vec3 orignalPosition = aObject->GetPosition();
        vec3 newPosition = orignalPosition;//Variable to store the new position the object will have if he is out of bounds

        //X check
        //if (orignalPosition.x - objectHalfSize.x < FG_MIN_BOUNDARIES.x)//Left side check
        //{
        //    newPosition.x = GAME_MIN_BOUNDARIES.x + objectHalfSize.x + 0.1f;
        //    aObject->BounceXObjectBody(FG_SCENE_BOUNDS_RESTITUTION);
        //}
        //else if (orignalPosition.x + objectHalfSize.x > FG_MAX_BOUNDARIES.x)//Right Side Check
        //{
        //    newPosition.x = FG_MAX_BOUNDARIES.x - objectHalfSize.x - 0.1f;
        //    aObject->BounceXObjectBody(FG_SCENE_BOUNDS_RESTITUTION);
        //}

        ////Y check
        //if (orignalPosition.y - objectHalfSize.y < FG_MIN_BOUNDARIES.y)//Down side check
        //{
        //    newPosition.y = FG_MIN_BOUNDARIES.y + objectHalfSize.y + 0.1f;
        //    aObject->BounceYObjectBody(FG_SCENE_BOUNDS_RESTITUTION);
        //}
        //else if (orignalPosition.y + objectHalfSize.y > FG_MAX_BOUNDARIES.y)//Up Side Check
        //{
        //    newPosition.y = FG_MAX_BOUNDARIES.y - objectHalfSize.y - 0.1f;
        //    aObject->BounceYObjectBody(FG_SCENE_BOUNDS_RESTITUTION);
        //}

        //Z check
        if (orignalPosition.z - objectHalfSize.z < FG_MIN_BOUNDARIES.z)//Close side check
        {
            newPosition.z = FG_MIN_BOUNDARIES.z + objectHalfSize.z + 0.1f;
        }
        else if (orignalPosition.z + objectHalfSize.z > FG_MAX_BOUNDARIES.z)//Far Side Check
        {
            newPosition.z = FG_MAX_BOUNDARIES.z - objectHalfSize.z - 0.1f;
        }

        //Update the position
        aObject->SetPhysicsAndGameObjectPosition(newPosition + aOffset);
    }
}

void FinalGameScene::EndLevel(FGEndStates aEndState)
{
    m_updateable = false;

    int finalScore;
    int health;

    if (m_pPlayerHealth != nullptr && m_pScoreTimer != nullptr)
    {
        //Calculate the score
        finalScore = m_pScoreTimer->GetNumber();
        health = m_pPlayerHealth->GetNumber();
        health *= FG_GAME_HEALTH_SCORE_MODIFIER;
        finalScore *= FG_GAME_SCORE_MODIFIER;
        finalScore += health;
    }

    //Stop the music
    AudioCue* sound = g_pGame->GetAudioCue("FGMusic");
    if (sound != nullptr)
    {
        sound->StopAudio();
    }

    SceneManager* sceneManager = g_pGame->GetSceneManager();
    if (sceneManager != nullptr)
    {
        switch (aEndState)
        {
        case FGEndStates_Dead:
            sceneManager->PushAndLoadScene(new EndGame(0, FGEndStates_Dead));//Pass a score of 0
            break;
        case FGEndStates_TimeOver:
            sceneManager->PushAndLoadScene(new EndGame(finalScore, FGEndStates_TimeOver));
            break;
        case FGEndStates_Completed:
            sceneManager->PushAndLoadScene(new EndGame(finalScore, FGEndStates_Completed));
            break;
        default:
            break;
        }
    }

}

void FinalGameScene::UpdatePlayerHealthDisplay()
{
    if (m_pPlayer != nullptr)
    {

        //Update the player score
        NumberFontSystem* pPlayerHealthDisplay = dynamic_cast<NumberFontSystem*>(m_pGameObjects["PlayerHealth"]);
        if (pPlayerHealthDisplay != nullptr)
        {
            int playerCurrentHealth = m_pPlayer->GetHealth();

            if (playerCurrentHealth < 0)//Ensure there are no negative values
            {
                playerCurrentHealth = 0;
            }

            pPlayerHealthDisplay->SetNumber(playerCurrentHealth);//Update the display

            //Change the color of text according to game
            if (playerCurrentHealth > FG_PLAYER_HEALTH_MIDDLE_HIGH)//HIgh life
            {
                pPlayerHealthDisplay->SetColor(FG_PLAYER_COLOR_HEALTH_HIGH);
            }
            else if (playerCurrentHealth <= FG_PLAYER_HEALTH_MIDDLE_HIGH && playerCurrentHealth >= FG_PLAYER_HEALTH_MIDDLE_LOW)//Mid life
            {
                pPlayerHealthDisplay->SetColor(FG_PLAYER_COLOR_HEALTH_MIDDLE);
            }
            else//Low life
            {
                pPlayerHealthDisplay->SetColor(FG_PLAYER_COLOR_HEALTH_LOW);
            }

            if (playerCurrentHealth == 0)
            {
                EndLevel(FGEndStates_Dead);
            }

        }
    }
}

void FinalGameScene::PauseGame()
{
    //Customize how the scene will be draw and set so that it doesn't update
    m_updateable = false;
    m_usePostProcessEffects = true;
    m_pLinkedPostProcessingShaders.clear();
    AddPostProcessingShader(g_pGame->GetShader("PostEffectBW"));

    //Push pause menu
    SceneManager* sceneManager = g_pGame->GetSceneManager();
    if (sceneManager != nullptr)
    {
        sceneManager->PushAndLoadScene(new PauseScene());
        g_pGame->PauseAllSound();
    }
}

void FinalGameScene::SetUpdateable(bool aStat)
{
    //If the scene is being set as updateable ensure its no using postProcess
    if (aStat == true)
    {
        m_usePostProcessEffects = false;
        m_pLinkedPostProcessingShaders.clear();
        g_pGame->ResumeAllSound();
    }

    m_updateable = aStat;
}