#include "CommonHeader.h"

void MyMatrix::SetIdentity()
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
    m11 = m22 = m33 = m44 = 1;
}

void MyMatrix::SetAxesView(const Vector3& right, const Vector3& up, const Vector3& at, const Vector3& pos)
{
    m11 = right.x; m21 = right.y; m31 = right.z; m41 = pos.x;
    m12 = up.x;    m22 = up.y;    m32 = up.z;    m42 = pos.y;
    m13 = at.x;    m23 = at.y;    m33 = at.z;    m43 = pos.z;
    m14 = 0;       m24 = 0;       m34 = 0;       m44 = 1;
}

void MyMatrix::SetAxesWorld(const Vector3& right, const Vector3& up, const Vector3& at, const Vector3& pos)
{
    m11 = right.x; m21 = up.x; m31 = at.x; m41 = pos.x;
    m12 = right.y; m22 = up.y; m32 = at.y; m42 = pos.y;
    m13 = right.z; m23 = up.z; m33 = at.z; m43 = pos.z;
    m14 = 0;       m24 = 0;    m34 = 0;    m44 = 1;
}

void MyMatrix::SetTranslation(Vector3 pos)
{
    m41 = pos.x;
    m42 = pos.y;
    m43 = pos.z;
}

void MyMatrix::SetTranslation(float x, float y, float z)
{
    m41 = x;
    m42 = y;
    m43 = z;
}

void MyMatrix::CreateScale(float scale)
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
    m11 = m22 = m33 = scale;
    m44 = 1;
}

void MyMatrix::CreateScale(float x, float y, float z)
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
    m11 = x;
    m22 = y;
    m33 = z;
    m44 = 1;
}

void MyMatrix::CreateScale(Vector3 scale)
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
    m11 = scale.x;
    m22 = scale.y;
    m33 = scale.z;
    m44 = 1;
}

void MyMatrix::CreateSRT(float scale, Vector3 rot, Vector3 pos)
{
    SetIdentity();
    Scale( scale );
    Rotate( rot.z, 0, 0, 1 ); // roll
    Rotate( rot.x, 1, 0, 0 ); // pitch
    Rotate( rot.y, 0, 1, 0 ); // yaw
    Translate( pos.x, pos.y, pos.z );
}

void MyMatrix::CreateSRT(Vector3 scale, Vector3 rot, Vector3 pos)
{
    CreateScale( scale.x, scale.y, scale.z );
    Rotate( rot.z, 0, 0, 1 ); // roll
    Rotate( rot.x, 1, 0, 0 ); // pitch
    Rotate( rot.y, 0, 1, 0 ); // yaw
    Translate( pos.x, pos.y, pos.z );
}

void MyMatrix::Scale(float scale)
{
    m11 *= scale; m21 *= scale; m31 *= scale; m41 *= scale;
    m12 *= scale; m22 *= scale; m32 *= scale; m42 *= scale;
    m13 *= scale; m23 *= scale; m33 *= scale; m43 *= scale;
}

void MyMatrix::Scale(float sx, float sy, float sz)
{
    m11 *= sx; m21 *= sx; m31 *= sx; m41 *= sx;
    m12 *= sy; m22 *= sy; m32 *= sy; m42 *= sy;
    m13 *= sz; m32 *= sz; m33 *= sz; m43 *= sz;
}

void MyMatrix::Rotate(float angle, float x, float y, float z)
{
    float sinAngle, cosAngle;
    float mag = sqrtf(x * x + y * y + z * z);
      
    sinAngle = sinf( angle * PI / 180.0f );
    cosAngle = cosf( angle * PI / 180.0f );
    if( mag > 0.0f )
    {
        float xx, yy, zz, xy, yz, zx, xs, ys, zs;
        float oneMinusCos;
   
        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        MyMatrix rotMat;
        rotMat.m11 = (oneMinusCos * xx) + cosAngle;
        rotMat.m12 = (oneMinusCos * xy) - zs;
        rotMat.m13 = (oneMinusCos * zx) + ys;
        rotMat.m14 = 0.0f; 

        rotMat.m21 = (oneMinusCos * xy) + zs;
        rotMat.m22 = (oneMinusCos * yy) + cosAngle;
        rotMat.m23 = (oneMinusCos * yz) - xs;
        rotMat.m24 = 0.0f;

        rotMat.m31 = (oneMinusCos * zx) - ys;
        rotMat.m32 = (oneMinusCos * yz) + xs;
        rotMat.m33 = (oneMinusCos * zz) + cosAngle;
        rotMat.m34 = 0.0f; 

        rotMat.m41 = 0.0f;
        rotMat.m42 = 0.0f;
        rotMat.m43 = 0.0f;
        rotMat.m44 = 1.0f;

        *this = rotMat * *this;
    }
}

void MyMatrix::TranslatePreRotScale(Vector3 translate)
{
    m41 += m11 * translate.x  +  m21 * translate.y  +  m31 * translate.z;
    m42 += m12 * translate.x  +  m22 * translate.y  +  m32 * translate.z;
    m43 += m13 * translate.x  +  m23 * translate.y  +  m33 * translate.z;
    m44 += m14 * translate.x  +  m24 * translate.y  +  m34 * translate.z;
}

void MyMatrix::TranslatePreRotScale(float tx, float ty, float tz)
{
    m41 += m11 * tx  +  m21 * ty  +  m31 * tz;
    m42 += m12 * tx  +  m22 * ty  +  m32 * tz;
    m43 += m13 * tx  +  m23 * ty  +  m33 * tz;
    m44 += m14 * tx  +  m24 * ty  +  m34 * tz;
}

void MyMatrix::Translate(Vector3 pos)
{
    m41 += pos.x;
    m42 += pos.y;
    m43 += pos.z;
}

void MyMatrix::Translate(float x, float y, float z)
{
    m41 += x;
    m42 += y;
    m43 += z;
}

void MyMatrix::CreateFrustum(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;

    assert( nearZ > 0.0f && farZ > 0.0f && deltaX > 0.0f && deltaY > 0.0f && deltaZ > 0.0f );

    m11 = 2.0f * nearZ / deltaX;
    m12 = m13 = m14 = 0.0f;

    m22 = 2.0f * nearZ / deltaY;
    m21 = m23 = m24 = 0.0f;

    m31 = (right + left) / deltaX;
    m32 = (top + bottom) / deltaY;
#if MYFW_RIGHTHANDED
    m33 = -(nearZ + farZ) / deltaZ;
    m34 = -1.0f;
#else
    m33 = (nearZ + farZ) / deltaZ;
    m34 = 1.0f;
#endif

    m43 = -2.0f * nearZ * farZ / deltaZ;
    m41 = m42 = m44 = 0.0f;
}

void MyMatrix::CreatePerspectiveVFoV(float halfvertfovdegrees, float aspect, float nearZ, float farZ)
{
    GLfloat frustumRight, frustumTop;
   
    frustumTop = tanf( halfvertfovdegrees / 360.0f * PI ) * nearZ;
    frustumRight = frustumTop * aspect;

    CreateFrustum( -frustumRight, frustumRight, -frustumTop, frustumTop, nearZ, farZ );
}

void MyMatrix::CreatePerspectiveHFoV(float halfhorfovdegrees, float aspect, float nearZ, float farZ)
{
    GLfloat frustumRight, frustumTop;
   
    frustumRight = tanf( halfhorfovdegrees / 360.0f * PI ) * nearZ;
    frustumTop = frustumRight / aspect;

    CreateFrustum( -frustumRight, frustumRight, -frustumTop, frustumTop, nearZ, farZ );
}

void MyMatrix::CreateOrtho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = (right - left);
    float deltaY = (top - bottom);
    float deltaZ = (farZ - nearZ);

    assert( (deltaX != 0.0f) && (deltaY != 0.0f) && (deltaZ != 0.0f) );

    m11 = 2.0f / deltaX;
    m12 = m13 = m14 = 0;

    m22 = 2.0f / deltaY;
    m21 = m23 = m24 = 0;

#if MYFW_RIGHTHANDED
    m33 = -2.0f / deltaZ;
#else
    m33 = 2.0f / deltaZ;
#endif
    m31 = m32 = m34 = 0;

    m41 = -(right + left) / deltaX;
    m42 = -(top + bottom) / deltaY;
    m43 = -(farZ + nearZ) / deltaZ;
    m44 = 1;
}

void MyMatrix::CreateLookAtViewLeftHanded(const Vector3 &eye, const Vector3 &up, const Vector3 &at)
{
    Vector3 zaxis = (at - eye).Normalize();
    Vector3 xaxis = (up.Cross(zaxis)).Normalize();
    Vector3 yaxis = zaxis.Cross(xaxis);

    Vector3 pos = Vector3( -xaxis.Dot( eye ), -yaxis.Dot( eye ), -zaxis.Dot( eye ) );

    SetAxesView( xaxis, yaxis, zaxis, pos );
}

void MyMatrix::CreateLookAtView(const Vector3 &eye, const Vector3 &up, const Vector3 &at)
{
    Vector3 zaxis = (eye - at).Normalize();
    Vector3 xaxis = (up.Cross(zaxis)).Normalize();
    Vector3 yaxis = zaxis.Cross(xaxis);

    Vector3 pos = Vector3( -xaxis.Dot( eye ), -yaxis.Dot( eye ), -zaxis.Dot( eye ) );

    SetAxesView( xaxis, yaxis, zaxis, pos );
}

void MyMatrix::CreateLookAtWorld(const Vector3& objpos, const Vector3& up, const Vector3& at)
{
    Vector3 zaxis = (at - objpos).Normalize();
    Vector3 xaxis = (up.Cross(zaxis)).Normalize();
    Vector3 yaxis = zaxis.Cross(xaxis);

    SetAxesWorld( xaxis, yaxis, zaxis, objpos );
}

Vector3 MyMatrix::GetEulerAngles()
{
    // from http://www.geometrictools.com/Documentation/EulerAngles.pdf and adapted to fit

    //if( m32 < +1 )
    //{
    //    if( m32 > -1 )
    //    {
    //        float x = asin( m32 );
    //        float y = atan2( -m31, m33 );
    //        float z = atan2( -m12, m22 );
    //        return vector3( x, y, z );
    //    }
    //    else // m32 = -1
    //    {
    //        // not a unique solution: thetaz - thetay = atan2(-m21,m11)
    //        float x = pi/2;
    //        float y = atan2( m21, m11 );
    //        float z = 0;
    //        return vector3( x, y, z );
    //    }
    //}
    //else // m32 = +1
    //{
    //    // not a unique solution: thetaz + thetay = atan2(-m21,m11)
    //    float x = -pi/2;
    //    float y = -atan2( m21, m11 );
    //    float z = 0;
    //    return vector3( x, y, z );
    //}

    // rearranged from above and using FEQUALEPSILON to give special cases more chance of hitting
    if( m32 > 1.0f - FEQUALEPSILON ) // Not a unique solution: thetaZ - thetaY = atan2( -m21, m11 )
    {
        float x = PI/2;
        float y = atan2f( m21, m11 );
        float z = 0.0f;
        return Vector3( x, y, z );
    }
    else if( m32 < -1.0f + FEQUALEPSILON ) // Not a unique solution: thetaZ + thetaY = atan2( -m21, m11 )
    {
        float x = -PI/2;
        float y = -atan2f( m21, m11 );
        float z = 0.0f;
        return Vector3( x, y, z );
    }
    else
    {
        float x = asinf( m32 );
        float y = atan2f( -m31, m33 );
        float z = atan2f( -m12, m22 );
        return Vector3( x, y, z );
    }
}

Vector3 MyMatrix::GetUp()
{
    return Vector3( m21, m22, m23 );
}

Vector3 MyMatrix::GetRight()
{
    return Vector3( m11, m12, m13 );
}

Vector3 MyMatrix::GetAt()
{
    return Vector3( m31, m32, m33 );
}
