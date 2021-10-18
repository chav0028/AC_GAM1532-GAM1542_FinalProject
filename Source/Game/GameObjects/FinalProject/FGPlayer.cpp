#include "CommonHeader.h"

using namespace std;

FGPlayer::FGPlayer(int aMaxHealth, int collisionDamage)
{
    m_collisionDamage = collisionDamage;
    m_maxHealth = aMaxHealth;
    m_pEmitter = nullptr;
    m_gravityDown = true;
    Reset();
}

FGPlayer::~FGPlayer()
{
}

bool FGPlayer::HandleInput(InputEvent& aInputevent, double aDelta)
{
    if (aInputevent.type == InputEventType_Key)//Check for keyboard events
    {
        if (aInputevent.state == InputEventState_Down || aInputevent.state == InputEventState_Held)
        {
            if (m_pPhysicsBody != nullptr)
            {
                //USING ELSE IF SO THAT THE PLAYER CAN ONLY DO 1 THING AT THE TIME.

                //Movement X and Y
                if (aInputevent.keycode == 'W')//Move forward
                {
                    float velocity;
                    //Inverse the velocity according to direction of gravity
                    if (m_gravityDown == true)
                    {
                        velocity = -FG_PLAYER_DEFAULT_FORWARD_SPEED - m_speedBoost;
                    }
                    else
                    {
                        velocity = FG_PLAYER_DEFAULT_FORWARD_SPEED + m_speedBoost;
                    }

                    ApplyTorqueToWheels(velocity);
                }
                else if (aInputevent.keycode == 'W') //Move backward
                {
                    float velocity;
                    //Inverse the velocity according to direction of gravity
                    if (m_gravityDown == true)
                    {
                        velocity = -FG_PLAYER_DEFAULT_BACKWARD_SPEED - m_speedBoost;
                    }
                    else
                    {
                        velocity = FG_PLAYER_DEFAULT_BACKWARD_SPEED + m_speedBoost;
                    }

                    ApplyTorqueToWheels(velocity);
                }

                //Movement Z
                float translationSpeed = (float)aDelta*FG_PLAYER_DEFAULT_FORWARD_SPEED;//Calculate speed since box2d is....2D, and not 3D
                if (aInputevent.keycode == 'A')//Move deeper
                {
                    MoveZAxis(translationSpeed);
                }
                else if (aInputevent.keycode == 'D')//Move closer
                {
                    MoveZAxis(-translationSpeed);
                }
            }

        }
        else if (aInputevent.state == InputEventState_Up)
        {
            //Change gravity
            if (aInputevent.keycode == 'T')
            {
                ChangeGravity();
            }

            //Jump
            if (aInputevent.keycode == VK_SPACE)
            {
                if (m_canJump == true)
                {
                    if (m_pPhysicsBody != nullptr)
                    {
                        AudioCue* sound = g_pGame->GetAudioCue("PlayerJump");
                        if (sound != nullptr)
                        {
                            sound->PlayAudio();
                        }

                        //Apply impulse according to player direction, impulse is on the center of the body
                        if (m_gravityDown == true)
                        {
                            m_pPhysicsBody->ApplyLinearImpulse(b2Vec2(0, FG_PLAYER_JUMP_SPEED), m_pPhysicsBody->GetWorldCenter(), true);
                        }
                        else
                        {
                            m_pPhysicsBody->ApplyLinearImpulse(b2Vec2(0, -FG_PLAYER_JUMP_SPEED), m_pPhysicsBody->GetWorldCenter(), true);
                        }

                        m_canJump = false;
                    }
                }
            }

            //"SIDE" jumps, used in case the player is flipped
            if (aInputevent.keycode == 'Q')
            {
                if (m_canJump == true)
                {
                    if (m_pPhysicsBody != nullptr)
                    {
                        //Apply impulse according to player direction, impulse is offset in order to make the player flip
                        if (m_gravityDown == true)
                        {
                            m_pPhysicsBody->ApplyAngularImpulse(-FG_PLAYER_SIDE_JUMP_SPEED, true);
                        }
                        else
                        {
                            m_pPhysicsBody->ApplyAngularImpulse(FG_PLAYER_SIDE_JUMP_SPEED, true);
                        }

                        m_canJump = false;
                    }
                }
            }
            else if (aInputevent.keycode == 'E')
            {
                if (m_canJump == true)
                {
                    if (m_pPhysicsBody != nullptr)
                    {
                        //Apply impulse according to player direction, impulse is offset in order to make the player flip
                        if (m_gravityDown == true)
                        {
                            m_pPhysicsBody->ApplyAngularImpulse(FG_PLAYER_SIDE_JUMP_SPEED, true);
                        }
                        else
                        {
                            m_pPhysicsBody->ApplyAngularImpulse(-FG_PLAYER_SIDE_JUMP_SPEED, true);
                        }

                        m_canJump = false;
                    }
                }
            }

        }
    }

    return false;
}

void FGPlayer::Update(double aTimePassed)
{
    //Decrease timers
    if (m_isInvincible == true)
    {
        m_invincibilityTimer -= aTimePassed;
        if (m_invincibilityTimer <= 0)
        {
            //Reset the collsion mask
            SetFixtureCollisionsFiltering(-1, FGCollisionFilteringGroup_PlayerAndWheels, FGCollisionFilteringCategory_Player, FG_PLAYER_DEFAULT_COLLISION_MASK);

            m_isInvincible = false;
        }
    }

    if (m_isBoosted == true)
    {
        m_speedTimer -= aTimePassed;
        if (m_speedTimer <= 0)
        {
            //Remove speed boost
            m_speedBoost=0.0f;

            m_isBoosted = false;
        }
    }

    if (m_canShootParticles == false)
    {
        m_particleTimer -= aTimePassed;
        if (m_particleTimer <= 0)
        {
            m_canShootParticles = true;
            m_particleTimer = m_particleMinCooldown;
        }
    }

    LivingObject::Update(aTimePassed);

}

void FGPlayer::AddWheel(FGPlayerWheel* aWheel, b2RevoluteJoint* aWheelJoint)
{
    if (aWheel != nullptr && aWheelJoint != nullptr)
    {
        aWheel->SetAttachedPlayer(this);
        m_pWheels.push_back(aWheel);
        m_pWheelJoints.push_back(aWheelJoint);
    }
}

void FGPlayer::ChangeJointsMotorMaxVelocity(float aMaxVelocity)
{
    for (unsigned int i = 0; i < m_pWheelJoints.size(); i++)
    {
        if (m_pWheelJoints.at(i) != nullptr)
        {
            m_pWheelJoints.at(i)->SetMaxMotorTorque(aMaxVelocity);
        }
    }
}

void FGPlayer::ChangeJointsVelocity(float aVelocity)
{
    for (unsigned int i = 0; i < m_pWheelJoints.size(); i++)
    {
        if (m_pWheelJoints.at(i) != nullptr)
        {

            m_pWheelJoints.at(i)->SetMotorSpeed(m_pWheelJoints.at(i)->GetMotorSpeed() + aVelocity);
        }
    }
}

void FGPlayer::BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching)
{

    if (fixtureCollidedAgainst->IsSensor() == false)
    {
        if (m_gravityDown == true)
        {
            // check down 
            if (collisionNormal.y > 0.8f)
            {
                m_canJump = true;

                if (m_canShootParticles == true)
                {
                    if (m_pEmitter != nullptr)
                    {
                        m_pEmitter->SetAllParticles(Vector4(1, 1, 0, 1), Vector3(0.5f, 0.5f, 0.5f), 5, Vector3(0, 9.8f*m_pPhysicsBody->GetGravityScale(), 0));
                        m_pEmitter->SetUseDirectionRange(true);
                        m_pEmitter->SetFiringAngleRange(Vector3(-5, 1, -5), Vector3(5, 5.0f, 5));
                        m_pEmitter->FireParticles(50, Vector3(m_pPhysicsBody->GetLinearVelocity().x, collisionNormal.y, 0), 5);
                        m_canShootParticles = false;
                    }
                }
            }
        }
        else
        {  // check up
            if (collisionNormal.y < -0.8f)
            {
                m_canJump = true;

                if (m_canShootParticles == true)
                {
                    if (m_pEmitter != nullptr)
                    {
                        m_pEmitter->SetAllParticles(Vector4(1, 1, 0, 1), Vector3(0.5f, 0.5f, 0.5f), 5, Vector3(0, 9.8f*m_pPhysicsBody->GetGravityScale(), 0));
                        m_pEmitter->SetUseDirectionRange(true);
                        m_pEmitter->SetFiringAngleRange(Vector3(5, -5, -5), Vector3(10, 5.0f, 5));
                        m_pEmitter->FireParticles(30, Vector3(m_pPhysicsBody->GetLinearVelocity().x, collisionNormal.y, 0), 5);
                        m_canShootParticles = false;
                    }
                }
            }

        }

        if (touching == true)
        {

            //Stop the music
            AudioCue* sound = g_pGame->GetAudioCue("PlayerHit");
            if (sound != nullptr)
            {
                sound->PlayAudio();
            }


            //Add the overlapping object
            if (objectCollidedagainst != nullptr)
            {
                if (objectCollidedagainst->GetName() != FG_SCENE_FLOOR_NAME &&objectCollidedagainst->GetName() != FG_SCENE_ROOF_NAME)
                {
                    m_pOverlappingObjects.push_back(objectCollidedagainst);
                }
            }

        }


    }

    LivingObject::BeginCollision(fixtureCollided, fixtureCollidedAgainst, objectCollidedagainst, collisionNormal, contactPoint, touching);
}

void FGPlayer::EndCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching)
{
    if (touching == false)
    {
        //Remove the overlapping object
        if (objectCollidedagainst != nullptr)
        {
            //Search all the overlapping objects
            for (unsigned int i = 0; i < m_pOverlappingObjects.size(); i++)
            {
                if (m_pOverlappingObjects.at(i) != nullptr)
                {

                    if (m_pOverlappingObjects.at(i) == objectCollidedagainst)
                    {
                        m_pOverlappingObjects.erase(m_pOverlappingObjects.begin() + i);//Remove the object
                        break;

                    }
                }
            }
        }
    }
}

void FGPlayer::ChangeGravity()
{
    //Check that the player isn't currently rotating
    if (GetFloatTweenerOver(GAMEOBJECT_ROTATION_X_TWEEN_NAME) == true)
    {
        AudioCue* sound = g_pGame->GetAudioCue("GravityChange");
        if (sound != nullptr)
        {
            sound->PlayAudio();
        }

        m_canJump = false;

        //Rotate the player according to the direction of gravity
        if (m_gravityDown == true)
        {
            SetTweenXRotation(0 + 180.0f, m_changeGravityAnimationTime, TweeningType_Linear);//Set the tween animation for the change in gravity
        }
        else
        {
            SetTweenXRotation(0, m_changeGravityAnimationTime, TweeningType_Linear);//Set the tween animation for the change in gravity
        }

        //Change the gravity scale to simulate change in gravity
        if (m_pPhysicsBody != nullptr)
        {
            m_pPhysicsBody->SetGravityScale(m_pPhysicsBody->GetGravityScale()*-1);//Inverse the current gravity scale

            //Change the gravity scale also in all the wheels attached to the player (their will match the player rotation in their update)
            for (unsigned int i = 0; i < m_pWheels.size(); i++)
            {
                if (m_pWheels.at(i) != nullptr)
                {
                    b2Body* wheelBody = m_pWheels.at(i)->GetPhysicsBody();

                    if (wheelBody != nullptr)
                    {
                        wheelBody->SetGravityScale(m_pPhysicsBody->GetGravityScale());
                    }
                }
            }
        }

        m_gravityDown = !m_gravityDown;//Inverse the current status of the gravity

    }
}

void FGPlayer::ApplyTorqueToWheels(float aForce)
{
    //Go through all the wheels
    for (unsigned int i = 0; i < m_pWheels.size(); i++)
    {
        if (m_pWheels.at(i) != nullptr)
        {
            //Get the wheel physic body
            b2Body* wheelBody = m_pWheels.at(i)->GetPhysicsBody();

            if (wheelBody != nullptr)
            {
                //Apply torque to each wheel
                wheelBody->ApplyTorque(aForce, true);
            }
        }
    }
}

void FGPlayer::SetTweenXRotation(float aEndValue, double aEndTime, TweeningType aType, double aDelay)
{
    GameObject::SetTweenXRotation(aEndValue, aEndTime, aType, aDelay); //Rotate the main body

    //Rotate the wheels
    for (unsigned int i = 0; i < m_pWheels.size(); i++)
    {
        if (m_pWheels.at(i) != nullptr)
        {
            float wheelCurrentXRot = m_pWheels.at(i)->GetRotationX();

            //Rotate the player according to the direction of gravity
            if (m_gravityDown == true)
            {
                m_pWheels.at(i)->SetTweenXRotation(0 + 180.0f, m_changeGravityAnimationTime, TweeningType_Linear);//Set the tween animation for the change in gravity
            }
            else
            {
                m_pWheels.at(i)->SetTweenXRotation(0, m_changeGravityAnimationTime, TweeningType_Linear);//Set the tween animation for the change in gravity
            }
        }
    }
}

void FGPlayer::Reset()
{
    m_isInvincible = false;
    m_isBoosted = false;
    m_speedBoost = 0;
    m_invincibilityTimer = 0.0;
    m_speedTimer = 0.0;
    m_particleTimer=0.0;
    m_particleMinCooldown=5.0;
    m_canShootParticles = true;

    if (m_gravityDown == false)
    {
        ChangeGravity();
    }
    m_changeGravityAnimationTime = FG_PLAYER_DEFAULT_CHANGE_GRAVITY_ANIMATION_TIME;
    SetEnabled(true);

    //Ensure the collision filtering is set
    SetFixtureCollisionsFiltering(-1, FGCollisionFilteringGroup_PlayerAndWheels, FGCollisionFilteringCategory_Player, FG_PLAYER_DEFAULT_COLLISION_MASK);

    for (unsigned int i = 0; i < m_pWheels.size(); i++)
    {
        if (m_pWheels.at(i) != nullptr)
        {
            m_pWheels.at(i)->Reset();
        }
    }

    LivingObject::Reset();
}

void FGPlayer::MoveZAxis(float aSpeed)
{
    //Calculate the new position the player will have
    vec3 newPosition = m_Position;
    newPosition += aSpeed;
    float objectBHalfDepth = GetZLength() / 2.0f;
    float minZ = newPosition.z - objectBHalfDepth;
    float maxZ = newPosition.z + objectBHalfDepth;

    bool canMove = true;
    for (unsigned int i = 0; i < m_pOverlappingObjects.size(); i++)
    {
        if (m_pOverlappingObjects.at(i) != nullptr)
        {
            //If already disabled ,remove from list
            if (m_pOverlappingObjects.at(i)->GetEnabled() == false || m_pOverlappingObjects.at(i)->GetPhysicsBody()->IsActive()==false)
            {
                m_pOverlappingObjects.erase(m_pOverlappingObjects.begin() + i);
                continue;
            }

            if (m_pOverlappingObjects.at(i)->DepthCollisionCheck(m_Position.z + objectBHalfDepth, m_Position.z - objectBHalfDepth) == true)//Check if the object is already inside the overlapping object
            {
            }
            else if (m_pOverlappingObjects.at(i)->DepthCollisionCheck(maxZ, minZ) == true)//Check if the new position will overlap any object
            {
                canMove = false;
                break;
            }
        }
    }

    if (canMove == true)
    {
        SetPositionZ(newPosition.z);//Move the player
    }

}

void FGPlayer::OnDeath()
{
    AudioCue* sound = g_pGame->GetAudioCue("PlayerDeath");
    if (sound != nullptr)
    {
        sound->PlayAudio();
    }

    LivingObject::OnDeath();
}

void FGPlayer::AddInvincibilityTime(double aTime)
{
    m_invincibilityTimer += aTime;
    m_isInvincible = true;

    //Ensure the collision filtering is set
    SetFixtureCollisionsFiltering(-1, FGCollisionFilteringGroup_PlayerAndWheels, FGCollisionFilteringCategory_Player, FG_PLAYER_INVINCIBLE_COLLISION_MASK);
}

void FGPlayer::AddSpeedBoostTime(double aTime, float aSpeedBoost)
{
    m_speedTimer += aTime;
    m_isBoosted = true;
    m_speedBoost += aSpeedBoost;
}

void FGPlayer::ApplyDamage(int damage)
{
    AudioCue* sound = g_pGame->GetAudioCue("PlayerHurt");
    if (sound != nullptr)
    {
        sound->PlayAudio();
    }

    LivingObject::ApplyDamage(damage);
}