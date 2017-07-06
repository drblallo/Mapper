#pragma once
#include <QVector4D>

namespace renderer
{
    enum SourceBlendingFactor
    {
        SourceBlendingZero,
        SourceBlendingOne,
        SourceBlendingSourceAlpha,
        SourceBlendingOneMinusAlpha,
        SourceBlendingDestinationAlpha,
        SourceBlendingOneMinusDestinationAlpha,
        SourceBlendingDestinationColor,
        SourceBlendingOneMinusDestinationColor,
        SourceBlendingSourceAlphaSaturate,
        SourceBlendingConstantColor,
        SourceBlendingOneMinusConstantColor,
        SourceBlendingConstantAlpha,
        SourceBlendingOneMinusConstantAlpha
    };

    enum DestinationBlendingFactor
    {
        DestinationBlendingZero,
        DestinationBlendingOne,
        DestinationBlendingSourceAlpha,
        DestinationBlendingOneMinusAlpha,
        DestinationBlendingDestinationAlpha,
        DestinationBlendingOneMinusDestinationAlpha,
        DestinationBlendingDestinationColor,
        DestinationBlendingOneMinusDestinationColor,
        DestinationBlendingSourceAlphaSaturate,
        DestinationBlendingConstantColor,
        DestinationBlendingOneMinusConstantColor,
        DestinationBlendingConstantAlpha,
        DestinationBlendingOneMinusConstantAlpha
    };

    enum BlendEquation
    {
        BlendEquationAdd,
        BlendEquationSubtract,
        BlendEquationReverseSubtract,
        BlendEquationMinimum,
        BlendEquationMaximum
    };

    class Blending
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            inline Blending() :
                Enabled(false),
                SourceRGBFactor(SourceBlendingFactor::SourceBlendingOne),
                SourceAlphaFactor(SourceBlendingFactor::SourceBlendingOne),
                DestinationRGBFactor(DestinationBlendingFactor::DestinationBlendingZero),
                DestinationAlphaFactor(DestinationBlendingFactor::DestinationBlendingZero),
                RGBEquation(BlendEquation::BlendEquationAdd),
                AlphaEquation(BlendEquation::BlendEquationAdd),
                Color(0, 0, 0, 0)
            {}

        //########################################
        //#####         Accessors            #####
        //########################################
            inline bool isEnabled() const {return Enabled;}
            inline SourceBlendingFactor getSourceRGBFactor() const {return SourceRGBFactor;}
            inline SourceBlendingFactor getSourceAlphaFactor() const {return SourceAlphaFactor;}
            inline DestinationBlendingFactor getDestinationRGBFactor() const {return DestinationRGBFactor;}
            inline DestinationBlendingFactor getDestinationAlphaFactor() const {return DestinationAlphaFactor;}
            inline BlendEquation getRGBEquation() const {return RGBEquation;}
            inline BlendEquation getAlphaEquation() const {return AlphaEquation;}
            inline QVector4D getColor() const {return Color;}

        //########################################
        //#####           Methods            #####
        //########################################
            inline void setEnabled(bool enabled){Enabled = enabled;}
            inline void setSourceRGBFactor(SourceBlendingFactor factor){SourceRGBFactor = factor;}
            inline void setSourceAlphaFactor(SourceBlendingFactor factor){SourceAlphaFactor = factor;}
            inline void setDestinationRGBFactor(DestinationBlendingFactor factor){DestinationRGBFactor = factor;}
            inline void setDestinationAlphaFactor(DestinationBlendingFactor factor){DestinationAlphaFactor = factor;}
            inline void setRGBEquation(BlendEquation equation){RGBEquation = equation;}
            inline void setAlphaEquation(BlendEquation equation){AlphaEquation = equation;}
            inline void setColor(QVector4D color){Color = color;}

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            bool Enabled;
            SourceBlendingFactor SourceRGBFactor;
            SourceBlendingFactor SourceAlphaFactor;
            DestinationBlendingFactor DestinationRGBFactor;
            DestinationBlendingFactor DestinationAlphaFactor;
            BlendEquation RGBEquation;
            BlendEquation AlphaEquation;
            QVector4D Color;
   };
}
