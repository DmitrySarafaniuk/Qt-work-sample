#include "authorizationandoutcome.h"

#include <QDir>
#include <QCryptographicHash>


AuthorizationAndOutcome::AuthorizationAndOutcome(QString Login, QString Password):
    Login(Login), Password(Password) {}

AuthorizationAndOutcome::~AuthorizationAndOutcome()
{}

QString AuthorizationAndOutcome::getLogin() const
{
    return this->Login;
}

QString AuthorizationAndOutcome::getPassword() const
{
    return this->Password;
}

void AuthorizationAndOutcome::setLogin(const QString Login)
{
    this->Login = Login;
}

void AuthorizationAndOutcome::setPassword(const QString Password)
{
    this->Password = Password;
}


int AuthorizationAndOutcome::signup(QString Login, QString Password)
{
    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

    QSqlQuery SignUpQuery(UsersDB);
    SignUpQuery.prepare("SELECT id FROM users WHERE login='"+Login+"'");
    SignUpQuery.exec();
    SignUpQuery.first();

    if(!SignUpQuery.isValid())
    {
        SignUpQuery.prepare("SELECT id FROM users");
        SignUpQuery.exec();
        int id = 0;
        SignUpQuery.last();
        id = SignUpQuery.value(0).toInt() + 1;


        QByteArray PasswordHash (Password.toStdString().c_str());
        QString CipheredPasswordHash = QCryptographicHash::hash(PasswordHash, QCryptographicHash::Keccak_512).toHex();
        QString lectinos_scores="";

        SignUpQuery.prepare("INSERT INTO users (id, login, password, lections, scores, time) "
                    "VALUES (:id, :login, :password, :lections, :scores, :time)");

        SignUpQuery.bindValue(":id", id);
        SignUpQuery.bindValue(":login", Login);
        SignUpQuery.bindValue(":password", CipheredPasswordHash);
        SignUpQuery.bindValue(":lections", lectinos_scores);
        SignUpQuery.bindValue(":scores", lectinos_scores);
        SignUpQuery.bindValue(":time", 0);

        SignUpQuery.exec();

        return 1;//successful sign up
    }

    return 0;//duplication
}

int AuthorizationAndOutcome::login(QString Login, QString Password)
{
    QByteArray PasswordHash (Password.toStdString().c_str());
    QString CipheredPasswordHash = QCryptographicHash::hash(PasswordHash, QCryptographicHash::Keccak_512).toHex();

    QDir dirpath(QDir::toNativeSeparators(QDir::currentPath() + "/Data/Users.db"));
    QString DBname = (QDir::toNativeSeparators(dirpath.path()));

    if(UsersPath.size() == 0)
        UsersPath = DBname;

    if(!dirpath.exists())
        QDir().mkdir("Data");


    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

    QSqlQuery LoginQuery(UsersDB);
    LoginQuery.prepare("SELECT password FROM users WHERE login='"+Login+"'");
    LoginQuery.exec();
    LoginQuery.first();

    if(LoginQuery.value(0).toString()!=CipheredPasswordHash)
    {
        return 0;//unsuccessful access
    }
    else
    {
        return 1;//successful login
    }

}

int AuthorizationAndOutcome::update(QString Login, QString NewLogin, QString NewPassword)
{
    QByteArray PasswordHash (NewPassword.toStdString().c_str());
    QString CipheredPasswordHash = QCryptographicHash::hash(PasswordHash, QCryptographicHash::Keccak_512).toHex();

    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

    QSqlQuery UpdateQuery(UsersDB);
    UpdateQuery.prepare("UPDATE users SET login='"+NewLogin+"', password='"+CipheredPasswordHash+"' WHERE login='"+Login+"'");
    if(UpdateQuery.exec())
    {
        if(UpdateQuery.isValid())
            return 1;
        else
            return 2;
    }
    else
        return 0;
}

int AuthorizationAndOutcome::deleteUser(QString Login)
{
    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);
    QSqlQuery DeleteQuery(UsersDB);
    QSqlQuery UpdateQuery(UsersDB);

    UpdateQuery.prepare("SELECT id FROM users WHERE login='"+Login+"'");
    UpdateQuery.exec();
    UpdateQuery.first();
    int changeId = UpdateQuery.value(0).toInt();

    DeleteQuery.prepare("DELETE FROM users WHERE "
                        "id='"+UpdateQuery.value(0).toString()+"' AND id > 0");
    if(DeleteQuery.exec())
    {
        DeleteQuery.prepare("SELECT login FROM users WHERE id > :id");
        DeleteQuery.bindValue(":id", changeId);
        DeleteQuery.exec();
        while(DeleteQuery.next())
        {
            UpdateQuery.prepare("UPDATE users SET id=:id "
                                "WHERE login='"+DeleteQuery.value(0).toString()+"'");
            UpdateQuery.bindValue(":id", changeId);
            UpdateQuery.exec();
            ++changeId;
        }
        return 1;
    }
    else
        return 0;
}

QString AuthorizationAndOutcome::getUsersPath() const
{
    return this->UsersPath;
}

QString AuthorizationAndOutcome::getOutcome(QString Login) const
{
    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);
    QSqlQuery outcomeQuery(UsersDB);

    QString outcome="Тести\n", temp;

    outcomeQuery.prepare("SELECT id, lections, scores FROM users WHERE login='"+Login+"'");
    outcomeQuery.exec();
    outcomeQuery.first();
    if(outcomeQuery.value(0).toString()=="")
    {
        outcome = "\v";
        return outcome;
    }

    QStringList lectionsList = outcomeQuery.value(1).toString().split("|", QString::SkipEmptyParts);
    QStringList scoresList = outcomeQuery.value(2).toString().split(",", QString::SkipEmptyParts);
    QStringList tempList, unPassedTest;

    for(int i = 0; i < scoresList.size(); ++i)
        if(scoresList.at(i)!="-1")
        {
            temp = lectionsList.at(i);
            temp.replace("/", " | ");
            outcome+=temp+"\tоцінка: "+scoresList.at(i)+"\n";

            temp = scoresList.at(i);
            tempList = temp.split("/");
            int r = tempList.at(0).toInt(), a = tempList.at(1).toInt();
            if(double(r/a) < 0.54)
            {
                temp = lectionsList.at(i);
                temp.replace("/", "|");
                tempList = temp.split("|");

                unPassedTest << tempList.at(1);
            }
        }

    temp = "";
    unPassedTest.removeDuplicates();
    for(int i = 0; i < unPassedTest.size(); ++i, temp+="\n")
        temp+=unPassedTest.at(i);


    if(outcome == "Тести\n")
        outcome = "Цей студент ще не пройшов жоден тест";
    else if (temp!="" || temp.count("\n")!=temp.size())
        outcome+="\nТеми з нескладеними тестами:\n"+temp;

    outcomeQuery.prepare("SELECT time FROM users WHERE login='"+Login+"'");
    outcomeQuery.exec();
    outcomeQuery.first();

    unsigned int time = outcomeQuery.value(0).toInt();
    unsigned int hours, minutes, seconds;
    hours = time/3600;
    minutes = (time - (hours*3600))/60;
    seconds = time -  hours*3600 - minutes*60;

    outcome+="\n\nЗагальний час роботи студента:\t"
             "  часів: "+QString::number(hours)+"\tмінут: "+QString::number(minutes)+"\tсекунд: "+QString::number(seconds);

    return outcome;
}
