#pragma once
#include "characteristic.h"
#include <QString>
#include "informativeobject.h"

namespace mechanics
{
    class StockPile : public InformativeObject
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            StockPile(int baseValue, QString n);
            inline operator float() const {return count;}

        //########################################
        //#####          Accessors           #####
        //########################################
            inline float getValue() const {return count;}
            inline Characteristic* getIncreaseModifier(){return &addModifiers;}
            inline Characteristic* getDecreaseModifier(){return &removeModifiers;}
            inline virtual QString getName() const {return name;}
            virtual QString getToolTip() const;

        //########################################
        //#####           Methods            #####
        //########################################
            void add(float t);

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            float count;
            Characteristic addModifiers;
            Characteristic removeModifiers;
            QString name;
    };
}
