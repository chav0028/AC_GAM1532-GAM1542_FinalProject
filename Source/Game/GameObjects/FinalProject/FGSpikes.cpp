#include "CommonHeader.h"

FGPSpikes::FGPSpikes(int aCollisionDamage, bool destructible, int maxHealth) :LivingObject()
{
    m_collisionDamage = aCollisionDamage;
    m_destroyOnCollision = destructible;
    m_maxHealth = maxHealth;
}

FGPSpikes::~FGPSpikes()
{
}

void FGPSpikes::OnDeath()
{
    AudioCue* sound = g_pGame->GetAudioCue("EnemyExplosion");
    if (sound != nullptr)
    {
        sound->PlayAudio();
    }

    LivingObject::OnDeath();
}