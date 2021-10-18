#include "CommonHeader.h"
#include <time.h>

using namespace std;

ThrustersGame::ThrustersGame() :
m_gameState(GameStates_Intro),
m_gameScoreTimer(GAME_STARTING_TIME),
m_gameOver(false),
m_menuScreens(nullptr),
m_inputSound(nullptr),
m_backgroundMusic(nullptr),
m_finalScore(0)
{
    //Generate random number seed
    srand((unsigned int)time(0));//Use time for random
}

ThrustersGame::~ThrustersGame()
{
    SAFE_DELETE(m_menuScreens);

    SAFE_DELETE(m_inputSound);
    SAFE_DELETE(m_backgroundMusic);
}

void ThrustersGame::OnSurfaceChanged(unsigned int aWidth, unsigned int aHeight)
{
    Scene::OnSurfaceChanged(aWidth, aHeight);
}

void ThrustersGame::LoadContent(b2Vec2 aWorldGravity)
{

    // create cameras
    // Game camera
    m_pGameObjects[GAME_DEFAULT_CAMERA_NAME] = new CameraObject(this, GAME_DEFAULT_CAMERA_NAME, GAME_CAMERA_DEFAULT_OFFSET, Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);
    CameraObject*pGameCamera = (CameraObject*)m_pGameObjects[GAME_DEFAULT_CAMERA_NAME];
    pGameCamera->SetCameraOffset(GAME_CAMERA_DEFAULT_OFFSET);
    //AddInputHandler(m_pGameObjects[GAME_DEFAULT_CAMERA_NAME]);//FOR TESTING ONLY

    //HUD camera
    m_pGameObjects[HUD_DEFAULT_CAMERA_NAME] = new CameraObject(this, HUD_DEFAULT_CAMERA_NAME, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0, false);

    LoadResources(GAME_WORLD_GRAVITY);//Load the textures,shaders,etc. This is done after the camera has been created so that we can set the camera view matrix

    //Load sound
    m_inputSound = new Sound("Data/Audio/InputSound.wav", false);//Sound when input pressed
    m_backgroundMusic = new Sound("Data/Audio/Megaman3title.mp3", false);//background music

    LoadGameObjects();//Load the game itself

    LoadHUD();//load HUD elements score, etc.
}

void ThrustersGame::LoadGameObjects()
{
    //Floor
    Vector3 gameCenterPosition;
    gameCenterPosition.x = (GAME_MAX_BOUNDARIES.x + GAME_MIN_BOUNDARIES.x) / 2;
    gameCenterPosition.y = (GAME_MAX_BOUNDARIES.y + GAME_MIN_BOUNDARIES.y) / 2;
    gameCenterPosition.z = (GAME_MAX_BOUNDARIES.z + GAME_MIN_BOUNDARIES.z) / 2;

    signed short floorMaskBit = CollisionFilteringCategory_Enemy |
        CollisionFilteringCategory_Player |
        CollisionFilteringCategory_PlayerProjectile;

    m_pGameObjects["Floor"] = new GameObject(this, "Floor", Vector3(gameCenterPosition.x - 10, GAME_MIN_BOUNDARIES.y + 10, gameCenterPosition.z - 10), Vector3(0, 0, 0), Vector3(1, 1, 1), g_pGame->GetMesh("Plane"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Sand"), vec2(1 / (PLANE_SIZE.x / 100), 1 / (PLANE_SIZE.z / 100)));
    m_pGameObjects["Floor"]->CreatePhysicsBox(vec2(PLANE_SIZE.x, PLANE_SIZE.y), b2_staticBody, vec2(0, 0), false, 0, CollisionFilteringCategory_Enviroment, floorMaskBit);

    //Background
    m_pGameObjects["Background"] = new GameObject(this, "Background", Vector3(gameCenterPosition.x - 10, gameCenterPosition.y, GAME_MAX_BOUNDARIES.z+0.5f), Vector3(0, 0, 0), Vector3(1, 1, 1), g_pGame->GetMesh("VerticalPlane"), g_pGame->GetShader("MovingWater"), g_pGame->GetTexture("UnderWater"), vec2(1.0f, 1.0f));
    m_pGameObjects["Background"]->SetColor(Vector4(0, 0.3f, 0.8f, 1));

    //End level Sensor
    Vector3 endLevelSensorSize = vec3(GAME_MAX_BOUNDARIES.x - GAME_OBJECT_SPAWN_MAX_X, GAME_MAX_BOUNDARIES.y - GAME_MIN_BOUNDARIES.y, GAME_MAX_BOUNDARIES.z - GAME_MIN_BOUNDARIES.z);
    m_pGameObjects["EndLevelSensor"] = new EndLevelSensor(this, "EndLevelSensor", Vector3(GAME_OBJECT_SPAWN_MAX_X + endLevelSensorSize.x / 2, 0, 0), Vector3(0, 0, 0), endLevelSensorSize, g_pGame->GetMesh("Cube"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Gold"));

    //Player
    m_pGameObjects["Player"] = new PlayerObject(PLAYER_DEFAULT_HEALTH, this, "Player", Vector3(-20, 0, 5), Vector3(0, 0, 0), Vector3(1, 1, 1), g_pGame->GetMesh("SubmarineOBJ"), g_pGame->GetShader("Color"), g_pGame->GetTexture("MegamanX"));

    GameObject* tempObject = nullptr;

    //HealthPowerUp
    for (unsigned int i = 0; i < GAME_NUMBER_HEALTH_PICK_UPS; i++)
    {
        //Create the object
        m_pGameObjects["HealthPowerUp" + to_string(i)] = new HealthPowerUp(20, this, "HealthPowerUp" + to_string(i), Vector3(GAME_OBJECT_SPAWN_MIN_X, 0, 0), Vector3(0, 0, 0), Vector3(5, 5, 5), g_pGame->GetMesh("Cube"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("HealthPowerUp"));

        //Save object pointer for easier use
        tempObject = m_pGameObjects["HealthPowerUp" + to_string(i)];

        //Calculate a randomPositon
        vec3 randomPosition = CalculateRandomXYZPosition(tempObject);

        //Set its position
        tempObject->SetPosition(randomPosition);

        //Set its default position for use in reset
        tempObject->SetDefaultTransform(randomPosition, tempObject->GetRotation(), tempObject->GetScale());
    }

    //Revoluting Enemy
    for (unsigned int i = 0; i < GAME_NUMBER_REVOLUTING_ENEMIES; i++)
    {
        //Create the object
        m_pGameObjects["RevolutingEnemy" + to_string(i)] = new RevolutingEnemy(20, 20, 10, this, "RevolutingEnemy" + to_string(i), Vector3(GAME_OBJECT_SPAWN_MIN_X, 0, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), g_pGame->GetMesh("SphereOBJ"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Blocks"),
            g_pGame->GetMesh("RevolutingEnemyArmOBJ"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Blocks"));

        //Save object pointer for easier use
        tempObject = m_pGameObjects["RevolutingEnemy" + to_string(i)];

        //Calculate a randomPositon
        vec3 randomPosition = CalculateRandomXYZPosition(tempObject);

        //Set its position
        tempObject->SetPosition(randomPosition);

        //Set its default position for use in reset
        tempObject->SetDefaultTransform(randomPosition, tempObject->GetRotation(), tempObject->GetScale());
    }

    //Prismatic Enemy
    for (unsigned int i = 0; i < GAME_NUMBER_PRISMATIC_ENEMIES; i++)
    {
        //Create the object
        m_pGameObjects["PrismaticEnemy" + to_string(i)] = new PrismaticEnemy(10, 30, 1, 0.5, 80, this, "PrismaticEnemy" + to_string(i), Vector3(GAME_OBJECT_SPAWN_MIN_X, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1),
            g_pGame->GetMesh("PrismaticEnemyOBJ"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Blocks"), g_pGame->GetMesh("PrismaticEnemyShieldOBJ"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Blocks"));

        //Save object pointer for easier use
        tempObject = m_pGameObjects["PrismaticEnemy" + to_string(i)];

        //Calculate a randomPositon
        vec3 randomPosition = CalculateRandomXYZPosition(tempObject);

        //Set its position
        tempObject->SetPosition(randomPosition);

        //Set its default position for use in reset
        tempObject->SetDefaultTransform(randomPosition, tempObject->GetRotation(), tempObject->GetScale());
    }

}

void ThrustersGame::LoadHUD()
{
    //Screen Tint
    m_pGameObjects["ScreenTint"] = new GameObject(this, "ScreenTint", Vector3(0, 0, 2), Vector3(0, 0, 0), DEFAULT_HUD_PROJECTION_SCALE * 2, g_pGame->GetMesh("Box"), g_pGame->GetShader("SinTextureColor"), g_pGame->GetTexture("Water2"), vec2(2, 2), vec2(0, 0), HUD_DEFAULT_CAMERA_NAME, 3);
    m_pGameObjects["ScreenTint"]->SetColor(Vector4(0, 0, 0.5f, 0.5f));
    m_pGameObjects["ScreenTint"]->SetElementOfHUD(true);

    //Score display
    m_pGameObjects["Score"] = new NumberFontSystem((unsigned int)m_gameScoreTimer, this, "Score", Vector3(3, -4, 1), Vector3(0, 0, 0), Vector3(4, 2, 1), g_pGame->GetMesh("Box"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("NumberFont"));
    NumberFontSystem* pScore = dynamic_cast<NumberFontSystem*>(m_pGameObjects["Score"]);

    if (pScore != nullptr)
    {
        pScore->LoadContent();
        pScore->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1));
        pScore->SetElementOfHUD(true);
    }

    //Player Health display
    m_pGameObjects["PlayerHealth"] = new NumberFontSystem(100, this, "PlayerHealth", Vector3(-4.5, 4, 1), Vector3(0, 0, 0), Vector3(4, 2, 1), g_pGame->GetMesh("Box"), g_pGame->GetShader("TextureColor"), g_pGame->GetTexture("NumberFont"));
    NumberFontSystem* pHealth = dynamic_cast<NumberFontSystem*>(m_pGameObjects["PlayerHealth"]);

    if (pHealth != nullptr)
    {
        pHealth->LoadContent();
        pHealth->SetColor(PLAYER_COLOR_HEALTH_HIGH);
        pHealth->SetElementOfHUD(true);
    }

    //Dont add it to scene, it will be handled manually on a state by state basis
    m_menuScreens = new GameObject(this, "MenuScreens", Vector3(0.0f, 0.0f, 2), Vector3(0, 0, 0), DEFAULT_HUD_PROJECTION_SCALE * 2, g_pGame->GetMesh("Box"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("ControlsScreen"), vec2(1, 1), vec2(0, 0), HUD_DEFAULT_CAMERA_NAME, 3);
    m_menuScreens->SetElementOfHUD(true);
}

void ThrustersGame::Update(double aTimePassed)
{
    int playerHealth = 0;

    switch (m_gameState)
    {
    case GameStates_Intro:
        if (m_menuScreens != nullptr)
        {
            m_pGameObjects[HUD_DEFAULT_CAMERA_NAME]->Update(aTimePassed);
            m_menuScreens->Update(aTimePassed);
        }
        break;

    case GameStates_Game:
    {
        //Update the game objects
        Scene::Update(aTimePassed);
        GameBoundCheck();

        //Move camera after the update
        //Check if player is still alive
        PlayerObject* pPlayer = dynamic_cast<PlayerObject*>(m_pGameObjects["Player"]);//Get player
        CameraObject* pCamera = dynamic_cast<CameraObject*>(m_pGameObjects[GAME_DEFAULT_CAMERA_NAME]);//get the camera

        if (pCamera != nullptr)
        {

            if (pPlayer != nullptr)
            {

                //Update the camera
                pCamera->SetTargetPosition(vec3(pPlayer->GetPosition()));//Set camera target as the player position

                //Calculate the offset of the camera "LookAt"using player velocity
                vec2 playerVelocity = pPlayer->GetPhysicsBodyVelocity();
                pCamera->CalculateLookAtOffset(Vector3(playerVelocity.x, playerVelocity.y, 0), aTimePassed);

                //Make the camera follow the player, this doesn't just snap the position. But it increases it over time until it matches target posoiton
                pCamera->FollowObjectXY(pPlayer, aTimePassed);
                pCamera->Update(aTimePassed);

                playerHealth = pPlayer->GetHealth();

                //If the player died
                if (pPlayer->GetAlive() == false)
                {
                    Reset();
                    m_gameOver = true;
                    m_gameScoreTimer = 0;//Set timer to 0
                }

                UpdatePlayerHealthDisplay();
            }
        }

        NumberFontSystem* pScore = dynamic_cast<NumberFontSystem*>(m_pGameObjects["Score"]);

        //Update score (timer)
        if (pScore != nullptr)
        {
            m_gameScoreTimer -= aTimePassed;

            //If time is over
            if (m_gameScoreTimer <= 0)
            {
                m_gameScoreTimer = 0;//Check for negative values

                m_gameOver = true;
                playerHealth = 0;//Ensure final score is 0
            }

            pScore->SetNumber((unsigned int)m_gameScoreTimer);

        }

        if (m_gameOver == true)
        {
            m_finalScore = (int)m_gameScoreTimer * (playerHealth*GAME_PLAYER_SCORE_MODIFIER);
            SetGameState(GameStates_GameOver);
        }

    }
    break;

    case GameStates_Pause:
        if (m_menuScreens != nullptr)
        {
            m_pGameObjects[HUD_DEFAULT_CAMERA_NAME]->Update(aTimePassed);
            m_menuScreens->Update(aTimePassed);
        }
        break;

    case GameStates_GameOver:
        if (m_menuScreens != nullptr)
        {
            m_pGameObjects[HUD_DEFAULT_CAMERA_NAME]->Update(aTimePassed);
            m_menuScreens->Update(aTimePassed);
        }
        break;

    default:
        break;
    }
}

void ThrustersGame::Draw()
{

    switch (m_gameState)
    {
    case GameStates_Intro:
        if (m_menuScreens != nullptr)
        {
            m_menuScreens->SetTexture(g_pGame->GetTexture("ControlsScreen"));
            m_menuScreens->Draw(m_menuScreens->GetRenderOrder());
        }
        break;

    case GameStates_Pause:
        Scene::Draw();

        if (m_menuScreens != nullptr)
        {
            m_menuScreens->SetTexture(g_pGame->GetTexture("PauseScreen"));
            m_menuScreens->Draw(m_menuScreens->GetRenderOrder());
        }
        break;

    case GameStates_Game:
        Scene::Draw();
        break;

    case GameStates_GameOver:
    {
        GameObject* score = m_pGameObjects["Score"];
        if (score != nullptr)
        {
            score->Draw(score->GetRenderOrder());
        }

        if (m_menuScreens != nullptr)
        {
            if (m_finalScore <= 0)
            {
                m_menuScreens->SetTexture(g_pGame->GetTexture("GameOverFailureScreen"));
            }
            else
            {
                m_menuScreens->SetTexture(g_pGame->GetTexture("GameOverSuccessScreen"));
            }

            m_menuScreens->Draw(m_menuScreens->GetRenderOrder());

            NumberFontSystem* scoreObject = dynamic_cast<NumberFontSystem*>(m_pGameObjects["Score"]);
            if (scoreObject != nullptr)
            {
                scoreObject->SetNumber(m_finalScore);
                scoreObject->SetPosition(vec3(-4, 2, 2));
                scoreObject->SetScale(vec3(1.5f, 1.5f, 1.5f));
                scoreObject->Draw(scoreObject->GetRenderOrder());
            }
        }
    }
    break;
    default:
        break;
    }

}

void ThrustersGame::Reset()
{
    m_gameOver = false;
    m_finalScore = 0;
    SetGameState(GameStates_Game);

    //Reset the score timer
    NumberFontSystem* pScore = dynamic_cast<NumberFontSystem*>(m_pGameObjects["Score"]);

    if (pScore != nullptr)
    {
        m_gameScoreTimer = GAME_STARTING_TIME;
        pScore->SetNumber((unsigned int)m_gameScoreTimer);
    }

    Scene::Reset();

    ////Set the view in the debug draw
    CameraObject* pCamera = dynamic_cast<CameraObject*>(GetGameObject(GAME_DEFAULT_CAMERA_NAME));//get the camera
    if (pCamera != nullptr)
    {
        pCamera->SetTargetPosition(m_pGameObjects["Player"]->GetPosition());
    }
}

void ThrustersGame::UpdatePlayerHealthDisplay()
{
    PlayerObject* pPlayer = dynamic_cast<PlayerObject*>(m_pGameObjects["Player"]);//Get player

    if (pPlayer != nullptr)
    {

        //Update the player score
        NumberFontSystem* pPlayerHealthDisplay = dynamic_cast<NumberFontSystem*>(m_pGameObjects["PlayerHealth"]);
        if (pPlayerHealthDisplay != nullptr)
        {
            int playerCurrentHealth = pPlayer->GetHealth();

            if (playerCurrentHealth < 0)//Ensure there are no negative values
            {
                playerCurrentHealth = 0;
            }

            pPlayerHealthDisplay->SetNumber(playerCurrentHealth);//Update the display

            //Change the color of text according to game
            if (playerCurrentHealth > PLAYER_HEALTH_MIDDLE_HIGH)//HIgh life
            {
                pPlayerHealthDisplay->SetColor(PLAYER_COLOR_HEALTH_HIGH);
            }
            else if (playerCurrentHealth <= PLAYER_HEALTH_MIDDLE_HIGH && playerCurrentHealth >= PLAYER_HEALTH_MIDDLE_LOW)//Mid life
            {
                pPlayerHealthDisplay->SetColor(PLAYER_COLOR_HEALTH_MIDDLE);
            }
            else//Low life
            {
                pPlayerHealthDisplay->SetColor(PLAYER_COLOR_HEALTH_LOW);
            }

        }
    }
}

void ThrustersGame::SetGameState(GameStates aState)
{
    switch (aState)
    {
    case GameStates_Intro:
        break;
    case GameStates_Pause:
        if (m_backgroundMusic != nullptr)
        {
            m_backgroundMusic->PauseAudio(true);
        }
        break;
    case GameStates_Game:
        if (m_backgroundMusic != nullptr)
        {
            m_backgroundMusic->PlayAudio(true);
        }
        break;
    case GameStates_GameOver:
        if (m_backgroundMusic != nullptr)
        {
            m_backgroundMusic->StopAudio();
        }
        break;
    default:
        break;
    }


    m_gameState = aState;

}

bool ThrustersGame::HandleInput(InputEvent& aInputevent, double aDelta)
{
    switch (m_gameState)
    {
    case GameStates_Intro:
        if (aInputevent.type == InputEventType_Key)//Check for keyboard events
        {
            if (aInputevent.state == InputEventState_Up)
            {

                if (aInputevent.keycode == VK_SPACE)
                {
                    if (m_inputSound != nullptr)
                    {
                        m_inputSound->PlayAudio();
                    }

                    SetGameState(GameStates_Game);

                }
            }
        }
        break;

    case GameStates_Pause:
        if (aInputevent.type == InputEventType_Key)//Check for keyboard events
        {
            if (aInputevent.state == InputEventState_Up)
            {
            

                if (aInputevent.keycode == 'P')
                {
                    if (m_inputSound != nullptr)
                    {
                        m_inputSound->PlayAudio();
                    }
                    SetGameState(GameStates_Game);
                }

                if (aInputevent.keycode == 'R')
                {
                    if (m_inputSound != nullptr)
                    {
                        m_inputSound->PlayAudio();
                    }
                    Reset();
                }
            }
        }
        break;

    case GameStates_Game:  //Only do objects input if it game state
        if (aInputevent.state == InputEventState_Up)
        {

            if (aInputevent.keycode == 'P')
            {
                if (m_inputSound != nullptr)
                {
                    m_inputSound->PlayAudio();
                }

                SetGameState(GameStates_Pause);
            }

        }

        Scene::HandleInput(aInputevent, aDelta);
        break;

    case GameStates_GameOver:
        if (aInputevent.type == InputEventType_Key)//Check for keyboard events
        {
            if (aInputevent.state == InputEventState_Up)
            {
               
                if (aInputevent.keycode == 'R')
                {
                    if (m_inputSound != nullptr)
                    {
                        m_inputSound->PlayAudio();
                    }

                    Reset();
                }
            }
        }

        break;
    default:
        break;
    }
    
    return false;
}

//Check game bounds for all objects in the scene
void ThrustersGame::GameBoundCheck()
{
    for each(auto iterator in m_pGameObjects)
    {
        if (iterator.second != nullptr)
        {
            CameraObject* pCamera = dynamic_cast<CameraObject*>(iterator.second);//Dynamic cast it to check if object is a camera

            //If the object is not a camera
            if (pCamera == nullptr)
            {
                if (iterator.first != "Floor" && iterator.first != "Background" && iterator.second->GetCameraDrawerName() == GAME_DEFAULT_CAMERA_NAME)
                {
                    ObjectBoundCheck(iterator.second);// bound check normally
                }
            }
            else if (pCamera != nullptr)//If the object is a camera
            {
                //Don't bound check camera, at least with the same game boundaries

                //if (pCamera->GetName() == GAME_DEFAULT_CAMERA_NAME)//Only affect the game camera
                //{
                //    //ObjectBoundCheck(iterator.second, pCamera->GetCameraOffset());//Bound check, but consider its offset.
                //}
            }
        }
    }
}

//Function that prevents the object grom going outside the bounds of the game.
void ThrustersGame::ObjectBoundCheck(GameObject* aObject, Vector3 aOffset)
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
        if (orignalPosition.x - objectHalfSize.x < GAME_MIN_BOUNDARIES.x)//Left side check
        {
            newPosition.x = GAME_MIN_BOUNDARIES.x + objectHalfSize.x + 0.1f;
            aObject->BounceXObjectBody(SCENE_BOUNDS_RESTITUTION);
        }
        else if (orignalPosition.x + objectHalfSize.x > GAME_MAX_BOUNDARIES.x)//Right Side Check
        {
            newPosition.x = GAME_MAX_BOUNDARIES.x - objectHalfSize.x - 0.1f;
            aObject->BounceXObjectBody(SCENE_BOUNDS_RESTITUTION);
        }

        //Y check
        if (orignalPosition.y - objectHalfSize.y < GAME_MIN_BOUNDARIES.y)//Down side check
        {
            newPosition.y = GAME_MIN_BOUNDARIES.y + objectHalfSize.y + 0.1f;
            aObject->BounceYObjectBody(SCENE_BOUNDS_RESTITUTION);
        }
        else if (orignalPosition.y + objectHalfSize.y > GAME_MAX_BOUNDARIES.y)//Up Side Check
        {
            newPosition.y = GAME_MAX_BOUNDARIES.y - objectHalfSize.y - 0.1f;
            aObject->BounceYObjectBody(SCENE_BOUNDS_RESTITUTION);
        }

        //Z check
        if (orignalPosition.z - objectHalfSize.z < GAME_MIN_BOUNDARIES.z)//Close side check
        {
            newPosition.z = GAME_MIN_BOUNDARIES.z + objectHalfSize.z + 0.1f;
        }
        else if (orignalPosition.z + objectHalfSize.z > GAME_MAX_BOUNDARIES.z)//Far Side Check
        {
            newPosition.z = GAME_MAX_BOUNDARIES.z - objectHalfSize.z - 0.1f;
        }

        //Update the position
        aObject->SetPhysicsAndGameObjectPosition(newPosition + aOffset);
    }
}

//Function only to calculate a random xyz position withing game boundaries,
//it takes into account the object size. It doesn't actually place the object.
Vector3 ThrustersGame::CalculateRandomXYZPosition(GameObject* aObject)
{
    vec3 randomPosition = vec3(0, 0, 0);

    if (aObject != nullptr)
    {
        vec3 objectHalfSizes = aObject->GetSizes() / 2;

        //randomNumber=min + ( std::rand() % ( max - min + 1 ) )
        //Calculate the random position of each object

        //X axis
        randomPosition.x = (GAME_OBJECT_SPAWN_MIN_X + objectHalfSizes.x) + (rand() % (int)(GAME_OBJECT_SPAWN_MAX_X + objectHalfSizes.x) - (GAME_OBJECT_SPAWN_MIN_X + objectHalfSizes.x) + 1);

        //Y axis
        randomPosition.y = (GAME_MIN_BOUNDARIES.y + objectHalfSizes.y) + (rand() % (int)(GAME_MAX_BOUNDARIES.y + objectHalfSizes.y) - (GAME_MIN_BOUNDARIES.y + objectHalfSizes.y) + 1);

        //Z axis
        randomPosition.z = (GAME_MIN_BOUNDARIES.z + objectHalfSizes.z) + (rand() % (int)(GAME_MAX_BOUNDARIES.z + objectHalfSizes.z) - (GAME_MIN_BOUNDARIES.z + objectHalfSizes.z) + 1);
    }


    return randomPosition;
}


void ThrustersGame::LoadResources(b2Vec2 aWorldGravity)
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
    }

    // Create meshes.
    {
        g_pGame->AddMesh("Box", Mesh::CreateBox(vec2(1, 1)));

        g_pGame->AddMesh("Circle", Mesh::CreateCircle(0.5f));
        g_pGame->AddMesh("Cube", Mesh::CreateCube(vec3(1, 1, 1)));
        g_pGame->AddMesh("OffsetCube", Mesh::CreateCube(vec3(1, 1, 1), vec3(0, 0, 1.5)));
        g_pGame->AddMesh("Plane", Mesh::CreatePlane(vec2(PLANE_SIZE.x, PLANE_SIZE.z), Vector2Int(PLANE_NUMBER_COLUMNS_ROWS.x, PLANE_NUMBER_COLUMNS_ROWS.y)));
        g_pGame->AddMesh("VerticalPlane", Mesh::CreateVerticalPlane(vec2(BACKGROUND_SIZE.x, BACKGROUND_SIZE.y), Vector2Int(BACKGROUND_NUMBER_COLUMNS_ROWS.x, BACKGROUND_NUMBER_COLUMNS_ROWS.y)));

        g_pGame->AddMesh("SphereOBJ", Mesh::LoadObj("OBJs/Sphere.obj", vec3(1, 1, 1)));
        g_pGame->AddMesh("CubeOBJ", Mesh::LoadObj("OBJs/Cube.obj", vec3(1, 1, 1)));
        g_pGame->AddMesh("SubmarineOBJ", Mesh::LoadObj("OBJs/Submarine.obj"));
        g_pGame->AddMesh("TorpedoOBJ", Mesh::LoadObj("OBJs/Torpedo.obj", OBJ_MESH_DEFAULT_SCALE * 2));
        g_pGame->AddMesh("RevolutingEnemyArmOBJ", Mesh::LoadObj("OBJs/RevolutingArm.obj", OBJ_MESH_DEFAULT_SCALE * 5));
        g_pGame->AddMesh("PrismaticEnemyOBJ", Mesh::LoadObj("OBJs/PrismaticEnemy.obj", OBJ_MESH_DEFAULT_SCALE * 4));
        g_pGame->AddMesh("PrismaticEnemyShieldOBJ", Mesh::LoadObj("OBJs/PrismaticEnemyShield.obj", OBJ_MESH_DEFAULT_SCALE * 4));
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
        g_pGame->AddTexture("NumberFont", LoadTexture("Data/Textures/NumberFont.png"));
        g_pGame->AddTexture("Water", LoadTexture("Data/Textures/Water.png"));
        g_pGame->AddTexture("Water2", LoadTexture("Data/Textures/WaterTexture2.png"));
        g_pGame->AddTexture("UnderWater", LoadTexture("Data/Textures/UnderWaterBackground.png"));
        g_pGame->AddTexture("Sand", LoadTexture("Data/Textures/Sand.png"));
        g_pGame->AddTexture("HealthPowerUp", LoadTexture("Data/Textures/HealthPowerUp.png"));
        g_pGame->AddTexture("Gold", LoadTexture("Data/Textures/GoldTexture.png"));

        g_pGame->AddTexture("ControlsScreen", LoadTexture("Data/Textures/ControlsScreen.png"));
        g_pGame->AddTexture("PauseScreen", LoadTexture("Data/Textures/PauseScreens.png"));
        g_pGame->AddTexture("GameOverFailureScreen", LoadTexture("Data/Textures/GameOverFailureScreen.png."));
        g_pGame->AddTexture("GameOverSuccessScreen", LoadTexture("Data/Textures/GameOverScreenSuccess.png"));
    }

    ////Set the view in the debug draw
    CameraObject* pCamera = dynamic_cast<CameraObject*>(GetGameObject(GAME_DEFAULT_CAMERA_NAME));//get the camera

    if (m_pSceneWorld != nullptr)
    {

        if (pCamera != nullptr)
        {
            m_pSceneWorld->SetDebugDrawViewMatrix(pCamera->GetViewMatrix());//Pass the debug draw the view matrix of the camera
        }

        m_pSceneWorld->SetDebugDrawShader(g_pGame->GetShader("Color"));//Set which sahder the debug draw will use
    }
}