#ifndef __LoadingSampleScene_H__
#define __LoadingSampleScene_H__

const b2Vec2 LOADING_SAMPLE_SCENE_SCENE_WORLD_GRAVITY = b2Vec2(0, -9.8f);
const Vector3 LOADING_SAMPLE_SCENE_CAMERA_DEFAULT_OFFSET = Vector3(0, 0, -5);


class LoadingSampleScene : public Scene
{
protected:
	int m_score;
    int m_highScore;

public:
    LoadingSampleScene();
    virtual ~LoadingSampleScene();

	virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

	virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
	virtual void LoadResources(b2Vec2 worldGravity);

	void Reset();

	virtual bool HandleInput(InputEvent& inputevent, double delta);
	virtual void Update(double TimePassed);
	virtual void Draw();

};

#endif //__Scene_H__
