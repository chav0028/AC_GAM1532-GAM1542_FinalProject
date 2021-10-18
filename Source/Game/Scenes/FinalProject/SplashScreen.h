#ifndef __SplashScreen_H__
#define __SplashScreen_H__

const Vector3 SPLASH_SCREEN_CAMERA_DEFAULT_OFFSET = Vector3(0, 0, -5);

//All the resources will be loaded in this scene, because no one really cares about a splash screen
class SplashScreen : public Scene
{
protected:
    GameObject* m_gameTitle;//Saved for easier access
    bool m_gameTitleInPosition;
    bool m_mainMenuLoaded;

public:
    SplashScreen();
    virtual ~SplashScreen();

    virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
    virtual void LoadResources(b2Vec2 worldGravity);
    virtual void LoadGameObjects();
    virtual bool HandleInput(InputEvent& inputevent, double delta);
    virtual void Update(double TimePassed);


    void GoToMainMenu();
};

#endif //__Scene_H__