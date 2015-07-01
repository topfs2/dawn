#include "OpenGLRenderer.h"
#include "OpenGLUtils.h"
#include "OpenGLDebug.h"

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;
using namespace dawn;
using boost::any_cast;

OpenGLRenderer::OpenGLRenderer() : m_width(0), m_height(0)
{
  InitializeGL();
}

void OpenGLRenderer::render(Scene3D *scene)
{
  render(scene->camera(), scene->stage(), scene->width(), scene->height());
}

void OpenGLRenderer::render(Camera *camera, Object3D *stage, unsigned int width, unsigned int height)
{
  cout << "Rendering " << width << " " << height << endl;
  OpenGLRenderTarget::revertToDisplayRenderTarget();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_width != width || m_height != height) {
    cout << "Resizing from [" << m_width << ", " << m_height << "] to [" << width << ", " << height << "]" << endl;
    m_width = width;
    m_height = height;
    m_targets.clear();

    glViewport(0.0f, 0.0f, (float)m_width, (float)m_height);
  }

  Render(camera->projection(), camera->view(), stage);
  OpenGLDebug::WriteFBO("final");
}

void OpenGLRenderer::RenderFullscreenQuad(OpenGLShaderProgramPtr shader, UniformMap uniforms)
{
  uniforms["uMVP"] = ortho(-0.5f, 0.5f, 0.5f, -0.5f, -1.0f, 1.0f);
  ApplyShader(shader, uniforms);
  RenderPlane(1.0f, 1.0f);
}

void OpenGLRenderer::Render(const mat4f &projection, const mat4f viewmodel, Object3D *object, OpenGLRenderTargetPtr target)
{
  FilterList filters = object->filters();
  vector<OpenGLFilter> passes;

  for (FilterList::iterator itr = filters.begin(); itr != filters.end(); itr++) {
    GetFilterPasses(*itr, passes);
  }

  if (filters.size() > 0 && passes.size() == 0) {
    // TODO This may be normal but warn during development
    cout << "WARNING Object has filters but was unable to create passes" << endl;
  }

  if (passes.size() > 0) {
    unsigned int targets_index = 0;
    OpenGLRenderTargetPtr targets[2];
    targets[0] = AcquireRenderTarget();
    targets[1] = AcquireRenderTarget();

    targets[0]->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderObject(projection, viewmodel, object, targets[0]);

    OpenGLDebug::WriteFilterInput(object);

    int pass = 0;
    for (vector<OpenGLFilter>::iterator itr = passes.begin(); itr != passes.end(); itr++) {
      OpenGLShaderProgramPtr shader = itr->first;

      UniformMap uniforms = itr->second;
      uniforms["map"] = targets[targets_index]->colorAttachments[0];
      uniforms["texelSize"] = vec2f(1.0f / WINDOW_WIDTH, 1.0f / WINDOW_HEIGHT);

      targets[1 - targets_index]->Bind();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      RenderFullscreenQuad(shader, uniforms);
      targets_index = 1 - targets_index;

      OpenGLDebug::WriteFilterPass(object, pass);
    }

    // TODO Optimize so that final output in above loop pushes to screen
    OpenGLShaderProgramPtr shader = m_shaders.GetResource("shaders/map");
    UniformMap uniforms;
    uniforms["map"] = targets[targets_index]->colorAttachments[0];

    if (target) {
      target->Bind();
    } else {
      OpenGLRenderTarget::revertToDisplayRenderTarget();
    }
    RenderFullscreenQuad(shader, uniforms);

    OpenGLDebug::WriteFilterOutput(object);

    ReleaseRenderTarget(targets[0]);
    ReleaseRenderTarget(targets[1]);
  } else {
    RenderObject(projection, viewmodel, object, target);
  }
}

void OpenGLRenderer::GetFilterPasses(Filter filter, vector<OpenGLFilter> &passes)
{
  if (filter.type == FILTER_GRAYSCALE) {
    passes.push_back(OpenGLFilter(m_shaders.GetResource("shaders/filter.bw"), filter.uniforms));
  } else if (filter.type == FILTER_GAUSIAN_BLUR) {
    passes.push_back(OpenGLFilter(m_shaders.GetResource("shaders/filter.bh"), filter.uniforms));
    passes.push_back(OpenGLFilter(m_shaders.GetResource("shaders/filter.bv"), filter.uniforms));
  }
}

void OpenGLRenderer::ApplyBasicMaterial(Material *material)
{
  glUseProgram(0);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  check_gl_error();
}

void OpenGLRenderer::ApplyShaderMaterial(const mat4f &mvp, ShaderMaterial *material)
{
  std::string path = material->path();
  OpenGLShaderProgramPtr shader = m_shaders.GetResource(path);

  UniformMap uniforms = material->uniforms();
  uniforms["uMVP"] = mvp;
  ApplyShader(shader, uniforms);
}

void OpenGLRenderer::ApplyShader(OpenGLShaderProgramPtr shader, UniformMap uniforms)
{
  shader->Bind();

  GLint textureUnit = 0;
  for (UniformMap::iterator itr = uniforms.begin(); itr != uniforms.end(); itr++) {
    uniform_t u = itr->second;

    if (u.type() == typeid(int)) {
      shader->uniform(itr->first, any_cast<int>(u));
    } else if (u.type() == typeid(float)) {
      shader->uniform(itr->first, any_cast<float>(u));
    } else if (u.type() == typeid(vec2f)) {
      shader->uniform(itr->first, any_cast<vec2f>(u));
    } else if (u.type() == typeid(vec3f)) {
      shader->uniform(itr->first, any_cast<vec3f>(u));
    } else if (u.type() == typeid(vec4f)) {
      shader->uniform(itr->first, any_cast<vec4f>(u));
    } else if (u.type() == typeid(mat4f)) {
      shader->uniform(itr->first, any_cast<mat4f>(u));
    } else if (u.type() == typeid(Image*)) {
      Image *i = any_cast<Image *>(u);

      shader->uniform(itr->first, textureUnit);
      m_textures.GetResource(i)->Bind(textureUnit);
      textureUnit++;
    } else if (u.type() == typeid(OpenGLTexturePtr)) {
      OpenGLTexturePtr t = any_cast<OpenGLTexturePtr>(u);

      shader->uniform(itr->first, textureUnit);
      t->Bind(textureUnit);
      textureUnit++;
    }

    check_gl_error();
  }
}

void OpenGLRenderer::ApplyMaterial(const mat4f &mvp, Material *material)
{
  switch (material->type())
  {
  case CONSTANTS::ShaderMaterial:
    ApplyShaderMaterial(mvp, (ShaderMaterial *)material);
    break;
  }
}

void OpenGLRenderer::RenderPlane(PlaneGeometry *plane)
{
  RenderPlane(plane->width(), plane->height());
}

void OpenGLRenderer::RenderPlane(float w, float h)
{
  float w2 = w * 0.5f;
  float h2 = h * 0.5f;

  /* TODO Use indices to avoid duplicate vertex upload */
  GLfloat aVertices[] = { -w2,  h2,
                          -w2, -h2,
                           w2, -h2,
                          -w2,  h2,
                           w2, -h2,
                           w2,  h2 };

  GLfloat aUV[] =       { 0.0f,  0.0f,
                          0.0f,  1.0f,
                          1.0f,  1.0f,
                          0.0f,  0.0f,
                          1.0f,  1.0f,
                          1.0f,  0.0f };

  glVertexAttribPointer(OpenGLShaderProgram::POSITION, 2, GL_FLOAT, GL_FALSE, 0, aVertices);
  glEnableVertexAttribArray(OpenGLShaderProgram::POSITION);

  glVertexAttribPointer(OpenGLShaderProgram::UV, 2, GL_FLOAT, GL_FALSE, 0, aUV);
  glEnableVertexAttribArray(OpenGLShaderProgram::UV);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  check_gl_error();
}

void OpenGLRenderer::RenderEllipsis(EllipsisGeometry *ellipsis)
{
  float w2 = ellipsis->width() * 0.5f;
  float h2 = ellipsis->height() * 0.5f;
  float degrees = 2.0f * M_PI / (float)ellipsis->segments();

  unsigned int vertexCount = ellipsis->segments() * 2 + 1;
  GLfloat aVertices[vertexCount * 2];
  GLfloat aUV[vertexCount * 2];

  aVertices[0] = 0.0f;
  aVertices[1] = 0.0f;

  aUV[0] = 0.5f;
  aUV[1] = 0.5f;

  unsigned int idx = 2;
  unsigned int outerVertexCount = vertexCount - 1;
  for (int i = 0; i < outerVertexCount; ++i) {
    float x = cos(degrees * (float)i);
    float y = sin(degrees * (float)i);

    aVertices[idx]     = x * w2;
    aVertices[idx + 1] = y * h2;

    aUV[idx]     = x *  0.5f + 0.5f;
    aUV[idx + 1] = y * -0.5f + 0.5f;

    idx += 2;
  }

  glVertexAttribPointer(OpenGLShaderProgram::POSITION, 2, GL_FLOAT, GL_FALSE, 0, aVertices);
  glEnableVertexAttribArray(OpenGLShaderProgram::POSITION);

  glVertexAttribPointer(OpenGLShaderProgram::UV, 2, GL_FLOAT, GL_FALSE, 0, aUV);
  glEnableVertexAttribArray(OpenGLShaderProgram::UV);

  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

  check_gl_error();
}

void OpenGLRenderer::RenderGeometry(Geometry *geometry)
{
  switch (geometry->type())
  {
  case CONSTANTS::PlaneGeometry:
    RenderPlane((PlaneGeometry *)geometry);
    break;

  case CONSTANTS::EllipsisGeometry:
    RenderEllipsis((EllipsisGeometry *)geometry);
    break;
  }
}

void OpenGLRenderer::RenderMesh(const mat4f &mvp, Mesh3D *mesh)
{
  ApplyMaterial(mvp, mesh->material());
  RenderGeometry(mesh->geometry());

  check_gl_error();
}

void OpenGLRenderer::RenderObject(const mat4f &projection, const mat4f viewmodel, Object3D *object, OpenGLRenderTargetPtr target)
{
  if (!object->visible()) {
    return;
  }

  mat4f m = viewmodel * object->transform();
  mat4f mvp = projection * m;

  switch (object->type())
  {
    case CONSTANTS::Mesh:
      RenderMesh(mvp, (Mesh3D *)object);
  }

  for (Object3D::iterator itr = object->begin(); itr != object->end(); itr++) {
    Render(projection, m, *itr, target);
  }
  check_gl_error();
}

void OpenGLRenderer::InitializeGL()
{
  glShadeModel(GL_SMOOTH);

  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
  glEnable(GL_CULL_FACE);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG_RENDER_WIREFRAME
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

  glEnable(GL_TEXTURE_2D);
}

OpenGLRenderTargetPtr OpenGLRenderer::AcquireRenderTarget()
{
  OpenGLRenderTargetPtr target;

  if (m_targets.size() > 0) {
    target = m_targets.back();
    m_targets.pop_back();
  } else {
    cout << "Creating a render target" << endl;
    target = OpenGLRenderTargetPtr(new OpenGLRenderTarget(m_width, m_height));
  }

  return target;
}

void OpenGLRenderer::ReleaseRenderTarget(OpenGLRenderTargetPtr target)
{
  m_targets.push_back(target);
}
