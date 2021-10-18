#include "CommonHeader.h"

FGPrismaticEnemy::FGPrismaticEnemy(int aCollisionDamage, int aHealth, double aShieldTimer, double aUnshieldTimer, int aShieldHealth, Scene* apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh* apMesh,
    ShaderProgram* apShader, GLuint aTexture, Mesh* apShieldMesh, ShaderProgram* apShieldShader, GLuint aShieldTexture,
    vec2 aUVScale, vec2 aUVOffset, const char* aCameraName, unsigned int aDrawRenderOrder) :
    PrismaticEnemy(aCollisionDamage, aHealth,aShieldTimer,aUnshieldTimer,aShieldHealth, apScene, aName, aPos, aRot, aScale, apMesh, apShader, aTexture, 
    apShieldMesh,apShieldShader,aShieldTexture,aUVScale, aUVOffset, aCameraName, aDrawRenderOrder)
{
    //Set it to collide with enviorment,player and its projectiles
    signed short maskBit = CollisionFilteringCategory_Enviroment |
        CollisionFilteringCategory_Player |
        CollisionFilteringCategory_PlayerProjectile;


    //Create physics body
    CreatePhysicsBox(vec2(GetXLength(), GetYLength()), b2_kinematicBody, vec2(0, 0), false, CollisionFilteringGroup_PowerUpsAndEnemies, CollisionFilteringCategory_Enemy, maskBit);
    m_pPhysicsBody->SetGravityScale(0);
    m_pPhysicsBody->SetFixedRotation(true);

    CreateShield(aShieldHealth, apShieldMesh, apShieldShader, aShieldTexture);

    Reset();
}

FGPrismaticEnemy::~FGPrismaticEnemy()
{
}

void FGPrismaticEnemy::Update(double aDelta)
{

    PrismaticEnemy::Update(aDelta);

}

void FGPrismaticEnemy::Draw(int aRenderorder)
{
    PrismaticEnemy::Draw(GetRenderOrder());//Draw the main object

}

void FGPrismaticEnemy::OnDeath()
{
    PrismaticEnemy::OnDeath();//Kill this object


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

void FGPrismaticEnemy::Reset()
{
    //Reset the object
    PrismaticEnemy::Reset();

}