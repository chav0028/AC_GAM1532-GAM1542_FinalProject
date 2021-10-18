#include "CommonHeader.h"

ElevatorObject::ElevatorObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale , vec2 UVOffset ) :
GameObject(pScene, name, pos, rot, scale, pMesh, pShader, texture, UVScale, UVOffset)
{
    
}

void ElevatorObject::BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, bool touching)
{
    if (m_pPhysicsBody != nullptr)
    {     
        m_pPhysicsBody->SetLinearVelocity(b2Vec2(0.0f, 1));
    }


}