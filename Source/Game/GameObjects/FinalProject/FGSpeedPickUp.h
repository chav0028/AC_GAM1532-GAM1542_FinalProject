#ifndef __FGSpeedPickUp__H__
#define __FGSpeedPickUp__H__

class AudioCue;

class FGSpeedPickUp :
    public LivingObject
{
protected:
    AudioCue* m_pPickedUpAudio;
    double m_bonusTime;
    float m_speedBoost;

public:
    FGSpeedPickUp(float boostAmount,double boostTime, const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
    virtual ~FGSpeedPickUp();
    virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
    virtual void OnDeath();
};


#endif