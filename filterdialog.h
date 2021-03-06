#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = 0);
    ~FilterDialog();

private:
    Ui::FilterDialog *ui;

public slots:
    void accept();
    void reject();
};

#endif // FILTERDIALOG_H
