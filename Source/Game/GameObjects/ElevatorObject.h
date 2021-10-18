#ifndef __Elevator_H__
#define __Elevator_H__

class ElevatorObject : public GameObject
{
protected:

public:
    ElevatorObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0));
    virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, bool touching);
};

#endif //__PlayerObject_H__
