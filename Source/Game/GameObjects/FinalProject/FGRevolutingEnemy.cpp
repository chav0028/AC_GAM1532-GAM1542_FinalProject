#include "CommonHeader.h"

using namespace std;

FGRevolutingEnemy::FGRevolutingEnemy(unsigned int numArms, int aCollisionDamage, int aHealth, int aArmHealth, Scene* apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh* apMesh, ShaderProgram* apShader, GLuint aTexture,
    Mesh* aArmMesh, ShaderProgram* apArmShader, GLuint aArmTexture, vec2 aUVScale, vec2 aUVOffset, const char* aCameraName, unsigned int aDrawRenderOrder) :
    RevolutingEnemy(numArms, aCollisionDamage, aHealth, aArmHealth, apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, aArmMesh, apArmShader,aArmTexture, aUVScale, aUVOffset, aCameraName, aDrawRenderOrder)
{

    //SetFixtureCollisionsFiltering();
    //SetArmsCollisionFiltering
}


FGRevolutingEnemy::~FGRevolutingEnemy()
{

}

void FGRevolutingEnemy::Update(double aDelta)
{
    RevolutingEnemy::Update(aDelta);
}

void FGRevolutingEnemy::Draw(int aRenderorder)
{
    RevolutingEnemy::Draw(aRenderorder);
}

void FGRevolutingEnemy::OnDeath()
{
    RevolutingEnemy::OnDeath();//Kill this object and the arms

    //Search all the children
    for (unsigned int i = 0; m_pChildObjects.at(i); i++)
    {
        if (m_pChildObjects.at(i) != nullptr)
        {
            ParticleEmitter* emitter = dynamic_cast<ParticleEmitter*> (m_pChildObjects.at(i));//If the children is a emitter

            if (emitter != nullptr)
            {
                emitter->FireParticles(20, vec3(0, 0, 1), 20);//Fire particle
            }
        }
    }
}

void FGRevolutingEnemy::Reset()
{
    RevolutingEnemy::Reset();

}