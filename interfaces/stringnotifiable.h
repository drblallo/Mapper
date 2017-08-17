#ifndef STRINGNOTIFIABLE_H
#define STRINGNOTIFIABLE_H
#include <QString>

class StringNotifiable
{
public:
    StringNotifiable();
    virtual QString getNotifyText() = 0;
    virtual void setNotifyText(QString) = 0;
};

#endif // STRINGNOTIFIABLE_H
