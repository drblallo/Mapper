#pragma once
#include "mechanics/action.h"

namespace mechanics
{
    class PersistentAction : public Action
    {
        public:
            PersistentAction() : Action() {}
            virtual void remove() {deleteMe = true;}

        protected:
            virtual ~PersistentAction();
            virtual int triggered() = 0;

        private:
            virtual int execute();
    };
}
