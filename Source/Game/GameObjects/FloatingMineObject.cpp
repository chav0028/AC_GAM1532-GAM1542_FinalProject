#include "CommonHeader.h"

FloatingMineObject::FloatingMineObject()
{
    m_AttachedToPlayer = false;
}

FloatingMineObject::FloatingMineObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture)
{
    Init( pScene, name, pos, rot, scale, pMesh, pShader, texture );
    m_AttachedToPlayer = false;
}

FloatingMineObject::~FloatingMineObject()
{
}

void FloatingMineObject::Update(double TimePassed)
{
    FloatingObject::Update( TimePassed );
}
