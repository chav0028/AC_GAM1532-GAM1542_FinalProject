#ifndef __LightScene_H__
#define __LightScene_H__

const b2Vec2 LIGHT_SCENE_WORLD_GRAVITY = b2Vec2(0, -9.8f);
const Vector3 LIGHT_SCENE_CAMERA_DEFAULT_OFFSET = Vector3(0, 0, -5);//(0, 0, -5);


class LightScene : public Scene
{
protected:
	int m_score;
    int m_highScore;
	ParticleEmitter* m_testEmitter;

public:
    LightScene();
    virtual ~LightScene();

	virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

	virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
	virtual void LoadResources(b2Vec2 worldGravity);
	virtual void LoadGameObjects();

	void Reset();

	virtual bool HandleInput(InputEvent& inputevent, double delta);
	virtual void Update(double TimePassed);
	virtual void Draw();
};

#endif //__Scene_H__
