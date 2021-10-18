#ifndef __FGPrismaticEnemy__H__
#define __FGPrismaticEnemy__H__

#include "../PrismaticEnemy.h"

const float FG_PRISMATIC_ENEMY_DEFAULT_SHIELD_SPEED = 50.0f;
const float FG_PRISMATIC_ENEMY_DEFAULT_MAX_SHIELD_SPEED = 150.0f;
const float FG_PRISMATIC_ENEMY_MAX_SHIELD_LIMIT = 3;
const float FG_PRISMATIC_ENEMY_MIN_SHIELD_LIMIT = 0;
const Vector2 FG_PRISMATIC_ENEMY_DEFAULT_VELOCITY = Vector2(-5, 0);

class FGPrismaticEnemy :
	public PrismaticEnemy
{
protected:

public:
    FGPrismaticEnemy(int collisionDamage, int health, double shieldTimer, double unshieldTimer, int shieldHealth, Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture,
        Mesh* pShieldMesh, ShaderProgram* pshieldShader, GLuint shieldTexture,vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0), const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
	virtual ~FGPrismaticEnemy();

    virtual void Update(double delta);
    virtual void Draw(int renderorder);

    virtual void Reset();
    virtual void OnDeath();
};

#endif
