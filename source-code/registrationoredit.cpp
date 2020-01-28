#include "registrationoredit.h"
#include "ui_registrationoredit.h"

RegistrationOrEdit::RegistrationOrEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationOrEdit)
{
    ui->setupUi(this);
}

RegistrationOrEdit::~RegistrationOrEdit()
{
    delete ui;
}

void RegistrationOrEdit::on_Registrate_clicked()
{
    QString Login = ui->Login->text();
    QString Password = ui->Password->text();

    if(Login==""||Password=="")
        QMessageBox::information(this, tr("Пуста форма"), tr("Вам необхідно заповнити логін та пароль"));
    else
    {
        AuthorizationAndOutcome user(Login, Password);
        int check = user.signup(user.getLogin(), user.getPassword());
        if(check == 0)
            QMessageBox::information(this, tr("Дублювання"), tr("Цей користувач вже є в системі"));
        else if(check == 1)
            QMessageBox::information(this, tr("Успіх"), tr("Вы успішно зареєстрували студента"));
    }
}

void RegistrationOrEdit::on_Update_clicked()
{
    QString Login = ui->Login->text();
    QString Password = ui->Password->text();

    if(Login==""||Password=="")
        QMessageBox::information(this, tr("Пуста форма"), tr("Вам необхідно заповнити логін та пароль"));
    else
    {
        AuthorizationAndOutcome user(Login, Password);
        int check = user.update(PreviousLogin, user.getLogin(), user.getPassword());
        if(check == 0)
            QMessageBox::information(this, tr("Помилка"), tr("Невизначена помилка під час виконання редагування"));
        else if(check == 1)
            QMessageBox::information(this, tr("Успіх"), tr("Дані студента були змінені"));
        else
            QMessageBox::information(this, tr("Неправильні дані"), tr("Ви намагаєтесь редагувати студента,\nякого ще немає в системі"));
    }
}

void RegistrationOrEdit::setChoosenUser(QString PL)
{
    PreviousLogin = PL;
    ui->Login->setText(PreviousLogin);
}
