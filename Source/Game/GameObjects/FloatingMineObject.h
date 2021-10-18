#ifndef __FloatingMineObject_H__
#define __FloatingMineObject_H__

#include "FloatingObject.h"

class FloatingMineObject : public FloatingObject
{
public:
    bool m_AttachedToPlayer;

public:
    FloatingMineObject();
    FloatingMineObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture);
    virtual ~FloatingMineObject();

    virtual void Update(double TimePassed);
};

#endif //__FloatingMineObject_H__
