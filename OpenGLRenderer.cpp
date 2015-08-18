#include "OpenGLRenderer.h"
#include "OpenGLUtils.h"
#include "OpenGLDebug.h"

#include "GeometryUtils.h"

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
  OpenGLRenderTarget::revertToDisplayRenderTarget();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_width != width || m_height != height) {
    cout << "Resizing from [" << m_width << ", " << m_height << "] to [" << width << ", " << height << "]" << endl;
    m_width = width;
    m_height = height;
    m_targets.clear();

    glViewport(0.0f, 0.0f, (float)m_width, (float)m_height);
  }

  PrepareMask();
  Render(camera->projection(), camera->view(), stage);
  OpenGLDebug::WriteFBO("final");
}

void OpenGLRenderer::RenderFullscreenQuad(OpenGLShaderProgramPtr shader, UniformMap uniforms)
{
  uniforms["uMVP"] = ortho(-0.5f, 0.5f, 0.5f, -0.5f, -1.0f, 1.0f);
  ApplyShader(shader, uniforms);
  RenderPlane(1.0f, 1.0f, vec4f(0, 1, 0, 1));
}

void OpenGLRenderer::Render(const mat4f &projection, const mat4f viewmodel, Object3D *object, OpenGLRenderTargetPtr target)
{
  RenderObject3D(projection, viewmodel, object, target);
}

void OpenGLRenderer::GetFilterPasses(Filter *filter, vector<OpenGLFilter> &passes)
{
/*
  if (filter->type() == CONSTANTS::GrayscaleFilter) {
    passes.push_back(OpenGLFilter(m_shaders.GetResource("shaders/filter.bw"), filter.uniforms));
  } else if (filter.type == FILTER_GAUSIAN_BLUR) {
    passes.push_back(OpenGLFilter(m_shaders.GetResource("shaders/filter.bh"), filter.uniforms));
    passes.push_back(OpenGLFilter(m_shaders.GetResource("shaders/filter.bv"), filter.uniforms));
  }
*/
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
      RenderGeometry(itr->first->geometry());
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

void OpenGLRenderer::ApplyBasicMaterial(Material *material)
{
  glUseProgram(0);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  check_gl_error();
}

void OpenGLRenderer::ApplyFilterMaterial(const mat4f &mvp, FilterMaterial *material)
{
  FilterList filters = material->filters();
  Pixmap *pixmap = material->pixmap();
  unsigned int width = pixmap->width();
  unsigned int height = pixmap->height();

  glViewport(0.0f, 0.0f, (float)width, (float)height);

  unsigned int targets_index = 0;
  OpenGLRenderTargetPtr targets[2];
  targets[0] = AcquireRenderTarget(width, height);
  targets[1] = AcquireRenderTarget(width, height);

  uniform_t read = pixmap;

  for (FilterList::iterator itr = filters.begin(); itr != filters.end(); itr++) {
    OpenGLShaderProgramPtr shader;
    UniformMap uniforms;

    Filter *filter = *itr;

    switch (filter->type()) {
    case CONSTANTS::ShaderFilter:
      shader = m_shaders.GetResource(static_cast<ShaderFilter *>(filter)->path());
      uniforms = static_cast<ShaderFilter *>(filter)->uniforms();
      break;

    case CONSTANTS::GrayscaleFilter:
      shader = m_shaders.GetResource("shaders/filter.bw");
      uniforms["saturation"] = static_cast<GrayscaleFilter *>(filter)->saturation();
      break;
    }

    uniforms["map"] = read;
    uniforms["texelSize"] = vec2f(1.0f / width, 1.0f / height);


    targets[targets_index]->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderFullscreenQuad(shader, uniforms);
    read = targets[targets_index]->colorAttachments[0];
    targets_index = 1 - targets_index;
  }

  glViewport(0.0f, 0.0f, (float)m_width, (float)m_height);
  OpenGLRenderTarget::revertToDisplayRenderTarget();

  OpenGLShaderProgramPtr shader = m_shaders.GetResource("shaders/map");
  UniformMap uniforms;
  uniforms["map"] = read;
  uniforms["uMVP"] = mvp;

  ApplyShader(shader, uniforms);

  // TODO needs to fix this, one we can't actually release and should cache for further use.
  ReleaseRenderTarget(targets[0]);
  ReleaseRenderTarget(targets[1]);
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
    } else if (u.type() == typeid(mat3f)) {
      shader->uniform(itr->first, any_cast<mat3f>(u));
    } else if (u.type() == typeid(mat4f)) {
      shader->uniform(itr->first, any_cast<mat4f>(u));
    } else if (u.type() == typeid(Pixmap *)) {
      Pixmap *i = any_cast<Pixmap *>(u);

      shader->uniform(itr->first, textureUnit);

      if (i->type() == CONSTANTS::BackbufferPixmap) {
          m_backbufferTexture->Bind(textureUnit);
          BackbufferPixmap *bpixmap = static_cast<BackbufferPixmap *>(i);

          vec2i position = bpixmap->position();
          vec2i size = bpixmap->size();
          glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, position[0], position[1], size[0], size[1], 0);
      } else {
          m_textures.GetResource(i)->Bind(textureUnit);
      }

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

  case CONSTANTS::FilterMaterial:
    ApplyFilterMaterial(mvp, (FilterMaterial *)material);
    break;
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

void OpenGLRenderer::RenderPolygon(const vec2farray &positions, vec4f uv)
{
  vec2farray uvs;
  iarray indices;

  GeometryUtils::triangulate_ec(positions, indices);
  GeometryUtils::create_uvs(positions, uvs, uv);

  GLfloat aPosition[positions.size() * 2];
  GLfloat aUV[positions.size() * 2];

  unsigned int i = 0;
  for (vec2farray::const_iterator itr = positions.begin(); itr != positions.end(); itr++) {
    aPosition[i++] = (*itr)[0];
    aPosition[i++] = (*itr)[1];
  }

  i = 0;
  for (vec2farray::iterator itr = uvs.begin(); itr != uvs.end(); itr++) {
    aUV[i++] = (*itr)[0];
    aUV[i++] = (*itr)[1];
  }

  glVertexAttribPointer(OpenGLShaderProgram::POSITION, 2, GL_FLOAT, GL_FALSE, 0, aPosition);
  glEnableVertexAttribArray(OpenGLShaderProgram::POSITION);

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

void OpenGLRenderer::RenderEllipsisArc(float w2, float h2, float angle1, float angle2, unsigned int segments, vec4f uv)
{
  float degrees = (angle2 - angle1) / ((float)segments - 1);

  float u0 = uv[0];
  float u1 = uv[1];
  float v0 = uv[2];
  float v1 = uv[3];

  unsigned int vertexCount = segments + 1;
  GLfloat aVertices[vertexCount * 2];
  GLfloat aUV[vertexCount * 2];

  aVertices[0] = 0.0f;
  aVertices[1] = 0.0f;

  aUV[0] = lerp(u0, u1, 0.5f);
  aUV[1] = lerp(u0, u1, 0.5f);

  unsigned int idx = 2;
  unsigned int outerVertexCount = vertexCount - 1;

  for (int i = 0; i < outerVertexCount; ++i) {
    float x = cos(degrees * (float)i + angle1);
    float y = sin(degrees * (float)i + angle1);

    aVertices[idx]     = x * w2;
    aVertices[idx + 1] = y * h2;

    aUV[idx]     = lerp(u0, u1, x *  0.5f + 0.5f);
    aUV[idx + 1] = lerp(v0, v1, y * -0.5f + 0.5f);

    idx += 2;
  }

  glVertexAttribPointer(OpenGLShaderProgram::POSITION, 2, GL_FLOAT, GL_FALSE, 0, aVertices);
  glEnableVertexAttribArray(OpenGLShaderProgram::POSITION);

  glVertexAttribPointer(OpenGLShaderProgram::UV, 2, GL_FLOAT, GL_FALSE, 0, aUV);
  glEnableVertexAttribArray(OpenGLShaderProgram::UV);

  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

  check_gl_error();
}

void OpenGLRenderer::RenderRaw(RawGeometry *raw)
{
  RenderRaw(raw->position(), raw->color(), raw->uv(), raw->indices());
}

void OpenGLRenderer::RenderPolygon(PolygonGeometry *polygon)
{
  RenderPolygon(polygon->vertices(), polygon->uv());
}

void OpenGLRenderer::RenderPlane(PlaneGeometry *plane)
{
  RenderPlane(plane->width(), plane->height(), plane->uv());
}

void OpenGLRenderer::RenderRoundedRectangle(RoundedRectangleGeometry *rect)
{
  float w2 = rect->width() * 0.5f;
  float h2 = rect->height() * 0.5f;
  vec4f radius = rect->radius();

  float xs[4] = { -w2,  w2,  w2, -w2 };
  float ys[4] = { -h2, -h2,  h2,  h2 };

  vec2farray positions;

  float sweep = M_PI / 2.0f;

  for (unsigned int i = 0; i < 4; i++) {
    float r = max(0.0f, min(radius[i], min(w2, h2)));
    float start = (M_PI * (float)i - M_PI - M_PI) / 2.0f;

    cout << "radius[i]=" << radius[i] << " r=" << r << " start=" << start << endl;

    if (r > 0) {
        float x = xs[i];
        float y = ys[i];

        float cx = x + r * (x > 0 ? -1.0 : 1.0);
        float cy = y + r * (y > 0 ? -1.0 : 1.0);

        cout << "x=" << x << " y=" << y << endl;
        cout << "cx=" << cx << " cy=" << cy << endl;

        GeometryUtils::arc(positions, cx, cy, r, start, sweep);
    } else {
      positions.push_back(vec2f(xs[i], ys[i]));
    }
  }

/*
  if (r > 0 && r < w2 && r < h2) {
    GeometryUtils::arc(positions, x2, y1, r, start0, sweep);
    GeometryUtils::arc(positions, x2, y2, r, start1, sweep);
    GeometryUtils::arc(positions, x1, y2, r, start2, sweep);
    GeometryUtils::arc(positions, x1, y1, r, start3, sweep);
  } else {
    positions.push_back(vec2f(x0, y0));
    positions.push_back(vec2f(x3, y0));
    positions.push_back(vec2f(x3, y3));
    positions.push_back(vec2f(x0, y3));
  }
*/

  RenderPolygon(positions, rect->uv());
}

void OpenGLRenderer::RenderArc(ArcGeometry *arc)
{
  RenderEllipsisArc(arc->radius(), arc->radius(), arc->angle1(), arc->angle2(), arc->segments(), arc->uv());
}

void OpenGLRenderer::RenderEllipsis(EllipsisGeometry *ellipsis)
{
  RenderEllipsisArc(ellipsis->width() / 2.0f, ellipsis->height() / 2.0f, 0, 2.0f * M_PI, ellipsis->segments(), ellipsis->uv());
}

void OpenGLRenderer::RenderFillPathGeometry(FillPathGeometry *g)
{
  std::cout << "RenderFillPathGeometry" << endl;

  Path *path = g->path();
  vec4f uv = g->uv();

  vec2farray positions;
  vec2farray uvs;
  std::vector<uint8_t> indices;

  GeometryUtils::fill(path, positions, indices);
  GeometryUtils::create_uvs(positions, uvs, uv);

  GLfloat aPosition[positions.size() * 2];
  GLfloat aUV[positions.size() * 2];

  std::cout << "posititions.length=" << positions.size() << " uv.length=" << uvs.size() << " indices.length=" << indices.size() << endl;

  unsigned int i = 0;
  for (vec2farray::const_iterator itr = positions.begin(); itr != positions.end(); itr++) {
    aPosition[i++] = (*itr)[0];
    aPosition[i++] = (*itr)[1];
  }

  i = 0;
  for (vec2farray::iterator itr = uvs.begin(); itr != uvs.end(); itr++) {
    aUV[i++] = (*itr)[0];
    aUV[i++] = (*itr)[1];
  }

  glVertexAttribPointer(OpenGLShaderProgram::POSITION, 2, GL_FLOAT, GL_FALSE, 0, aPosition);
  glEnableVertexAttribArray(OpenGLShaderProgram::POSITION);

  glVertexAttribPointer(OpenGLShaderProgram::UV, 2, GL_FLOAT, GL_FALSE, 0, aUV);
  glEnableVertexAttribArray(OpenGLShaderProgram::UV);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, &indices.front());
}

void OpenGLRenderer::RenderStrokePathGeometry(StrokePathGeometry *g)
{
  std::cout << "StrokePathGeometry" << endl;

  Path *path = g->path();
  float strokewidth = g->strokewidth();
  vec4f uv = g->uv();

  vec2farray positions;
  vec2farray uvs;
  std::vector<uint8_t> indices;

  GeometryUtils::stroke(path, strokewidth, positions, indices);
  GeometryUtils::create_uvs(positions, uvs, uv);

  GLfloat aPosition[positions.size() * 2];
  GLfloat aUV[positions.size() * 2];

  std::cout << "posititions.length=" << positions.size() << " uv.length=" << uvs.size() << " indices.length=" << indices.size() << endl;

  unsigned int i = 0;
  for (vec2farray::const_iterator itr = positions.begin(); itr != positions.end(); itr++) {
    aPosition[i++] = (*itr)[0];
    aPosition[i++] = (*itr)[1];
  }

  i = 0;
  for (vec2farray::iterator itr = uvs.begin(); itr != uvs.end(); itr++) {
    aUV[i++] = (*itr)[0];
    aUV[i++] = (*itr)[1];
  }

  glVertexAttribPointer(OpenGLShaderProgram::POSITION, 2, GL_FLOAT, GL_FALSE, 0, aPosition);
  glEnableVertexAttribArray(OpenGLShaderProgram::POSITION);

  glVertexAttribPointer(OpenGLShaderProgram::UV, 2, GL_FLOAT, GL_FALSE, 0, aUV);
  glEnableVertexAttribArray(OpenGLShaderProgram::UV);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, &indices.front());
}

void OpenGLRenderer::RenderGeometry(Geometry *geometry)
{
  switch (geometry->type())
  {
  case CONSTANTS::RawGeometry:
    RenderRaw((RawGeometry *)geometry);
    break;

  case CONSTANTS::PlaneGeometry:
    RenderPlane((PlaneGeometry *)geometry);
    break;

  case CONSTANTS::RoundedRectangleGeometry:
    RenderRoundedRectangle((RoundedRectangleGeometry *)geometry);
    break;

  case CONSTANTS::EllipsisGeometry:
    RenderEllipsis((EllipsisGeometry *)geometry);
    break;

  case CONSTANTS::ArcGeometry:
    RenderArc((ArcGeometry *)geometry);
    break;

  case CONSTANTS::PolygonGeometry:
    RenderPolygon((PolygonGeometry *)geometry);
    break;

  case CONSTANTS::FillPathGeometry:
    RenderFillPathGeometry((FillPathGeometry *)geometry);
    break;

  case CONSTANTS::StrokePathGeometry:
    RenderStrokePathGeometry((StrokePathGeometry *)geometry);
    break;
  }
}

void OpenGLRenderer::RenderMesh3D(const mat4f &mvp, Mesh3D *mesh)
{
  ApplyMaterial(mvp, mesh->material());
  RenderGeometry(mesh->geometry());

  check_gl_error();
}

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
