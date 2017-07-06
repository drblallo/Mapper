#include "entityaction.h"

using namespace mechanics;
EntityAction::EntityAction(Entity* en, PersistentAction* action) :
    stat(action),
    entity(en)
{
    if (!en)
        throw new exception();
    entity->addAction(this);
}

EntityAction::~EntityAction()
{
    if (entity)
        entity->removeAction(this);
    if (stat)
    {
        Action::uncall(stat);
        stat->remove();
    }
}
