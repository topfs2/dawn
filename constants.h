#pragma once

namespace dawn
{
    namespace CONSTANTS
    {
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

        enum MaterialType {
            ShaderMaterial = 0,
            FilterMaterial,
            VideoMaterial, // TODO Probably only need clear _or_ video material?
            ClearMaterial
        };

        enum PixmapType
        {
            ImagePixmap = 0,
            TextImagePixmap,
            BackbufferPixmap
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
