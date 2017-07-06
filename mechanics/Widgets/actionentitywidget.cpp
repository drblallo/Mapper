#include "actionentitywidget.h"
#include "ui_actionentitywidget.h"
#include "mechanics/entityaction.h"

ActionEntityWidget::ActionEntityWidget(QWidget *parent, EntityAction* act) :
    QWidget(parent),
    ui(new Ui::ActionEntityWidget),
    action(act)
{
    ui->setupUi(this);
    if (!act)
        return;
    if (!act->getAction()->isPending())
    {
        ui->pushButton->setText(act->getAction()->getName());
        if (act->getAction()->canBeCalled())
        {
            setToolTip(act->getAction()->getToolTip());
            ui->pushButton->setEnabled(true);
        }
        else
        {
            setToolTip(act->getAction()->getcannotCallReason());
            ui->pushButton->setEnabled(false);
        }
    }
    else
    {
        ui->pushButton->setText(act->getAction()->getName());
        if (act->getAction()->canBeUncalled())
        {
            setToolTip(act->getAction()->getToolTip());
            ui->pushButton->setEnabled(true);
        }
        else
        {
            setToolTip(act->getAction()->getcannotUncallReason());
            ui->pushButton->setEnabled(false);
        }
    }

}

ActionEntityWidget::~ActionEntityWidget()
{
    delete ui;
}
