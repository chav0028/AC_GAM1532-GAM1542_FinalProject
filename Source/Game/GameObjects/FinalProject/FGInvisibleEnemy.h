#ifndef __FGInvinsibleEnemy_H__
#define __FGInvinsibleEnemy_H__

class FGInvisibleEnemy : public LivingObject
{

protected:
    bool m_isInvisible;
    double m_invisibleTimer;
    double m_invisibleTime;
    
public:
    FGInvisibleEnemy(int collsionDamage, int maxHealth);
    virtual ~FGInvisibleEnemy();

    virtual void Update(double TimePassed);

    virtual void OnDeath();
    virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
    virtual void EndCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
    void InitializeEnemy(GLint skyBoxTexture, GLint objectTexture);
    void Reset();

};

#endif
