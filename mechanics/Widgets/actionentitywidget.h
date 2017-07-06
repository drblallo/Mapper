#ifndef ACTIONENTITYWIDGET_H
#define ACTIONENTITYWIDGET_H

#include <QWidget>

namespace mechanics
{
    class EntityAction;
}

using namespace mechanics;

namespace Ui {
class ActionEntityWidget;
}

class ActionEntityWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionEntityWidget(QWidget *parent = 0, EntityAction* act = NULL);
    ~ActionEntityWidget();

private:
    Ui::ActionEntityWidget *ui;
    EntityAction* action;
};

#endif // ACTIONENTITYWIDGET_H
