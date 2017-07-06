#pragma once
#include "vector"
#include <QString>
#include "informativeobject.h"

using namespace std;

namespace mechanics
{
    class RawBonus;
    class Characteristic : public InformativeObject
    {
        public:
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            Characteristic(float base, QString nm);
            Characteristic(const Characteristic&);
            Characteristic(const Characteristic*);
            Characteristic& operator= (const Characteristic&);
            operator float() const;
            ~Characteristic();

        //########################################
        //#####           Methods            #####
        //########################################
            void addRawBonus(RawBonus* bonus);
            void addMultiplierBonus(RawBonus* bonus);
            void removeRawBonuss(RawBonus *bonus);
            void removeMultiplierBonus(RawBonus* bonus);
            void removeBonus(RawBonus* bonus);

        //########################################
        //#####         Accessors            #####
        //########################################
            float getTotalRaw() const;
            float getTotalMultiplier() const;
            float getBaseValue() const;
            const vector<RawBonus*>* getRawBonuses() const;
            const vector<RawBonus*>* getMultiplierBonuses() const;
            virtual QString getName() const;
            virtual QString getToolTip() const;
            inline float getValue() const {return chached;}


        private:
            float calculate();
        //########################################
        //#####           Fields             #####
        //########################################
            vector<RawBonus* > rawExtraBonus;
            vector<RawBonus*> multiplierExtraBonus;
            float chached;
            float baseValue;
            QString name;

    };
}
