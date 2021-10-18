#ifndef __PARTICLESYSTEM__H__
#define __PARTICLESYSTEM__H__

class ParticleRenderer;
class ParticleEmitter;
class ShaderProgram;

//Basic class to tie together a particle emitter with a particle renderer
class ParticleSystem
{
public:
    ParticleSystem(Scene* aScene, ShaderProgram* pShader, GLuint texture = 0, bool particles3D = false, const char* cameraName = GAME_DEFAULT_CAMERA_NAME);
    ParticleSystem(unsigned int maxParticles, Scene* aScene, ShaderProgram* pShaderm, GLuint texture = 0, bool particles3D = false, const char* cameraName = GAME_DEFAULT_CAMERA_NAME);
	~ParticleSystem();

    void SetShaderProgram(ShaderProgram* pShader) { m_ShaderProgram = pShader; }
    ShaderProgram* GetShaderProgram() { return m_ShaderProgram; }

    void CreateAddParticleEmitter(unsigned int maxNumberParticles, Vector3 emitterPosition=Vector3(0,0,0),std::string emitterName = "");
    void AddParticleEmitter(ParticleEmitter* emitter);

	ParticleEmitter* GetParticleEmitter(unsigned int aIndex=0);
	ParticleRenderer* GetParticleRenderer() { return m_particleRenderer; }

	virtual void Update(double delta);
    virtual void Draw();

	Scene* GetScene() { return m_pScene; }

private:
	unsigned int m_maxParticles;

	ParticleRenderer* m_particleRenderer;
	std::vector<ParticleEmitter*> m_particleEmitters;

    ShaderProgram* m_ShaderProgram;

	Scene* m_pScene;
};

#endif