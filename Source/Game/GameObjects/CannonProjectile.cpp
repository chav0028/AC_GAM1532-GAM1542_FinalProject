#include "CommonHeader.h"

CannonProjectile::CannonProjectile(Pool<CannonProjectile>* aPool, const char* aCameraName, unsigned int aDrawRenderOrder):
m_pPoolOwner(aPool)
{
    SetCameraDrawerName(aCameraName);
    SetRenderOrder(aDrawRenderOrder);
	m_pendingPoolRemoval = false;
}

CannonProjectile::~CannonProjectile()
{
}

void CannonProjectile::Init(Scene * apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh * apMesh, ShaderProgram * apShader, GLuint aTexture, vec2 aUVScale, vec2 aUVOffset)
{
    GameObject::Init(apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aUVScale, aUVOffset);

    //Create physics body
    CreatePhysicsCircle(GetXYRadius(), b2_dynamicBody, vec2(0, 0), false, CannonSceneFilteringGroups_CannonAndProjectile);
}


//Only moves projectile in X and Y, the z value doesn't change
void CannonProjectile::LaunchProjectile(vec3 aLaunchPosition, float aXYAngle, vec2 aLaunchingVelocity)
{

	SetPhysicsAndGameObjectPosition(aLaunchPosition);
	ResetPhysicsBody();

	float radianAngle = aXYAngle*PI / 180;//Convert angle to radian

	//Convert the angle into a vector
	b2Vec2 angleVector;
	angleVector.x = cos(radianAngle);
	angleVector.y = sin(radianAngle);

	if (m_pPhysicsBody != nullptr)
	{
		SetPhysicsBodyTransform(vec2(aLaunchPosition.x, aLaunchPosition.y), m_Rotation.z);//Set the position of the body to match the launch position, and be oriented to the angle it is being launched

		//Not used since we want to launch with a specific speed, regardless of object mass
		//m_pPhysicsBody->ApplyLinearImpulse(b2Vec2((angleVector.x*m_speed) + aLaunchingVelocity.x, (angleVector.y*m_speed) + aLaunchingVelocity.y), m_pPhysicsBody->GetPosition(), true);//Move the enemy

		//Launch the projectile
		m_pPhysicsBody->SetLinearVelocity(b2Vec2((angleVector.x*m_speed) + aLaunchingVelocity.x, (angleVector.y*m_speed) + aLaunchingVelocity.y));
	}

	SetEnabled(true);//Ensure the projectile is enabled
}


void CannonProjectile::Update(double aDelta)
{
	m_projectileLiveTimer -= aDelta;//Decrease timer
	if (m_projectileLiveTimer <= 0)//if time is over
	{
		m_pendingPoolRemoval = true;
		SetEnabled(false);
	}

	GameObject::Update(aDelta);//Update the object, it will check the enabled status to see if it ihas to activate/deactivate the body

	if (m_pendingPoolRemoval == true)
	{
		DisableProjectileFromPool();//Remove the projectile from the pool.
	}


}

void CannonProjectile::DisableProjectileFromPool()
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
void CannonProjectile::Reset()
{
	m_projectileLiveTimer = PROJECTILE_TRAJECTORY_SECONDS_TO_PLOT;
	m_pendingPoolRemoval = false;
	SetEnabled(false);
	SetActivePhysicsBody(false);
}