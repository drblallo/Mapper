#pragma once
#include <QDialog>

namespace Ui {
class BlackMapLoaderDialog;
}

class BlackMapLoaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlackMapLoaderDialog(QWidget *parent = 0);
    ~BlackMapLoaderDialog();
    QString getOutput();
    QString getInput();
    int getProvinceCount();

private:
    Ui::BlackMapLoaderDialog *ui;
    void startUI();
    void selectInputLine();
    void selectOutputLine();
};
