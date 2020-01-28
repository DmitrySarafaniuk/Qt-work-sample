#ifndef TESTANDSCORES_H
#define TESTANDSCORES_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDir>
#include <QDirIterator>
#include <QDateTime>

#include <QDebug>

class TestAndScores
{
public:
    TestAndScores(QString Path);
    ~TestAndScores();

    int addQuestion(QString Material, QString question, QStringList RightAnswers,
                    QStringList FalseAnswers, int value, QStringList AllAnswers);

    int changeQuestion(QString oldQuestion, QString Material, QString question, QStringList RightAnswers,
                       QStringList FalseAnswers, int value, QStringList AllAnswers);

    int changeOnlyQuestion(QString oldQuestion, QString Material, QString question, int value);

    int changeOnlyAnswers(QString Material, QString question, QStringList RightAnswers,
                          QStringList FalseAnswers, QStringList AllAnswers);

    int deleteQuestion(QString Material, QString question);
    int checkTestAvaiability(QString Material, int student);

    QSqlQuery* getTest(QString Material);

    int setScore(QString Material, int StudentId, QString Score);

    double rightAnswerScore = 0;
    double falseAnswerScore = 0;

private:
    QString UsersPath = "";
    QString Material;
    int page;
};

#endif // TESTANDSCORES_H
