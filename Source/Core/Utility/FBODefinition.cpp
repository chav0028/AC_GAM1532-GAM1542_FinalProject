#include "CommonHeader.h"

FBODefinition::FBODefinition()
{
    m_FullyLoaded = false;
    m_FailedToInit = false;
    m_OnlyFreeOnShutdown = false;

    m_ColorTextureHandle = 0;
    m_DepthTextureHandle = 0;
    m_FrameBufferID = 0;

    m_Width = 0;
    m_Height = 0;

    m_TextureWidth = 0;
    m_TextureHeight = 0;

    m_MinFilter = GL_LINEAR;
    m_MagFilter = GL_LINEAR;

    m_NeedColorTexture = true;
    m_DepthBits = 32;
    m_DepthIsTexture = false;

    m_LastFrameBufferID = -1;
}

FBODefinition::~FBODefinition()
{
    glDeleteTextures( 1, &m_ColorTextureHandle );
    if( m_DepthIsTexture )
        glDeleteTextures( 1, &m_DepthTextureHandle );
    else
        glDeleteRenderbuffers( 1, &m_DepthTextureHandle );
    glDeleteFramebuffers( 1, &m_FrameBufferID );
}

// returns true if a new texture needs to be created.
bool FBODefinition::Setup(unsigned int width, unsigned int height)
{
    unsigned int NewTextureWidth = 0;
    unsigned int NewTextureHeight = 0;

    // loop from 64 to 4096 and find appropriate size.
    for( unsigned int pow=6; pow<12; pow++ )
    {
        unsigned int powsize = (unsigned int)(1 << pow);

        if( powsize >= width && NewTextureWidth == 0 )
            NewTextureWidth = powsize;
        if( powsize >= height && NewTextureHeight == 0 )
            NewTextureHeight = powsize;
    }

    bool newtextureneeded = false;

    if( m_TextureWidth != NewTextureWidth || m_TextureHeight != NewTextureHeight )
        newtextureneeded = true;

    m_TextureWidth = NewTextureWidth;
    m_TextureHeight = NewTextureHeight;

    m_Width = width;
    m_Height = height;
    m_MinFilter = GL_LINEAR;
    m_MagFilter = GL_LINEAR;

    m_NeedColorTexture = true;
    m_DepthBits = 32;
    m_DepthIsTexture = true;

    return Create();
}

bool FBODefinition::Create()
{
    if( glGenFramebuffers == 0 )
        return false;

    GLint maxsize;

    glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, &maxsize );

    if( m_TextureWidth > (unsigned int)maxsize || m_TextureHeight > (unsigned int)maxsize )
    {
        // requested size is too big.
        return false;
    }

    assert( m_FrameBufferID == 0 );

    // get a framebuffer, render buffer and a texture from opengl.
    glGenFramebuffers( 1, &m_FrameBufferID );
    
    //Create texture buffer
    if( m_NeedColorTexture )
    {
        glGenTextures( 1, &m_ColorTextureHandle );
    }

    //Create depth buffer
    if( m_DepthBits != 0 )
    {
        assert( m_DepthBits == 16 || m_DepthBits == 24 || m_DepthBits == 32 );

        if( m_DepthIsTexture )
        {
            glGenTextures( 1, &m_DepthTextureHandle );//Texture can be sampled, but is slower
        }
        else
        {
            glGenRenderbuffers( 1, &m_DepthTextureHandle);//can only be written to, but is faster
        }
    }

    // create the texture
    if( m_ColorTextureHandle != 0 )
    {
     //Set color texture
        glBindTexture( GL_TEXTURE_2D, m_ColorTextureHandle );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_TextureWidth, m_TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_MinFilter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_MagFilter );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    // create a depth renderbuffer.
    if( m_DepthTextureHandle != 0 )
    {
        GLint depthformat = GL_DEPTH_COMPONENT32;
        if( m_DepthBits == 24 )
            depthformat = GL_DEPTH_COMPONENT24;
        else if( m_DepthBits == 16 )
            depthformat = GL_DEPTH_COMPONENT16;

        if( m_DepthIsTexture )
        {
            glBindTexture( GL_TEXTURE_2D, m_DepthTextureHandle );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            glTexImage2D( GL_TEXTURE_2D, 0, depthformat, m_TextureWidth, m_TextureHeight, 0,
                          GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0 );
            glBindTexture( GL_TEXTURE_2D, 0 );
        }
        else
        {
            glBindRenderbuffer( GL_RENDERBUFFER, m_DepthTextureHandle );
            glRenderbufferStorage( GL_RENDERBUFFER, depthformat, m_TextureWidth, m_TextureHeight );
        }
    }

    // attach everything to the FBO
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferID );

        // attach color texture
        if( m_ColorTextureHandle != 0 )
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureHandle, 0 );

        // attach depth renderbuffer
        if( m_DepthTextureHandle != 0 )
        {
            if( m_DepthIsTexture )
            {
                glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureHandle, 0 );
            }
            else
            {
               glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthTextureHandle );
            }
        }

        // any problems?
        GLint status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
        if( status != GL_FRAMEBUFFER_COMPLETE )
        {
            glBindFramebuffer( GL_FRAMEBUFFER, 0 );
            glDeleteTextures( 1, &m_ColorTextureHandle );
            m_ColorTextureHandle = 0;
            if( m_DepthIsTexture )
                glDeleteTextures( 1, &m_DepthTextureHandle );
            else
                glDeleteRenderbuffers( 1, &m_DepthTextureHandle );
            m_DepthTextureHandle = 0;

            assert( false );
            return false;
        }

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    CheckForGLErrors();

    return true;
}

void FBODefinition::Bind()
{
    glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferID );
    CheckForGLErrors();
}

void FBODefinition::Unbind()
{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    CheckForGLErrors();
}
