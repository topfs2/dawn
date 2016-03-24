#pragma once
#include "IRenderer.h"

#include "OpenGLRenderTarget.h"
#include "OpenGLTexturePool.h"
#include "OpenGLShaderProgramPool.h"

#include "dawn.h"

namespace dawn
{
  class OpenGLRenderer : public IRenderer
  {
  public:
    OpenGLRenderer();

    virtual ~OpenGLRenderer() { }

    void render(const Element &scene);

  private:

    void RenderCamera(const Element &camera);
    void RenderElement(const Element &element, const mat4f &projection, const mat4f &viewmodel);

    void RenderMesh3D(const Element &element, const mat4f &mvp);

    void RenderFullscreenQuad(OpenGLShaderProgramPtr shader, UniformMap uniforms);

    void PrepareMask();

    void ApplyShader(OpenGLShaderProgramPtr shader, UniformMap uniforms);
    void ApplyShaderMaterial(const ShaderMaterial &material, const mat4f &mvp);
    void ApplyMaterial(const boost::any &material, const mat4f &mvp);


    void RenderRaw(const vec3farray &positions, const vec4farray color, const vec2farray &uv, const iarray &indices);

    void RenderPlane(float w, float h, vec4f uv);

/*
    void RenderRaw(RawGeometry *raw);
    void RenderPolygon(PolygonGeometry *polygon);
    void RenderArc(ArcGeometry *arc);
    void RenderEllipsis(EllipsisGeometry *ellipsis);
*/
    void RenderPlane(const PlaneGeometry &plane);

    void RenderGeometry(const boost::any &geometry);
/*
    void RenderRoundedRectangle(RoundedRectangleGeometry *rect);
    void RenderFillPathGeometry(FillPathGeometry *path);
    void RenderStrokePathGeometry(StrokePathGeometry *path);
*/

    void InitializeGL();

/*
    typedef std::pair<OpenGLShaderProgramPtr, UniformMap> OpenGLFilter; // TODO Move to opengl_types.h?
    void GetFilterPasses(Filter *filter, std::vector<OpenGLFilter> &passes);

    OpenGLRenderTargetPtr AcquireRenderTarget(unsigned int width, unsigned int height); // TODO Move to a util of sorts?
    void ReleaseRenderTarget(OpenGLRenderTargetPtr target);
*/

    unsigned int m_width;
    unsigned int m_height;
/*
    typedef std::vector<OpenGLRenderTargetPtr> RenderTargetList;
    RenderTargetList m_targets;
*/

    OpenGLTexturePool m_textures;
    OpenGLShaderProgramPool m_shaders;


    OpenGLTexturePtr m_backbufferTexture;

    typedef std::pair<boost::any, mat4f> Mask;
    typedef std::vector<Mask> MaskList;
    MaskList m_masks;
  };
}
