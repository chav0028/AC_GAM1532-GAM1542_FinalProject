#ifndef __PauseScene_H__
#define __PauseScene_H__

const Vector3 PAUSE_SCREEN_CAMERA_DEFAULT_OFFSET = Vector3(0, 0, -5);
const float PAUSE_SCREEN_BUTTONS_FINAL_Y = -4.0f;
const double PAUSE_SCREEN_BUTTONS_ANIMATION_TIME = 0.5;

class BackButton;

class PauseScene: public Scene
{
protected:
    BackButton* m_resumeButton;

public:
    PauseScene();
    virtual ~PauseScene();

    virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
    virtual void LoadResources(b2Vec2 worldGravity);
    virtual void LoadGameObjects();
    virtual bool HandleInput(InputEvent& inputevent, double delta);
    virtual void Update(double TimePassed);

};

#endif //__Scene_H__