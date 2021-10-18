#ifndef __ThrustersGame_H__
#define __ThrustersGame_H__
																								 
const vec3 GAME_MIN_BOUNDARIES = vec3(-40, -30, -5);
const vec3 GAME_MAX_BOUNDARIES= vec3(800, 30, 5);
const float GAME_OBJECT_SPAWN_MIN_X = GAME_MIN_BOUNDARIES.x+40;
const float GAME_OBJECT_SPAWN_MAX_X = GAME_MAX_BOUNDARIES.x-20;
const float SCENE_BOUNDS_RESTITUTION = 0.1f;

const vec3 PLANE_SIZE = vec3(980.0f, 0.0f, 60.0f);
const Vector2Int PLANE_NUMBER_COLUMNS_ROWS = Vector2Int(2, 2);

const vec3 BACKGROUND_SIZE = vec3(980, 120, 0.5);
const Vector2Int BACKGROUND_NUMBER_COLUMNS_ROWS = Vector2Int(1600,800);

const unsigned int GAME_STARTING_TIME = 70;

const Vector3 GAME_CAMERA_DEFAULT_OFFSET = Vector3(-30, 15, -12);
const b2Vec2 GAME_WORLD_GRAVITY = b2Vec2(-1.5f, -2.0f);

const Vector4 PLAYER_COLOR_HEALTH_HIGH = Vector4(0, 255, 0, 255);
const Vector4 PLAYER_COLOR_HEALTH_MIDDLE = Vector4(230, 230, 0, 255);
const Vector4 PLAYER_COLOR_HEALTH_LOW = Vector4(255, 0, 0, 255);

const int PLAYER_HEALTH_MIDDLE_HIGH = 69;
const int PLAYER_HEALTH_MIDDLE_LOW = 31;

const unsigned int GAME_NUMBER_HEALTH_PICK_UPS = 5;
const unsigned int GAME_NUMBER_REVOLUTING_ENEMIES = 20;
const unsigned int GAME_NUMBER_PRISMATIC_ENEMIES = 30;

const unsigned int GAME_PLAYER_SCORE_MODIFIER = 2;

enum CollisionFilteringCategory
{
	CollisionFilteringCategory_Enviroment = 0x0001,
	CollisionFilteringCategory_Player = 0x0002,
	CollisionFilteringCategory_Enemy = 0x0004,
	CollisionFilteringCategory_PlayerProjectile = 0x0008,
	CollisionFilteringCategory_PowerUp = 0x0010,
	CollisionFilteringCategory_EndLevel=0x0020

};

enum CollisionFilteringGroups
{
    CollisionFilteringGroup_PlayerAndProjectiles=-1,
    CollisionFilteringGroup_PowerUpsAndEnemies=-2

};

enum GameStates
{
    GameStates_Intro,
    GameStates_Pause,
    GameStates_Game,
    GameStates_GameOver
};

class ThrustersGame : public Scene
{
protected:
	GameStates m_gameState;
	double m_gameScoreTimer;
    int m_finalScore;

	bool m_gameOver;//Have a bool, since when the player reaches the goal, we can't reset it directly in a box2d world step
    GameObject* m_menuScreens;

    Sound* m_inputSound;
    Sound* m_backgroundMusic;

public:
    ThrustersGame();
    virtual ~ThrustersGame();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

    virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f , -9.8f));
    virtual void LoadResources(b2Vec2 worldGravity);
    virtual void LoadHUD();
    virtual void LoadGameObjects();

	void Reset();

	virtual bool HandleInput(InputEvent& inputevent, double delta);
    virtual void Update(double TimePassed);
    virtual void Draw();

    void UpdatePlayerHealthDisplay();
    void SetGameState(GameStates state);

    void GameBoundCheck();
    void ObjectBoundCheck(GameObject* object, Vector3 offset=Vector3(0,0,0));

	void SetGameOver(bool aStatus) { m_gameOver = aStatus; }

    Vector3 CalculateRandomXYZPosition(GameObject* aObject);
};

#endif //__Scene_H__
