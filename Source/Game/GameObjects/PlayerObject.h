#ifndef __PlayerObject_H__
#define __PlayerObject_H__

const float PLAYER_DEFAULT_FORWARD_SPEED = 20.0f;
const float PLAYER_DEFAULT_BACKWARD_SPEED = -10.0f;
const float PLAYER_DEFAULT_UP_SPEED = 30.0f;

const int PLAYER_DEFAULT_HEALTH = 100;
const unsigned int PLAYER_DEFAULT_NUMBER_PROJECTILES = 20;
const int PLAYER_COLLISION_DAMAGE = 5;

class Projectile;

class PlayerObject : public LivingObject
{
protected:
    Pool<Projectile> m_poolProjectiles;

    Sound* m_playerHurtSound;
    Sound* m_playerFireSound;

public:
    PlayerObject(int maxHealth,Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture);
    virtual ~PlayerObject();

    virtual void Update(double TimePassed);
    bool HandleInput(InputEvent& aInputevent, double aDelta);

    virtual void ApplyDamage(int damage);
    void FireProjectile();

    void Reset();
};

#endif //__PlayerObject_H__
