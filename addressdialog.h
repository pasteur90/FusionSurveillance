#ifndef ADDRESSDIALOG_H
#define ADDRESSDIALOG_H

#include <QDialog>

namespace Ui {
class AddressDialog;
}

class AddressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddressDialog(QWidget *parent = 0);
    ~AddressDialog();

private:
    Ui::AddressDialog *ui;

signals:
    void connectRadar();

public slots:
    void saveAddress();

private slots:
    void on_savePushButton_released();
};

#endif // ADDRESSDIALOG_H
