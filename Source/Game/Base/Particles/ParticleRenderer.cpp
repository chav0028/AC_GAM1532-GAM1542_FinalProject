/*
Student:    Alvaro Chavez Mixco
Date:       Monday, October 19 , 2015
course:     GAM1546-Games and Graphics Computations II
Professor:  Jimmy Lord
Purpose:    Draw all the particles of a single type, this is done
uisng dynamic vertices


December 1: =Adapted code from class to file
December 6: n ParticleRenderer.cpp, made function setUpRenderer to fill IBO and create a block of memory. Made into a function in
case the total number of particle changes in the Particle sys
*/

#include "CommonHeader.h"

ParticleRenderer::ParticleRenderer(ParticleSystem* aOwnerParticleSystem, GLuint aTexture, const char* aCameraName) :
	m_ownerParticleSystem(aOwnerParticleSystem),
	m_numberOfParticles(0),
	m_numberOfIndices(0),
	m_blockOfMemory(nullptr),
	m_texture(aTexture),
	m_cameraDrawerName(aCameraName),
	m_billboardParticles(true)
{
	//Generate VBO
	glGenBuffers(1, &m_VBO);

	//Save the shader program
	if (aOwnerParticleSystem != nullptr)
	{
		m_pShader = m_ownerParticleSystem->GetShaderProgram();
	}

	SetUpRenderer();//Creates the block of memory and fill the ibo
}


ParticleRenderer::ParticleRenderer(bool aSetRenderer, ParticleSystem* aOwnerParticleSystem, GLuint aTexture, const char* aCameraName) :
	m_ownerParticleSystem(aOwnerParticleSystem),
	m_numberOfParticles(0),
	m_numberOfIndices(0),
	m_blockOfMemory(nullptr),
	m_texture(aTexture),
	m_cameraDrawerName(aCameraName),
	m_billboardParticles(true)
{
	//Generate VBO
	glGenBuffers(1, &m_VBO);

	//Save the shader program
	if (aOwnerParticleSystem != nullptr)
	{
		m_pShader = m_ownerParticleSystem->GetShaderProgram();
	}

	if (aSetRenderer == true)
	{
		SetUpRenderer();//Creates the block of memory and fill the ibo
	}
}

ParticleRenderer::~ParticleRenderer()
{
	if (m_blockOfMemory != nullptr)
	{
		delete[] m_blockOfMemory;
		m_blockOfMemory = nullptr;
	}

	//Delete the buffers
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);

}

void ParticleRenderer::Draw()
{
	if (m_numberOfIndices == 0 || m_numberOfParticles == 0)
	{
		return;
	}

	GLuint shader = m_pShader->GetProgram();
	glUseProgram(shader);


	FillVBO();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	//store the values in the matrix
	mat4 objectMatrix;
	objectMatrix.SetIdentity();//Set in the matrix its scale, rotation and position values.
	mat4* pViewMatrix = 0;
	mat4* pProjectionMatrix = 0;
	Vector3 cameraPosition = 0;

	if (m_ownerParticleSystem != nullptr)
	{

		Scene* currentScene = m_ownerParticleSystem->GetScene();

		if (currentScene != nullptr)
		{
			CameraObject* pCamera = dynamic_cast<CameraObject*>(currentScene->GetGameObject(GAME_DEFAULT_CAMERA_NAME));//get the camera that will draw this gameobject

			if (pCamera != nullptr)
			{
				cameraPosition = pCamera->GetPosition();

				pViewMatrix = pCamera->GetViewMatrix();//Get camera view matrix
			}

			pProjectionMatrix = currentScene->GetProjectionMatrix();//Get the projecion matrix

		}
	}

	Mesh::SetupUniforms(m_pShader, &objectMatrix, pViewMatrix, pProjectionMatrix, cameraPosition, m_texture);
	SetAttributes(m_pShader);

	CheckForGLErrors();

	glDrawElements(GL_TRIANGLES, m_numberOfIndices, GL_UNSIGNED_INT, (void*)0);

	CheckForGLErrors();
}

//Creates the block of memory, and fills the IBO
void ParticleRenderer::SetUpRenderer()
{
	//Create a block of memory
	m_blockOfMemory = new VertexFormat[PARTICLES_RENDERER_MAX_NUMBER_OF_PARTICLES * PARTICLE_NUMBER_VERTICES];//create an array with the maximum number of particles *4 because it will have 4 indices

	//Enter 4 empty positions for vertices object space
	m_objectSpaceVertices.push_back(0);	//Top left
	m_objectSpaceVertices.push_back(0);	//Top right
	m_objectSpaceVertices.push_back(0);	//Bottom Right
	m_objectSpaceVertices.push_back(0);	//Bottom left

	//Fill the IBO
	FillIBO();
}

void ParticleRenderer::PlotObjectSpaceParticles(Vector3 aParticleSize)
{
	vec3 halfSize = aParticleSize / 2.0f;

	//Plot the vertices for the size of teh particles
	//Top Left
	m_objectSpaceVertices.at(0) = vec3(-halfSize.x, halfSize.y, 0);

	// Top right
	m_objectSpaceVertices.at(1) = vec3(halfSize.x, halfSize.y, 0);

	//Bottom right
	m_objectSpaceVertices.at(2) = vec3(halfSize.x, -halfSize.y, 0);

	//Bottom Left
	m_objectSpaceVertices.at(3) = vec3(-halfSize.x, -halfSize.y,0);

	//Check if the particles are to be rotated according to the camera
	if (m_billboardParticles == true)
	{
		mat4 cameraRotationMatrix;
		if (m_ownerParticleSystem != nullptr)
		{
			Scene* currentScene = m_ownerParticleSystem->GetScene();

			if (currentScene != nullptr)
			{
				CameraObject* pCamera = dynamic_cast<CameraObject*>(currentScene->GetGameObject(GAME_DEFAULT_CAMERA_NAME));//get the camera that will draw this gameobject

				if (pCamera != nullptr)
				{
					cameraRotationMatrix = pCamera->GetRotationMatrix();//Get the camea rotation matrix
					Vector3 tempPositon;

					//Rotate the object space positons of the particles according to the camera
					tempPositon = m_objectSpaceVertices.at(0);
                    tempPositon = cameraRotationMatrix*tempPositon;
 					m_objectSpaceVertices.at(0) = Vector3(tempPositon.x, tempPositon.y, tempPositon.z);

                    tempPositon = cameraRotationMatrix*m_objectSpaceVertices.at(1);
					m_objectSpaceVertices.at(1) = Vector3(tempPositon.x, tempPositon.y, tempPositon.z);

                    tempPositon = cameraRotationMatrix*m_objectSpaceVertices.at(2);
					m_objectSpaceVertices.at(2) = Vector3(tempPositon.x, tempPositon.y, tempPositon.z);

                    tempPositon = cameraRotationMatrix*m_objectSpaceVertices.at(3);
					m_objectSpaceVertices.at(3) = Vector3(tempPositon.x, tempPositon.y, tempPositon.z);

				}

			}
		}



	}
}

void ParticleRenderer::FillVBO()
{
	// fill buffer with our attributes
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * (m_numberOfParticles * PARTICLE_NUMBER_VERTICES), m_blockOfMemory, GL_DYNAMIC_DRAW);

	// check for errors
	CheckForGLErrors();
}

void ParticleRenderer::FillIBO()
{
	//Generate IBO
	glGenBuffers(1, &m_IBO);

	unsigned int maxNumParticles = PARTICLES_RENDERER_MAX_NUMBER_OF_PARTICLES;

	if (maxNumParticles > 0)//If there are actually particles to render
	{
		unsigned int* indices;
		indices = new unsigned int[maxNumParticles * PARTICLE_NUMBER_INDICES];//* 6 because it uses 6 indices

		//Fill the indices array
		for (unsigned int i = 0; i < maxNumParticles; i++)
		{
			//I*6 is the number of the particle currently being modified, since each particle has 6 indices
			indices[(i * PARTICLE_NUMBER_INDICES) + 0] = i * PARTICLE_NUMBER_VERTICES + 0;//i*4 represents the vertices, since the particle has 4 vertice.
			indices[(i * PARTICLE_NUMBER_INDICES) + 1] = i * PARTICLE_NUMBER_VERTICES + 1;
			indices[(i * PARTICLE_NUMBER_INDICES) + 2] = i * PARTICLE_NUMBER_VERTICES + 2;
			indices[(i * PARTICLE_NUMBER_INDICES) + 3] = i * PARTICLE_NUMBER_VERTICES + 0;
			indices[(i * PARTICLE_NUMBER_INDICES) + 4] = i * PARTICLE_NUMBER_VERTICES + 2;
			indices[(i * PARTICLE_NUMBER_INDICES) + 5] = i * PARTICLE_NUMBER_VERTICES + 3;
		}

		//Generate the IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);//Use elemnt array buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * (maxNumParticles * PARTICLE_NUMBER_INDICES), indices, GL_STATIC_DRAW);

		delete[] indices;
	}
}

void ParticleRenderer::SetAttributes(ShaderProgram* apShaderProgram)
{

	assert(m_VBO != 0);

	if (apShaderProgram != nullptr)
	{
		// bind our vertex and index buffers.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		if (m_IBO != 0)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		GLuint programhandle = apShaderProgram->GetProgram();

		// get the attribute locations.
		GLint aPos = glGetAttribLocation(programhandle, "a_Position");
		GLint aColor = glGetAttribLocation(programhandle, "a_Color");
		GLint aUV = glGetAttribLocation(programhandle, "a_UV");
		GLint aNormal = glGetAttribLocation(programhandle, "a_Normal");

		// setup our vbo and attributes.
		{
			// setup the position attribute.
			assert(aPos != -1);
			glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, pos));
			glEnableVertexAttribArray(aPos);

			// setup the color attribute.
			if (aColor != -1)
			{
				glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, color));
				glEnableVertexAttribArray(aColor);
			}

			// setup the uv attribute.
			if (aUV != -1)
			{
				glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, uv));
				glEnableVertexAttribArray(aUV);
			}

			// setup the normal attribute.
			if (aNormal != -1)
			{
				glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, normal));
				glEnableVertexAttribArray(aNormal);
			}
		}
	}
}


void ParticleRenderer::AddParticle(Particle aParticle)
{
	//Check we aren't over the particle limit
	if (m_numberOfParticles < PARTICLES_RENDERER_MAX_NUMBER_OF_PARTICLES)
	{
		//Get properties of particle
		//Vector3 halfSize = aParticle.GetSize() / 2; Size and other properties callculated behore hand
		Vector3 particlePosition = aParticle.GetPosition();
		Vector4 particleColor = aParticle.GetColor();

		//Copy the data into the block of memory
		//Top Left
		m_blockOfMemory[m_numberOfParticles * PARTICLE_NUMBER_VERTICES + 0].FillVertexFormat(particlePosition + m_objectSpaceVertices.at(0)
			, particleColor, Vector2(0.0f, 1.0f), Vector3(0, 0, -1));

		// Top right
		m_blockOfMemory[m_numberOfParticles * PARTICLE_NUMBER_VERTICES + 1].FillVertexFormat(particlePosition + m_objectSpaceVertices.at(1),
			particleColor, Vector2(1.0f, 1.0f), Vector3(0, 0, -1));

		//Bottom right
		m_blockOfMemory[m_numberOfParticles * PARTICLE_NUMBER_VERTICES + 2].FillVertexFormat(particlePosition + m_objectSpaceVertices.at(2),
			particleColor, Vector2(1.0f, 0.0f), Vector3(0, 0, -1));

		//Bottom Left
		m_blockOfMemory[m_numberOfParticles * PARTICLE_NUMBER_VERTICES + 3].FillVertexFormat(particlePosition + m_objectSpaceVertices.at(3),
			particleColor, Vector2(0.0f, 0.0f), Vector3(0, 0, -1));

		//Update the variables data 
		m_numberOfParticles++;
		m_numberOfIndices = m_numberOfParticles * PARTICLE_NUMBER_INDICES;
	}

}

void ParticleRenderer::SetTexture(GLuint aTexture)
{
	m_texture = aTexture;
}