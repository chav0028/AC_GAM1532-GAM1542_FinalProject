#ifndef __FloatingObject_H__
#define __FloatingObject_H__

class FloatingObject : public GameObject
{
public:

public:
    FloatingObject();
    FloatingObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture);
    virtual ~FloatingObject();

    virtual void Update(double TimePassed);
};

#endif //__FloatingObject_H__
