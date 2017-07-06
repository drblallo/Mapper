#pragma once
#include "primitiverestart.h"
#include "faceculling.h"
#include "scisortest.h"
#include "stenciltest.h"
#include "depthtest.h"
#include "depthrange.h"
#include "blending.h"
#include "colormask.h"

namespace renderer
{
    enum ProgramPointSize
    {
        ProgramPointSizeEnabled,
        ProgramPointSizeDisabled
    };

    enum RasterizationMode
    {
        RasterizationModePoint,
        RasterizationModeLine,
        RasterizationModeFill
    };

    class RenderState
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            inline RenderState() :
                primitiveRestart(),
                facetCulling(),
                programPointSize(ProgramPointSize::ProgramPointSizeDisabled),
                rasterizationMode(RasterizationMode::RasterizationModeFill),
                scissorTest(),
                stencilTest(),
                depthTest(),
                depthRange(),
                blending(),
                colorMask(true, true, true, true),
                depthMask(true)
            {}

        //########################################
        //#####           Fields             #####
        //########################################
            PrimitiveRestart primitiveRestart;
            FacetCulling facetCulling;
            ProgramPointSize programPointSize;
            RasterizationMode rasterizationMode;
            ScissorTest scissorTest;
            StencilTest stencilTest;
            DepthTest depthTest;
            DepthRange depthRange;
            Blending blending;
            ColorMask colorMask;
            bool depthMask;
    };
}
