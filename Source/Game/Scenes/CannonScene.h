#ifndef __CannonScene_H__
#define __CannonScene_H__

const b2Vec2 CANNON_SCENE_WORLD_GRAVITY = b2Vec2(0, -9.8f);
const Vector3 CANNON_SCENE_CAMERA_DEFAULT_OFFSET = Vector3(0, 0, -150);

const vec3 CANNON_SCENE_MIN_BOUNDARIES = vec3(-50, -50, -5);
const vec3 CANNON_SCENE_MAX_BOUNDARIES = vec3(50, 50, 5);

const vec3 CANNON_SCENE_PLANE_SIZE= vec3(200.0f, 0.0f, 1.0f);

const unsigned int CANNON_SCENE_NUMBER_OBSTACLES = 10;//10
const vec3 CANNON_SCENE_MOVING_WALLS_SIZE = vec3(2, 10, 1);
const float CANNON_SCENE_MIN_DISTANCE_BETWEEN_OBSTACLES = 7.0f;
const float CANNON_SCENE_MAX_DISTANCE_BETWEEN_OBSTACLES = 3.0f;
const double CANNON_SCENE_MOVING_WALLS_MIN_TIME = 2.0f;
const double CANNON_SCENE_MOVING_WALLS_MAX_TIME = 7.0f;

enum CannonSceneFilteringGroups
{
	CannonSceneFilteringGroups_CannonAndProjectile=-1,
};


class CannonScene : public Scene
{
protected:
	int m_score;
    int m_highScore;

public:
	CannonScene();
	virtual ~CannonScene();

	virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

	virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
	virtual void LoadResources(b2Vec2 worldGravity);
	virtual void LoadGameObjects();

	void Reset();

	virtual bool HandleInput(InputEvent& inputevent, double delta);
	virtual void Update(double TimePassed);
	virtual void Draw();

	void AddToScore(int amount);

};

#endif //__Scene_H__
