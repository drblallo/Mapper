#pragma once

namespace renderer
{

    struct ColorMask
    {
    public:
    //########################################
    //##### Constructors and Conversions #####
    //########################################
        inline ColorMask(bool red, bool green, bool blue, bool alpha)
            :
            _red(red),
            _green(green),
            _blue(blue),
            _alpha(alpha)
        {}

        inline bool Equals(const ColorMask other) const
        {
            return
                _red == other._red &&
                _green == other._green &&
                _blue == other._blue &&
                _alpha == other._alpha;
        }

        inline bool operator ==(const ColorMask right) const
        {
            return Equals(right);
        }

        inline bool operator !=(ColorMask right) const
        {
            return !Equals(right);
        }

    //########################################
    //#####         Accessors            #####
    //########################################
        inline bool Red() const { return _red;}
        inline bool Green() const { return _green;}
        inline bool Blue() const { return _blue;}
        inline bool Alpha() const { return _alpha;}

        private:
    //########################################
    //#####           Fields             #####
    //########################################
            bool _red;
            bool _green;
            bool _blue;
            bool _alpha;
    };
}
