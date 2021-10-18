#ifndef __FGPlayer_H__
#define __FGPlayer_H__

const float FG_PLAYER_DEFAULT_FORWARD_SPEED = 30.0f;
const float FG_PLAYER_DEFAULT_BACKWARD_SPEED = -20.0f;
const float FG_PLAYER_DEFAULT_DEPTH_SPEED = 5.0f;
const float FG_PLAYER_JUMP_SPEED = 15.0f;
const float FG_PLAYER_SIDE_JUMP_SPEED = 20.0f;
const float FG_PLAYER_SIDE_JUMP_OFFSET = 0.3f;

const int FG_PLAYER_DEFAULT_HEALTH = 100;
const int FG_PLAYER_COLLISION_DAMAGE = 20;
const double FG_PLAYER_DEFAULT_CHANGE_GRAVITY_ANIMATION_TIME = 1.0;

const float FG_PLAYER_FORCE_BY_PARTICLE_MULTIPLIER = 35.0f;

const signed short FG_PLAYER_DEFAULT_COLLISION_MASK = FGCollisionFilteringCategory_Enemy | FGCollisionFilteringCategory_EnviromentStatic | FGCollisionFilteringCategory_PickUp |
FGCollisionFilteringCategory_EndLevel | FGCollisionFilteringCategory_EnviromentDynamic;
const signed short FG_PLAYER_INVINCIBLE_COLLISION_MASK = FGCollisionFilteringCategory_EnviromentStatic | FGCollisionFilteringCategory_PickUp |
FGCollisionFilteringCategory_EndLevel;

class FGPlayerWheel;
class LivingObject;
class ParticleEmitter;

class FGPlayer : public LivingObject
{

protected:
    std::vector<GameObject*>m_pOverlappingObjects;
	double m_changeGravityAnimationTime;
    bool m_gravityDown;
	bool m_canJump;
    std::vector<FGPlayerWheel*>m_pWheels;
    std::vector<b2RevoluteJoint*> m_pWheelJoints;

    bool m_isInvincible;
    bool m_isBoosted;
    double m_invincibilityTimer;
    double m_speedTimer;
    float m_speedBoost;
    ParticleEmitter* m_pEmitter;

    double m_particleTimer;
    double m_particleMinCooldown;
    bool m_canShootParticles;

public:
    FGPlayer(int maxHealth = FG_PLAYER_DEFAULT_HEALTH, int collisionDamage = FG_PLAYER_COLLISION_DAMAGE);
    virtual ~FGPlayer();

    virtual void Update(double TimePassed);
    bool HandleInput(InputEvent& aInputevent, double aDelta);
    
    void AddWheel(FGPlayerWheel* aWheel, b2RevoluteJoint* wheelJoint);

    void MoveZAxis(float speed);
    void ChangeJointsMotorMaxVelocity(float aMaxVelocity);
    void ChangeJointsVelocity(float aVelocity);
    void ApplyTorqueToWheels(float aTorque);
    bool GetGravityDown(){ return m_gravityDown; }

    bool GetIsInvincible(){ return m_isInvincible; }
    bool GetIsBoosted(){ return m_isBoosted; }

    void SetEmitter(ParticleEmitter* aEmitter){ if (aEmitter != nullptr)m_pEmitter = aEmitter; }
    void FireParticles(float aForce, Vector3 aLaunchDirection,Vector3 aLaunchPosition);

	void ChangeGravity();

	virtual void SetTweenXRotation(float aEndValue, double aEndTime, TweeningType aType,double delay=0);
	virtual void Reset();

    virtual void ApplyDamage(int damage);
    virtual void OnDeath();

    virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
    virtual void EndCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);

    virtual void AddInvincibilityTime(double aTime);
    virtual void AddSpeedBoostTime(double aTime, float aSpeedBoost);
};

#endif
