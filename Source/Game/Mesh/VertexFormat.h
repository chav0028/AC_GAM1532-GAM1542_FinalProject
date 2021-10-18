#ifndef __VertexFormat_H__
#define __VertexFormat_H__

struct VertexFormat
{
    Vector3 pos;
    MyColor color;
    Vector2 uv;
    Vector3 normal;

    VertexFormat() {}
    VertexFormat(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v, float nx, float ny, float nz)
    {
        pos.Set( x, y, z );
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
        uv.Set( u, v );
        normal.Set( nx, ny, nz );
    }    

    VertexFormat(vec3 aPos, vec2 aUV, vec3 aNormals)
    {
        pos.Set(aPos.x, aPos.y, aPos.z);
        uv.Set(aUV.x, aUV.y);
        normal.Set(aNormals.x, aNormals.y, aNormals.z);

    }
    void FillVertexFormat(Vector3 aPosition, Vector4 aColor, Vector2 aUV, Vector3 aNormals)
    {
        pos.Set(aPosition.x, aPosition.y, aPosition.z);
        color.r = (char)aColor.x*255;
        color.g = (char)aColor.y * 255;
        color.b = (char)aColor.z * 255;
        color.a = (char)aColor.y * 255;
        uv.Set(aUV.x, aUV.y);
        normal.Set(aNormals.x, aNormals.y, aNormals.z);
    }
};

#endif //__VertexFormat_H__
