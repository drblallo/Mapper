#ifndef DIALOGENTITY_H
#define DIALOGENTITY_H

#include <QDialog>

namespace mechanics
{
    class Entity;
}
using namespace mechanics;
namespace Ui {
class DialogEntity;
}

class DialogEntity : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEntity(QWidget *parent = 0, Entity* entity = NULL);
    ~DialogEntity();

private:
    Ui::DialogEntity *ui;
    Entity* entity;
};

#endif // DIALOGENTITY_H
