#ifndef __PARTICLERENDERER3D__H__
#define __PARTICLERENDERER3D__H__

class ShaderProgram;
class ParticleSystem;

const unsigned int PARTICLE3D_NUMBER_VERTICES = 24;
const unsigned int PARTICLE3D_NUMBER_INDICES = 36;

/*the classs uses Dynamic Vertex  generation to draw the particles.*/
class ParticleRenderer3D :public ParticleRenderer
{
public:
    ParticleRenderer3D(ParticleSystem* ownerParticleSystem, GLuint texture = 0, const char* cameraName = GAME_DEFAULT_CAMERA_NAME);
    ~ParticleRenderer3D();

	void AddParticle(Particle particle);

    void SetUpRenderer();

private:
	void FillVBO();
	void FillIBO();

};

#endif
