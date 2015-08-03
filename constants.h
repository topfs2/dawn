#pragma once

namespace dawn
{
  namespace CONSTANTS
  {
    enum Type {
      Object3D = 0,
      Camera,
      Scene3D,
      Mesh
    };

    enum CameraType {
      OrthographicCamera = 0,
      PerspectiveCamera
    };

    enum GeometryType {
      PlaneGeometry = 0,
      RoundedRectangleGeometry,
      EllipsisGeometry,
      ArcGeometry,
      PolygonGeometry,
      RawGeometry,
      FillPathGeometry,
      StrokePathGeometry
    };

    enum PathSegmentType {
      MoveToSegment = 0,
      CloseSegment,
      LineToSegment,
      QuadradicCurveToSegment,
      CubicCurveToSegment,
      BezierCurveToSegment
    };

    enum PathType {
      RectanglePath = 0,
      RoundedRectanglePath,
      EllipsisPath,
      ArcPath,
      PolygonPath,
      SegmentedPath
    };

    enum MaterialType {
      ShaderMaterial = 0,
      FilterMaterial,
      VideoMaterial, // TODO Probably only need clear _or_ video material?
      ClearMaterial
    };

    enum FilterType
    {
      ShaderFilter = 0,
      GrayscaleFilter
    };

    enum PixelFormat {
      UnknownFormat = 0,

      AlphaFormat,

      RGBFormat,
      RGBAFormat,

      BGRFormat,
      BGRAFormat,

      LuminanceFormat
    };

    enum TextAlign {
        Left = 0,
        Center,
        Right
    };
  }
}
