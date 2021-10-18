#ifndef __WaterScene_H__
#define __WaterScene_H__

const unsigned int WATER_SCENE_NUMBER_MINE_OBJECT = 20;

class WaterScene : public Scene
{
public:

protected:

public:
    WaterScene();
    virtual ~WaterScene();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

    virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
	virtual void LoadResources(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
	virtual void LoadGameObjects();

    virtual void Update(double TimePassed);
    virtual void Draw();
};

#endif //__Scene_H__
