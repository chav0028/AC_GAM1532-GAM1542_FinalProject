#ifndef __ControslScene_H__
#define __ControslScene_H__

const Vector3 CONTROLS_SCENE_SCREEN_CAMERA_DEFAULT_OFFSET = Vector3(0, 0, -5);

class ControlsScene: public Scene
{
protected:

public:
    ControlsScene();
    virtual ~ControlsScene();

    virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
    virtual void LoadResources(b2Vec2 worldGravity);
    virtual void LoadGameObjects();
    virtual bool HandleInput(InputEvent& inputevent, double delta);
    virtual void Update(double TimePassed);

};

#endif //__Scene_H__