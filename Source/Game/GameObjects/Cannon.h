#ifndef __Cannon_H__
#define __Cannon_H__


const float CANNON_ROTATION_SPEED=50.0f;  
const unsigned int PROJECTILE_TRAJECTORY_SECONDS_TO_PLOT = 10;

const unsigned int CANNON_DEFAULT_NUMBER_PROJECTILES = 10;

class CannonProjectile;

class Cannon : public GameObject
{
protected:
    Pool<CannonProjectile> m_poolProjectiles;
    b2RevoluteJoint* m_revoluteJoint;


    Mesh* m_pMeshTrajectory;
    float m_launchSpeed;

    std::vector<VertexFormat> m_trajectoryPoints;
	Box2DRayCast m_trajectoryRayCast;
	unsigned int m_numberActiveTrajectoryPoints;
	Vector2 m_projectileSize;

public:
    Cannon(b2Body* aBodyBase, float launchSpeed,Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture);
    virtual ~Cannon();

    virtual void Update(double timeStep);
	virtual void Draw(int renderorder);
    bool HandleInput(InputEvent& aInputevent, double aDelta);

	Vector3 CalculateLaunchPositionAndDirectionVector(vec2* directionVector,float* directionAngle);
    void FireProjectile();

    void CalculateTrajectory();
	void DrawTrajectory(int aRenderOrder);
	void RayCastTrajectory(unsigned int aStartingIndex = 1);


};

#endif
