#ifndef __PrismaticEnemy__H__
#define __PrismaticEnemy__H__

#include "LivingObject.h"

const float PRISMATIC_ENEMY_DEFAULT_SHIELD_SPEED = 50.0f;
const float PRISMATIC_ENEMY_DEFAULT_MAX_SHIELD_SPEED = 150.0f;
const float PRISMATIC_ENEMY_MAX_SHIELD_LIMIT = 3;
const float PRISMATIC_ENEMY_MIN_SHIELD_LIMIT = 0;
const Vector2 PRISMATIC_ENEMY_DEFAULT_VELOCITY = Vector2(-5, 0);

class PrismaticEnemy :
	public LivingObject
{
private:
    

protected:
    double m_defaultUnshieldTimer;
    double m_unshieldTimer;
    double m_defaultShieldTimer;
    double m_shieldTimer;
    bool m_shielded;

    LivingObject* m_pShield;
    b2PrismaticJoint* m_pJoint;
    float m_shieldSpeed;
	Vector2 m_enemyVelocity;

   Vector2 m_defaultEnemyVelocity;
   float m_defaultShieldSpeed;
   float m_defaultShieldMaxLimit;
   float m_defaultShieldMinLimit;
   float m_defaultShieldMaxSpeed;

   virtual void SetDefaultConditions(Vector2 enemyVelocity = PRISMATIC_ENEMY_DEFAULT_VELOCITY, float shieldSpeed = PRISMATIC_ENEMY_DEFAULT_SHIELD_SPEED, float shieldMaxLimit = PRISMATIC_ENEMY_MAX_SHIELD_LIMIT, float shieldMinLimit = PRISMATIC_ENEMY_MIN_SHIELD_LIMIT, float shieldMaxSpeed = PRISMATIC_ENEMY_DEFAULT_MAX_SHIELD_SPEED);

public:
    PrismaticEnemy(int collisionDamage, int health, double shieldTimer, double unshieldTimer, int shieldHealth, Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture,
        Mesh* pShieldMesh, ShaderProgram* pshieldShader, GLuint shieldTexture,vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0), const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
	virtual ~PrismaticEnemy();

    void CreateShield(int shieldHealth, Mesh* pShieldMesh, ShaderProgram* pshieldShader, GLuint shieldTexture);

    virtual void Update(double delta);
    virtual void Draw(int renderorder);

    virtual void Reset();
    virtual void OnDeath();

	vec2 GetMinFixtureXY(int aFixtureIndex=-1);
	vec2 GetMaxFixtureXY(int aFixtureIndex=-1);
	void BounceXObjectBody(float restitutionCollidingObject = 0.5f, float frictionCollidingObject = 0.5f);
	void BounceYObjectBody(float restitutionCollidingObject = 0.5f, float frictionCollidingObject = 0.5f);
	void SetPhysicsAndGameObjectPosition(vec3 aPosition);
	Vector3 GetSizes();
    void SetShieldCollisionFiltering(int fixtureIndex = 0, signed short fixtureGroup = 0, unsigned short fixtureCategory = 1, unsigned short fixtureMasks = 1);
};

#endif
