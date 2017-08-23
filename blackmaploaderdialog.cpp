#include "blackmaploaderdialog.h"
#include "ui_blackmaploaderdialog.h"
#include <QFileDialog>

BlackMapLoaderDialog::BlackMapLoaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackMapLoaderDialog)
{
    ui->setupUi(this);
    startUI();
}

BlackMapLoaderDialog::BlackMapLoaderDialog(QString s, QWidget *parent) :  QDialog(parent), ui(new Ui::BlackMapLoaderDialog), path(s)
{
    ui->setupUi(this);
    startUI();

}

BlackMapLoaderDialog::~BlackMapLoaderDialog()
{
    delete ui;
}

void BlackMapLoaderDialog::startUI()
{
    connect(ui->outPutLineButton, &QPushButton::pressed, this, &BlackMapLoaderDialog::selectOutputLine);
    connect(ui->inputLineButton, &QPushButton::pressed, this, &BlackMapLoaderDialog::selectInputLine);
}

void BlackMapLoaderDialog::selectInputLine()
{
    QFileDialog f(this, tr("Load Map"));
    f.setDirectory(path);
    f.setFileMode(QFileDialog::ExistingFile);
    f.setNameFilter(tr("Image file (*.png)"));
    f.setOption(QFileDialog::DontUseNativeDialog, true);

    if (!f.exec())
        return;

    ui->inputLineEdit->setText(f.selectedFiles()[0]);

}

void BlackMapLoaderDialog::selectOutputLine()
{
    QFileDialog f(this, tr("Load Map"));
    f.setDirectory(path);
    f.setFileMode(QFileDialog::AnyFile);
    f.setNameFilter(tr("Image file (*.png)"));
    f.setOption(QFileDialog::DontUseNativeDialog, true);

    if (!f.exec())
        return;

    QString s(f.selectedFiles()[0]);
    if (!s.endsWith(".png"))
        s.append(".png");

    ui->outPutLineOutput->setText(s);
}

QString BlackMapLoaderDialog::getOutput()
{
    return ui->outPutLineOutput->text();
}

QString BlackMapLoaderDialog::getInput()
{
    return ui->inputLineEdit->text();
}

int BlackMapLoaderDialog::getProvinceCount()
{
    return ui->provinceCountEdit->value();
}
