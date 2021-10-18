#include "CommonHeader.h"


Box2DDebugDraw::Box2DDebugDraw():
m_pShaderProgram(nullptr),
m_pViewMatrix(nullptr),
m_pProjectionMatrix(nullptr
)
{
  
}

void Box2DDebugDraw::SetShader(ShaderProgram* aShader)
{
    if (aShader != nullptr)
    {
        m_pShaderProgram=aShader;
    }
}

Box2DDebugDraw::~Box2DDebugDraw()
{
}

void Box2DDebugDraw::Draw(const b2Vec2* aVertices, unsigned int aVertexCount, const b2Color& aColor, unsigned char aAlpha, GLenum aPrimitiveType)
{

    if (m_pShaderProgram != nullptr)
    {
        GLuint programhandle = m_pShaderProgram->GetProgram();

        //Set VBO to 0, so that no actual VBO is used.
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // get the attribute locations.
        GLint aPos = glGetAttribLocation(programhandle, "a_Position");
        
        // setup the position attribute.
        assert(aPos != -1);
        glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE,sizeof(float) * 2, aVertices+0);//Since there is no VBO bind, the last paramer
        //will be a pointer to the actual vertices. So these will be drawn directly instead of a VBO. Add 0, since this is the first
        //, and in this case only, attribute of vertices

        //glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2), aVertices+(sizeof(float)*2));

        glEnableVertexAttribArray(aPos);

        //Set an empty world matrix, since the vertices are already in world space
        mat4 worldMatrix;
        worldMatrix.SetIdentity();

        //Set the uniforms
        Mesh::SetupUniforms(m_pShaderProgram, &worldMatrix, m_pViewMatrix, m_pProjectionMatrix, 0, 0, Vector4(aColor.r, aColor.g, aColor.b, (float)aAlpha / 255));

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        glDrawArrays(aPrimitiveType, 0, aVertexCount);//Draw the verices, no VBO being used

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    }
}


/// Draw a closed polygon provided in CCW order.
void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    glLineWidth(DEBUG_DRAW_OUTLINE_WIDTH);
    Draw(vertices, vertexCount, color, DEBUG_DRAW_OUTLINE_ALPHA, GL_LINE_LOOP);

}

/// Draw a solid closed polygon provided in CCW order.
void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
        Draw(vertices, vertexCount, color, DEBUG_DRAW_SHAPE_ALPHA, GL_TRIANGLE_FAN);//Draw shape

        glLineWidth(DEBUG_DRAW_OUTLINE_WIDTH);
        Draw(vertices, vertexCount, color, DEBUG_DRAW_OUTLINE_ALPHA, GL_LINE_LOOP);//Draw outline
}

/// Draw a circle.
void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    b2Vec2 vertices[DEBUG_DRAW_DEFAULT_CIRCLE_SLICES + 2];//Make vertex array
    
    PlotCirclePoints(vertices, center, radius);//Plot circle vertices

    Draw(vertices, DEBUG_DRAW_DEFAULT_CIRCLE_SLICES + 2, color, DEBUG_DRAW_OUTLINE_ALPHA, GL_LINE_LOOP);//Draw outline

}

/// Draw a solid circle.
void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    b2Vec2 vertices[DEBUG_DRAW_DEFAULT_CIRCLE_SLICES + 2];//Make vertex array

    PlotCirclePoints(vertices, center, radius);//Plot circle vertices

    Draw(vertices, DEBUG_DRAW_DEFAULT_CIRCLE_SLICES + 2, color, DEBUG_DRAW_SHAPE_ALPHA, GL_TRIANGLE_FAN);//Draw shape

    glLineWidth(DEBUG_DRAW_OUTLINE_WIDTH);
    Draw(vertices, DEBUG_DRAW_DEFAULT_CIRCLE_SLICES + 2, color, DEBUG_DRAW_OUTLINE_ALPHA, GL_LINE_LOOP);//Draw outline

    //Draw the axis
    DrawSegment(center, center + radius*axis, color);

}

/// Draw a line segment.
void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    b2Vec2 vertices[] = { p1, p2 };//Make the vertices into an array

    glLineWidth(DEBUG_DRAW_OUTLINE_WIDTH);
    Draw(vertices, 2, color, DEBUG_DRAW_OUTLINE_ALPHA, GL_LINES);//Draw the line

}

/// Draw a transform. Choose your own length scale.
void Box2DDebugDraw::DrawTransform(const b2Transform& xf)
{
    b2Vec2 center = xf.p;

    b2Vec2 pointOne = xf.p + DEBUG_DRAW_TRANSFORM_SCALE*xf.q.GetXAxis();
    DrawSegment(center, pointOne, b2Color(1, 0, 0, 1));


    b2Vec2 pointTwo = xf.p + DEBUG_DRAW_TRANSFORM_SCALE*xf.q.GetYAxis();
    DrawSegment(center, pointTwo, b2Color(0, 1, 0, 1));

}

const unsigned int BOX2D_DEBUG_DRAW_CIRCLE_SLICES = 40;

void Box2DDebugDraw::PlotCirclePoints(b2Vec2* vertices, const b2Vec2& center, float32 radius)
{
    //Create vertex arrray
    int circleSlices = BOX2D_DEBUG_DRAW_CIRCLE_SLICES;//Constant for slices to look round
    int index = 0;

    vertices[0] = center;//Center of circle

    //Create variabels to use in circle equation
    float tempX = 0;
    float tempY = 0;

    float PiDouble = PI * 2;//Circle = 2 PI radians
    float angleSlice = PiDouble / (float)circleSlices;//Calculate the angle of each slice

    for (int i = 0; i < circleSlices; i++)
    {

        tempX = center.x + (radius * cosf(i * angleSlice));//Calculates the x position by using the radius and cos, + center of the circle
        tempY = center.y + (radius * sinf(i * angleSlice)); // Calculates the y position by using the radius and cos, + enter of the circle

        index++;//Increase the index of element we will modify

        vertices[index] = b2Vec2(tempX, tempY);
    }

    //Set the coordinates of the last point as 1,0 so we can close the  drawing  of the "circumference"
    index++;

    vertices[index] =center+b2Vec2(radius, 0);//Close the circle
}

void Box2DDebugDraw::SetViewMatrix(mat4* aViewMatrix)
{
    if (aViewMatrix != nullptr)
    {
        m_pViewMatrix = aViewMatrix;
    }
}

void Box2DDebugDraw::SetProjectionMatrix(mat4* aProjectionMatrix)
{
    if (aProjectionMatrix != nullptr)
    {
        m_pProjectionMatrix = aProjectionMatrix;
    }
}