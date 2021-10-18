#include "CommonHeader.h"

MovingWall::MovingWall(double timerToChange,Vector2 aMoveVelocity,Scene * apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh * apMesh, ShaderProgram * apShader, GLuint aTexture):GameObject(apScene,aName,aPos,aRot,aScale,apMesh,apShader,aTexture),
m_timeChangeMovement(timerToChange),
m_changeMovementTimer(timerToChange),
m_velocity(aMoveVelocity)
{
	CreatePhysicsBox(vec2(GetXLength(), GetYLength()), b2_kinematicBody);
	m_pPhysicsBody->SetLinearVelocity(b2Vec2(m_velocity.x, m_velocity.y));
}

MovingWall::~MovingWall()
{
}

void MovingWall::Update(double aTimeStep)
{
	//Substract to timer
	m_changeMovementTimer -= aTimeStep;

	if (m_changeMovementTimer <= 0)
	{
		m_changeMovementTimer += m_timeChangeMovement;//Reset timer
		m_velocity *= -1;//Inverse velocity
		
		if (m_pPhysicsBody != nullptr)
		{
			m_pPhysicsBody->SetLinearVelocity(b2Vec2(m_velocity.x, m_velocity.y));	//Set physics body velocity
		}
	}

	GameObject::Update(aTimeStep);

}
