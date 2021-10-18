#ifndef __RevolutingEnemy__H__
#define __RevolutingEnemy__H__

const unsigned int REVOLUTING_ENEMY_DEFAULT_NUMBER_ARMS =4;
const float REVOLUTING_ENEMY_ARMS_DEFAULT_SPEED = -100;
const float REVOLUTING_ENEMY_ARMS_MAX_SPEED = -200;
const float REVOLUTING_ENEMY_ARMS_SPEED_INCREASE = -50;

#include "LivingObject.h"

class RevolutingEnemy :
    public LivingObject
{
private:
	void CreateArms(int NumberArms, int armHealth, Mesh* armMesh, ShaderProgram* apArmShader, GLuint armTexture);
    void CreateBody();

protected:
	std::vector<LivingObject*> m_arms;
	std::vector<b2RevoluteJoint*> m_armsJoints;
	float m_armPositionRadius;
   
    float m_armsSpeedIncrease;
    float m_defaultArmsMaxSpeed;
    float m_defaultArmsSpeed;
	
    void SetDefaultConditions(float armsSpeedIncrease = REVOLUTING_ENEMY_ARMS_SPEED_INCREASE, float defaultArmsMaxSpeed = REVOLUTING_ENEMY_ARMS_MAX_SPEED, float defaultArmsSpeed = REVOLUTING_ENEMY_ARMS_DEFAULT_SPEED);
public:
	RevolutingEnemy(int collisionDamage,int health,int armHealth, Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, 
        Mesh* armMesh, ShaderProgram* apArmShader, GLuint armTexture,vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0),const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
    RevolutingEnemy(unsigned int numArms,int collisionDamage, int health, int armHealth, Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture,
        Mesh* armMesh, ShaderProgram* apArmShader, GLuint armTexture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0), const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);

    virtual ~RevolutingEnemy();

	virtual void Update(double delta);
    virtual void Draw(int renderorder);

	virtual void Reset();
    virtual void OnDeath();

    void SetArmColors(Vector4 color);


	vec2 GetMinFixtureXY(int aFixtureIndex = -1);
	vec2 GetMaxFixtureXY(int aFixtureIndex = -1);
	void BounceXObjectBody(float restitutionCollidingObject = 0.5f, float frictionCollidingObject = 0.5f);
	void BounceYObjectBody(float restitutionCollidingObject = 0.5f, float frictionCollidingObject = 0.5f);
	void SetPhysicsAndGameObjectPosition(vec3 aPosition);
	Vector3 GetSizes();
    void SetArmsCollisionFiltering(int fixtureIndex = 0, signed short fixtureGroup = 0, unsigned short fixtureCategory = 1, unsigned short fixtureMasks = 1);
};

#endif