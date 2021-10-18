#ifndef __EnemyObject_H__
#define __EnemyObject_H__

const float ENEMIES_DEFAULT_ALIVE_TIME = 5.0f;
const float ENEMY_DISAPPEAR_BOTTOM_DEPTH = -5.0f;

class EnemyObject : public GameObject
{
protected:
	Pool<EnemyObject>* m_pPoolOwner;//the pool in which the enemy is
	float m_enemyAliveTimer;

public:
	EnemyObject(Pool<EnemyObject>* pool);
	virtual ~EnemyObject();

	virtual void Update(double TimePassed);

	void LaunchEnemy(float angle);
	void DisableEnemy();

	virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
	virtual void CollsionPreSolve(b2Contact* aContact, b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal);
};

#endif //__PlayerObject_H__
