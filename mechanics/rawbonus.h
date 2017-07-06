#pragma once
#include <QString>
#include <vector>
#include "informativeobject.h"


using namespace std;

namespace mechanics
{
    class Characteristic;
    class RawBonus : public InformativeObject
    {
        public:
        friend class Characteristic;

        //########################################
        //##### Constructors and Conversions #####
        //########################################
            RawBonus(float val, const QString* des);
            RawBonus(float val, const QString des);
            operator float() const;
            virtual ~RawBonus();

        //########################################
        //#####           Methods            #####
        //########################################
            void setValue(float newValue);
            void setDescription(const QString *newDes);
            void setEnabled(bool ena);

        //########################################
        //#####          Accessors           #####
        //########################################
            virtual QString getToolTip() const;
            virtual QString getName() const;
            float getValue() const;
            bool isEnabled() const;
            const vector<Characteristic*>* getCarateristics() const;

        //########################################
        //#####         Privatizing          #####
        //########################################
            RawBonus& operator= (const RawBonus&) = delete;
            RawBonus(const RawBonus&) = delete;

        private:
        //########################################
        //#####           Fields             #####
        //########################################
            float value;
            QString description;
            bool enabled;
            vector<Characteristic*> chars;

    };
}
