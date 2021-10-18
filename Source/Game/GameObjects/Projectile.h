#ifndef __Projectile__H__
#define __Projectile__H__

#include "LivingObject.h"

const int PROJECTILE_DAMAGE = 10;
const float PROJECTILE_SPEED = 2.0f;
const double PROJECTILE_DEFAULT_LIVE_TIME = 1.0f;

class Projectile :
    public LivingObject
{
protected:
    Pool<Projectile>* m_pPoolOwner;//the pool in which the enemy is
    double m_projectileLiveTimer;
    bool m_pendingPoolRemoval;

    Sound* m_projectileExplodeSound;

public:
    Projectile(Pool<Projectile>* aPool, const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
    ~Projectile();
    
	virtual void Init(int maxHealth, Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0));
    void Update(double aDelta);

    virtual void OnDeath();
    void Reset();

	void LaunchProjectile(vec3 launchPosition, float XYAngle, vec2 launchingVelocity = vec2(0, 0));
    void DisableProjectileFromPool();
};

#endif