#ifndef CHARACTERISTICENTITYWIDGET_H
#define CHARACTERISTICENTITYWIDGET_H

#include <QWidget>
namespace mechanics
{
    class EntityCharacteristic;
}
using namespace mechanics;
namespace Ui {
class CharacteristicEntityWidget;
}

class CharacteristicEntityWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CharacteristicEntityWidget(QWidget *parent = 0, EntityCharacteristic* chara = NULL);
    ~CharacteristicEntityWidget();

private:
    Ui::CharacteristicEntityWidget *ui;
    EntityCharacteristic* characteristic;
};

#endif // CHARACTERISTICENTITYWIDGET_H
