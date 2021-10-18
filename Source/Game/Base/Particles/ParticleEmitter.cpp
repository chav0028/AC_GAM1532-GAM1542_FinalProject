/*
Student:    Alvaro Chavez Mixco
Date : Tuesday, December 14, 2015
course : GAM1546 - Games and Graphics Computations II
Professor : Jimmy Lord
Purpose : Emits the particles from a pool

December 14
-Chande emitter to have more random values
*/


#include "CommonHeader.h"

using namespace std;

ParticleEmitter::ParticleEmitter(Scene* aScene,string aName,Vector3 aPosition, unsigned int aMaxParticles, ParticleSystem* ownerParticleSystem) :GameObject(aScene,aName,aPosition,
    Vector3(0,0,0),Vector3(1,1,1),nullptr,nullptr,0),
m_poolParticles(aMaxParticles),
m_maxNumberParticles(aMaxParticles),
m_ownerParticleSystem(ownerParticleSystem),
m_minDirectionRange(Vector3(0,0,0)),
m_maxDirectionRange(Vector3(0, 0, 0)),
m_speedRange(0),
m_size(Vector3(1, 1,1)),
m_useDirectionRange(false),
m_useSpeedRange(false),
m_useRandomEmitterPosition(false),
m_bursting(0),
m_burstFireNumParticles(0),
m_burstFireDirection(0),
m_burstFireSpeed(0),
m_burstFireTime(0),
m_burstTimer(0)
{
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::FireParticles(unsigned int aAmountOfParticles, Vector3 aDirection, float aSpeed)
{
        for (unsigned int i = 0; i < aAmountOfParticles; i++)
        {
            Particle* tempParticle = m_poolParticles.GetActivateObject();//Get and activate a particle
            Vector3 fireDirection;//The actual angle in which the projectile will be fired
            float fireSpeed; //The actual speed in which the projectile will be fired
            Vector3 launctPosition=GetWorldPosition();

            if (tempParticle != nullptr)//Check it is a valid particle
            {
                //Check if the particle will need a random firing angle
                if (m_useDirectionRange==true )//If the particle angle is going to be from a random range
                {

                    //X Direction
                    float minAngleX = aDirection.x + m_minDirectionRange.x;//Get the least possible firing angle
                    float maxAngleX = aDirection.x + m_maxDirectionRange.x;//Get the max possible firing range

                    //min + (std::rand() % (max - min + 1))- Get a random number between a range
                    fireDirection.x = minAngleX + rand() % (unsigned int)(maxAngleX - minAngleX + 1);//Get a random number between min angle and max angle.Cast to unsigned int for rand to work 
               
                    //Y Direction
                    float minAngleY = aDirection.y + m_minDirectionRange.y;//Get the least possible firing angle
                    float maxAngleY = aDirection.y + m_maxDirectionRange.y;//Get the max possible firing range

                    //min + (std::rand() % (max - min + 1))- Get a random number between a range
                    fireDirection.y = minAngleY + rand() % (unsigned int)(maxAngleY - minAngleY + 1);//Get a random number between min angle and max angle.Cast to unsigned int for rand to work 

                    //Z Direction
                    float minAngleZ = aDirection.z + m_minDirectionRange.z;//Get the least possible firing angle
                    float maxAngleZ = aDirection.z + m_maxDirectionRange.z;//Get the max possible firing range

                    //min + (std::rand() % (max - min + 1))- Get a random number between a range
                    fireDirection.z= minAngleZ + rand() % (unsigned int)(maxAngleZ - minAngleZ + 1);//Get a random number between min angle and max angle.Cast to unsigned int for rand to work           
                
                }
                else//If the particle is going to have an exact angle
                {
                    fireDirection = aDirection;
                }

                //Check if the particle will need a random speed
                if (m_useSpeedRange==true)//If the particle speed is going to be from a random range
                {
                    //X speed
                    float minSpeed= aSpeed - m_speedRange / 2.0f;//Get the least possible firing angle
                    float maxSpeed = aSpeed+ m_speedRange / 2.0f;//Get the max possible firing range

                    //min + (std::rand() % (max - min + 1))- Get a random number between a range
                    fireSpeed = minSpeed + std::rand() % (unsigned int)(maxSpeed - minSpeed + 1);//Get a random number between min speed and max speed.Cast to unsigned int for rand to work with % 
                
                }
                else//If the particle is going to have an exact angle
                {
                    fireSpeed = aSpeed;
                }

                if (m_useRandomEmitterPosition == true)
                {
                    //Calculate a random position inside the size of the emitter
                    float halfWidth = (m_size.x*m_Scale.x) / 2.0f;
                    float halfHeight = (m_size.y*m_Scale.y) / 2.0f;
                    float halfDepth = (m_size.z*m_Scale.z) / 2.0f;
                    float minX = launctPosition.x - halfWidth;
                    float maxX = launctPosition.x + halfWidth;
                    float minY = launctPosition.y - halfHeight;
                    float maxY = launctPosition.y + halfHeight;
                    float minZ = launctPosition.z - halfDepth;
                    float maxZ = launctPosition.z + halfDepth;

                    //Get a random x,y and Z value
                    launctPosition.x = minX + rand() % (unsigned int)(maxX - minX + 1);
                    launctPosition.y = minY + rand() % (unsigned int)(maxY - minY + 1);
                    launctPosition.z = minZ + rand() % (unsigned int)(maxZ - minZ + 1);
                }

				fireDirection.Normalize();//Normalize the direction
                tempParticle->FireParticle(launctPosition, fireDirection, fireSpeed);//Fire the particle
            }
        }
}


void ParticleEmitter::Update(double aDelta)
{
    //Check if the emiiter is set to fire atuomatically
    if (m_bursting == true)
    {
        m_burstTimer -= aDelta;

        //If time is over
        if (m_burstTimer <= 0)
        {
            m_burstTimer = m_burstFireTime;
            FireParticles(m_burstFireNumParticles, m_burstFireDirection, m_burstFireSpeed);//Fire particles
        }

    }



    std::vector<Particle*> currrentlyActiveParticles = m_poolParticles.GetCurrentlyActiveObjects();//Get the particles that are active

	//Plot the size of the particles according to their camera
	ParticleRenderer* renderer = m_ownerParticleSystem->GetParticleRenderer();
	if (renderer != nullptr && currrentlyActiveParticles.empty()==false)
	{
		if (currrentlyActiveParticles.at(0) != nullptr)
		{
			//Set the size of all particles according to the first one found
			renderer->PlotObjectSpaceParticles(currrentlyActiveParticles.at(0)->GetSize());
		}
	}

    //Go through all the active particles
	for (unsigned int i = 0; i < currrentlyActiveParticles.size(); i++)
	{
		if (currrentlyActiveParticles.at(i) != nullptr)
		{
			currrentlyActiveParticles.at(i)->Update(aDelta);//Update each particle

			//Add all the active particles to the renderer so that they are drawn
            if (renderer != nullptr)
            {   
                renderer->AddParticle(*currrentlyActiveParticles.at(i));
            }
		}
	}
}

void ParticleEmitter::SetInactiveParticle(unsigned int aIndex, Vector4 aColor, Vector3 aSize, double aDuration,vec3 aConstantAcceleration)
{
    if (aIndex < m_maxNumberParticles)//If it is a valid index
    {
        Particle* inactiveParticle = m_poolParticles.GetInactiveObject(aIndex);//Get an inactive particle DOESN'T acitvate it

        if (inactiveParticle!=nullptr)
        {
            inactiveParticle->SetParticle(aColor, aSize, aDuration, aConstantAcceleration);//Set the particle
        }
    }
}

void ParticleEmitter::SetAllParticles(Vector4 aColor, Vector3 aSize, double aDuration,Vector3 aParticleConstantAcceleration)
{
    //Set the active particles
    std::vector<Particle*> currrentlyActiveParticles = m_poolParticles.GetCurrentlyActiveObjects();//Get the particles that are active

    for (unsigned int i = 0; i < currrentlyActiveParticles.size(); i++)//Go through all the active particles
    {
        currrentlyActiveParticles.at(i)->SetParticle(aColor, aSize, aDuration, aParticleConstantAcceleration);//set each particle
    }

    //Set the inactive particles
    std::vector<Particle*> currrentlyInactiveParticles = m_poolParticles.GetCurrentlyInactiveObjects();//Get the particles that are inactive

    for (unsigned int i = 0; i < currrentlyInactiveParticles.size(); i++)//Go through all the inactive particles
    {
        currrentlyInactiveParticles.at(i)->SetParticle(aColor, aSize, aDuration, aParticleConstantAcceleration);//set each particle
    }

}

void ParticleEmitter::SetFiringAngleRange(Vector3 aMinRange, Vector3 aMaxRange)
{ 
    //Assign the values
    m_minDirectionRange = aMinRange; 
    m_maxDirectionRange = aMaxRange;

    //Normalize the directions
    ///m_minDirectionRange.Normalize();
    //m_maxDirectionRange.Normalize();
}

void ParticleEmitter::SetBurstingProperties(bool burstingOn, unsigned int numParticles, Vector3 fireDirection, float burstSpeed, double burstTime)
{
    m_bursting = burstingOn;
    m_burstFireNumParticles = numParticles;
    m_burstFireDirection = fireDirection;
    m_burstFireSpeed = burstSpeed;
    m_burstFireTime = burstTime;
    m_burstTimer = burstTime;
}