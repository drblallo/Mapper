#include "characteristicentitywidget.h"
#include "ui_characteristicentitywidget.h"
#include "mechanics/entitycharacteristic.h"

CharacteristicEntityWidget::CharacteristicEntityWidget(QWidget *parent, EntityCharacteristic *chara) :
    QWidget(parent),
    ui(new Ui::CharacteristicEntityWidget),
    characteristic(chara)
{
    ui->setupUi(this);
    if (chara == NULL)
        return;
    setToolTip(chara->getCharacteristic()->getToolTip());
    ui->nameLabel->setText(chara->getCharacteristic()->getName());
    ui->valueLabel->setText(QString::number(chara->getCharacteristic()->getValue()));
}

CharacteristicEntityWidget::~CharacteristicEntityWidget()
{
    delete ui;
}
