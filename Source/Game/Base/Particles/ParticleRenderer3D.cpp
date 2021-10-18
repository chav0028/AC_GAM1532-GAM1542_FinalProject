#include "CommonHeader.h"

ParticleRenderer3D::ParticleRenderer3D(ParticleSystem* aOwnerParticleSystem, GLuint aTexture, const char* aCameraName) : 
ParticleRenderer(false,aOwnerParticleSystem, aTexture, aCameraName)
{
    //All in done particle renderer constructor
    SetUpRenderer();
}

ParticleRenderer3D::~ParticleRenderer3D()
{
    //All cleaned up in particle render parent
}

//Creates the block of memory, and fills the IBO
void ParticleRenderer3D::SetUpRenderer()
{
	//Create a block of memory
    m_blockOfMemory = new VertexFormat[PARTICLES_RENDERER_3D_MAX_NUMBER_OF_PARTICLES * PARTICLE3D_NUMBER_VERTICES];//create an array with the maximum number of particles *24 because it will have 24 indices

	//Fill the IBO
	FillIBO();
}

void ParticleRenderer3D::FillVBO()
{
	// fill buffer with our attributes
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * (m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES), m_blockOfMemory, GL_DYNAMIC_DRAW);

	// check for errors
	CheckForGLErrors();
}

void ParticleRenderer3D::FillIBO()
{
	//Generate IBO
	glGenBuffers(1, &m_IBO);

    unsigned int maxNumParticles = PARTICLES_RENDERER_3D_MAX_NUMBER_OF_PARTICLES;

	if (maxNumParticles > 0)//If there are actually particles to render
	{
		unsigned int* indices;
        indices = new unsigned int[maxNumParticles * PARTICLE3D_NUMBER_INDICES];//* 36 because it uses 36 indices

		//Fill the indices array
		for (unsigned int i = 0; i < maxNumParticles; i++)
		{
			//I*36 is the number of the particle currently being modified, since each particle has 36 indices
			
            //Front face
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 0] = i * PARTICLE3D_NUMBER_VERTICES + 0;//i*24 represents the vertices, since the particle has 24 vertice.
			indices[(i * PARTICLE3D_NUMBER_INDICES) + 1] = i * PARTICLE3D_NUMBER_VERTICES + 1;
			indices[(i * PARTICLE3D_NUMBER_INDICES) + 2] = i * PARTICLE3D_NUMBER_VERTICES + 2;
			indices[(i * PARTICLE3D_NUMBER_INDICES) + 3] = i * PARTICLE3D_NUMBER_VERTICES + 0;
			indices[(i * PARTICLE3D_NUMBER_INDICES) + 4] = i * PARTICLE3D_NUMBER_VERTICES + 2;
			indices[(i * PARTICLE3D_NUMBER_INDICES) + 5] = i * PARTICLE3D_NUMBER_VERTICES + 3;

            //Top face
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 6] = i * PARTICLE3D_NUMBER_VERTICES + 4;//i*24 represents the vertices, since the particle has 24 vertice.
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 7] = i * PARTICLE3D_NUMBER_VERTICES + 5;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 8] = i * PARTICLE3D_NUMBER_VERTICES + 6;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 9] = i * PARTICLE3D_NUMBER_VERTICES + 4;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 10] = i * PARTICLE3D_NUMBER_VERTICES + 6;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 11] = i * PARTICLE3D_NUMBER_VERTICES + 7;

            //Right face
            indices[(i * PARTICLE3D_NUMBER_INDICES) +12] = i * PARTICLE3D_NUMBER_VERTICES + 8;//i*24 represents the vertices, since the particle has 24 vertice.
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 13] = i * PARTICLE3D_NUMBER_VERTICES + 9;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 14] = i * PARTICLE3D_NUMBER_VERTICES + 10;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 15] = i * PARTICLE3D_NUMBER_VERTICES + 8;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 16] = i * PARTICLE3D_NUMBER_VERTICES + 10;
            indices[(i * PARTICLE3D_NUMBER_INDICES) +17] = i * PARTICLE3D_NUMBER_VERTICES + 11;

            //Bottom face
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 18] = i * PARTICLE3D_NUMBER_VERTICES + 12;//i*24 represents the vertices, since the particle has 24 vertice.
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 19] = i * PARTICLE3D_NUMBER_VERTICES + 13;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 20] = i * PARTICLE3D_NUMBER_VERTICES + 14;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 21] = i * PARTICLE3D_NUMBER_VERTICES + 12;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 22] = i * PARTICLE3D_NUMBER_VERTICES + 14;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 23] = i * PARTICLE3D_NUMBER_VERTICES + 15;

            //Left face
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 24] = i * PARTICLE3D_NUMBER_VERTICES + 19;//i*24 represents the vertices, since the particle has 24 vertice.
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 25] = i * PARTICLE3D_NUMBER_VERTICES + 18;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 26] = i * PARTICLE3D_NUMBER_VERTICES + 16;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 27] = i * PARTICLE3D_NUMBER_VERTICES + 18;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 28] = i * PARTICLE3D_NUMBER_VERTICES + 17;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 29] = i * PARTICLE3D_NUMBER_VERTICES + 16;

            //Back face
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 30] = i * PARTICLE3D_NUMBER_VERTICES + 23;//i*24 represents the vertices, since the particle has 24 vertice.
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 31] = i * PARTICLE3D_NUMBER_VERTICES + 22;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 32] = i * PARTICLE3D_NUMBER_VERTICES + 20;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 33] = i * PARTICLE3D_NUMBER_VERTICES + 22;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 34] = i * PARTICLE3D_NUMBER_VERTICES + 21;
            indices[(i * PARTICLE3D_NUMBER_INDICES) + 35] = i * PARTICLE3D_NUMBER_VERTICES + 20;
		}

		//Generate the IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);//Use elemnt array buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * (maxNumParticles * PARTICLE3D_NUMBER_INDICES), indices, GL_STATIC_DRAW);

		delete[] indices;
	}
}

void ParticleRenderer3D::AddParticle(Particle aParticle)
{
	//Check we aren't over the particle limit
	if (m_numberOfParticles < PARTICLES_RENDERER_3D_MAX_NUMBER_OF_PARTICLES)
	{
		//Get properties of particle
		Vector3 halfSize = aParticle.GetSize() / 2;
		Vector3 particlePosition = aParticle.GetPosition();
		Vector4 particleColor = aParticle.GetColor();

		//Copy the data into the block of memory
        float widthUVface = (1.0f / 6.0f);//The percent width of a UV

        //Front face                                                                                         //Positions if face seen straight on
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 0].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y - halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(0.0f, 0.0f), Vector3(0, 0, -1));//Bottom left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 1].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y + halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(0.0f, 1.0f), Vector3(0, 0, -1));//Top left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 2].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y + halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface, 1.0f), Vector3(0, 0, -1));//Top right
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 3].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y - halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface, 0.0f), Vector3(0, 0, -1));//Bottom right
                                                                                               
        //Top face																											
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 4].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y + halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface * 2.0f, 0.0f), Vector3(0, 1, 0));//Bottom left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 5].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y + halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 2.0f, 1.0f), Vector3(0, 1, 0));//Top left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 6].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y + halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 3.0f, 1.0f), Vector3(0, 1, 0));//Top right
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 7].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y + halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface * 3.0f, 0.0f), Vector3(0, 1, 0));//Bottom right

        //Right face																											
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 8].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y - halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface * 1.0f, 0.0f), Vector3(1, 0, 0));//Bottom left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 9].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y + halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface * 1.0f, 1.0f), Vector3(1, 0, 0));//Top left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 10].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y + halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 2.0f, 1.0f), Vector3(1, 0, 0));//Top right
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 11].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y - halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 2.0f, 0.0f), Vector3(1, 0, 0));//Bottom right

        //Bottom face																											
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 12].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y - halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 3.0f, 0.0f), Vector3(0, -1, 0));//Bottom left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 13].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y - halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface * 3.0f, 1.0f), Vector3(0, -1, 0));//Top left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 14].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y - halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface * 4.0f, 1.0f), Vector3(0, -1, 0));//Top right
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 15].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y - halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 4.0f, 0.0f), Vector3(0, -1, 0));//Bottom right

        //Left face																												
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 16].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y - halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface * 4.0f, 0.0f), Vector3(-1, 0, 0));//Bottom left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 17].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y + halfSize.y, particlePosition.z - halfSize.z), particleColor, Vector2(widthUVface * 4.0f, 1.0f), Vector3(-1, 0, 0));//Top left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 18].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y + halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 5.0f, 1.0f), Vector3(-1, 0, 0));//Top right
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 19].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y - halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 5.0f, 0.0f), Vector3(-1, 0, 0));//Bottom right

        //Back face																											
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 20].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y - halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 5.0f, 0.0f), Vector3(0, 0, 1));//Bottom left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 21].FillVertexFormat(Vector3(particlePosition.x - halfSize.x, particlePosition.y + halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 5.0f, 1.0f), Vector3(0, 0, 1));//Top left
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 22].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y + halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 6.0f, 1.0f), Vector3(0, 0, 1));//Top right
        m_blockOfMemory[m_numberOfParticles * PARTICLE3D_NUMBER_VERTICES + 23].FillVertexFormat(Vector3(particlePosition.x + halfSize.x, particlePosition.y - halfSize.y, particlePosition.z + halfSize.z), particleColor, Vector2(widthUVface * 6.0f, 0.0f), Vector3(0, 0, 1));//Bottom right

		//Update the variables data 
		m_numberOfParticles++;
        m_numberOfIndices = m_numberOfParticles * PARTICLE3D_NUMBER_INDICES;
	}
}
