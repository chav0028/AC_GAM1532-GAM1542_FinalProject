#ifndef __PARTICLERENDERER__H__
#define __PARTICLERENDERER__H__

class ShaderProgram;
class ParticleSystem;

const unsigned int PARTICLE_NUMBER_VERTICES = 4;
const unsigned int PARTICLE_NUMBER_INDICES = 6;

/*the classs uses Dynamic Vertex  generation to draw the particles.*/
class ParticleRenderer
{
public:
    ParticleRenderer(ParticleSystem* ownerParticleSystem, GLuint texture = 0, const char* cameraName = GAME_DEFAULT_CAMERA_NAME);
    ParticleRenderer(bool setRenderer,ParticleSystem* ownerParticleSystem, GLuint texture = 0, const char* cameraName = GAME_DEFAULT_CAMERA_NAME);
    ~ParticleRenderer();

	virtual void AddParticle(Particle particle);
    void Draw();

    void SetNumberOfParticles(unsigned int aNumber){ m_numberOfParticles = aNumber; }
	virtual void SetUpRenderer();

    void SetTexture(GLuint texture);

    virtual void SetCameraDrawerName(const char* aCameraName){ m_cameraDrawerName = aCameraName; }
    virtual const char* GetCameraDrawerName(){ return m_cameraDrawerName; }

	void SetBillboardParticles(bool aState) { m_billboardParticles = aState; }
	bool GetBillobardParticles() { return m_billboardParticles; }

	void PlotObjectSpaceParticles(Vector3 particleSize);

protected:
	VertexFormat* m_blockOfMemory;
	GLuint m_VBO;
	GLuint m_IBO;

	virtual void FillVBO();
	virtual void FillIBO();

	unsigned int m_numberOfIndices;
	unsigned int m_numberOfParticles;

	ShaderProgram* m_pShader;
	ParticleSystem* m_ownerParticleSystem;

	GLuint m_texture;

	void SetAttributes(ShaderProgram* pShaderProgram);

private:
    bool m_billboardParticles;
    const char* m_cameraDrawerName;

	std::vector<Vector3> m_objectSpaceVertices;

};

#endif
