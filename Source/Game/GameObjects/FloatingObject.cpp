#include "CommonHeader.h"

FloatingObject::FloatingObject()
{
}

FloatingObject::FloatingObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture)
{
    Init( pScene, name, pos, rot, scale, pMesh, pShader, texture );
}

FloatingObject::~FloatingObject()
{
}

void FloatingObject::Update(double TimePassed)
{
    GameObject::Update( TimePassed );

    //TODO: make the objects float on the water
    
    //Shader modifies plane in world space. So same formula can be aplied here.
    m_Position.y = 0;//Set Y  position to 0, so that it matches the vertex position the shader receives

	//Wave movement, must match the one in wave shader
    m_Position.y += sin(m_Position.x + (float)MyGetSystemTime() * 0.5f)*0.6f;
    m_Position.y += sin(m_Position.z + (float)MyGetSystemTime() * 2.0f)*0.2f;
}
