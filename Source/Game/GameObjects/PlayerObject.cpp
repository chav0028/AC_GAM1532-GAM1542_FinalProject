#include "CommonHeader.h"

using namespace std;

PlayerObject::PlayerObject(int aMaxHealth, Scene* apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh* apMesh, ShaderProgram* apShader, GLuint aTexture) :
LivingObject(aMaxHealth, apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture),
m_poolProjectiles(0)//Set starting pool number to 0, this enemies will be created and added to pool in load game content
{
    m_Color = Vector4(0.9f, 0.8f, 0.2f, 1);
	m_collisionDamage = PLAYER_COLLISION_DAMAGE;

    if (apScene != nullptr)
    {
		//Set it to collide with practically everything except projectiles
		signed short maskBit = CollisionFilteringCategory_Enemy |
			CollisionFilteringCategory_Enviroment|
			CollisionFilteringCategory_PowerUp|
			CollisionFilteringCategory_EndLevel;

        //Create the physcis body
        CreatePhysicsBox(vec2(GetXLength(), GetYLength()), b2_dynamicBody, vec2(0, 0), false, CollisionFilteringGroup_PlayerAndProjectiles,CollisionFilteringCategory_Player, maskBit);
        m_pPhysicsBody->SetGravityScale(1);
        SetPhysicsBodyFriction(1.0f);
        SetPhysicsBodyRestitution(0.0f);
		m_pPhysicsBody->SetFixedRotation(true);
        apScene->AddInputHandler(this);//Set it so that the player can handle inputs

        //Initialize the player projectiles
        Projectile* tempProjectile = nullptr;

        //Create all the projectiles
        for (unsigned int i = 0; i < PLAYER_DEFAULT_NUMBER_PROJECTILES; i++)
        {
            tempProjectile = new Projectile(&m_poolProjectiles);//Add enemy to scene
            tempProjectile->Init(1, apScene, "PlayerProjectile" + to_string(i), aPos, aRot, aScale, g_pGame->GetMesh("TorpedoOBJ"), g_pGame->GetShader("Texture"), g_pGame->GetTexture("Blocks"));
          
            m_poolProjectiles.AddExistingObject(tempProjectile);//Add the enemy to the pool
        }
    }

    m_playerFireSound = new Sound("Data/Audio/PlayerFire.wav", true);//Sound when player fires
    m_playerHurtSound = new Sound("Data/Audio/PlayerHurt.wav", true);//Sound when player is hurt
}

PlayerObject::~PlayerObject()
{
    //Remove all the currently active objects
    vector<Projectile*> currentlyActiveProjectiles = m_poolProjectiles.GetCurrentlyActiveObjects();//Get the projectiles that are currently active

    if (currentlyActiveProjectiles.empty() == false)
    {
        for (unsigned int i = 0; i < currentlyActiveProjectiles.size(); i++)
        {
            if (currentlyActiveProjectiles.at(i) != nullptr)
            {
                if (m_pScene != nullptr)
                {
                    m_pScene->RemoveGameObject(currentlyActiveProjectiles.at(i)->GetName());//Remove the object from the scene

                    //Pool deletes objects from active and inactive pool, no need to deactivate
                }
            }
        }
    }

    SAFE_DELETE(m_playerFireSound);
    SAFE_DELETE(m_playerHurtSound);
}

bool PlayerObject::HandleInput(InputEvent& aInputevent, double aDelta)
{
    if (aInputevent.type == InputEventType_Key)//Check for keyboard events
    {
        if (aInputevent.state == InputEventState_Down || aInputevent.state == InputEventState_Held)
        {
            if (m_pPhysicsBody != nullptr)
            {
                //USING ELSE IF SO THAT THE PLAYER CAN ONLY DO 1 THING AT THE TIME.

                //Movement X and Y
                if (aInputevent.keycode == VK_SPACE)//Move up
                {
                    m_pPhysicsBody->ApplyForceToCenter(b2Vec2(0, PLAYER_DEFAULT_UP_SPEED), true);
                }
                else if (aInputevent.keycode == 'W')//Move forward
                {
                    m_pPhysicsBody->ApplyForceToCenter(b2Vec2(PLAYER_DEFAULT_FORWARD_SPEED, 0), true);
                }
                else if (aInputevent.keycode == 'S')//Move backward
                {
                    m_pPhysicsBody->ApplyForceToCenter(b2Vec2(PLAYER_DEFAULT_BACKWARD_SPEED, 0), true);
                }

                //Movement Z
                float translationSpeed = (float)aDelta*PLAYER_DEFAULT_FORWARD_SPEED;//Calculate speed since box2d is....2D, and not 3D
                if (aInputevent.keycode == 'A')//Move deeper
                {
                    SetPositionZ(m_Position.z + translationSpeed);
                }
                else if (aInputevent.keycode == 'D')//Move closer
                {
					SetPositionZ(m_Position.z - translationSpeed);
                }
            }

            if (aInputevent.state == InputEventState_Down)
            {
                //Player firing
                if (aInputevent.keycode == 'E')
                {
                    FireProjectile();
                }
            }

        }
    }

    return false;

}

void PlayerObject::Update(double aTimePassed)
{
    LivingObject::Update(aTimePassed);
}

void PlayerObject::FireProjectile()
{
    if (m_playerFireSound != nullptr)
    {
        m_playerFireSound->PlayAudio();
    }

    Projectile* projectile = m_poolProjectiles.GetActivateObject();///Get an object from the pool

    if (projectile != nullptr)
    {
        string projectileID = projectile->GetName();
        m_pScene->AddGameObject(projectileID, projectile);//Add projectile to scene

        //The projectile will be launched from front (right) middle of the player
        vec3 launchPosition = m_Position;//Get the player center position , considering pivot is in middle
        launchPosition.x += GetXLength()/2;//Add the half width of the object
 	
		vec2 objectVelocity = vec2(0, 0);

		//If we wanted to use object velocity, it doesn't look good. It works, but has been commented for game design reasons
		//Get the object current velocity
		if (m_pPhysicsBody != nullptr)
		{
			objectVelocity = vec2(m_pPhysicsBody->GetLinearVelocity().x, m_pPhysicsBody->GetLinearVelocity().y);

			//Ignore negative values
			if (objectVelocity.x < 0)
			{
				objectVelocity = 0;
			}
		}

        projectile->LaunchProjectile(launchPosition, m_Rotation.z, vec2(objectVelocity.x,0.0f));
    }
}

void PlayerObject::ApplyDamage(int aDamage)
{
    if (m_playerHurtSound != nullptr)
    {
        m_playerHurtSound->PlayAudio();
    }

    LivingObject::ApplyDamage(aDamage);
}

void PlayerObject::Reset()
{
    vector<Projectile*> currentlyActiveProjectiles = m_poolProjectiles.GetCurrentlyActiveObjects();//Get the projectiles that are currently active

    if (currentlyActiveProjectiles.empty() == false)
    {
        for (unsigned int i = 0; i < currentlyActiveProjectiles.size(); i++)
        {
            if (currentlyActiveProjectiles.at(i) != nullptr)
            {
                if (m_pScene != nullptr)
                {
                    m_pScene->RemoveGameObject(currentlyActiveProjectiles.at(i)->GetName());//Remove the object from the scene
                }
                currentlyActiveProjectiles.at(i)->DisableProjectileFromPool();//Disable the projectile
            }
        }
    }

    LivingObject::Reset();//Reset the position and enabled status of the player

}

