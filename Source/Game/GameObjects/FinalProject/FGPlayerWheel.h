#ifndef __FGPlayerWheel_H__
#define __FGPlayerWheel_H__

class FGPlayer;

class FGPlayerWheel : public LivingObject
{

protected:
    FGPlayer* m_pPlayer;

public:
    FGPlayerWheel();
    virtual ~FGPlayerWheel();

    virtual void Update(double TimePassed);

    void SetAttachedPlayer(FGPlayer* aPlayer);

    virtual void ApplyDamage(int damage);
    virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
    virtual void EndCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
};

#endif
