#ifndef REGISTRATIONOREDIT_H
#define REGISTRATIONOREDIT_H

#include <QDialog>
#include "authorizationandoutcome.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>


namespace Ui {
class RegistrationOrEdit;
}

class RegistrationOrEdit : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationOrEdit(QWidget *parent = nullptr);
    ~RegistrationOrEdit();
    void setChoosenUser(QString PL);

private slots:
    void on_Registrate_clicked();

    void on_Update_clicked();

private:
    Ui::RegistrationOrEdit *ui;
    QString PreviousLogin;
};

#endif // REGISTRATIONOREDIT_H
