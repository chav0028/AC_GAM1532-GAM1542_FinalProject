#ifndef __MovingWall_H__
#define __MovingWall_H__

class MovingWall : public GameObject
{
protected:
	double m_changeMovementTimer;
	double m_timeChangeMovement;
	Vector2 m_velocity;

public:
	MovingWall(double timerToChange,Vector2 moveVelocity,Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture);
	virtual ~MovingWall();

	virtual void Update(double timeStep);

};

#endif