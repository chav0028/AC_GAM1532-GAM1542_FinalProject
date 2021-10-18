#include "CommonHeader.h"

using namespace std;

LivingObject::LivingObject(int aMaxHealth, Scene * apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh * apMesh, ShaderProgram * apShader, GLuint aTexture, vec2 aUVScale, vec2 aUVOffset, const char * aCameraName, unsigned int aDrawRenderOrder) :
GameObject(aCameraName, aDrawRenderOrder),
m_alive(true),
m_collisionDamage(0),
m_immortal(false),
m_destroyOnCollision(false),
m_destroyOnCollisionWithNotLivingObjects(false)
{
    Init(aMaxHealth, apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aUVScale, aUVOffset);
}

LivingObject::LivingObject(int aCollisionDamage, int aMaxHealth, Scene * apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh * apMesh, ShaderProgram * apShader, GLuint aTexture, vec2 aUVScale, vec2 aUVOffset, bool aImmortality, const char * aCameraName, unsigned int aDrawRenderOrder) :
GameObject(aCameraName, aDrawRenderOrder),
m_alive(true),
m_collisionDamage(aCollisionDamage),
m_immortal(aImmortality),
m_destroyOnCollisionWithNotLivingObjects(false)
{
    Init(aMaxHealth, apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aUVScale, aUVOffset);
}

LivingObject::LivingObject(const char* aCameraName, unsigned int aDrawRenderOrder) :
GameObject(aCameraName, aDrawRenderOrder),
m_maxHealth(100),
m_health(100),
m_alive(true),
m_collisionDamage(0),
m_immortal(false),
m_destroyOnCollisionWithNotLivingObjects(false)
{

}

void LivingObject::Init(int aMaxHealth, Scene * apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh * apMesh, ShaderProgram * apShader, GLuint aTexture, vec2 aUVScale, vec2 aUVOffset)
{
    GameObject::Init(apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aUVScale, aUVOffset);
    m_maxHealth = aMaxHealth;
    m_health = aMaxHealth;
}

LivingObject::~LivingObject()
{
}

void LivingObject::Update(double aDelta)
{
    SetEnabled(m_alive);
    
    //if (m_alive == false)
    //{
    //    SetEnabled(false);
    //   /* if (m_pPhysicsBody != nullptr)
    //    {
    //        m_pPhysicsBody->SetActive(false);
    //    }*/
    //}

        GameObject::Update(aDelta);
}

void LivingObject::ApplyDamage(int aDamage)
{

    if (aDamage > 0)//If there is damage being applied
    {
        if (m_immortal == false)//If object can be damaged
        {
            m_health -= aDamage;
        }

    }
    else if (aDamage < 0)//If the damage is negative, heal the object
    {
        AddHealth(abs(aDamage));//Heal the object by the negative amount, converted to positive
    }

    if (m_health <= 0 && m_immortal==false)//If the health is 0 or less, AND the object is not immortal
    {
        OnDeath();//Set that the user died
    }
}

//Function to enable all the stats of an object, and set him at full health.
void LivingObject::SpawnObject(vec3 aLocation)
{
	ReplenishFullHealth();//This function will also set object as alive
    SetEnabled(true);
    SetPhysicsAndGameObjectPosition(aLocation);	
	ResetPhysicsBody();
}

void LivingObject::OnDeath()
{
	m_alive = false;
}

//Helper function to simply add an amount to the current object health. It uses the function SetHealth in order to actually increase the value of the health.
void LivingObject::AddHealth(int aHealth)
{
    int currentHealth = GetHealth();//Get the object current Health
    SetHealth(currentHealth + aHealth);//Add to the current health the amount desired
}

//Function to set the total health of the object, it DOESN'T add to the current health.
void LivingObject::SetHealth(int aHealth)
{
    //Check that the health being set is less than the maximum health possible
    if (aHealth <= m_maxHealth)
    {
        m_health = aHealth;//Set health normally

    }
    else if (aHealth > m_maxHealth)//If the health is bigger than the maximum, set the health to be the maximum
    {
        m_health = m_maxHealth;
    }
    else if (aHealth <= 0)//if the health is 0  or less
    {
        m_health = 0;//Set the health to 0
        OnDeath();//Call the onDeath function
    }

    //If the health is bigger than 0, ensure the object is alive
    if (m_health > 0)
    {
        m_alive = true;
    }

}

void LivingObject::ReplenishFullHealth()
{
    m_health = m_maxHealth;//Set health back to maximum health
    m_alive = true;//Set object as alive
}

void LivingObject::Reset()
{
    ReplenishFullHealth();//Replenish the object health completely
    GameObject::Reset();
}

void LivingObject::BeginCollision(b2Fixture * aFixtureCollided, b2Fixture * aFixtureCollidedAgainst, GameObject * aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{
    if (aTouching == true && aObjectCollidedagainst!=nullptr)
    {
        vec3 objectCollidedPosition = aObjectCollidedagainst->GetPosition();
        float objectCollidedSizeDepth = aObjectCollidedagainst->GetZLength() / 2.0f;

        //Check if the objects are colliding, in the z depth. Done in here and presolve to account for sensors that don't call for presolve.
        if (DepthCollisionCheck(objectCollidedPosition.z + objectCollidedSizeDepth, objectCollidedPosition.z - objectCollidedSizeDepth))
        {

            LivingObject* pLivingObject = dynamic_cast<LivingObject*>(aObjectCollidedagainst);//Check if the object collided with is a living object

			//Don't check if they are alive, otherwise the order in which begin collision is called may affect the object
            if (aObjectCollidedagainst->GetPhysicsBody()->IsActive() == true && m_pPhysicsBody->IsActive()==true)//Check that the other object is enabled
            {
                 
                //if (GetAlive() == true)//Don't check if they are alive, otherwise the order in which begin collision is called may affect the object.

                    if (pLivingObject != nullptr)//If it is a living object
                    {
                        pLivingObject->ApplyDamage(m_collisionDamage);
                    }
                    else//If it is NOT a living object
                    {
                        if (m_destroyOnCollisionWithNotLivingObjects == true)
                        {
                            OnDeath();//Kill this, living object.
                        }
                    }

                    //If we are to destroy after a collision
                    if (m_destroyOnCollision == true)
                    {
                        OnDeath();
                    }
           }
        }
    }
}