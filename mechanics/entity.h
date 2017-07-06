#pragma once
#include <vector>
#include <QString>
#include <algorithm>

using namespace std;
namespace mechanics
{
    class EntityCharacteristic;
    class EntityStockpile;
    class EntityAction;
    class Entity
    {
        public:
            friend class EntityCharacteristic;
            friend class EntityStockpile;
            friend class EntityAction;
        //########################################
        //##### Constructors and Conversions #####
        //########################################
            Entity(QString nm);
            virtual ~Entity();

        //########################################
        //#####          Accessors           #####
        //########################################
            virtual QString getToolTip() const;
            inline const vector<EntityCharacteristic*>* getStats() const {return &stats;}
            inline const vector<EntityStockpile*>* getStockpiles() const {return &stockpiles;}
            inline const vector<EntityAction*>* getAction() const {return &actions;}
            virtual QString getName() const {return name;}

            inline bool hasTag(QString tag) const
            {
                return (find(tags.begin(), tags.end(), tag) != tags.end());
            }

            inline void addTag(QString tag)
            {
                if (hasTag(tag))
                    return;
                tags.push_back(tag);
            }

            inline void removeTag(QString tag)
            {
                tags.erase(remove(tags.begin(), tags.end(), tag), tags.end());
            }

        //########################################
        //#####         Privatizing          #####
        //########################################
            Entity(const Entity&) = delete;
            Entity& operator= (const Entity&) = delete;


        private:
            void removeCharacteristic(EntityCharacteristic* chara);
            void addCharacteristic(EntityCharacteristic* chara);
            vector<EntityCharacteristic*> stats;

            void removeStockpile(EntityStockpile* chara);
            void addStockpile(EntityStockpile* chara);
            vector<EntityStockpile*> stockpiles;

            void removeAction(EntityAction* chara);
            void addAction(EntityAction* chara);
            vector<EntityAction*> actions;
            QString name;

            vector<QString> tags;
    };
}


/*
#pragma once
#include "mechanics/entity"

using namespace mechanics;

namespace game
{
    class y : public Entity
    {
        public:
            y(QString name);
            virtual ~y();

        private:
            EntityCharacteristic stat;
    };
}

#include "y"

using namespace game;

y::y(QString name) : Entity(name)
{

}

y::~y()
{

}
*/
