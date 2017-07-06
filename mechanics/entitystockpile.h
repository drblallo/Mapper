#pragma once
#include "stockpile.h"
#include "entity.h"
#include "Widgets/stockpileentitywidget.h"

namespace mechanics
{
    class EntityStockpile
    {
        public:
            EntityStockpile(Entity* entity, float baseValue,  QString name);
            ~EntityStockpile();
            inline StockPile* getStockpile() {return &stat;}
            inline Entity* getEntity(){return entity;}
            StockpileEntityWidget* getWidget(QWidget* parent) {return new StockpileEntityWidget(parent, this);}

        //########################################
        //#####         Privatizing          #####
        //########################################
            EntityStockpile(const EntityStockpile&) = delete;
            EntityStockpile& operator= ( const EntityStockpile&) = delete;

        private:
            StockPile stat;
            Entity* entity;
    };
}
