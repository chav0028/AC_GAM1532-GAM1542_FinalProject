#ifndef __FGRevolutingEnemy__H__
#define __FGRevolutingEnemy__H__

const unsigned int FG_REVOLUTING_ENEMY_DEFAULT_NUMBER_ARMS =4;
const float FG_REVOLUTING_ENEMY_ARMS_DEFAULT_SPEED = -100;
const float FG_REVOLUTING_ENEMY_ARMS_MAX_SPEED = -200;
const float FG_REVOLUTING_ENEMY_ARMS_SPEED_INCREASE = -50;

#include "../RevolutingEnemy.h"

class FGRevolutingEnemy :
    public RevolutingEnemy
{
protected:

public:
    FGRevolutingEnemy(unsigned int numArms,int collisionDamage, int health, int armHealth, Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture,
        Mesh* armMesh, ShaderProgram* apArmShader, GLuint armTexture,vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0),const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
    virtual ~FGRevolutingEnemy();

	virtual void Update(double delta);
    virtual void Draw(int renderorder);

	virtual void Reset();
    virtual void OnDeath();
};

#endif