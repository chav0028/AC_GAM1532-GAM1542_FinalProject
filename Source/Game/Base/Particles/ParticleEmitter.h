#ifndef __PARTICLEEMITTER__H__
#define __PARTICLEEMITTER__H__

#include "Particle.h"

class ParticleSystem;

/*The particle emitter stores the particles, updates them and is in charge of setting them and firing the,*/
class ParticleEmitter:public GameObject
{
public:
    ParticleEmitter(Scene* scene, std::string name,Vector3 position, unsigned int maxParticles, ParticleSystem* ownerParticleSystem);
    ~ParticleEmitter();

    void FireParticles(unsigned int amountOfParticles, Vector3 direction, float speed);
    
    void Update(double delta);
    
    void SetInactiveParticle(unsigned int index, Vector4 color, Vector3 size, double duration, Vector3 constantAcceleration = Vector3(0, 0, 0));
    void SetAllParticles(Vector4 color, Vector3 size, double duration, Vector3 constantAcceleration = Vector3(0, 0, 0));

	unsigned int GetMaxNumberParticles() { return m_maxNumberParticles; }
    
    void SetSpeedRange(float aRange){ m_speedRange = aRange; }
    void SetFiringAngleRange(Vector3 aMinRange, Vector3 aMaxRange);
    Vector3 GetMinFiringAngleRange(){ return m_minDirectionRange; }
    Vector3 GetMaxFiringAngleRange(){ return m_maxDirectionRange; }
    void SetSize(Vector3 aSize){ m_size = aSize; }

    void SetUseSpeedRange(bool aStatus){ m_useSpeedRange = aStatus; }
    void SetUseDirectionRange(bool aStatus){ m_useDirectionRange = aStatus; }
    void SetUseRandomEmitterPosition(bool aStatus){ m_useRandomEmitterPosition = aStatus; }

    void SetBurstingProperties(bool burstingOn, unsigned int numParticles, Vector3 fireDirection, float burstSpeed, double burstTime);

private:
    //Burst variables
    bool m_bursting;    
    unsigned int m_burstFireNumParticles;
    Vector3 m_burstFireDirection;
    float m_burstFireSpeed;
    double m_burstFireTime;
    double m_burstTimer;

	//Variables used when generating a "random" firing range
    bool m_useSpeedRange;
    bool m_useDirectionRange;
    bool m_useRandomEmitterPosition;
    Vector3 m_maxDirectionRange;
    Vector3 m_minDirectionRange;
	float m_speedRange;

	unsigned int m_maxNumberParticles;

    Vector3 m_size;

    Pool<Particle>m_poolParticles;
    ParticleSystem* m_ownerParticleSystem;
};

#endif