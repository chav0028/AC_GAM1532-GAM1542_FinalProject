/*
Student:    Alvaro Chavez Mixco
Date:       Tuesday, December 14 , 2015
course:     GAM1546-Games and Graphics Computations II
Professor:  Jimmy Lord
Purpose:    Apply the properties for which the projectile was fired

December 6
-In Particles.cpp, modified the fireParticle function so that it can fire a particle between a
//range of angles and a range of speed.
*/

#include "CommonHeader.h"

Particle::Particle(Pool<Particle>* aPool):
	m_position(Vector2(0.0f,0.0f)),
	m_launchSpeed(0),
    m_direction(Vector3(0,0,0)),
    m_color(Vector4(0, 0, 0, 0)),
	m_size(Vector2(0.0f,0.0f)),
	m_duration(0),
    m_entireLifeTime(0),
    m_poolOwner(aPool),
    m_constantAcceleration(Vector3(0,0,0)),
    m_enabled(false),
    m_fadeOut(true),
    m_velocity(Vector3(0,0,0))
{
    m_remainingTime = m_duration;//Reset timer
}

void Particle::SetParticle(Vector4 aColor, Vector3 aSize, double aDuration,vec3 aConstantAcceleration,bool aFadeOut)
{
    m_color = aColor;
    m_size = aSize;
    m_duration = aDuration;
    m_entireLifeTime = aDuration;
    m_constantAcceleration = aConstantAcceleration;
    m_remainingTime = m_duration;//Reset timer
    m_fadeOut = aFadeOut;
}


void Particle::FireParticle(Vector3 aStartingPosition, Vector3 aDirection,float aLaunchSpeed)
{
    m_enabled = true;
    m_position = aStartingPosition;
    m_direction = aDirection;
    m_launchSpeed = aLaunchSpeed;

    //Ensure direction has been normalized
    m_direction.Normalize();

    //Calculate the velocity
    m_velocity.x = m_direction.x* m_launchSpeed;
    m_velocity.y = m_direction.y* m_launchSpeed;
    m_velocity.z = m_direction.z* m_launchSpeed;
}

Particle::~Particle()
{
}

void Particle::Update(double aDelta)
{
    //If the particle is enabled
    if (m_enabled == true)
    {

        //Calculate the acceleration per frame
        vec3 accelerationPerFrame = m_constantAcceleration * (float)(aDelta*aDelta);

        //Calculate the new velocity
        m_velocity = m_velocity + accelerationPerFrame;

        m_position = m_position + m_velocity*(float)aDelta;//Add the amount we moved to the current position

        //Reduce the life of the parameter
        m_remainingTime -= aDelta;

        if (m_fadeOut == true)
        {
            m_color.w -= (float)m_duration / (float)m_entireLifeTime;//Set alpha according to percentage left
        }

        //If the time of the particle is over
        if (m_remainingTime <= 0)
        {
            m_enabled = false;//Disable the particle
            m_remainingTime = m_duration;//Restart the duration timer

            //If the particle belongs to a pool
            if (m_poolOwner != nullptr)
            {
               m_poolOwner->DeactivateObject(this);//Tell the pool to deactivate this particle
                m_remainingTime = m_duration;//Reset timer
            }
        }      
    }
}
