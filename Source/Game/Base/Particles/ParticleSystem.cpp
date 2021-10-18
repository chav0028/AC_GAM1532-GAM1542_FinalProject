/*
Student:    Alvaro Chavez Mixco
Date:       Tuesday, December 14 , 2015
course:     GAM1546-Games and Graphics Computations II
Professor:  Jimmy Lord
Purpose:    Link togetheer the particle emitters with the renderer

December 6
--Modified ParticleSystem.cpp so that it has function updateMaxParticlesCount so that it can keep track of the total max number of particles according to all the emitters.
-In ParticleEmitter.cpp, modified the update method so that all the active particles are added to the renderer;
*/


#include "CommonHeader.h"

///Creates the renderer
ParticleSystem::ParticleSystem(Scene* aScene, ShaderProgram* pShader, GLuint aTexture, bool aParticles3D, const char* aCameraName) :
m_ShaderProgram(pShader),
	m_maxParticles(0),
	m_particleRenderer(nullptr),
	m_pScene(aScene)
{
    if (aParticles3D == true)
    {
        m_particleRenderer = new ParticleRenderer3D(this, aTexture, aCameraName);
    }
    else
    {
        m_particleRenderer = new ParticleRenderer(this, aTexture, aCameraName);
    }

}

//Creates the render and a emitter
ParticleSystem::ParticleSystem(unsigned int aMaxParticles, Scene* aScene, ShaderProgram* pShader, GLuint aTexture, bool aParticles3D, const char* aCameraName) :
m_ShaderProgram(pShader),
	m_particleRenderer(nullptr),
	m_pScene(aScene)
{
	CreateAddParticleEmitter(aMaxParticles);//Create the particle emitter, this is done first. So that ther renderer
	//can create a block of memory of the right size.

    if (aParticles3D == true)
    {
        m_particleRenderer = new ParticleRenderer3D(this, aTexture,aCameraName);
    }
    else
    {
        m_particleRenderer = new ParticleRenderer(this, aTexture, aCameraName);
    }

}


ParticleSystem::~ParticleSystem()
{
	//Delete all the emitters
	while (m_particleEmitters.empty() == false)
	{
		//Delete the emitter
		if (m_particleEmitters.back() != nullptr)
		{
			delete m_particleEmitters.back();
			m_particleEmitters.back() = nullptr;
		}

		//Remove from vector
		m_particleEmitters.pop_back();
	}

	//Delete the renderer
	if (m_particleRenderer != nullptr)
	{
		delete m_particleRenderer;
		m_particleRenderer = nullptr;
	}

}


void ParticleSystem::CreateAddParticleEmitter(unsigned int aMaxNumberParticles,Vector3 aEmmiterPosition,std::string aEmitterName)
{
    //If no name for emitter is given, number the name
    if (aEmitterName == "")
    {
        aEmitterName = "Emitter" + std::to_string(m_particleEmitters.size());
    }

    m_particleEmitters.push_back(new ParticleEmitter(m_pScene, aEmitterName, aEmmiterPosition, aMaxNumberParticles, this));//Create and add a new emiiter
}

void ParticleSystem::AddParticleEmitter(ParticleEmitter * aEmitter)
{
	if (aEmitter != nullptr)
	{
		m_particleEmitters.push_back(aEmitter);//Add the emiter to vector
	}
}

ParticleEmitter * ParticleSystem::GetParticleEmitter(unsigned int aIndex)
{
	if (aIndex < m_particleEmitters.size())
	{
		return m_particleEmitters.at(aIndex);//Return the emitter at the index
	}

	return nullptr;//If the index is out of range return null
}

void ParticleSystem::Update(double aDelta)
{
	//Reset the renderer
	if (m_particleRenderer != nullptr)
	{
		//Reset the particle render
		m_particleRenderer->SetNumberOfParticles(0);
	}

	//Update all the emitters
	for (unsigned int i = 0; i < m_particleEmitters.size(); i++)
	{
		if (m_particleEmitters.at(i) != nullptr)
		{
			if (m_particleEmitters.at(i)->GetEnabled() == true)
			{
				m_particleEmitters.at(i)->Update(aDelta);
			}
		}
	}

}

void ParticleSystem::Draw()
{
	if (m_particleRenderer != nullptr)
	{
        m_particleRenderer->Draw();//Draw the particle renderer
	}
}