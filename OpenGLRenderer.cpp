#include "OpenGLRenderer.h"
#include "OpenGLUtils.h"
#include "OpenGLDebug.h"
#include "LinearAlgebra.h"

//#include "GeometryUtils.h"

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

void OpenGLRenderer::render(const Element &scene)
{
    cout << "Rendering " << scene.type() << endl;

    if (scene.is("Scene3D") && scene.has("width") && scene.has("height")) {
        int width = scene.get<int>("width", m_width);
        int height = scene.get<int>("height", m_height);

        OpenGLRenderTarget::revertToDisplayRenderTarget();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_width != width || m_height != height) {
            cout << "Resizing from [" << m_width << ", " << m_height << "] to [" << width << ", " << height << "]" << endl;
            m_width = width;
            m_height = height;
            //m_targets.clear();

            glViewport(0.0f, 0.0f, (float)m_width, (float)m_height);
        }

        PrepareMask();
        for (ChildVector::const_iterator itr = scene.children.begin(); itr != scene.children.end(); itr++) {
            RenderCamera(*itr);
        }
    }
}

void OpenGLRenderer::RenderCamera(const Element &camera)
{
    cout << "Rendering " << camera.type() << endl;

    if (camera.is("OrthographicCamera")) {
        float width = camera.get<float>("width", 2.0f);
        float height = camera.get<float>("height", 2.0f);
        float near = camera.get<float>("near", -1.0f);
        float far = camera.get<float>("far", 1.0f);

        mat4f projection = ortho(width / -2.0f, width / 2.0f, height / 2.0f, height / -2.0f, near, far);

        for (ChildVector::const_iterator itr = camera.children.begin(); itr != camera.children.end(); itr++) {
            RenderElement(*itr, projection, mat4f::Identity());
        }
    }
}

void OpenGLRenderer::RenderElement(const Element &element, const mat4f &projection, const mat4f &viewmodel)
{
    cout << "Rendering " << element.type() << endl;

    mat4f transform = viewmodel * element.get<mat4f>("transform", mat4f::Identity());

    if (element.has("mask")) {
        PropertyMap::const_iterator mItr = element.props.find("mask");

        if (mItr->second.type() == typeid(PlaneGeometry)) {
            m_masks.push_back(Mask(boost::any_cast<PlaneGeometry>(mItr->second), projection * transform));
        } else {
            cout << "Bad mask" << endl;
            return;
        }

        PrepareMask();
    }

    if (element.is("Mesh3D")) {
        RenderMesh3D(element, projection * transform);
    }

    for (ChildVector::const_iterator itr = element.children.begin(); itr != element.children.end(); itr++) {
        RenderElement(*itr, projection, transform);
    }

    if (element.has("mask")) {
        m_masks.pop_back();
        PrepareMask();
    }
}

void OpenGLRenderer::RenderMesh3D(const Element &element, const mat4f &mvp)
{
    PropertyMap::const_iterator mItr = element.props.find("material");
    PropertyMap::const_iterator gItr = element.props.find("geometry");

    if (mItr != element.props.end() && gItr != element.props.end()) {
        ApplyMaterial(mItr->second, mvp);
        RenderGeometry(gItr->second);
    }
}

void OpenGLRenderer::RenderFullscreenQuad(OpenGLShaderProgramPtr shader, UniformMap uniforms)
{
  uniforms["uMVP"] = ortho(-0.5f, 0.5f, 0.5f, -0.5f, -1.0f, 1.0f);
  ApplyShader(shader, uniforms);
  RenderPlane(1.0f, 1.0f, vec4f(0, 1, 0, 1));
}


void OpenGLRenderer::PrepareMask()
{
  if (m_masks.size() > 0) {
    glStencilMask(~0);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    OpenGLShaderProgramPtr shader = m_shaders.GetResource("shaders/color");
    UniformMap uniforms;
    uniforms["color"] = vec4f(1, 1, 1, 1);

    for (MaskList::iterator itr = m_masks.begin(); itr != m_masks.end(); itr++) {
      uniforms["uMVP"] = itr->second;
      ApplyShader(shader, uniforms);
      RenderGeometry(itr->first);
    }

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0x00);
    glStencilFunc(GL_EQUAL, 1, 0xFF);

    OpenGLDebug::WriteStencil("prepare");
  } else {
    glDisable(GL_STENCIL_TEST);
  }
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
    } else if (u.type() == typeid(mat3f)) {
      shader->uniform(itr->first, any_cast<mat3f>(u));
    } else if (u.type() == typeid(mat4f)) {
      shader->uniform(itr->first, any_cast<mat4f>(u));
    } else if (u.type() == typeid(Pixmap *)) {
      Pixmap *i = any_cast<Pixmap *>(u);

      shader->uniform(itr->first, textureUnit);

/*
      if (i->type() == CONSTANTS::BackbufferPixmap) {
          m_backbufferTexture->Bind(textureUnit);
          BackbufferPixmap *bpixmap = static_cast<BackbufferPixmap *>(i);

          vec2i position = bpixmap->position();
          vec2i size = bpixmap->size();
          glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, position[0], position[1], size[0], size[1], 0);
      } else {
*/
          m_textures.GetResource(i)->Bind(textureUnit);
//      }

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

void OpenGLRenderer::ApplyShaderMaterial(const ShaderMaterial &material, const mat4f &mvp)
{
  std::string path = material.path();
  OpenGLShaderProgramPtr shader = m_shaders.GetResource(path);

  UniformMap uniforms = material.uniforms();
  uniforms["uMVP"] = mvp;
  ApplyShader(shader, uniforms);
}

void OpenGLRenderer::ApplyMaterial(const boost::any &material, const mat4f &mvp)
{
    if (material.type() == typeid(ShaderMaterial)) {
        ApplyShaderMaterial(boost::any_cast<ShaderMaterial>(material), mvp);
    } else {
        cout << "Bad mask" << endl;
    }
}

void OpenGLRenderer::RenderRaw(const vec3farray &positions, const vec4farray color, const vec2farray &uv, const iarray &indices)
{
  GLfloat aPosition[positions.size() * 3];
  GLfloat aColor[color.size() * 4];
  GLfloat aUV[uv.size() * 2];

  unsigned int i = 0;
  for (vec3farray::const_iterator itr = positions.begin(); itr != positions.end(); itr++) {
    aPosition[i++] = (*itr)[0];
    aPosition[i++] = (*itr)[1];
    aPosition[i++] = (*itr)[2];
  }

  i = 0;
  for (vec4farray::const_iterator itr = color.begin(); itr != color.end(); itr++) {
    aColor[i++] = (*itr)[0];
    aColor[i++] = (*itr)[1];
    aColor[i++] = (*itr)[2];
    aColor[i++] = (*itr)[3];
  }

  i = 0;
  for (vec2farray::const_iterator itr = uv.begin(); itr != uv.end(); itr++) {
    aUV[i++] = (*itr)[0];
    aUV[i++] = (*itr)[1];
  }

  glVertexAttribPointer(OpenGLShaderProgram::POSITION, 3, GL_FLOAT, GL_FALSE, 0, aPosition);
  glEnableVertexAttribArray(OpenGLShaderProgram::POSITION);

  glVertexAttribPointer(OpenGLShaderProgram::COLOR, 4, GL_FLOAT, GL_FALSE, 0, aColor);
  glEnableVertexAttribArray(OpenGLShaderProgram::COLOR);

  glVertexAttribPointer(OpenGLShaderProgram::UV, 2, GL_FLOAT, GL_FALSE, 0, aUV);
  glEnableVertexAttribArray(OpenGLShaderProgram::UV);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, &indices.front());
}

void OpenGLRenderer::RenderPlane(float w, float h, vec4f uv)
{
  float w2 = w * 0.5f;
  float h2 = h * 0.5f;

  float u0 = uv[0];
  float u1 = uv[1];
  float v0 = uv[2];
  float v1 = uv[3];

  /* TODO Use indices to avoid duplicate vertex upload */
  GLfloat aVertices[] = { -w2,  h2,
                          -w2, -h2,
                           w2, -h2,
                          -w2,  h2,
                           w2, -h2,
                           w2,  h2 };

  GLfloat aUV[] =       { u0,  v0,
                          u0,  v1,
                          u1,  v1,
                          u0,  v0,
                          u1,  v1,
                          u1,  v0 };

  glVertexAttribPointer(OpenGLShaderProgram::POSITION, 2, GL_FLOAT, GL_FALSE, 0, aVertices);
  glEnableVertexAttribArray(OpenGLShaderProgram::POSITION);

  glVertexAttribPointer(OpenGLShaderProgram::UV, 2, GL_FLOAT, GL_FALSE, 0, aUV);
  glEnableVertexAttribArray(OpenGLShaderProgram::UV);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  check_gl_error();
}

void OpenGLRenderer::RenderPlane(const PlaneGeometry &plane)
{
  RenderPlane(plane.width(), plane.height(), plane.uv());
}

void OpenGLRenderer::RenderGeometry(const boost::any &geometry)
{
    if (geometry.type() == typeid(PlaneGeometry)) {
        RenderPlane(boost::any_cast<PlaneGeometry>(geometry));
    } else {
        cout << "Bad geometry" << endl;
    }
}

/*
void OpenGLRenderer::RenderObject3D(const mat4f &projection, const mat4f viewmodel, Object3D *object, OpenGLRenderTargetPtr target)
{
  if (!object->visible()) {
    return;
  }

  mat4f m = viewmodel * object->transform();
  mat4f mvp = projection * m;

  switch (object->type())
  {
    case CONSTANTS::Mesh3D:
      RenderMesh3D(mvp, (Mesh3D *)object);
      break;
    case CONSTANTS::Mask3D:
      m_masks.push_back(Mask((Mask3D *)object, mvp));
      PrepareMask();
      break;
  }

  for (Object3D::iterator itr = object->begin(); itr != object->end(); itr++) {
    Render(projection, m, *itr, target);
  }

  if (object->type() == CONSTANTS::Mask3D) {
    m_masks.pop_back();
    PrepareMask();
  }

  check_gl_error();
}
*/

void OpenGLRenderer::InitializeGL()
{
  glShadeModel(GL_SMOOTH);

//  glCullFace(GL_BACK);
//  glFrontFace(GL_CW);
//  glEnable(GL_CULL_FACE);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // post multiplied alpha
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE); // pre multiplied alpha according to https://developer.nvidia.com/content/alpha-blending-pre-or-not-pre
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // pre multiplied alpha according to http://www.gamedev.net/topic/413204-premultiplied-alpha/ which makes text_trial generate fbo perfectly matching pango output

#ifdef DEBUG_RENDER_WIREFRAME
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

  glEnable(GL_TEXTURE_2D);

  m_backbufferTexture = OpenGLTexturePtr(new OpenGLTexture(OpenGLUtils::CreateTexture()));

  glDisable(GL_STENCIL_TEST);
  glStencilMask(0x00);
}

#if 0
OpenGLRenderTargetPtr OpenGLRenderer::AcquireRenderTarget(unsigned int width, unsigned int height)
{
  for (RenderTargetList::iterator itr = m_targets.begin(); itr != m_targets.end(); itr++) {
    if ((*itr)->width == width && (*itr)->height == height) {
      OpenGLRenderTargetPtr target = *itr;
      m_targets.erase(itr);
      return target;
    }
  }

  cout << "Creating a render target [" << width << ", " << height << "]" << endl;
  return OpenGLRenderTargetPtr(new OpenGLRenderTarget(width, height));
}

void OpenGLRenderer::ReleaseRenderTarget(OpenGLRenderTargetPtr target)
{
  m_targets.push_back(target);
}
#endif
