#include "CommonHeader.h"
#include "HealthPowerUp.h"


HealthPowerUp::HealthPowerUp(int aHealingAmmount, Scene* apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh* apMesh, ShaderProgram* apShader, GLuint aTexture, vec2 aUVScale, vec2 aUVOffset,
    const char* aCameraName, unsigned int aDrawRenderOrder):
    LivingObject(-aHealingAmmount, 1,apScene,aName,aPos,aRot,aScale,apMesh,apShader,aTexture,aUVScale,aUVOffset,true,aCameraName,aDrawRenderOrder),
    m_pickUpSound(nullptr)
{
    m_destroyOnCollision = true;
    m_maxHealth = 1;

    m_pickUpSound = new Sound("Data/Audio/PowerUp.wav", false);//Sound when pick up is presset

	//Create physics body,Set it to collide with only the player
    CreatePhysicsBox(vec2(GetXLength(), GetYLength()), b2_staticBody, vec2(0, 0), true,CollisionFilteringGroup_PowerUpsAndEnemies,CollisionFilteringCategory_PowerUp,CollisionFilteringCategory_Player);
}

HealthPowerUp::HealthPowerUp(int healingAmount, const char* cameraName, unsigned int drawRenderOrder) :LivingObject(cameraName,drawRenderOrder)
{
    m_collisionDamage = -healingAmount;
    m_maxHealth = 1;
    m_destroyOnCollision = true;
}

HealthPowerUp::~HealthPowerUp()
{
    SAFE_DELETE(m_pickUpSound);
}

void HealthPowerUp::OnDeath()
{
    LivingObject::OnDeath();

    if (m_pickUpSound != nullptr)
    {
        m_pickUpSound->PlayAudio();
    }
}

//Function so that there isn't a collision when the colliding object has full health. This isn't done in presolve, because it is a sensor, so presolve/postsolve don't get called.
void HealthPowerUp::BeginCollision(b2Fixture* aFixtureCollided, b2Fixture* aFixtureCollidedAgainst, GameObject* aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{
    if (aObjectCollidedagainst != nullptr)
    {
        //If the object is a living object
        LivingObject* pLivingObject = dynamic_cast<LivingObject*>(aObjectCollidedagainst);

        if (pLivingObject != nullptr)
        {
            //Check if the object has less than max health
            if (pLivingObject->GetHealth() < pLivingObject->GetMaxHealth())
            {
                LivingObject::BeginCollision(aFixtureCollided, aFixtureCollidedAgainst, aObjectCollidedagainst, aCollisionNormal, aContactPoint, aTouching);//Do the collision as normal
            }
        }
    }
}