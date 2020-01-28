#ifndef AUTHORIZATIONANDOUTCOME_H
#define AUTHORIZATIONANDOUTCOME_H

#include <QString>
#include <QtSql>
#include <QSqlQuery>
#include <QDebug>

class AuthorizationAndOutcome
{
public:
    AuthorizationAndOutcome(QString Login, QString Password);
    ~AuthorizationAndOutcome();

    QString getLogin() const;
    QString getPassword() const;
    void setLogin(const QString Login);
    void setPassword(const QString Password);

    int signup(QString Login, QString Password);
    int login(QString Login, QString Password);
    int update(QString Login, QString NewLogin, QString NewPassword);
    int deleteUser(QString Login);

    QString getUsersPath() const;
    QString getOutcome(QString Login) const;

private:
    QString Login = "";
    QString Password = "";
    QString Outcome = "";
    QString UsersPath = "";
};

#endif // AUTHORIZATIONANDOUTCOME_H
