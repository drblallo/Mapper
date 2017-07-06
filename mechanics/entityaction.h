#pragma once
#include "persistentaction.h"
#include "entity.h"
#include "Widgets/actionentitywidget.h"

namespace mechanics
{
    class EntityAction
    {
        public:
            EntityAction(Entity* entity, PersistentAction* action);
            ~EntityAction();
            inline PersistentAction* getAction() const {return stat;}
            inline Entity* getEntity(){return entity;}
            ActionEntityWidget* getWidget(QWidget* parent) {return new ActionEntityWidget(parent, this);}


        //########################################
        //#####         Privatizing          #####
        //########################################
            EntityAction(const EntityAction&) = delete;
            EntityAction& operator= (const EntityAction&) = delete;

        private:
            PersistentAction* stat;
            Entity* entity;
    };
}
