#include "CommonHeader.h"

using namespace std;

const unsigned char DEBUG_COLORS[] = { 0, 255, 0, 255 };

const unsigned int MESH_SHAPES_BOX_NUMBER_VERTICES = 4;
const  unsigned int MESH_SHAPES_BOX_NUMBER_INDICES = 6;

Mesh* Mesh::CreateBox(Vector2 aSize, vec3 aPivotPosition)
{
    VertexFormat verts[MESH_SHAPES_BOX_NUMBER_VERTICES];

    verts[0] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 0, 1, 0, 0);//Bottom left
    verts[1] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 1, 1, 0, 0);//Top left
    verts[2] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 1, 1, 1, 0, 0);//Top right
    verts[3] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 1, 0, 1, 0, 0);//Bottom right

    unsigned int indices[MESH_SHAPES_BOX_NUMBER_INDICES] = { 0, 1, 2, 0, 2, 3 };

    Mesh* pMesh = new Mesh();
    pMesh->Init(verts, MESH_SHAPES_BOX_NUMBER_VERTICES, indices, MESH_SHAPES_BOX_NUMBER_INDICES, GL_STATIC_DRAW);

    return pMesh;
};

const unsigned int CIRCLE_SLICES = 40;//36 is just a value to have an actual circle, we can modify it to have different shapes (diamond,pentagon, etc).

Mesh* Mesh::CreateCircle(float aRadius, vec3 aPivotPosition)
{
    unsigned int numIndices = 0;
    unsigned int numVertices = CIRCLE_SLICES + 2;

    //Create vertex arrray
    VertexFormat verts[CIRCLE_SLICES + 2];
    int index = 0;

    verts[0] = VertexFormat(aPivotPosition.x, aPivotPosition.y, aPivotPosition.z, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0.5, 0.5, 0, 0, 0);//Center of circle

    //Create variabels to use in circle equation
    float tempX = 0;
    float tempY = 0;

    float tempU = 0;
    float tempV = 0;

    float PiDouble = PI * 2;//Circle = 2 PI radians
    float angleSlice = PiDouble / (float)CIRCLE_SLICES;//Calculate the angle of each slice
    float currentAngle = 0;

    for (int i = 0; i < CIRCLE_SLICES; i++)
    {
        currentAngle = (float)i*angleSlice;
        tempX = aPivotPosition.x + (aRadius * cosf(-currentAngle));//Calculates the x position by using the radius and cos, + center of the circle
        tempY = aPivotPosition.y + (aRadius * sinf(-currentAngle)); // Calculates the y position by using the radius and cos, + enter of the circle

        tempU = (0.5f * cosf(currentAngle)) + 0.5f;
        tempV = (0.5f * sinf(currentAngle)) + 0.5f;


        index++;//Increase the index of element we will modify

        verts[index] = VertexFormat(tempX, tempY, aPivotPosition.z, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], tempU, tempV, 0, 0, 0);
    }

    //Set the coordinates of the last point as 1,0 so we can close the  drawing  of the "circumference"
    index++;

    verts[index] = VertexFormat(aRadius + aPivotPosition.x, aPivotPosition.y, aPivotPosition.z, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 1.0f, 0.5f, 0, 0, 0);//Close the circle

    Mesh* pMesh = new Mesh();
    pMesh->Init(verts, numVertices, 0, 0, GL_STATIC_DRAW);
    pMesh->m_PrimitiveType = GL_TRIANGLE_FAN;

    return pMesh;
}

const unsigned int MESH_SHAPES_CUBE_NUMBER_VERTICES = 24;
const  unsigned int MESH_SHAPES_CUBE_NUMBER_INDICES = 36;

Mesh* Mesh::CreateCube(Vector3 aSize, vec3 aPivotPosition)
{
    VertexFormat verts[MESH_SHAPES_CUBE_NUMBER_VERTICES];
    float widthUVface = (1.0f / 6.0f);//The percent width of a UV

    //Front face                                                                                         //Positions if face seen straight on
    verts[0] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 0, 0, 0, -1);//Bottom left
    verts[1] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 1, 0, 0, -1);//Top left
    verts[2] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface, 1, 0, 0, -1);//Top right
    verts[3] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface, 0, 0, 0, -1);//Bottom right

    //Top face																												
    verts[4] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 2, 0, 0, 1, 0);//Bottom left
    verts[5] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 2, 1, 0, 1, 0);//Top left
    verts[6] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 3, 1, 0, 1, 0);//Top right
    verts[7] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 3, 0, 0, 1, 0);//Bottom right

    //Right face																											
    verts[8] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 1, 0, 1, 0, 0);//Bottom left
    verts[9] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 1, 1, 1, 0, 0);//Top left
    verts[10] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 2, 1, 1, 0, 0);//Top right
    verts[11] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 2, 0, 1, 0, 0);//Bottom right

    //Bottom face																											
    verts[12] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 3, 0, 0, -1, 0);//Bottom left
    verts[13] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 3, 1, 0, -1, 0);//Top left
    verts[14] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 4, 1, 0, -1, 0);//Top right
    verts[15] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 4, 0, 0, -1, 0);//Bottom right

    //Left face																												
    verts[16] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 4, 0, -1, 0, 0);//Bottom left
    verts[17] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 4, 1, -1, 0, 0);//Top left
    verts[18] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 5, 1, -1, 0, 0);//Top right
    verts[19] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 5, 0, -1, 0, 0);//Bottom right

    //Back face																											
    verts[20] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 5, 0, 0, 0, 1);//Bottom left
    verts[21] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 5, 1, 0, 0, 1);//Top left
    verts[22] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 6, 1, 0, 0, 1);//Top right
    verts[23] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 6, 0, 0, 0, 1);//Bottom right

    unsigned int indices[MESH_SHAPES_CUBE_NUMBER_INDICES] = {
        0, 1, 2, 0, 2, 3, //Front face
        4, 5, 6, 4, 6, 7,//Top face
        8, 9, 10, 8, 10, 11,//Right face
        12, 13, 14, 12, 14, 15,//Bottom face
        //16, 17, 18, 16, 18, 19,//Left face
        19, 18, 16, 18, 17, 16,//Left face-Index order "Inversed" for culling
        23, 22, 20, 22, 21, 20//Back face-Index order "Inversed" for culling
        //20, 21, 22, 20, 22, 23//Back face
    };

    Mesh* pMesh = new Mesh();
    pMesh->Init(verts, MESH_SHAPES_CUBE_NUMBER_VERTICES, indices, MESH_SHAPES_CUBE_NUMBER_INDICES, GL_STATIC_DRAW);

    return pMesh;
};

const unsigned int MESH_SHAPES_INVERSED_CUBE_NUMBER_VERTICES = 24;
const  unsigned int MESH_SHAPES_INVERSED_CUBE_NUMBER_INDICES = 36;

Mesh* Mesh::CreateInversedCube(vec3 aSize, vec3 aPivotPosition)
{
    VertexFormat verts[MESH_SHAPES_INVERSED_CUBE_NUMBER_VERTICES];
    float widthUVface = (1.0f / 6.0f);//The percent width of a UV

    //Front face                                                                                         //Positions if face seen straight on
	vec3 normals;
	normals = vec3(-1, -1, 1);
	normals.Normalize();
	verts[0] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 0, normals.x, normals.y, normals.z);// Left Top Front
	
	normals = vec3(1, 1, 1);
	normals.Normalize();
	verts[1] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 1, normals.x, normals.y, normals.z);// Left Bot Front

	normals = vec3(-1, 1, 1);
	normals.Normalize();
	verts[2] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface, 1, normals.x, normals.y, normals.z);// Right Bot Front
	
	normals = vec3(-1, -1, 1);
	normals.Normalize();
	verts[3] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface, 0, normals.x, normals.y, normals.z);//Right Top Front																											
	
	normals = vec3(-1, -1, -1);
	normals.Normalize();
	verts[4] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 2, 0, normals.x, normals.y, normals.z);//Right Top Back
	
	normals = vec3(1, -1, -1);
	normals.Normalize();
	verts[5] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 2, 1, normals.x, normals.y, normals.z);//Left Top Back

	normals = vec3(-1, 1, -1);
	normals.Normalize();
    verts[6] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 3, 1, normals.x, normals.y, normals.z);//Right Bot Back

	normals = vec3(1, 1, -1);
	normals.Normalize();
    verts[7] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 3, 0, normals.x, normals.y, normals.z);//Left Bot Back

    //verts[0] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 0, 0, 0, -1);// Left Top Front
    //verts[1] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 1, 0, 0, -1);// Left Bot Front
    //verts[2] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface, 1, 0, 0, -1);// Right Bot Front
    //verts[3] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z - aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface, 0, 0, 0, -1);//Right Top Front																											
    //verts[4] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 2, 0, 0, 1, 0);//Right Top Back
    //verts[5] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y + aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 2, 1, 0, 1, 0);//Left Top Back
    //verts[6] = VertexFormat(aPivotPosition.x + aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 3, 1, 0, 1, 0);//Right Bot Back
    //verts[7] = VertexFormat(aPivotPosition.x - aSize.x / 2, aPivotPosition.y - aSize.y / 2, aPivotPosition.z + aSize.z / 2, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], widthUVface * 3, 0, 0, 1, 0);//Left Bot Back

    //0 = -x, +y, -z // Left Top Front
    //1 = -x, -y, -z // Left Bot Front
    //2 = +x, -y, -z // Right Bot Front
    //3 = +x, +y, -z // Right Top Front
    //4 = +x, +y, +z // Right Top Back
    //5 = -x, +y, +z // Left Top Back
    //6 = +x, -y, +z // Right Bot Back
    //7 = -x, -y, +z // Left Bot Back

    unsigned int indices[MESH_SHAPES_INVERSED_CUBE_NUMBER_INDICES] = {
        3, 0, 1, 1, 2, 3, //Front face
        3, 4, 0, 0, 4, 5,//Top face
        6, 4, 3, 2, 6, 3, //Right face
        7, 6, 1, 6, 2, 1,//Bottom face
        0, 7, 1, 0, 5, 7,//Left face
        4, 6, 7, 7, 5, 4 //Back face
    };

    Mesh* pMesh = new Mesh();
    pMesh->Init(verts, MESH_SHAPES_INVERSED_CUBE_NUMBER_VERTICES, indices, MESH_SHAPES_INVERSED_CUBE_NUMBER_INDICES, GL_STATIC_DRAW);

    return pMesh;


}

Mesh* Mesh::CreatePlane(vec2 aSizeXZ, Vector2Int aColumnsRows)
{
    unsigned int numVertices = (aColumnsRows.x*aColumnsRows.y);//Calculate the total number of vertices

    VertexFormat* verts;
    verts = new VertexFormat[numVertices];

    //Calculate the distance between each vertex
    vec2 distanceBetweenVertex;
    distanceBetweenVertex.x = aSizeXZ.x / (aColumnsRows.x - 1);
    distanceBetweenVertex.y = aSizeXZ.y / (aColumnsRows.y - 1);

    //Calculate the distance between each UV coord (vertex)
    vec2 distanceBetweenUV;
    distanceBetweenUV.x = 1.0f / (aColumnsRows.x - 1);
    distanceBetweenUV.y = 1.0f / (aColumnsRows.y - 1);

    float stepX = 0;
    float stepZ = 0;

    float stepU = 0;
    float stepV = 0;

    unsigned int vertsArrayIndex = 0;

    //Set the vertices
    //Go through all the rows
    for (unsigned int row = 0; row < (unsigned int)aColumnsRows.y; row++)
    {
        //Go through each column
        for (unsigned int col = 0; col < (unsigned int)aColumnsRows.x; col++)
        {
            //Set the position of the current vertex, - the size/2  so that the pivot is in the center.
            stepX = (distanceBetweenVertex.x*(float)col) - aSizeXZ.x / 2;
            stepZ = (distanceBetweenVertex.y *(float)row) - aSizeXZ.y / 2;

            //Set UV
            stepU = (distanceBetweenUV.x*(float)col);
            stepV = (distanceBetweenUV.y *(float)row);

            //Set the vertex
            verts[vertsArrayIndex] = VertexFormat(stepX, 0,
                stepZ, 1, 0, 0, 1, stepU, stepV, 0, 0, 0);

            vertsArrayIndex++;//Go to the next vertex   
        }
    }

    unsigned int* indices;
    unsigned int numIndices = ((aColumnsRows.x - 1) * (aColumnsRows.y - 1));//Multiply the number of horizontal and vertical boxes,   
    numIndices = 6 * numIndices;//multiplied by 6. The number of indices used per square

    indices = new unsigned int[numIndices];

    unsigned int indicesStartingIndex = 0;//starting index is the top left vert of a square in grid
    unsigned int indicesArrayIndex = 0;//Just the number of the index in indices array we are modifyng

    //Set the indices
    //Go through the plane, square by square
    for (unsigned int row = 0; row < (unsigned int)aColumnsRows.y - 1; row++)
    {
        //Go through each column
        for (unsigned int col = 0; col < (unsigned int)aColumnsRows.x - 1; col++)
        {
            indicesStartingIndex = row*aColumnsRows.x + col;

            indices[indicesArrayIndex] = indicesStartingIndex + 1;//Top Right
            indices[indicesArrayIndex + 1] = indicesStartingIndex + 0;//Top Left
            indices[indicesArrayIndex + 2] = indicesStartingIndex + aColumnsRows.x + 1;//Bottom Right
            indices[indicesArrayIndex + 3] = indicesStartingIndex + 0;//Top Left
            indices[indicesArrayIndex + 4] = indicesStartingIndex + aColumnsRows.x + 0;//Bottom Left
            indices[indicesArrayIndex + 5] = indicesStartingIndex + aColumnsRows.x + 1;//Bottom Right

            indicesArrayIndex += 6;
        }
    }

    //Initialize the mesh
    Mesh* pMesh = new Mesh();
    pMesh->Init(verts, numVertices, indices, numIndices, GL_STATIC_DRAW);
    pMesh->CalculateMeshMinAndMaxXYZ(verts, numVertices);

    //Plane has a LOT of vertices, you may want to reduce its numbers
    //glPointSize(5);
    //pMesh->SetPrimitiveType(GL_POINTS);

    //Delete the arrays
    delete[]verts;
    delete[] indices;

    return pMesh;
}

Mesh* Mesh::LoadObj(const char* aFileName, vec3 aScale)
{
    //Temporary variables used when 
    vector<vec3> verticesPosition;
    vector<vec2> verticesTexture;
    vector < vec3 > verticesNormal;
    vector <VertexFormat> meshFaces;

    //Load the entire file into a memory buffer
    long length = 0;
    char* buffer = LoadCompleteFile(aFileName, &length);
    if (buffer == 0 || length == 0)
    {
        delete buffer;
        return 0;
    }

    char* next_token = 0;
    char* line = strtok_s(buffer, "\n", &next_token);//Gets the characters in the first line of the file (characters until \n character is found)
    while (line)
    {
        //Parse the file
        if (line[0] == 'v')//If the line is a vertex
        {

            if (line[1] == 't')//Vertex texture
            {
                verticesTexture.push_back(0);
                sscanf_s(line, "vt %f %f", &verticesTexture.back().x, &verticesTexture.back().y);//Add 2 floats of the file into texture vector
            }
            else if (line[1] == 'n')//Vertex normal
            {
                verticesNormal.push_back(0);
                sscanf_s(line, "vn %f %f %f", &verticesNormal.back().x, &verticesNormal.back().y, &verticesNormal.back().z);//Add 3 floats of normals into normals vector
            }
            else//Vertex position
            {
                verticesPosition.push_back(0);
                sscanf_s(line, "v %f %f %f", &verticesPosition.back().x, &verticesPosition.back().y, &verticesPosition.back().z);//Add 3 floats of positions into positions vector
                verticesPosition.back() = vec3(verticesPosition.back().x*  aScale.x, verticesPosition.back().y*  aScale.y, verticesPosition.back().z * aScale.z);//Scale the mesh
            }

        }
        else if (line[0] == 'f')//If the line is a face
        {
            //Vertex 1
            int indexPosition;
            int indexUVCoords;
            int indexNormals;

            //Vertex 2
            int indexPosition2;
            int indexUVCoords2;
            int indexNormals2;

            //Vertex 3
            int indexPosition3;
            int indexUVCoords3;
            int indexNormals3;

            //Read all the 3 vertices of a single face
            sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &indexPosition, &indexUVCoords, &indexNormals, &indexPosition2, &indexUVCoords2, &indexNormals2, &indexPosition3, &indexUVCoords3, &indexNormals3);

            //This arrays start at 1 and not 0, so we must add 1 to all of them
            VertexFormat tempVertex(verticesPosition.at(indexPosition - 1), verticesTexture.at(indexUVCoords - 1), verticesNormal.at(indexNormals - 1));//Set vertex 1
            meshFaces.push_back(tempVertex);

            VertexFormat tempVertext2(verticesPosition.at(indexPosition2 - 1), verticesTexture.at(indexUVCoords2 - 1), verticesNormal.at(indexNormals2 - 1));//Set vertex 2
            meshFaces.push_back(tempVertext2);

            VertexFormat tempVertext3(verticesPosition.at(indexPosition3 - 1), verticesTexture.at(indexUVCoords3 - 1), verticesNormal.at(indexNormals3 - 1));//Set vertex 3
            meshFaces.push_back(tempVertext3);

        }

        //OutputMessage("%s\n", line);

        line = strtok_s(0, "\n", &next_token);
    }

    Mesh* pMesh = new Mesh();
    pMesh->Init(meshFaces.data(), meshFaces.size(), nullptr, 0, GL_STATIC_DRAW);

    return pMesh;
}

Mesh* Mesh::CreateVerticalPlane(vec2 aSizeXY, Vector2Int aColumnsRows)
{
    unsigned int numVertices = (aColumnsRows.x*aColumnsRows.y);//Calculate the total number of vertices

    VertexFormat* verts;
    verts = new VertexFormat[numVertices];

    //Calculate the distance between each vertex
    vec2 distanceBetweenVertex;
    distanceBetweenVertex.x = aSizeXY.x / (aColumnsRows.x - 1);
    distanceBetweenVertex.y = aSizeXY.y / (aColumnsRows.y - 1);

    //Calculate the distance between each UV coord (vertex)
    vec2 distanceBetweenUV;
    distanceBetweenUV.x = 1.0f / (aColumnsRows.x - 1);
    distanceBetweenUV.y = 1.0f / (aColumnsRows.y - 1);

    float stepX = 0;
    float stepY = 0;

    float stepU = 0;
    float stepV = 0;

    unsigned int vertsArrayIndex = 0;

    //Set the vertices
    //Go through all the rows
    for (unsigned int row = 0; row < (unsigned int)aColumnsRows.y; row++)
    {
        //Go through each column
        for (unsigned int col = 0; col < (unsigned int)aColumnsRows.x; col++)
        {
            //Set the position of the current vertex, - the size/2  so that the pivot is in the center.
            stepX = (distanceBetweenVertex.x*(float)col) - aSizeXY.x / 2;
            stepY = (distanceBetweenVertex.y *(float)row) - aSizeXY.y / 2;

            //Set UV
            stepU = (distanceBetweenUV.x*(float)col);
            stepV = (distanceBetweenUV.y *(float)row);

            //Set the vertex
            verts[vertsArrayIndex] = VertexFormat(stepX, stepY,
                0, 1, 0, 0, 1, stepU, stepV, 0, 0, 0);

            vertsArrayIndex++;//Go to the next vertex   
        }
    }

    unsigned int* indices;
    unsigned int numIndices = ((aColumnsRows.x - 1) * (aColumnsRows.y - 1));//Multiply the number of horizontal and vertical boxes,   
    numIndices = 6 * numIndices;//multiplied by 6. The number of indices used per square

    indices = new unsigned int[numIndices];

    unsigned int indicesStartingIndex = 0;//starting index is the top left vert of a square in grid
    unsigned int indicesArrayIndex = 0;//Just the number of the index in indices array we are modifyng

    //Set the indices
    //Go through the plane, square by square
    for (unsigned int row = 0; row < (unsigned int)aColumnsRows.y - 1; row++)
    {
        //Go through each column
        for (unsigned int col = 0; col < (unsigned int)aColumnsRows.x - 1; col++)
        {
            indicesStartingIndex = row*aColumnsRows.x + col;

            indices[indicesArrayIndex] = indicesStartingIndex + 1;//Top Right
            indices[indicesArrayIndex + 1] = indicesStartingIndex + 0;//Top Left
            indices[indicesArrayIndex + 2] = indicesStartingIndex + aColumnsRows.x + 1;//Bottom Right
            indices[indicesArrayIndex + 3] = indicesStartingIndex + 0;//Top Left
            indices[indicesArrayIndex + 4] = indicesStartingIndex + aColumnsRows.x + 0;//Bottom Left
            indices[indicesArrayIndex + 5] = indicesStartingIndex + aColumnsRows.x + 1;//Bottom Right

            indicesArrayIndex += 6;
        }
    }

    //Initialize the mesh
    Mesh* pMesh = new Mesh();
    pMesh->Init(verts, numVertices, indices, numIndices, GL_STATIC_DRAW);

    //Plane has a LOT of vertices, you may want to reduce its numbers
    //glPointSize(5);
    //pMesh->SetPrimitiveType(GL_POINTS);

    //Delete the arrays
    delete[]verts;
    delete[] indices;

    return pMesh;
}

const unsigned int MESH_SHAPES_POSTPROCESS_SCREEN_NUMBER_VERTICES = 4;
const  unsigned int MESH_SHAPES_POSTPROCESS_SCREEN_NUMBER_INDICES = 6;

Mesh* Mesh::CreatePostProcessScreen()
{
	VertexFormat verts[MESH_SHAPES_POSTPROCESS_SCREEN_NUMBER_VERTICES];

	verts[0] = VertexFormat(-1.0f, -1.0f,-1.0f, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 0, 0, 0, 0);//Bottom left
	verts[1] = VertexFormat(-1.0f, 1.0f, -1.0f, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 0, 1, 0, 0, 0);//Top left
	verts[2] = VertexFormat(1.0f, 1.0f, -1.0f, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 1, 1, 0, 0, 0);//Top right
	verts[3] = VertexFormat(1.0f, -1.0f, -1.0f, DEBUG_COLORS[0], DEBUG_COLORS[1], DEBUG_COLORS[2], DEBUG_COLORS[3], 1, 0, 0, 0, 0);//Bottom right
   
	unsigned int indices[MESH_SHAPES_POSTPROCESS_SCREEN_NUMBER_INDICES] = { 0, 1, 2, 0, 2, 3 };

	Mesh* pMesh = new Mesh();
	pMesh->Init(verts, MESH_SHAPES_POSTPROCESS_SCREEN_NUMBER_VERTICES, indices, MESH_SHAPES_POSTPROCESS_SCREEN_NUMBER_INDICES, GL_STATIC_DRAW);

	return pMesh;
};