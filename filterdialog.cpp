#include "filterdialog.h"
#include "ui_filterdialog.h"

#include <QSettings>
#include <QDebug>

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    QSettings settings;
    ui->longDisplDiffSpinBox->setValue(settings.value("filter/longDisplDiff").toDouble());
    ui->latDisplDiffSpinBox->setValue(settings.value("filter/latDisplDiff").toDouble());
    ui->lifeTimeSpinBox->setValue(settings.value("filter/lifeTime").toDouble());
    ui->rcsSpinBox->setValue(settings.value("filter/RCS").toDouble());
    ui->normToLifeTimeSpinBox->setValue(settings.value("filter/normToLifeTime").toDouble());

}

void FilterDialog::accept()
{
    QSettings settings;
    settings.setValue("filter/longDisplDiff",
                      ui->longDisplDiffSpinBox->value());
    settings.setValue("filter/latDisplDiff",
                      ui->latDisplDiffSpinBox->value());
    settings.setValue("filter/lifeTime",
                      ui->lifeTimeSpinBox->value());
    settings.setValue("filter/RCS",
                      ui->rcsSpinBox->value());
    settings.setValue("filter/normToLifeTime",
                      ui->normToLifeTimeSpinBox->value());

    done(0);

}

void FilterDialog::reject()
{
    done(0);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}
