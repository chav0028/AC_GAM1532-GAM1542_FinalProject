#include "CommonHeader.h"

FGInvisibleEnemy::FGInvisibleEnemy(int collsionDamage, int maxHealth) :LivingObject()
{
    m_collisionDamage = collsionDamage;
    m_maxHealth = maxHealth;
    
    //Random time (also not being changed in reset) so that not all the enemies are synchronized
    //min + (std::rand() % (max - min + 1))- Get a random number between a range
    m_invisibleTime=3;

    m_invisibleTimer = 0 + std::rand() % ((int)m_invisibleTime - 0 + 1);
}

FGInvisibleEnemy::~FGInvisibleEnemy()
{
    
}

void FGInvisibleEnemy::Update(double TimePassed)
{
    m_invisibleTimer -= TimePassed;

    //If time is over
    if (m_invisibleTime <= 0)
    {
        m_invisibleTime = m_invisibleTime;
        m_isInvisible = !m_isInvisible;//Flip between visible and "invisible"
    }

    if (m_isInvisible == true)
    {
        m_reflectivity = 0.0f;
    }
    else
    {
        m_reflectivity = 0.9f;
    }

    LivingObject::Update(TimePassed);
}

void FGInvisibleEnemy::OnDeath()
{
    AudioCue* sound = g_pGame->GetAudioCue("EnemyExplosion");
    if (sound != nullptr)
    {
        sound->PlayAudio();
    }

    LivingObject::OnDeath();
}

void FGInvisibleEnemy::InitializeEnemy(GLint skyBoxTexture, GLint objectTexture)
{
    if (m_pPhysicsBody != nullptr)
    {
        m_pPhysicsBody->SetGravityScale(0);
    }
    m_secondaryTexture = skyBoxTexture;
    m_TextureHandle = objectTexture;

    m_isInvisible = false;
}

void FGInvisibleEnemy::BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching)
{
    if (fixtureCollided->IsSensor() == false)
    {
        LivingObject::BeginCollision(fixtureCollided,fixtureCollidedAgainst,objectCollidedagainst,collisionNormal,contactPoint,touching);
    }
}

void FGInvisibleEnemy::Reset()
{
    LivingObject::Reset();
}

void FGInvisibleEnemy::EndCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching)
{
    if (fixtureCollided->IsSensor() == false)
    {
        LivingObject::EndCollision(fixtureCollided,fixtureCollidedAgainst,objectCollidedagainst,collisionNormal,contactPoint,touching);
    }

}