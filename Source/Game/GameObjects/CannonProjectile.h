#ifndef __CannonProjectile__H__
#define __CannonProjectile__H__
									   
class CannonProjectile :
    public GameObject
{
protected:
    Pool<CannonProjectile>* m_pPoolOwner;//the pool in which the enemy is
    double m_projectileLiveTimer;
	bool m_pendingPoolRemoval;

public:
    CannonProjectile(Pool<CannonProjectile>* aPool, const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
    ~CannonProjectile();

    virtual void Init(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0));
	void LaunchProjectile(vec3 aLaunchPosition, float aXYAngle, vec2 aLaunchingVelocity);
	void Update(double aDelta);
	void DisableProjectileFromPool();
    void Reset();

};

#endif