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

    void render(Scene3D *scene);
    void render(Camera *camera, Object3D *stage, unsigned int width, unsigned int height);

  private:
    void Render(const mat4f &projection, const mat4f viewmodel, Object3D *object, OpenGLRenderTargetPtr target = NULL);
    void RenderFullscreenQuad(OpenGLShaderProgramPtr shader, UniformMap uniforms);

    void ApplyBasicMaterial(Material *material);
    void ApplyFilterMaterial(const mat4f &mvp, FilterMaterial *material);
    void ApplyShaderMaterial(const mat4f &mvp, ShaderMaterial *material);
    void ApplyShader(OpenGLShaderProgramPtr shader, UniformMap uniforms);
    void ApplyMaterial(const mat4f &mvp, Material *material);

    void RenderPlane(float w, float h, vec4f uv);
    void RenderEllipsisArc(float w2, float h2, float angle1, float angle2, unsigned int segments, vec4f uv);

    void RenderPolygon(PolygonGeometry *polygon);
    void RenderArc(ArcGeometry *arc);
    void RenderEllipsis(EllipsisGeometry *ellipsis);
    void RenderPlane(PlaneGeometry *plane);

    void RenderGeometry(Geometry *geometry);

    void RenderMesh(const mat4f &mvp, Mesh3D *mesh);
    void RenderObject(const mat4f &projection, const mat4f viewmodel, Object3D *object, OpenGLRenderTargetPtr target);
    void InitializeGL();

    typedef std::pair<OpenGLShaderProgramPtr, UniformMap> OpenGLFilter; // TODO Move to opengl_types.h?
    void GetFilterPasses(Filter *filter, std::vector<OpenGLFilter> &passes);

    OpenGLRenderTargetPtr AcquireRenderTarget(unsigned int width, unsigned int height); // TODO Move to a util of sorts?
    void ReleaseRenderTarget(OpenGLRenderTargetPtr target);

    unsigned int m_width;
    unsigned int m_height;

    typedef std::vector<OpenGLRenderTargetPtr> RenderTargetList;
    RenderTargetList m_targets;

    OpenGLTexturePool m_textures;
    OpenGLShaderProgramPool m_shaders;
  };
}
