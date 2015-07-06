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
    void ApplyShaderMaterial(const mat4f &mvp, ShaderMaterial *material);
    void ApplyShader(OpenGLShaderProgramPtr shader, UniformMap uniforms);
    void ApplyMaterial(const mat4f &mvp, Material *material);
    void RenderPolygon(PolygonGeometry *polygon);
    void RenderEllipsisArc(float w2, float h2, float angle1, float angle2, unsigned int segments);
    void RenderArc(ArcGeometry *arc);
    void RenderEllipsis(EllipsisGeometry *ellipsis);
    void RenderPlane(PlaneGeometry *plane);
    void RenderPlane(float w, float h);
    void RenderGeometry(Geometry *geometry);
    void RenderMesh(const mat4f &mvp, Mesh3D *mesh);
    void RenderObject(const mat4f &projection, const mat4f viewmodel, Object3D *object, OpenGLRenderTargetPtr target);
    void InitializeGL();

    typedef std::pair<OpenGLShaderProgramPtr, UniformMap> OpenGLFilter; // TODO Move to opengl_types.h?
    void GetFilterPasses(Filter filter, std::vector<OpenGLFilter> &passes);

    OpenGLRenderTargetPtr AcquireRenderTarget(); // TODO Move to a util of sorts?
    void ReleaseRenderTarget(OpenGLRenderTargetPtr target);

    unsigned int m_width;
    unsigned int m_height;

    std::vector<OpenGLRenderTargetPtr> m_targets;

    OpenGLShaderProgramPtr filterBW;
    OpenGLShaderProgramPtr filterBlurV;
    OpenGLShaderProgramPtr filterBlurH;

//    Material *m_clearMaterial;
    OpenGLTexturePool m_textures;
    OpenGLShaderProgramPool m_shaders;
  };
}
