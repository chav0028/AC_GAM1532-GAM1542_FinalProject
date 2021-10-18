#include "CommonHeader.h"

EnemyObject::EnemyObject(Pool<EnemyObject>* aPool) :
m_pPoolOwner(aPool),
m_enemyAliveTimer(ENEMIES_DEFAULT_ALIVE_TIME)
{

}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::Update(double aTimePassed)
{
    GameObject::Update(aTimePassed);
	m_enemyAliveTimer -= (float)aTimePassed;

    //If enemy goes offscreen
    if (m_Position.y < ENEMY_DISAPPEAR_BOTTOM_DEPTH ||m_enemyAliveTimer<=0.0f)
    {
		DisableEnemy();//Disable the enemy
    }
}


void EnemyObject:: LaunchEnemy(float aAngle)
{
    SetEnabled(true);//Enable the enemy

    float radianAngle = aAngle*PI / 180;//Convert angle to radian

    //Convert the angle into a vector
    b2Vec2 angleVector;
    angleVector.x = cos(radianAngle);
    angleVector.y = sin(radianAngle);

    //X speed times 2 so that the game is funnier, since if not enemies will almost not move horizontally
    m_pPhysicsBody->ApplyLinearImpulse(b2Vec2(angleVector.x*(m_speed*2.0f), angleVector.y*m_speed), m_pPhysicsBody->GetPosition(), true);//Move the enemy

}

void EnemyObject::DisableEnemy()
{
	SetEnabled(false);//Disable the object
	ResetPhysicsBodyVelocities();//Reset the velocities of the physics body
	m_enemyAliveTimer = ENEMIES_DEFAULT_ALIVE_TIME;//Reset the alive timer

	m_pPoolOwner->DeactivateObject(this);//Set it as deactivated in the pool

	if (m_pScene != nullptr)
	{
		m_pScene->RemoveGameObject(m_Name);//Remove this game object from scene
	}
}

void EnemyObject::BeginCollision(b2Fixture * aFixtureCollided, b2Fixture * aFixtureCollidedAgainst, GameObject * aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{

}

void EnemyObject::CollsionPreSolve(b2Contact* aContact,b2Fixture * aFixtureCollided, b2Fixture * aFixtureCollidedAgainst, GameObject * aObjectCollidedagainst, b2Vec2 aCollisionNormal)
{

	PlaneRippleObject* notFloorCheck = dynamic_cast<PlaneRippleObject*>(aObjectCollidedagainst);

	if (aObjectCollidedagainst != nullptr&& notFloorCheck==nullptr)
	{
		vec3 objectCollidedPosition = aObjectCollidedagainst->GetPosition();
		float objectCollidedSizeDepth = aObjectCollidedagainst->GetZLength()/2.0f;	

		//This methods doesn't work with the floor since its scale isn't used as its size, so we ignore the floor and always colide with it regardless of Z

		//Check if the objects are colliding, in the z depth
        if (DepthCollisionCheck(objectCollidedPosition.z + objectCollidedSizeDepth, objectCollidedPosition.z - objectCollidedSizeDepth))
		{
			//If they are, procede with normal collision
		}
		else
		{
			//Otherwise disable the contact
			if (aContact != nullptr)
			{
				aContact->SetEnabled(false);
			}
		}
	}
}

