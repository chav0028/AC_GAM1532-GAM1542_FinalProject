#include "CommonHeader.h"
#include "Projectile.h"


Projectile::Projectile(Pool<Projectile>* aPool,  const char * aCameraName, unsigned int aDrawRenderOrder) :
m_pPoolOwner(aPool),
m_pendingPoolRemoval(false),
m_projectileExplodeSound(nullptr)
{
}

void Projectile::Init(int aMaxHealth, Scene * apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh * apMesh, ShaderProgram * apShader, GLuint aTexture, vec2 aUVScale, vec2 aUVOffset)
{
	GameObject::Init(apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aUVScale, aUVOffset);
	
	m_maxHealth = aMaxHealth;
	m_health = aMaxHealth;
	m_collisionDamage = PROJECTILE_DAMAGE;//Set the damage the projectile will do
	m_speed = PROJECTILE_SPEED;
	m_projectileLiveTimer = PROJECTILE_DEFAULT_LIVE_TIME;
	m_destroyOnCollisionWithNotLivingObjects = true;
    m_destroyOnCollision = true;

	//Create physics body
	signed short maskBit = CollisionFilteringCategory_Enemy |//Set it to collide only with enemies and enviroment
		CollisionFilteringCategory_Enviroment;

	CreatePhysicsBox(vec2(GetXLength(), GetYLength()), b2_dynamicBody, vec2(0, 0), false, CollisionFilteringGroup_PlayerAndProjectiles,CollisionFilteringCategory_PlayerProjectile, maskBit);
	m_pPhysicsBody->SetGravityScale(0);
	m_pPhysicsBody->SetFixedRotation(true);
	m_pPhysicsBody->SetActive(false);
	SetPhysicsBodyDensity(0.1f);

    m_projectileExplodeSound = new Sound("Data/Audio/ProjectileExplode.wav", false);//Sound when projectile dies

	Reset();//Disable the projectile
}

Projectile::~Projectile()
{
    SAFE_DELETE(m_projectileExplodeSound);
}

void Projectile::Update(double aDelta)
{
		m_projectileLiveTimer -= aDelta;//Decrease timer
		if (m_projectileLiveTimer <= 0)//if time is over
		{
			OnDeath();//Kill projectile
		}

    SetEnabled(m_alive);//Set enabled according to the stat of the projectile
    LivingObject::Update(aDelta);//Update the object, it will check the enabled status to see if it ihas to activate/deactivate the body

    if (m_pendingPoolRemoval == true)
    {
        DisableProjectileFromPool();//Remove the projectile from the pool.
    }

    
}

void Projectile::OnDeath()
{    
	//If the object is alive
	if (m_alive == true)
	{
        if (m_projectileExplodeSound != nullptr)
        {
            m_projectileExplodeSound->PlayAudio();
        }

        m_pendingPoolRemoval = true;
		LivingObject::OnDeath();//Kill projectiles in here to ensure the call isn't make multiple times.
	}
}

void Projectile::Reset()
{
    m_projectileLiveTimer = PROJECTILE_DEFAULT_LIVE_TIME;
    m_pendingPoolRemoval = false;
	SetEnabled(false);
}

//Only moves projectile in X and Y, the z value doesn't change
void Projectile::LaunchProjectile(vec3 aLaunchPosition, float aXYAngle, vec2 aLaunchingVelocity)
{

	SpawnObject(aLaunchPosition);//Enable the projectile and set it at the desired position

    float radianAngle = aXYAngle*PI / 180;//Convert angle to radian

    //Convert the angle into a vector
    b2Vec2 angleVector;
    angleVector.x = cos(radianAngle);
    angleVector.y = sin(radianAngle);

    if (m_pPhysicsBody != nullptr)
    {
        SetPhysicsBodyTransform(vec2(aLaunchPosition.x, aLaunchPosition.y), m_Rotation.z);//Set the position of the body to match the launch position, and be oriented to the angle it is being launched

        m_pPhysicsBody->ApplyLinearImpulse(b2Vec2((angleVector.x*m_speed)+ aLaunchingVelocity.x, (angleVector.y*m_speed)+ aLaunchingVelocity.y), m_pPhysicsBody->GetPosition(), true);//Move the enemy
    }

    SetEnabled(true);//Ensure the projectile is enabled
}

void Projectile::DisableProjectileFromPool()
{
		Reset();

		if (m_pScene != nullptr)
		{
			m_pScene->RemoveGameObject(m_Name);//Remove this game object from scene
		}

		if (m_pPoolOwner != nullptr)
		{
			m_pPoolOwner->DeactivateObject(this);//Set it as deactivated in the pool
		}
}