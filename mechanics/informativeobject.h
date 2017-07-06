#pragma once
#include <QString>

namespace mechanics
{
    class InformativeObject
    {
        public:
            virtual QString getToolTip() const = 0;
            virtual QString getName() const = 0;
            inline virtual bool canBeShown() {return true;}

        protected:
            InformativeObject(){}
            ~InformativeObject(){}

    };
}
