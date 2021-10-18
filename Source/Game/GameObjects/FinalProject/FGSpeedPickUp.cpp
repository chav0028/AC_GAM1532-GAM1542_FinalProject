#include "CommonHeader.h"

FGSpeedPickUp::FGSpeedPickUp(float boostAmount,double aTimer, const char* cameraName, unsigned int drawRenderOrder) :LivingObject(cameraName, drawRenderOrder)
{
    m_bonusTime = aTimer;
    m_speedBoost = boostAmount;
    m_collisionDamage = 0;
    m_destroyOnCollision = true;
    m_maxHealth = 1;
}


FGSpeedPickUp::~FGSpeedPickUp()
{

}

void FGSpeedPickUp::OnDeath()
{
    LivingObject::OnDeath();

    AudioCue* sound = g_pGame->GetAudioCue("SpeedPickUp");
    if (sound != nullptr)
    {
        sound->PlayAudio();
    }
}

//Function so that there isn't a collision when the colliding object has full health. This isn't done in presolve, because it is a sensor, so presolve/postsolve don't get called.
void FGSpeedPickUp::BeginCollision(b2Fixture* aFixtureCollided, b2Fixture* aFixtureCollidedAgainst, GameObject* aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{
    if (aObjectCollidedagainst != nullptr)
    {
        //If the object is a player object
        FGPlayer* pPlayer = dynamic_cast<FGPlayer*>(aObjectCollidedagainst);

        if (pPlayer != nullptr)
        {
            pPlayer->AddSpeedBoostTime(m_bonusTime,m_speedBoost);
           
        }
        LivingObject::BeginCollision(aFixtureCollided, aFixtureCollidedAgainst, aObjectCollidedagainst, aCollisionNormal, aContactPoint, aTouching);
    }
}