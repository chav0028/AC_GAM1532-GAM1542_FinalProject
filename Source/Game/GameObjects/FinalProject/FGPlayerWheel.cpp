#include "CommonHeader.h"

FGPlayerWheel::FGPlayerWheel():
m_pPlayer(nullptr)
{
}

FGPlayerWheel::~FGPlayerWheel()
{
}

void FGPlayerWheel::Update(double TimePassed)
{
    //Make the wheel match the properties of the player, this isn't done through object parenting to prevent messing with the Box2D physics
    if (m_pPlayer != nullptr)
    {
        SetPositionZ(m_pPlayer->GetPositionZ());
        SetRotationX(m_pPlayer->GetRotationX());
        SetRotationY(m_pPlayer->GetRotationY());
        SetScale(m_pPlayer->GetScale());
        m_alive = m_pPlayer->GetAlive();
        m_health = m_pPlayer->GetHealth();
        SetEnabled(m_pPlayer->GetEnabled());
    }  

	LivingObject::Update(TimePassed);
}

void FGPlayerWheel::SetAttachedPlayer(FGPlayer* aPlayer)
{
    if (aPlayer != nullptr)
    {
        m_pPlayer = aPlayer;
        
        //Make the wheel match the properties of the player, this isn't done through object parenting to prevent messing with the Box2D physics
        SetPositionZ(m_pPlayer->GetPositionZ());
        SetRotationX(m_pPlayer->GetRotationX());
        SetRotationY(m_pPlayer->GetRotationY());
        SetScale(m_pPlayer->GetScale());
        m_alive = m_pPlayer->GetAlive();
        m_health = m_pPlayer->GetHealth();
        SetEnabled(m_pPlayer->GetEnabled());

        m_collisionDamage = m_pPlayer->GetCollisionDamage();
        m_maxHealth = m_pPlayer->GetMaxHealth();
    }

}


void FGPlayerWheel::BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching)
{
    //Call the player on begin collision
    if (m_pPlayer != nullptr)
    {
        m_pPlayer->BeginCollision(fixtureCollided, fixtureCollidedAgainst, objectCollidedagainst, collisionNormal, contactPoint, touching);
    }
}

void FGPlayerWheel::EndCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching)
{
    //Call the player on end collision
    if (m_pPlayer != nullptr)
    {
        m_pPlayer->EndCollision(fixtureCollided, fixtureCollidedAgainst, objectCollidedagainst, collisionNormal, contactPoint, touching);
    }
}

void  FGPlayerWheel::ApplyDamage(int damage)
{
    //Call the player apply damage
    if (m_pPlayer != nullptr)
    {
        m_pPlayer->ApplyDamage(damage);
    }
}
