#include "CommonHeader.h"

FGHealthPickUp::FGHealthPickUp(int aHealingAmmount, Scene* apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh* apMesh, ShaderProgram* apShader, GLuint aTexture, vec2 aUVScale, vec2 aUVOffset,
    const char* aCameraName, unsigned int aDrawRenderOrder):
    HealthPowerUp(aHealingAmmount, apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aUVScale, aUVOffset,  aCameraName, aDrawRenderOrder)
{

    m_pickUpSound = nullptr;//Ensure taht the sound from FMOD sound system (no audio cue) is turned off

}

FGHealthPickUp::FGHealthPickUp(int healingAmount, const char* cameraName, unsigned int drawRenderOrder) :HealthPowerUp(healingAmount, cameraName, drawRenderOrder)
{
    m_pickUpSound = nullptr;//Ensure taht the sound from FMOD sound system (no audio cue) is turned off
}


FGHealthPickUp::~FGHealthPickUp()
{
    //Audio deleted by game
    //SAFE_DELETE(m_pPickedUpAudio);
}

void FGHealthPickUp::OnDeath()
{
    HealthPowerUp::OnDeath();

    AudioCue* sound = g_pGame->GetAudioCue("HealthPickUp");
    if (sound != nullptr)
    {
        sound->PlayAudio();
    }
}

//PARENT CLASS
////Function so that there isn't a collision when the colliding object has full health. This isn't done in presolve, because it is a sensor, so presolve/postsolve don't get called.
//void HealthPowerUp::BeginCollision(b2Fixture* aFixtureCollided, b2Fixture* aFixtureCollidedAgainst, GameObject* aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
//{
//    if (aObjectCollidedagainst != nullptr)
//    {
//        If the object is a living object
//        LivingObject* pLivingObject = dynamic_cast<LivingObject*>(aObjectCollidedagainst);
//
//        if (pLivingObject != nullptr)
//        {
//            Check if the object has less than max health
//            if (pLivingObject->GetHealth() < pLivingObject->GetMaxHealth())
//            {
//                LivingObject::BeginCollision(aFixtureCollided, aFixtureCollidedAgainst, aObjectCollidedagainst, aCollisionNormal, aContactPoint, aTouching);//Do the collision as normal
//            }
//        }
//    }
//}