#ifndef __Box2DDebugDraw__H__
#define  __Box2DDebugDraw__H__

const unsigned int DEBUG_DRAW_DEFAULT_CIRCLE_SLICES = 40;
const unsigned int DEBUG_DRAW_OUTLINE_ALPHA = 255;
const unsigned int DEBUG_DRAW_SHAPE_ALPHA = 100;
const float DEBUG_DRAW_OUTLINE_WIDTH = 1.0f;
const float DEBUG_DRAW_TRANSFORM_SCALE = 0.5f;

class Mesh;
class ShaderProgram;

class Box2DDebugDraw :public b2Draw
{
protected:
    ShaderProgram* m_pShaderProgram;
    mat4* m_pViewMatrix;
    mat4* m_pProjectionMatrix;

    virtual void PlotCirclePoints(b2Vec2* vertices, const b2Vec2& center, float32 radius);

public:
    Box2DDebugDraw();
    ~Box2DDebugDraw();

    virtual void Draw(const b2Vec2* vertices, unsigned int vertexCount, const b2Color& color, unsigned char alpha, GLenum primitiveType);

    /// Draw a closed polygon provided in CCW order.
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    /// Draw a solid closed polygon provided in CCW order.
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    /// Draw a circle.
    virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

    /// Draw a solid circle.
    virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

    /// Draw a line segment.
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

    /// Draw a transform. Choose your own length scale.
    /// @param xf a transform.
    virtual void DrawTransform(const b2Transform& xf);

    void SetShader(ShaderProgram* shader);
    void SetViewMatrix(mat4* viewMatrix);
    void SetProjectionMatrix(mat4* projectionMatrix);
};

#endif