#pragma once
#include "characteristic.h"
#include "entity.h"
#include "Widgets/characteristicentitywidget.h"

namespace mechanics
{
    class EntityCharacteristic
    {
        public:
            EntityCharacteristic(Entity* entity, float baseValue,  QString name);
            ~EntityCharacteristic();
            inline Characteristic* getCharacteristic() {return &stat;}
            inline Entity* getEntity(){return entity;}
            CharacteristicEntityWidget* getWidget(QWidget* parent) {return new CharacteristicEntityWidget(parent, this);}

        //########################################
        //#####         Privatizing          #####
        //########################################
            EntityCharacteristic(const EntityCharacteristic&) = delete;
            EntityCharacteristic& operator= ( const EntityCharacteristic&) = delete;

        private:
            Characteristic stat;
            Entity* entity;
    };
}
