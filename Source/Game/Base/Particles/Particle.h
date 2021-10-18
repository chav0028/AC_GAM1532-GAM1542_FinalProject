#ifndef __PARTICLE__H__
#define __PARTICLE__H__

#include "Game/Base/Pool.h"

const Vector3 PARTICLE_DEFAULT_SIZE = Vector3(2.0f, 2.0f,2.0f);
const Vector4Int PARTICLE_DEFAULT_COLOR = Vector4Int(255, 255, 0, 255);
const double PARTICLE_DEFAULT_DURATION = 1;
const Vector3 PARTICLES_DEFAULT_SCALE = Vector3(1, 1,1);
const Vector2 PARTICLES_DEFAULT_UV_SCALE = Vector2(1, 1);
const unsigned int PARTICLES_RENDERER_MAX_NUMBER_OF_PARTICLES = 100000;//The max number of particles that can be displayed//100000
const unsigned int PARTICLES_RENDERER_3D_MAX_NUMBER_OF_PARTICLES = 100000;//The max number of particles that can be displayed//100000

/*The particel handles moving and reducing its timer according to the position it has been given.*/
class Particle//Not inherited from gameobject, since because of the amount we will have on screen,being inherited from it may affect performance
{
public:
    Particle();
    Particle(Pool<Particle>* pool);
    ~Particle();

    void SetParticle(Vector4 color, Vector3 size, double duration,Vector3 constantAcceleration=Vector3(0,0,0),bool fadeOut=true);

    void Update(double delta);

    void FireParticle(Vector3 startingPosition, Vector3 direction, float launchSpeed);

    Vector3 GetPosition() { return m_position; }
    void SetPosition(Vector3 aPosition) { m_position = aPosition; }

    Vector3 GetSize() { return m_size; }
    void SetSize(Vector3 aSize) { m_size = aSize; }

    float GetLaunchSpeed() { return m_launchSpeed; }
    void SetLaunchSpeed(float aSpeed) { m_launchSpeed = aSpeed; }

    Vector3 GetDirection(){ return m_direction; }
    void SetDirection(Vector3 aDirection){ m_direction = aDirection; }

    void SetDuration(double aDuration) { m_duration = aDuration; }
    double GetDuration() { return m_duration; }

    void SetColor(Vector4 aColor) { m_color = aColor; }
    Vector4 GetColor() { return m_color; }

private:
    Vector3 m_position;
	float m_launchSpeed;
    Vector3 m_velocity;
    Vector3 m_direction;
    Vector4 m_color;
    Vector3 m_size;
    double m_duration;
    double m_entireLifeTime;
    Vector3 m_constantAcceleration;
    double m_remainingTime;//Counter of the time the projectile still has left
    bool m_enabled;
    bool m_fadeOut;

    Pool<Particle>* m_poolOwner;//the pool in which the projectile is
};

#endif