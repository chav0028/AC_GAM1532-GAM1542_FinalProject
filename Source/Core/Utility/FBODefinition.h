#ifndef __FBODefinition_H__
#define __FBODefinition_H__

//Wrapper around frabe buffer object
class FBODefinition
{
protected:

public:
    bool m_FullyLoaded;
    bool m_FailedToInit;
    bool m_OnlyFreeOnShutdown;

    GLuint m_ColorTextureHandle;
    GLuint m_DepthTextureHandle;
    GLuint m_FrameBufferID;

    unsigned int m_Width; // size requested, mainly used by glViewport call.
    unsigned int m_Height;

    unsigned int m_TextureWidth; // generally will be power of 2 bigger than requested width/height
    unsigned int m_TextureHeight;

    int m_MinFilter;
    int m_MagFilter;

    bool m_NeedColorTexture;
    int m_DepthBits;
    bool m_DepthIsTexture;

    int m_LastFrameBufferID;

protected:
    bool Create();

public:
    FBODefinition();
    virtual ~FBODefinition();

    bool Setup(unsigned int width, unsigned int height);

    void Bind();
    void Unbind();
};

#endif //__FBODefinition_H__
