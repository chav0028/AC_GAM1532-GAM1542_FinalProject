#ifndef __FloatingPlayerObject_H__
#define __FloatingPlayerObject_H__

const float FLOATING_PLAYER_COLLECTION_DISTANCE = 1;
const float FLOATING_PLAYER_ORBITING_RADIUS = 5.0f;
const float FLOATING_PLAYER_ORBITING_SPEED = 1.0f;
const float FLOATING_PLAYER_DEFAULT_MOVEMENT_SPEED = 5.0f;
const float FLOATING_PLAYER_OBJECT_ATTACHED_SPEED_BONUS_SCALE = 0.5f;

class FloatingPlayerObject : public FloatingObject
{
protected:
    unsigned int m_numObjectsAttached;
    float m_currentOrbitingAngle;//Made a double to prevent overflow

public:
    FloatingPlayerObject();
    FloatingPlayerObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture);
    virtual ~FloatingPlayerObject();

	virtual void Update(double TimePassed);
	bool HandleInput(InputEvent& aInputevent, double aDelta);

	virtual void CheckCloseObjects();
	virtual void OrbitAttachedObjects(double timePassed);

};

#endif //__FloatingPlayerObject_H__
