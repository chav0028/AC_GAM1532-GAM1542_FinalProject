#ifndef __Mesh_H__
#define __Mesh_H__

const vec3 OBJ_MESH_DEFAULT_SCALE = vec3(1.0f, 1.0f, 1.0f);//vec3(0.01f, 0.01f, 0.01f)

class LightObject;

class Mesh
{
protected:
	GLuint m_VBO;
	GLuint m_IBO;

	unsigned int m_NumVerts;
	unsigned int m_NumIndices;

	GLenum m_PrimitiveType;

	vec3 m_minXYZ;
	vec3 m_maxXYZ;

public:
	Mesh();
	virtual ~Mesh();

	virtual void Init(VertexFormat* verts, int numverts, unsigned int* indices, int numindices, GLenum usage);
	virtual void Init(const void* verts, int numverts, int vertexbytesize, GLenum usage);

	void SetupAttributes(ShaderProgram* pShaderProgram);
	static void SetupUniforms(ShaderProgram* pShaderProgram, mat4* worldMatrix, mat4* viewMatrix, mat4* projectionMatrix, Vector3 camPos, GLuint texturehandle,
		Vector4 color = Vector4(1, 0, 0, 1), vec2 aUVSCale = vec2(1, 1), vec2 aUVOffset = vec2(0, 0));
	static void SetupCollisionPointsUniform(ShaderProgram* pShaderProgram, std::vector<Vector4> collisionPointsPosition, std::vector<float> timerPercentageLeftCollisionPoints, int numberCollisionPoints);
	static void SetColorUniform(ShaderProgram* pShaderProgram, Vector4 color);
	static void SetupTexture2Uniform(ShaderProgram* pShaderProgram, GLuint texturehandle);
	static void SetLightUniform(ShaderProgram* pShaderProgram, std::vector<LightObject*>pSceneLights,float ambientLightPercent);
    static void SetMaterialLightingUniform(ShaderProgram* pShaderProgram, float materialReflectivity);

	virtual void Draw(ShaderProgram* pShaderProgram);

	void SetPrimitiveType(GLenum aPrimitiveType) { m_PrimitiveType = aPrimitiveType; }
	void SetLineWidth(float aSize) { glLineWidth(aSize); }
	void SetPointSize(float aSize) { glPointSize(aSize); }

	void CalculateMeshMinAndMaxXYZ(VertexFormat* vertices, unsigned int numberVertices);
	float GetMeshXLength();
	float GetMeshYLength();
	float GetMeshZLength();
	vec3 GetMeshMinXYZ() { return m_minXYZ; }
	vec3 GetMeshMaxXYZ() { return m_maxXYZ; }


	// defined in MeshShapes.cpp
	static Mesh* CreateBox(vec2 size, vec3 pivotPosition = vec3(0, 0, 0));
	static Mesh* CreateCircle(float radius, vec3 pivotPosition = vec3(0, 0, 0));
	static Mesh* CreateCube(vec3 size, vec3 pivotPosition = vec3(0, 0, 0));
    static Mesh* CreateInversedCube(vec3 size, vec3 pivotPosition = vec3(0, 0, 0));
	static Mesh* CreatePlane(vec2 sizeXZ, Vector2Int columnsRow);
	static Mesh* CreateVerticalPlane(vec2 sizeXY, Vector2Int columnsRow);
	static Mesh* LoadObj(const char* aFileName, vec3 aScale = OBJ_MESH_DEFAULT_SCALE);
	static Mesh* CreatePostProcessScreen();
};

#endif //__Game_H__
