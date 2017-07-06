#pragma once

namespace renderer
{
    enum CullFace
    {
        CullFaceFront,
        CullFaceBack,
        CullFaceFrontAndBack
    };

    enum WindingOrder
    {
        WindingOrderClockwise,
        WindingOrderCounterclockwise
    };

    class FacetCulling
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            inline FacetCulling() :
                Enabled(true),
                Face(CullFace::CullFaceBack),
                FrontFaceWindingOrder(WindingOrder::WindingOrderCounterclockwise)
            {
            }

        //########################################
        //#####         Accessors            #####
        //########################################
            inline bool isEnabled() const { return Enabled;}
            inline CullFace getCullFace() const { return Face;}
            inline WindingOrder getFrontFaceWiding() const {return FrontFaceWindingOrder;}

        //########################################
        //#####           Methods            #####
        //########################################
            inline void setEnabled(bool enable) {Enabled = enable;}
            inline void setCullFace(CullFace face){Face = face;}
            inline void setFrontFaceWiding(WindingOrder front) {FrontFaceWindingOrder = front;}

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            bool Enabled;
            CullFace Face;
            WindingOrder FrontFaceWindingOrder;
    };
}
