#include "testandscores.h"

TestAndScores::TestAndScores(QString Path)
{
    UsersPath = Path;

    QSqlDatabase Tests = QSqlDatabase::database(UsersPath);
    QSqlQuery testsQuery(Tests);
    testsQuery.prepare("CREATE TABLE IF NOT EXISTS tests (material text, page integer, question CLOB, rightanswers CLOB, falseanswers CLOB, value integer, allanswers CLOB)");
    testsQuery.exec();
}

TestAndScores::~TestAndScores()
{

}

int TestAndScores::addQuestion(QString Material, QString question, QStringList RightAnswers,
                               QStringList FalseAnswers, int value, QStringList AllAnswers)
{
    QSqlDatabase Tests = QSqlDatabase::database(UsersPath);
    QSqlQuery testsQuery(Tests);
    testsQuery.prepare("SELECT * FROM tests WHERE material='"+Material+"'");
    testsQuery.exec();

    int page = 0;

    QString q = question;
    q = q.toUpper();

    while(testsQuery.next())
    {
        QString qQ = testsQuery.value(2).toString();
        qQ = qQ.toUpper();

        if(qQ == q)
            return -1;
    }

    testsQuery.last();
    if(testsQuery.value(2).toString()!="")
        page = testsQuery.value(1).toInt() + 1;


    QString r = "\v";
    for(int i = 0; i<RightAnswers.size(); ++i)
        r+=RightAnswers.at(i)+"\v";

    if(r=="\v")
        r="";

    QString f = "\v";
    for(int i = 0; i<FalseAnswers.size(); ++i)
        f+=FalseAnswers.at(i)+"\v";

    if(f=="\v")
        f="";

    QString a = "\v";
    for(int i = 0; i<AllAnswers.size(); ++i)
        a+=AllAnswers.at(i)+"\v";

    if(a=="\v")
        a="";

    testsQuery.prepare("INSERT INTO tests (material, page, question, rightanswers, falseanswers, value, allanswers) "
                       "VALUES (:m, :p, :q, :r, :f, :v, :a)");
    testsQuery.bindValue(":m", Material);
    testsQuery.bindValue(":p", page);
    testsQuery.bindValue(":q", question);
    testsQuery.bindValue(":r", r);
    testsQuery.bindValue(":f", f);
    testsQuery.bindValue(":v", value);
    testsQuery.bindValue(":a", a);

    if(testsQuery.exec())
        return 1;
    else
        return 0;
}

int TestAndScores::changeQuestion(QString oldQuestion, QString Material, QString question, QStringList RightAnswers,
                                  QStringList FalseAnswers, int value, QStringList AllAnswers)
{
    QSqlDatabase Tests = QSqlDatabase::database(UsersPath);
    QSqlQuery testsQuery(Tests);

    QString v, page = question;
    page = page.toUpper();

    testsQuery.prepare("SELECT question FROM tests WHERE material='"+Material+"'");
    testsQuery.exec();
    while(testsQuery.next())
    {
        v = testsQuery.value(0).toString();
        v = v.toUpper();
        if(v == page)
            return 2;
    }

    QString r = "\v";
    for(int i = 0; i<RightAnswers.size(); ++i)
        r+=RightAnswers.at(i)+"\v";

    if(r=="\v")
        r="";

    QString f = "\v";
    for(int i = 0; i<FalseAnswers.size(); ++i)
        f+=FalseAnswers.at(i)+"\v";

    if(f=="\v")
        f="";

    QString a = "\v";
    for(int i = 0; i<AllAnswers.size(); ++i)
        a+=AllAnswers.at(i)+"\v";

    if(a=="\v")
        a="";

    testsQuery.prepare("SELECT page FROM tests WHERE material='"+Material+"' AND question='"+oldQuestion+"'");
    testsQuery.exec();
    testsQuery.first();
    page = testsQuery.value(0).toString();
    v = QString::number(value);

    testsQuery.prepare("UPDATE tests SET question='"+question+"', "
                       "rightanswers='"+r+"', falseanswers='"+f+"', value='"+v+"', allanswers='"+a+"' "
                       "WHERE material='"+Material+"' AND page='"+page+"'");

    if(testsQuery.exec())
        return 1;
    else
        return 0;
}

int TestAndScores::changeOnlyQuestion(QString oldQuestion, QString Material, QString question, int value)
{
    QSqlDatabase Tests = QSqlDatabase::database(UsersPath);
    QSqlQuery testsQuery(Tests);

    QString v, page = question;
    page = page.toUpper();

    testsQuery.prepare("SELECT question FROM tests WHERE material='"+Material+"'");
    testsQuery.exec();
    while(testsQuery.next())
    {
        v = testsQuery.value(0).toString();
        v = v.toUpper();
        if(v == page)
            return 2;
    }

    testsQuery.prepare("SELECT page FROM tests WHERE question='"+oldQuestion+"'");
    testsQuery.exec();
    testsQuery.first();
    page = testsQuery.value(0).toString();

    v = QString::number(value);

    testsQuery.prepare("UPDATE tests SET question='"+question+"', value='"+v+"' "
                       "WHERE material='"+Material+"' AND page='"+page+"'");

    if(testsQuery.exec())
        return 1;
    else
        return 0;
}

int TestAndScores::changeOnlyAnswers(QString Material, QString question, QStringList RightAnswers, QStringList FalseAnswers, QStringList AllAnswers)
{
    QSqlDatabase Tests = QSqlDatabase::database(UsersPath);
    QSqlQuery testsQuery(Tests);

    QString r = "\v";
    for(int i = 0; i<RightAnswers.size(); ++i)
        r+=RightAnswers.at(i)+"\v";

    if(r=="\v")
        r="";

    QString f = "\v";
    for(int i = 0; i<FalseAnswers.size(); ++i)
        f+=FalseAnswers.at(i)+"\v";

    if(f=="\v")
        f="";

    QString a = "\v";
    for(int i = 0; i<AllAnswers.size(); ++i)
        a+=AllAnswers.at(i)+"\v";

    if(a=="\v")
        a="";


    testsQuery.prepare("UPDATE tests SET rightanswers='"+r+"', falseanswers='"+f+"', allanswers='"+a+"' "
                       "WHERE material='"+Material+"' AND question='"+question+"'");

    if(testsQuery.exec())
        return 1;
    else
        return 0;
}

int TestAndScores::deleteQuestion(QString Material, QString question)
{
    QSqlDatabase Tests = QSqlDatabase::database(UsersPath);
    QSqlQuery testsQuery(Tests);
    QSqlQuery changePageQuery(Tests);

    testsQuery.prepare("SELECT page FROM tests WHERE material='"+Material+"' AND question='"+question+"'");
    testsQuery.exec();
    testsQuery.first();
    int page = testsQuery.value(0).toInt();


    testsQuery.prepare("DELETE FROM tests WHERE page='"+QString::number(page)+"'");
    if(testsQuery.exec())
    {
        testsQuery.prepare("SELECT question FROM tests WHERE material='"+Material+"' "
                           "AND page > '"+QString::number(page)+"'");
        testsQuery.exec();
        while(testsQuery.next())
        {
            changePageQuery.prepare("UPDATE tests SET page='"+QString::number(page)+"' "
                                    "WHERE material='"+Material+"' AND question='"+testsQuery.value(0).toString()+"'");
            changePageQuery.exec();
            ++page;
        }
        return 1;
    }
    else
        return 0;
}

int TestAndScores::checkTestAvaiability(QString Material, int student)
{
    QSqlDatabase Tests = QSqlDatabase::database(UsersPath);
    QSqlQuery testsQuery(Tests);


    testsQuery.prepare("SELECT * FROM tests WHERE material='"+Material+"'");
    testsQuery.exec();
    testsQuery.first();

    if(testsQuery.value(0).toString()=="")
    {
        qDebug()<<"НЕТ "<<Material;
        return 0;
    }


    QString id = QString::number(student);
    QStringList scores, lections;

    testsQuery.prepare("SELECT lections, scores FROM users WHERE id='"+id+"'");
    testsQuery.exec();
    testsQuery.first();
    lections = testsQuery.value(0).toString().split("|", QString::SkipEmptyParts);
    scores = testsQuery.value(1).toString().split(",", QString::SkipEmptyParts);


    for(int i = 0; i < lections.size(); ++i)
        if(lections.at(i)==Material && scores.at(i)!="-1")
        {
            qDebug()<<"УЖЕ ПРОЙДЕННЫЙ ТЕСТ";
            return 0;
        }
        else if(lections.at(i)==Material && Material.count("/")==2 && scores.at(i)=="-1")
            return 1;


    QString path = QDir::currentPath() + "/Data/Матеріали/"+Material;

    QStringList SplitList;
    QDirIterator it((QDir::toNativeSeparators(path)),
                    QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);
    QString mPath;

    QStringList tempCheckTestAvailability;
    QStringList dirsCheck;

    while (it.hasNext())
    {
        it.next();
        mPath = it.filePath();


        SplitList = mPath.split("/", QString::SkipEmptyParts);

        int s = SplitList.size();

        QString current = SplitList.at(s-3)+"/"+SplitList.at(s-2)+"/"+SplitList.at(s-1);
        tempCheckTestAvailability << current;

        current = SplitList.at(s-3)+"/"+SplitList.at(s-2);
        dirsCheck<<current;
    }


    dirsCheck.removeDuplicates();

    if(tempCheckTestAvailability.size()==0)
        if(!(lections.contains(Material)))
        {
            qDebug()<<"НЕ ПРОЧИТАНА ЛЕКЦИЯ " << Material;
            return 0;
        }

    for(int i = 0; i<tempCheckTestAvailability.size(); ++i)
        if(!(lections.contains(tempCheckTestAvailability.at(i))))
        {
            qDebug()<<"НЕ ПРОЧИТАНА ЛЕКЦИЯ " << tempCheckTestAvailability.at(i);
            return 0;
        }


    testsQuery.prepare("SELECT material FROM tests WHERE material LIKE :m");
    testsQuery.bindValue(":m", Material+"/%");
    testsQuery.exec();

    bool passed = true;
    while(testsQuery.next())
    {
        for(int i = 0; i < lections.size(); ++i)
            if(lections.at(i)==testsQuery.value(0).toString() && scores.at(i)!="-1")
            {
                qDebug()<<"ПРОЙДЕН ТЕСТ " << testsQuery.value(0).toString();
                continue;
            }
            else if(lections.at(i)==testsQuery.value(0).toString() && scores.at(i)=="-1")
            {
                passed = false;
                break;
            }

        if(!passed)
        {
            qDebug()<<"НЕ ПРОЙДЕН ТЕСТ " << testsQuery.value(0).toString();
            return 0;
        }
    }


    return 1;
}


QSqlQuery* TestAndScores::getTest(QString Material)
{
    QSqlDatabase Tests = QSqlDatabase::database(UsersPath);
    QSqlQuery *testsQuery = new QSqlQuery(Tests);

    testsQuery->prepare("SELECT page, question, rightanswers, falseanswers, value, "
                       "allanswers FROM tests WHERE material='"+Material+"'");
    testsQuery->exec();
    return testsQuery;
}

int TestAndScores::setScore(QString Material, int StudentId, QString Score)
{
    QSqlDatabase Scores = QSqlDatabase::database(UsersPath);
    QSqlQuery scoresQuery(Scores);

    scoresQuery.prepare("SELECT lections, scores FROM users WHERE id='"+QString::number(StudentId)+"'");
    scoresQuery.exec();
    scoresQuery.first();

    QStringList LectionsList = scoresQuery.value(0).toString().split("|", QString::SkipEmptyParts);
    QStringList ScorsList = scoresQuery.value(1).toString().split(",", QString::SkipEmptyParts);

    int i = 0;
    QString scores="", lections=scoresQuery.value(0).toString();

    for(i = 0; i < LectionsList.size(); ++i)
        if(LectionsList.at(i)==Material)
        {
            ScorsList.replace(i, Score);
            break;
        }


    if(i==LectionsList.size())
    {
        if(LectionsList.size() == 0)
        {
            lections = "|"+ Material +"|";
            scores = Score;
        }
        else
        {
            lections += Material +"|";
            scores = scoresQuery.value(1).toString()+","+Score;
            scores.replace(",,", ",");
        }
    }
    else
        for(int i = 0; i < ScorsList.size(); ++i, scores+=",")
            scores+=ScorsList.at(i);


    scoresQuery.prepare("UPDATE users SET lections='"+lections+"', scores='"+scores+"' WHERE id='"+QString::number(StudentId)+"'");
    if(scoresQuery.exec())
        return 1;
    else
        return 0;
}

