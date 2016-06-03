#include "addressdialog.h"
#include "ui_addressdialog.h"

#include <QSettings>

AddressDialog::AddressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressDialog)
{
    ui->setupUi(this);

    QSettings settings;
    ui->lidarAddressLineEdit->setText(settings.value("net/lidarAddress").toString());
    ui->lidarPortLineEdit->setText(settings.value("net/lidarPort").toString());
    ui->radarAddressLineEdit->setText(settings.value("net/radarAddress").toString());
    ui->radarPortLineEdit->setText(settings.value("net/radarPort").toString());
    ui->boardAddressLineEdit->setText(settings.value("net/boardAddress").toString());
    ui->boardPortLineEdit->setText(settings.value("net/boardPort").toString());
    ui->cameraAddressLineEdit->setText(settings.value("net/cameraAddress").toString());
    ui->cameraPortLineEdit->setText(settings.value("net/cameraPort").toString());
}

AddressDialog::~AddressDialog()
{
    delete ui;
}

void AddressDialog::saveAddress()
{
    QSettings settings;
    settings.setValue("net/lidarAddress", ui->lidarAddressLineEdit->text());
    settings.setValue("net/lidarPort", ui->lidarPortLineEdit->text().toInt());
    settings.setValue("net/radarAddress", ui->radarAddressLineEdit->text());
    settings.setValue("net/radarPort", ui->radarPortLineEdit->text().toInt());
    settings.setValue("net/boardAddress", ui->boardAddressLineEdit->text());
    settings.setValue("net/boardPort", ui->boardPortLineEdit->text().toInt());
    settings.setValue("net/cameraAddress", ui->cameraAddressLineEdit->text());
    settings.setValue("net/cameraPort", ui->cameraPortLineEdit->text().toInt());

}

void AddressDialog::on_savePushButton_released()
{
    saveAddress();
}

