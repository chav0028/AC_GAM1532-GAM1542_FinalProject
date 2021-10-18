#ifndef __MainMenu_H__
#define __MainMenu_H__

const Vector3 MAIN_MENU_SCREEN_CAMERA_DEFAULT_OFFSET = Vector3(0, 0, -5);
const float MAIN_MENU_BUTTONS_FINAL_X = -2.5;
const double MAIN_MENU_BUTTONS_ANIMATION_TIME = 1.0;
const float MAIN_MENU_TITLE_FINAL_Y = 2;
const double MAIN_MENU_TITLE_ANIMATION_TIME = 1.0;

class MainMenu: public Scene
{
protected:
    GameObject* m_levelSphere;

public:
	MainMenu();
    virtual ~MainMenu();

    virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
    virtual void LoadResources(b2Vec2 worldGravity);
    virtual void LoadGameObjects();
    virtual bool HandleInput(InputEvent& inputevent, double delta);
    virtual void Update(double TimePassed);

};

#endif //__Scene_H__