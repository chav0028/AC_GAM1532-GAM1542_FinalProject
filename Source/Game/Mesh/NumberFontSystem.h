#ifndef __NumberFontSystem__H__
#define __NumberFontSystem__H__

#include "Game/Base/GameObject.h"

const unsigned int NUMBER_FONT_SYSTEM_MAX_NUMBER = 999999;//The max number that can be displayed
const unsigned int NUMBER_FONT_SYSTEM_MAX_DIGITS = 6;
const unsigned int NUMBERS_DISPLAYED_IN_TEXTURE_STRIP = 10;

//Constants used for checking numbers, made into constants to avoid possible mispelling
const unsigned int HUNDRED_THOUSAND = 100000;
const unsigned int TEN_THOUSAND = 10000;
const unsigned int THOUSAND = 1000;
const unsigned int HUNDRED = 100;
const unsigned int TEN = 10;

/*Display the number being passed, using 10 digit boxes.*/
class NumberFontSystem :
    public GameObject
{
public:
    NumberFontSystem(unsigned int aNumber, Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, 
        const char* cameraName = HUD_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 4);
    ~NumberFontSystem();

    void SetNumber(unsigned int number);
    unsigned int GetNumber(){ return m_number; }
    void LoadContent();
    virtual void Draw(int renderOrder);
    virtual void Reset();

    //Setters overwritten so that the value of all the boxes are modified in a single call, getters will return the position of the system, which should
    //match the one of all the boxes.
    virtual void SetEnabled(bool enabledStatus);
    virtual void SetPosition(vec3 position);    //ANCHOR POINT is in the middle left

    virtual void SetScale(vec3 scale);
    virtual void SetRotation(vec3 rotation);
    virtual void SetColor(Vector4 color);

private:
    unsigned int m_number;
    unsigned int m_numberOfDigits;
    
    void ChangeNumberDisplay(unsigned int aNumber,GameObject* digitBox);


    std::vector<GameObject*> m_DigitBoxes;
};

#endif