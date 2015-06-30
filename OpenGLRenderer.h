#pragma once
#include "IRenderer.h"

#include "OpenGLRenderTarget.h"
#include "OpenGLTexturePool.h"
#include "OpenGLShaderProgramPool.h"

#include "Mesh3D.h"
#include "OrthographicCamera.h"
#include "ShaderMaterial.h"
#include "PlaneGeometry.h"
#include "CircleGeometry.h"

namespace dawn
{
  class OpenGLRenderer : public IRenderer
  {
  public:
    OpenGLRenderer();

    virtual ~OpenGLRenderer() { }

    void render(Camera *camera, Object3D *scene);

  private:
    void Render(const mat4f &projection, const mat4f viewmodel, Object3D *object, OpenGLRenderTargetPtr target = NULL);
    void RenderFullscreenQuad(OpenGLShaderProgramPtr shader, UniformMap uniforms);

    void ApplyBasicMaterial(Material *material);
    void ApplyShaderMaterial(const mat4f &mvp, ShaderMaterial *material);
    void ApplyShader(OpenGLShaderProgramPtr shader, UniformMap uniforms);
    void ApplyMaterial(const mat4f &mvp, Material *material);
    void RenderCircle(CircleGeometry *circle);
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

    std::vector<OpenGLRenderTargetPtr> m_targets;

    OpenGLShaderProgramPtr filterBW;
    OpenGLShaderProgramPtr filterBlurV;
    OpenGLShaderProgramPtr filterBlurH;

//    Material *m_clearMaterial;
    OpenGLTexturePool m_textures;
    OpenGLShaderProgramPool m_shaders;
  };
}
