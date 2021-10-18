#ifndef __FinalGameScene_H__
#define __FinalGameScene_H__

const int FG_GAME_HEALTH_SCORE_MODIFIER = 1;
const int FG_GAME_SCORE_MODIFIER = 5;

const Vector3 FG_GAME_CAMERA_DEFAULT_OFFSET = Vector3(-30, 10, -12);

const b2Vec2 FINAL_GAME_SCENE_DEFAULT_WORLD_GRAVITY = b2Vec2(0, -9.8f);
const Vector3 FINAL_GAME_SCENE_CAMERA_DEFAULT_OFFSET = Vector3(0, -5, -5);

const float FG_SCENE_BOUNDS_RESTITUTION = 0.0f;
const vec3 FG_MIN_BOUNDARIES = vec3(-40, -30, -4);
const vec3 FG_MAX_BOUNDARIES = vec3(800, 30, 3);

const unsigned int FG_GAME_STARTING_TIME = 130;

const Vector4 FG_PLAYER_COLOR_HEALTH_HIGH = Vector4(0, 240, 0, 255);
const Vector4 FG_PLAYER_COLOR_HEALTH_MIDDLE = Vector4(230, 230, 0, 255);
const Vector4 FG_PLAYER_COLOR_HEALTH_LOW = Vector4(240, 0, 0, 255);

const int FG_PLAYER_HEALTH_MIDDLE_HIGH = 40;
const int FG_PLAYER_HEALTH_MIDDLE_LOW = 21;

//Name of objects that take up the WHOLE Z value, and therefore will be ignored by the player
const char* const FG_SCENE_FLOOR_NAME = "Floor";
const char* const FG_SCENE_ROOF_NAME = "Roof";

enum FGCollisionFilteringCategory
{
    FGCollisionFilteringCategory_EnviromentStatic = 0x0001,
    FGCollisionFilteringCategory_Player = 0x0002,
    FGCollisionFilteringCategory_Enemy = 0x0004,
    FGCollisionFilteringCategory_PickUp = 0x0008,
    FGCollisionFilteringCategory_EndLevel = 0x0010,
    FGCollisionFilteringCategory_EnviromentDynamic = 0x0020
};

enum FGCollisionFilteringGroups
{
    FGCollisionFilteringGroup_PlayerAndWheels = -1,//Not really needed since they are joints
    FGCollisionFilteringGroup_PickUpsAndEnemies = -2

};

enum FGEndStates
{
    FGEndStates_Dead,
    FGEndStates_TimeOver,
    FGEndStates_Completed
};

class FGPlayer;

class FinalGameScene : public Scene
{
protected:
    //Variables for easier access
    CameraObject* m_pGameCamera;
    FGPlayer* m_pPlayer;
    NumberFontSystem* m_pScoreTimer;
    NumberFontSystem* m_pPlayerHealth;
    LightObject* m_playerLight;
    LightObject* m_playerPointLight;
    
    double m_gameScoreTimer;
    int m_finalScore;

public:
	FinalGameScene();
	virtual ~FinalGameScene();

	virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

	virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
	virtual void LoadResources(b2Vec2 worldGravity);
    virtual void LoadGameObjectsScene(cJSON* ajRoot);
    virtual void LoadLights();
    virtual void LoadHUD();

	void Reset();

	virtual bool HandleInput(InputEvent& inputevent, double delta);
	virtual void Update(double TimePassed);
	virtual void Draw();

    virtual void EndLevel(FGEndStates aEndState);

    void UpdatePlayerHealthDisplay();
    void GameBoundCheck();
    void ObjectBoundCheck(GameObject* object, Vector3 offset = Vector3(0, 0, 0));
    void PauseGame();
    virtual void SetUpdateable(bool aStat);
};

#endif //__Scene_H__

