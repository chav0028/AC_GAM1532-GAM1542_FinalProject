#ifndef __LivingObject__H__
#define __LivingObject__H__

#include <string>

/*Child class of gameobject used to handle the concept of hitpoints easily*/
class LivingObject : public GameObject
{
public:
	LivingObject(int maxHealth,Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0),
		const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);

    LivingObject(int collisionDamage,int maxHealth, Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0), bool immortality=false,
        const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);

    LivingObject(const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
	~LivingObject();

    virtual void Init(int maxHealth, Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0));

	virtual void Update(double delta);
    virtual void Reset();

    virtual void SpawnObject(vec3 location);//Function to enable all the stats of an object, and set him at full health.

	virtual void ApplyDamage(int damage);
	virtual void OnDeath();

    virtual void ReplenishFullHealth();
    virtual void AddHealth(int health);
    virtual void SetHealth(int health);

	unsigned int GetMaxHealth() { return m_maxHealth; }
	unsigned int GetHealth() { return m_health; }

    bool GetAlive() { return m_alive; }

    void SetImmortal(bool aImmortality){ m_immortal = aImmortality; }
    bool GetImmortal(){ return m_immortal; }

    virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
    virtual void EndCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching){ ; }

	int GetCollisionDamage() { return m_collisionDamage; }
	void SetCollisionDamage(int aDamage) { m_collisionDamage = aDamage; }

    void SetDestroyOnCollsiionWithNoLivingObjects(bool aStatus){ m_destroyOnCollisionWithNotLivingObjects = aStatus; }

protected:
	bool m_alive;
    bool m_destroyOnCollisionWithNotLivingObjects;
    bool m_destroyOnCollision;
	int m_collisionDamage;
	int m_health;//Int instead of unsigned int, since when substracting damage the health may become negative
	int m_maxHealth;

private:
    bool m_immortal;//Used when we want an object to only apply damage, but not be damaged. Note: That immortal only blocks the object from receing damage through apply damage, but its health can still
    //be set to 0 and therefore "killed"
};

#endif