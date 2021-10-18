#ifndef __FGInvincibilityPickUp__H__
#define __FGInvincibilityPickUp__H__

class FGInvincibilityPickUp:public LivingObject
{
protected:
    AudioCue* m_pPickedUpAudio;
    double m_bonusTime;

public:
    FGInvincibilityPickUp(double invincibilityTime, const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
    virtual ~FGInvincibilityPickUp();
    virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
    virtual void OnDeath();
};


#endif