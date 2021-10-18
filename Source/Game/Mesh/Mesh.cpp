#include "CommonHeader.h"
#include <sstream>

using namespace std;

Mesh::Mesh()
{
	m_VBO = 0;
	m_IBO = 0;
	m_PrimitiveType = GL_TRIANGLES;
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

void Mesh::Init(VertexFormat* aVerts, int aNumverts, unsigned int* aIndices, int aNumindices, GLenum aUsage)
{
	m_NumVerts = aNumverts;
	m_NumIndices = aNumindices;

	CalculateMeshMinAndMaxXYZ(aVerts, aNumverts);

	// gen and fill buffer with our vertex attributes.
	if (m_VBO == 0)
		glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat)*aNumverts, aVerts, aUsage);

	// gen and fill buffer with our indices.
	if (m_IBO == 0)
		glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*aNumindices, aIndices, aUsage);


	CheckForGLErrors();
}

void Mesh::Init(const void* aVerts, int aNumverts, int aVertexbytesize, GLenum aUsage)
{
	m_NumVerts = aNumverts;
	m_NumIndices = 0;

	//vertexbytesize is the size of the single vertex, to get the size of the buffer we multiply by the number of vertice

	// gen and fill buffer with our vertex attributes.
	if (m_VBO == 0)
		glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, aVertexbytesize*aNumverts, aVerts, aUsage);//Vertex byte size * number verts to calculate size of buffeer

	CheckForGLErrors();
}

void Mesh::SetupAttributes(ShaderProgram* apShaderProgram)
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

void Mesh::SetupUniforms(ShaderProgram* apShaderProgram, mat4* aWorldMatrix, mat4* aViewMatrix, mat4* aProjectionMatrix, Vector3 camPos, GLuint texturehandle, Vector4 color, vec2 aUVScale, vec2 aUVOffset)
{
	if (apShaderProgram != nullptr)
	{
		GLuint programhandle = apShaderProgram->GetProgram();

		// enable our shader program.
		glUseProgram(programhandle);

		// get the uniform locations.
		GLint uCameraPosition = glGetUniformLocation(programhandle, "u_CameraPosition");
		GLint uTexture = glGetUniformLocation(programhandle, "u_Texture");
        GLint uCubeMapTexture = glGetUniformLocation(programhandle, "u_CubeMapTexture");
		GLint uColor = glGetUniformLocation(programhandle, "u_Color");
		GLint uTime = glGetUniformLocation(programhandle, "u_Time");
        GLint uScreenSize = glGetUniformLocation(programhandle, "u_ScreenSize");

		GLint uWorldMatrix = glGetUniformLocation(programhandle, "u_WorldMatrix");
		GLint uViewMatrix = glGetUniformLocation(programhandle, "u_ViewMatrix");
		GLint uProjectionMatrix = glGetUniformLocation(programhandle, "u_ProjectionMatrix");

		GLint uUVScale = glGetUniformLocation(programhandle, "u_UVScale");
		GLint uUVOffset = glGetUniformLocation(programhandle, "u_UVOffset");

		// setup the texture.
		if (texturehandle != 0 && uTexture != -1)
		{
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, texturehandle);
			glUniform1i(uTexture, 0);
		}

        // setup the cube map texture.
        if (texturehandle != 0 && uCubeMapTexture != -1)
        {
            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texturehandle);
            glUniform1i(uCubeMapTexture, 0);
        }

		if (uCameraPosition != -1)
		{
			glUniform3f(uCameraPosition, camPos.x, camPos.y, camPos.z);
		}

		if (uColor != -1)
		{
			glUniform4f(uColor, color.x, color.y, color.z, color.w);
		}

		// setup time.
		if (uTime != -1)
		{
			glUniform1f(uTime, (float)MyGetSystemTime());
		}

		//Set the world transform matrix
		if (uWorldMatrix != -1 && aWorldMatrix != nullptr)
		{

			glUniformMatrix4fv(uWorldMatrix, 1, false, &aWorldMatrix->m11);
		}

		//Set the view matrix
		if (uViewMatrix != -1 && aViewMatrix != nullptr)
		{
			glUniformMatrix4fv(uViewMatrix, 1, false, &aViewMatrix->m11);
		}

		//Set the view matrix
		if (uProjectionMatrix != -1 && aProjectionMatrix != nullptr)
		{
			glUniformMatrix4fv(uProjectionMatrix, 1, false, &aProjectionMatrix->m11);
		}

		//Set the UV scale
		if (uUVScale != -1)
		{
            glUniform2f(uUVScale, aUVScale.x, aUVScale.y);
		}

		//Set the UV offset
		if (uUVOffset != -1)
		{
			glUniform2f(uUVOffset, aUVOffset.x, aUVOffset.y);
		}

        if (uScreenSize != -1)
        {
            glUniform2f(uScreenSize, (float)g_pGame->GetWindowWidth(), (float)g_pGame->GetWindowHeight());
        }

		CheckForGLErrors();
	}
}

void Mesh::SetupCollisionPointsUniform(ShaderProgram* apShaderProgram, std::vector<Vector4> aCollisionPointsPosition, std::vector<float> aTimerPercentageLeftCollisionPoints, int aNumberCollisionPoints)
{
	if (apShaderProgram != nullptr)
	{
		GLuint programhandle = apShaderProgram->GetProgram();

		// enable our shader program.
		glUseProgram(programhandle);

		// get the uniform locations.
		GLint uCollisionPoints = glGetUniformLocation(programhandle, "u_CollisionPoints");
		GLint uTimersCollisionPoints = glGetUniformLocation(programhandle, "u_CollisionPointsTimerPercentageLeft");
		GLint uNumberCollisionPoints = glGetUniformLocation(programhandle, "u_NumberOfCollisionPoints");

		if (aCollisionPointsPosition.empty() == false && aTimerPercentageLeftCollisionPoints.empty() == false)//Only set uniforms if there is data
		{
			if (uCollisionPoints != -1)
			{
				glUniform4fv(uCollisionPoints, aNumberCollisionPoints, &aCollisionPointsPosition[0].x);//Uniform location, number of elements in vector, and address of array
			}


			if (uTimersCollisionPoints != -1)
			{
				glUniform1fv(uTimersCollisionPoints, aNumberCollisionPoints, &aTimerPercentageLeftCollisionPoints[0]);
			}

			if (uNumberCollisionPoints != -1)
			{
				glUniform1i(uNumberCollisionPoints, aNumberCollisionPoints);
			}
		}
	}
}

void Mesh::SetColorUniform(ShaderProgram* apShaderProgram, Vector4 color)
{
	if (apShaderProgram != nullptr)
	{
		GLuint programhandle = apShaderProgram->GetProgram();

		// enable our shader program.
		glUseProgram(programhandle);

		//set only the color uniform
		GLint uColor = glGetUniformLocation(programhandle, "u_Color");

		if (uColor != -1)
		{
			glUniform4f(uColor, color.x, color.y, color.z, color.w);
		}
	}
}

void Mesh::SetupTexture2Uniform(ShaderProgram* apShaderProgram, GLuint texturehandle)
{
	if (apShaderProgram != nullptr)
	{
		GLuint programhandle = apShaderProgram->GetProgram();

		// enable our shader program.
		glUseProgram(programhandle);

		// get the uniform locations.
		GLint uTexture = glGetUniformLocation(programhandle, "u_Texture2");
        GLint uCubeMapTexture = glGetUniformLocation(programhandle, "u_CubeMapTexture2");

        // setup the texture.
        if (texturehandle != 0 && uTexture != -1)
        {
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, texturehandle);
            glUniform1i(uTexture, 1);
        }

        // setup the cube map texture.
        if (texturehandle != 0 && uCubeMapTexture != -1)
        {
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texturehandle);
            glUniform1i(uCubeMapTexture, 1);
        }
	}
}

void Mesh::SetLightUniform(ShaderProgram * apShaderProgram, std::vector<LightObject*> apSceneLights, float ambientLightPercent)
{
	if (apShaderProgram != nullptr)
	{
		GLuint programhandle = apShaderProgram->GetProgram();

		// Enable our shader program.
		glUseProgram(programhandle);

		//Set the ambient lights percent u_ambientPercentage
		GLint uambient = glGetUniformLocation(programhandle, "u_ambientPercentage");
		if (uambient != -1)
		{
			glUniform1f(uambient, ambientLightPercent);
		}

		//Set the number of lights
        GLint uNumberOfLights = glGetUniformLocation(programhandle, "u_NumberOfLights");
        if (uNumberOfLights != -1)
        {
            glUniform1i(uNumberOfLights, apSceneLights.size());
        }

		//Set all values in vector
		for (unsigned int i = 0; i < apSceneLights.size(); i++)
		{
            if (apSceneLights.at(i)!=nullptr)
            {
            string currentNumber = to_string(i);

			//Convert the current number to char*
            stringstream lightTypeUniformName;
            lightTypeUniformName << "u_Lights[" << currentNumber << "].m_lightType";

			stringstream positionUniformName;
			positionUniformName << "u_Lights[" << currentNumber << "].m_worldPosition";

			stringstream lightColorUniformName;
            lightColorUniformName << "u_Lights[" + currentNumber << "].m_lightColor";
            
            stringstream specularColorUniformName;
            specularColorUniformName << "u_Lights[" + currentNumber << "].m_specularColor";

            stringstream attenuationUniformName;
            attenuationUniformName << "u_Lights[" + currentNumber << "].m_attenuation";

			stringstream rangeUniformName;
			rangeUniformName << "u_Lights[" + currentNumber << "].m_angleRange";

			stringstream directionUniformName;
			directionUniformName << "u_Lights[" + currentNumber << "].m_facingDirection";

			// Get the uniform locations.
            GLint uLightType = glGetUniformLocation(programhandle, lightTypeUniformName.str().c_str());//Convert string stream to string and then to char*
			GLint uPosition = glGetUniformLocation(programhandle, positionUniformName.str().c_str());
            GLint uLightColor = glGetUniformLocation(programhandle, lightColorUniformName.str().c_str());
            GLint uSpecularColor = glGetUniformLocation(programhandle, specularColorUniformName.str().c_str());
            GLint uAttenuation = glGetUniformLocation(programhandle, attenuationUniformName.str().c_str());
			GLint uRange = glGetUniformLocation(programhandle, rangeUniformName.str().c_str());
			GLint uDirection = glGetUniformLocation(programhandle, directionUniformName.str().c_str());

            if (uLightType != -1)
            {
                glUniform1i(uLightType, apSceneLights.at(i)->GetLightType());
            }

			if (uPosition != -1)
			{
                Vector3 lightWorldSpacePosition = apSceneLights.at(i)->GetPosition();
                glUniform3f(uPosition, lightWorldSpacePosition.x, lightWorldSpacePosition.y, lightWorldSpacePosition.z);//Uniform location, number of elements in vector, and address of array
			}

            if (uLightColor != -1)
			{
                Vector4 lightColor = apSceneLights.at(i)->GetLightColor();
                glUniform4f(uLightColor, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			}

            if (uSpecularColor != -1)
            {
                Vector4 specularColor = apSceneLights.at(i)->GetSpecularColor();
                glUniform4f(uSpecularColor, specularColor.x, specularColor.y, specularColor.z, specularColor.w);
            }

            if (uAttenuation != -1)
            {
                Vector3 constantLinearQuadraticAttenuation = apSceneLights.at(i)->GetAttenuation();
                glUniform3f(uAttenuation, constantLinearQuadraticAttenuation.x, constantLinearQuadraticAttenuation.y, constantLinearQuadraticAttenuation.z);
            }

			if (uRange != -1)
			{
				float angleRange = apSceneLights.at(i)->GetAngleRange();
				glUniform1f(uRange, angleRange);
			}

			if (uDirection != -1)
			{
				Vector3 direction = apSceneLights.at(i)->GetDirection3D();
				glUniform3f(uDirection, direction.x, direction.y, direction.z);
			}
                        
            }
		}
	}

}

void Mesh::SetMaterialLightingUniform(ShaderProgram* apShaderProgram, float aMaterialReflectivity)
{
    if (apShaderProgram != nullptr)
    {
        GLuint programhandle = apShaderProgram->GetProgram();

        // enable our shader program.
        glUseProgram(programhandle);

        // get the uniform locations.
        GLint uMaterialReflectivity = glGetUniformLocation(programhandle, "u_MaterialReflectivity");

        // setup the  uniform
        if (uMaterialReflectivity != -1)
        {
            glUniform1f(uMaterialReflectivity, aMaterialReflectivity);
        }
    }

}

void Mesh::Draw(ShaderProgram* apShaderProgram)
{
	if (apShaderProgram != nullptr)
	{
		GLuint programhandle = apShaderProgram->GetProgram();

		// draw the shape.
		if (m_NumIndices > 0)
		{
			glDrawElements(m_PrimitiveType, m_NumIndices, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(m_PrimitiveType, 0, m_NumVerts);
		}

		GLint aPos = glGetAttribLocation(programhandle, "a_Position");
		GLint aColor = glGetAttribLocation(programhandle, "a_Color");
		GLint aUV = glGetAttribLocation(programhandle, "a_UV");
		GLint aNormal = glGetAttribLocation(programhandle, "a_Normal");

		// disable the attribute arrays used
        if (aPos!=-1)
		glDisableVertexAttribArray(aPos);

		if (aColor != -1)
			glDisableVertexAttribArray(aColor);

		if (aUV != -1)
			glDisableVertexAttribArray(aUV);

		if (aNormal != -1)
			glDisableVertexAttribArray(aNormal);

		CheckForGLErrors();
	}
}

void Mesh::CalculateMeshMinAndMaxXYZ(VertexFormat* aVertices, unsigned int aNumberVertices)
{
	//Temporary variables to store values
	vec3 tempMinValues = vec3(0, 0, 0);
	vec3 tempMaxValues = vec3(0, 0, 0);
	vec3 currentVertexPosition = vec3(0, 0, 0);

	//Go through all the vertices
	for (unsigned int i = 0; i < aNumberVertices; i++)
	{
		currentVertexPosition = aVertices[i].pos;//Save the vertex current position

		//Compare the current max and min values with the vertex position
		//MIN VALUES
		//X
		if (currentVertexPosition.x < tempMinValues.x)
		{
			tempMinValues.x = currentVertexPosition.x;
		}

		//Y
		if (currentVertexPosition.y < tempMinValues.y)
		{
			tempMinValues.y = currentVertexPosition.y;
		}

		//Z
		if (currentVertexPosition.z < tempMinValues.z)
		{
			tempMinValues.z = currentVertexPosition.z;
		}

		//MAX VALUES
		if (currentVertexPosition.x > tempMaxValues.x)
		{
			tempMaxValues.x = currentVertexPosition.x;
		}

		//Y
		if (currentVertexPosition.y > tempMaxValues.y)
		{
			tempMaxValues.y = currentVertexPosition.y;
		}

		//Z
		if (currentVertexPosition.z > tempMaxValues.z)
		{
			tempMaxValues.z = currentVertexPosition.z;
		}
	}

	//Save the values into the member variables
	m_minXYZ = tempMinValues;
	m_maxXYZ = tempMaxValues;
}

float Mesh::GetMeshXLength()
{
	//Get minimum and max value
	float minX = GetMeshMinXYZ().x;
	float maxX = GetMeshMaxXYZ().x;

	return abs(maxX - minX);//Length will be the difference between the 2 values,because it is a distance it will be absolute value
}

float Mesh::GetMeshYLength()
{
	//Get minimum and max value
	float minY = GetMeshMinXYZ().y;
	float maxY = GetMeshMaxXYZ().y;

	return abs(maxY - minY);//Length will be the difference between the 2 values,because it is a distance it will be absolute value
}

float Mesh::GetMeshZLength()
{
	//Get minimum and max value
	float minZ = GetMeshMinXYZ().z;
	float maxZ = GetMeshMaxXYZ().z;

	return abs(maxZ - minZ);//Length will be the difference between the 2 values,because it is a distance it will be absolute value
}