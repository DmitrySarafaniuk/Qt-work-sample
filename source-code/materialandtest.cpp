#include "materialandtest.h"

MaterialAndTest::MaterialAndTest()
{

}

MaterialAndTest::~MaterialAndTest()
{

}

int MaterialAndTest::addMaterial(QString FilePath, QWidget* Father)
{
    QString current = (QDir::toNativeSeparators(QDir::currentPath() + "/Data/Матеріали/"));
    int a = FilePath.count(current.at(current.size()-1)) - current.count(current.at(current.size()-1));

    if(a < 0)
    {
        QString name = QInputDialog::getText(Father, "Назва дисципліни", "Введіть назву дисципліни");
        if(name=="")
        {
            QMessageBox::information(Father, "Пуста форма", "Вам необхідно вказати назву дисципліни");
            return 2;
        }
        else
        {
            FilePath = current+name;

            QDir newSubject(FilePath);
            if(newSubject.mkdir(FilePath))
                return 1;
            else
                return 0;
        }
    }
    else if(a==0)
    {
        QString name = QInputDialog::getText(Father, "Назва теми", "Введіть назву теми");
        if(name=="")
        {
            QMessageBox::information(Father, "Пуста форма", "Вам необхідно вказати назву дисципліни");
            return 2;
        }
        else
        {
            FilePath=QDir::toNativeSeparators(FilePath+"/"+name);

            QDir newTheme;
            if(newTheme.mkdir(FilePath))
                return 1;
            else
                return 0;
        }
    }
    else if(a==1)
    {
        QString name = QInputDialog::getText(Father, "Назва заняття", "Введіть назву заняття");
        if(name=="")
            QMessageBox::information(Father, "Пуста форма", "Вам необхідно вказати назву заняття");


        if(name!="")
        {
            FilePath=QDir::toNativeSeparators(FilePath+"/"+name+".html");

            QFile file(FilePath);
            if(file.open(QIODevice::WriteOnly))
            {
                file.close();
                return 1;
            }
        }
        return 2;
    }
    return 0;
}

int MaterialAndTest::changeName(QString FilePath, QWidget *Father)
{
    QString current = (QDir::toNativeSeparators(QDir::currentPath() + "/Data/Матеріали/"));
    int a = FilePath.count(current.at(current.size()-1)) - current.count(current.at(current.size()-1));

    if(a<0)
    {
        QMessageBox::information(Father, "Константна назва", "Ви не можете змінити цю назву");
        return 2;
    }
    else if(a==0 || a==1)
    {
        QString name;
        if(a==0)
            name = QInputDialog::getText(Father, "Назва дисципліни", "Введіть нову назву дисципліни");
        else
            name = QInputDialog::getText(Father, "Назва теми", "Введіть нову назву теми");

        if(name=="")
            return 0;

        QStringList newNameList = FilePath.split(QDir::separator(), QString::SkipEmptyParts);
        QString newName="";

        for(int i=0; i<newNameList.size()-1; ++i, newName+="/")
            newName+=newNameList.at(i);

        newName=QDir::toNativeSeparators(newName+name);


        QDir changed(FilePath);
        if(changed.rename(FilePath, newName))
        {
            if(a==1)
            {
                newName=newNameList.at(newNameList.size()-2)+"/"+name;
                FilePath=newNameList.at(newNameList.size()-2)+"/"+newNameList.at(newNameList.size()-1);
            }
            else
            {
                newName=name;
                FilePath=newNameList.at(newNameList.size()-1);
            }


            databaseManipulation(FilePath, newName, a);
            return 1;
        }
    }
    else if(a==2)
    {
        QString name = QInputDialog::getText(Father, "Назва заняття", "Введіть нову назву заняття");
        if(name=="")
            return 0;

        QStringList newNameList = FilePath.split(QDir::separator(), QString::SkipEmptyParts);
        QString newName="";

        for(int i=0; i<newNameList.size()-1; ++i, newName+="/")
            newName+=newNameList.at(i);

        newName=QDir::toNativeSeparators(newName+name+".html");

        if(QFile::rename(FilePath, newName))
        {
            newName="";
            FilePath = "";

            for(int i=newNameList.size()-3; i<newNameList.size()-1;
                ++i, newName+="/", FilePath+="/")
            {
                newName+=newNameList.at(i);
                FilePath+=newNameList.at(i);
            }

            newName+=name+".html";
            FilePath+=newNameList.at(newNameList.size()-1);

            databaseManipulation(FilePath, newName, a);
            return 1;
        }
    }
    return 0;
}

int MaterialAndTest::changeMaterial(QString Path, QString Text)
{
    QFile file(Path);
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream in(&file);
        in<<Text;

        file.close();
        return 1;
    }
    return 0;
}

int MaterialAndTest::deleteMaterial(QString FilePath)
{
    QString current = (QDir::toNativeSeparators(QDir::currentPath() + "/Data/Materials/"));

    int a = FilePath.count(current.at(current.size()-1)) - current.count(current.at(current.size()-1));

    QStringList deleteFileName = FilePath.split(QDir::separator(), QString::SkipEmptyParts);
    if(a < 0)
        return 2;
    else if(a == 0 || a == 1)
    {
        QDir DelDir(FilePath);
        if(DelDir.removeRecursively() || (DelDir.exists() && DelDir.removeRecursively()) )
        {
            if(a==0)
                FilePath=deleteFileName.at(deleteFileName.size()-1);
            else
                FilePath=deleteFileName.at(deleteFileName.size()-2)+"/"
                        +deleteFileName.at(deleteFileName.size()-1);



            QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

            QSqlQuery deleteDD(UsersDB);
            QSqlQuery deleteDLC(UsersDB);

            deleteDD.prepare("SELECT id, lections, scores FROM users WHERE lections LIKE :l");
            deleteDD.bindValue(":l", "%|" +FilePath+ "|%");

            if(deleteDD.exec())
            {
                QString repl, atl, reps;
                QStringList s, deleteFileN;
                while (deleteDD.next())
                {
                    repl = deleteDD.value(1).toString();
                    deleteFileN=repl.split("|", QString::SkipEmptyParts);

                    reps=deleteDD.value(2).toString();
                    s=reps.split(",", QString::SkipEmptyParts);

                    repl="";
                    reps="";

                    for(int i = 0; i<deleteFileN.size(); ++i)
                    {
                        atl=deleteFileN.at(i);
                        if(atl != FilePath)
                        {
                            repl+="|"+atl;
                            reps+=s.at(i)+",";
                        }
                    }

                    if(repl.count("|")==repl.size())
                        repl="";
                    else
                        repl+="|";

                    if(reps.size()!=0)
                        reps.resize(reps.size()-1);


                    deleteDLC.prepare("UPDATE users SET lections=:l, scores=:s "
                                      "WHERE id='"+deleteDD.value(0).toString()+"'");
                    deleteDLC.bindValue(":l", repl);
                    deleteDLC.bindValue(":s", reps);
                    deleteDLC.exec();
                }
            }



            deleteDatabaseData(FilePath, deleteFileName, a);

            return 1;
        }
        return 0;
    }
    else if(a == 2)
    {
        QFile DelFile(FilePath);
        if(DelFile.remove())
        {
            FilePath="";
            for(int i=deleteFileName.size()-3; i<deleteFileName.size(); ++i, FilePath+="/")
                FilePath+=deleteFileName.at(i);

            FilePath.resize(FilePath.size()-1);

            QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

            QSqlQuery deleteDatabaseData(UsersDB);
            QSqlQuery deleteDatabaseLC(UsersDB);

            deleteDatabaseData.prepare("DELETE FROM lectionsconnections WHERE lection = '"+FilePath+"' ");
            deleteDatabaseData.exec();

            deleteDatabaseData.prepare("SELECT * FROM lectionsconnections WHERE connections LIKE :c");
            deleteDatabaseData.bindValue(":c", "%|" +FilePath+ "|%");

            if(deleteDatabaseData.exec())
            {
                QString repl;
                while (deleteDatabaseData.next())
                {
                    repl = deleteDatabaseData.value(1).toString();
                    repl.replace("|"+FilePath, "");
                    if(repl.count("|")==repl.size())
                        repl="";

                    deleteDatabaseLC.prepare("UPDATE lectionsconnections SET connections=:nc "
                                             "WHERE lection=:l");
                    deleteDatabaseLC.bindValue(":nc", repl);
                    deleteDatabaseLC.bindValue(":l", deleteDatabaseData.value(0).toString());
                    deleteDatabaseLC.exec();
                }
            }

            deleteDatabaseData.prepare("SELECT id, lections, scores FROM users WHERE lections LIKE :l");
            deleteDatabaseData.bindValue(":l", "%|" +FilePath+ "|%");

            if(deleteDatabaseData.exec())
            {
                QString repl, atl, reps;
                QStringList s;
                while (deleteDatabaseData.next())
                {
                    repl = deleteDatabaseData.value(1).toString();
                    deleteFileName=repl.split("|", QString::SkipEmptyParts);

                    reps=deleteDatabaseData.value(2).toString();
                    s=reps.split(",", QString::SkipEmptyParts);

                    repl="";
                    reps="";

                    for(int i = 0; i<deleteFileName.size(); ++i)
                    {
                        atl=deleteFileName.at(i);
                        if(atl.indexOf(FilePath)!=0)
                        {
                            repl+="|"+atl;
                            reps+=s.at(i)+",";
                        }
                    }

                    if(repl.count("|")==repl.size())
                        repl="";
                    else
                        repl+="|";

                    if(reps.size()!=0)
                        reps.resize(reps.size()-1);


                    deleteDatabaseLC.prepare("UPDATE users SET lections=:l, scores=:s "
                                             "WHERE id='"+deleteDatabaseData.value(0).toString()+"'");
                    deleteDatabaseLC.bindValue(":l", repl);
                    deleteDatabaseLC.bindValue(":s", reps);
                    deleteDatabaseLC.exec();
                }
            }

            deleteDatabaseData.prepare("DELETE FROM tests WHERE material='"+FilePath+"'");
            deleteDatabaseData.exec();

            return 1;
        }
        return 0;
    }
    return 0;
}

QString MaterialAndTest::getMaterial(QString Path)
{
    QFile file(Path);
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        QString text = in.readAll();
        file.close();
        return text;
    }
    return "\v";
}

QString MaterialAndTest::connectTheme(QString ConnectedLection, QString FilePath, QWidget *Father)
{
    QStringList lpath = QDir::toNativeSeparators(FilePath).split(QDir::separator(), QString::SkipEmptyParts);

    QString npath="";

    QString current = QDir::toNativeSeparators((QDir::currentPath() + "/Data/Materials/"));
    int a = FilePath.count(current.at(current.size()-1)) - current.count(current.at(current.size()-1));

    if (a==1)
        npath=lpath.at(lpath.size()-2)+"/"+lpath.at(lpath.size()-1);


    if(ConnectedLection=="")
    {
        if(npath=="")
            QMessageBox::information(Father, "Неправильні дані", "Вам необхідно вибрати тему для зв'язку");
        else
        {
            ConnectedLection = npath;
            QMessageBox::information(Father, "Початок з'єднання", "Тема для з'єднання була добавлена");
            return ConnectedLection;
        }
    }
    else
    {
        if(npath == ConnectedLection)
        {
            ConnectedLection="";
            QMessageBox::information(Father, "Кінець з'єднання", "З'єднання тем закінчено");
            return ConnectedLection;
        }
        else if (a==1)
        {
            QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

            QSqlQuery connectDatabaseData(UsersDB);
            connectDatabaseData.prepare("SELECT * FROM lectionsconnections WHERE lection='"+ConnectedLection+"'");
            connectDatabaseData.exec();

            QString connections="", lect="";
            int contain = 0;

            connectDatabaseData.first();
            lect = connectDatabaseData.value(0).toString();
            connections = connectDatabaseData.value(1).toString();

            if(lect != "")
            {
                if(connections.indexOf("|"+npath+"|") < 0)
                    contain = 1;
                else
                    contain = 2;
            }
            else
                contain = 0;


            if(contain == 0)
            {
                connectDatabaseData.prepare("INSERT INTO lectionsconnections (lection, connections) "
                                            "VALUES (:l, :c)");
                connectDatabaseData.bindValue(":l", ConnectedLection);
                connectDatabaseData.bindValue(":c", "|"+npath+"|");
                connectDatabaseData.exec();
                QMessageBox::information(Father, "Тема з'єднана", "Зв'язок встановлено");
            }
            else  if (contain == 1)
            {
                if(connections!="")
                    connections+=npath+"|";
                else
                    connections+="|"+npath+"|";

                connectDatabaseData.prepare("UPDATE lectionsconnections SET connections='"+connections+"' "
                                            "WHERE lection='"+ConnectedLection+"'");
                connectDatabaseData.exec();
                QMessageBox::information(Father, "Тема з'єднана", "Зв'язок встановлено");
            }
            else
                QMessageBox::information(Father, "Дублювання", "Зв'язок вже був встановлен");


        }
        else
            QMessageBox::information(Father, "Неправильні дані", "Вам необхідно вибрати тему для зв'язку");

    }
    return "\v";
}

QString MaterialAndTest::disconnectTheme(QString DisconnectedLection, QString FilePath, QWidget *Father)
{
    QStringList lpath = QDir::toNativeSeparators(FilePath).split(QDir::separator(), QString::SkipEmptyParts);

    QString npath="";

    QString current = QDir::toNativeSeparators((QDir::currentPath() + "/Data/Materials/"));
    int a = FilePath.count(current.at(current.size()-1)) - current.count(current.at(current.size()-1));

    if (a==1)
        npath=lpath.at(lpath.size()-2)+"/"+lpath.at(lpath.size()-1);


    if(DisconnectedLection=="")
    {
        if(npath=="")
            QMessageBox::information(Father, "Неправильні дані", "Вам необхідно вибрати тему для видалення зв'язку");
        else
        {
            DisconnectedLection = npath;
            QMessageBox::information(Father, "Початок роз'єднання", "Тема для роз'єднання була добавлена");
            return DisconnectedLection;
        }
    }
    else
    {
        if(npath == DisconnectedLection)
        {
            DisconnectedLection="";
            QMessageBox::information(Father, "Кінець роз'єднання", "Роз'єднання тем закінчено");
            return DisconnectedLection;
        }
        else if (a==1)
        {
            QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

            QSqlQuery connectDatabaseData(UsersDB);
            connectDatabaseData.prepare("SELECT * FROM lectionsconnections WHERE lection='"+DisconnectedLection+"'");
            connectDatabaseData.exec();

            QString connections="", lect="";
            int contain = 0;

            connectDatabaseData.first();
            lect = connectDatabaseData.value(0).toString();
            connections = connectDatabaseData.value(1).toString();

            if(lect != "")
            {
                if(connections.indexOf("|"+npath+"|") < 0)
                    contain = 1;
                else
                    contain = 2;
            }
            else
                contain = 0;


            if(contain == 0)
                QMessageBox::information(Father, "Немає зв'язків", "Обрана тема для роз'єднання ще не має зв'язків");
            else  if (contain == 1)
                QMessageBox::information(Father, "Немає вибраного зв'язку", "Обрана тема для роз'єднання ще не має вибраного зв'язків");
            else
            {
                connections = connections.replace("|"+npath+"|", "|");
                if(connections.count("|") == connections.size())
                    connections = "";


                connectDatabaseData.prepare("UPDATE lectionsconnections SET connections='"+connections+"' "
                                            "WHERE lection='"+DisconnectedLection+"'");
                connectDatabaseData.exec();
                QMessageBox::information(Father, "Тема роз'єднана", "Роз'єднання виконано");
            }
        }
        else
            QMessageBox::information(Father, "Неправильні дані", "Вам необхідно вибрати тему для зв'язку");
    }
    return "\v";
}


void MaterialAndTest::setDB(QString UP)
{
    UsersPath = UP;
}

void MaterialAndTest::databaseManipulation(QString FilePath, QString newName, int a)
{
    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

    QSqlQuery changeDatabaseData(UsersDB);
    QSqlQuery changeDatabaseLC(UsersDB);


    if(FilePath.count("/") > 0)
        changeDatabaseData.prepare("UPDATE lectionsconnections SET lection='"+newName+"' "
                                   "WHERE lection = '"+FilePath+"' ");
    else
    {
        changeDatabaseLC.prepare("SELECT lection FROM lectionsconnections "
                                 "WHERE lection LIKE :l");
        changeDatabaseLC.bindValue(":l", FilePath+"/%");
        changeDatabaseLC.exec();

        QStringList oldPath;
        QString newPath;
        while(changeDatabaseLC.next())
        {
            oldPath = changeDatabaseLC.value(0).toString().split("/", QString::SkipEmptyParts);
            newPath = newName;
            for(int i = 1; i<oldPath.size(); ++i)
                newPath+="/"+oldPath.at(i);


            changeDatabaseData.prepare("UPDATE lectionsconnections SET lection='"+newPath+"' "
                                       "WHERE lection='"+changeDatabaseLC.value(0).toString()+"'");
            changeDatabaseData.exec();
        }
    }
    changeDatabaseData.exec();


    changeDatabaseData.prepare("UPDATE tests SET material='"+newName+"' "
                               "WHERE material = '"+FilePath+"' ");
    changeDatabaseData.exec();


    QString repl;

    if(a!=2)
    {
        if(a == 1)
            repl = "%|" + FilePath + "|%";
        else
            repl = "%|" + FilePath + "/%";
        changeDatabaseData.prepare("SELECT * FROM lectionsconnections WHERE connections LIKE '"+repl+"'");
        if(changeDatabaseData.exec())
        {
            while(changeDatabaseData.next())
            {
                repl = changeDatabaseData.value(1).toString();
                if(a == 1)
                    repl.replace("|"+FilePath+"|", "|"+newName+"|");
                else
                    repl.replace("|"+FilePath+"/", "|"+newName+"/");

                changeDatabaseLC.prepare("UPDATE lectionsconnections SET connections='"+repl+"' "
                                         "WHERE lection='"+changeDatabaseData.value(0).toString()+"' ");
                changeDatabaseLC.exec();
            }
        }
    }

    if(a==0 || a==1)
        repl = "%|" + FilePath + "/%";
    else if(a==2)
        repl = "%|" + FilePath + "|%";

    changeDatabaseData.prepare("SELECT id, lections FROM users WHERE lections LIKE '"+repl+"'");
    if(changeDatabaseData.exec())
    {
        while (changeDatabaseData.next())
        {
            repl = changeDatabaseData.value(1).toString();
            if(a==0 || a==1)
                repl.replace("|"+FilePath+"/", "|"+newName+"/");
            else
                repl.replace("|"+FilePath+"|", "|"+newName+"|");

            changeDatabaseLC.prepare("UPDATE users SET lections='"+repl+"' "
                                     "WHERE id='"+changeDatabaseData.value(0).toString()+"'");
            changeDatabaseLC.exec();
        }
    }

    if(a==0 || a==1)
    {
        repl = "%|" + FilePath + "|%";
        changeDatabaseData.prepare("SELECT id, lections FROM users WHERE lections LIKE '"+repl+"'");
        if(changeDatabaseData.exec())
        {
            while (changeDatabaseData.next())
            {
                repl = changeDatabaseData.value(1).toString();
                repl.replace("|"+FilePath+"|", "|"+newName+"|");

                changeDatabaseLC.prepare("UPDATE users SET lections='"+repl+"' "
                                         "WHERE id='"+changeDatabaseData.value(0).toString()+"'");
                changeDatabaseLC.exec();
            }
        }
    }
}

void MaterialAndTest::deleteDatabaseData(QString FilePath, QStringList deleteFileName, int a)
{
    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

    QSqlQuery deleteDatabaseData(UsersDB);
    QSqlQuery deleteDatabaseLC(UsersDB);

    if(a==0)
    {
        deleteDatabaseData.prepare("DELETE FROM lectionsconnections WHERE lection LIKE :l ");
        deleteDatabaseData.bindValue(":l", FilePath+"/%");
    }
    else if(a==1)
    {
        deleteDatabaseData.prepare("DELETE FROM lectionsconnections WHERE lection=:l ");
        deleteDatabaseData.bindValue(":l", FilePath);
    }
    deleteDatabaseData.exec();


    if(a == 0 || a == 1)
    {
        deleteDatabaseData.prepare("DELETE FROM tests WHERE material LIKE :m");
        deleteDatabaseData.bindValue(":m", FilePath+"/%");
        deleteDatabaseData.exec();

        deleteDatabaseData.prepare("DELETE FROM tests WHERE material=:m");
        deleteDatabaseData.bindValue(":m", FilePath);
        deleteDatabaseData.exec();
    }


    QString add = "%|" +FilePath;
    if(a==0)
        add+="/%";
    else
        add+="|%";


    deleteDatabaseData.prepare("SELECT * FROM lectionsconnections WHERE connections LIKE '"+add+"'");

    if(a==0)
        add="/";
    else
        add="|";

    if(deleteDatabaseData.exec())
    {
        QString repl, atl, atl_copy;
        while (deleteDatabaseData.next())
        {
            repl = deleteDatabaseData.value(1).toString();
            deleteFileName=repl.split("|", QString::SkipEmptyParts);
            repl="";

            for(int i = 0; i<deleteFileName.size(); ++i)
            {
                atl = deleteFileName.at(i);
                atl_copy = atl+"|";
                if(atl_copy.indexOf(FilePath+add)!=0)
                    repl+="|"+atl;
            }
            if(repl.count("|")==repl.size())
                repl="";
            else
                repl+="|";

            deleteDatabaseLC.prepare("UPDATE lectionsconnections SET connections='"+repl+"' "
                                     "WHERE lection='"+deleteDatabaseData.value(0).toString()+"'");
            deleteDatabaseLC.exec();
        }
    }

    add="/";

    deleteDatabaseData.prepare("SELECT id, lections, scores FROM users WHERE lections LIKE :l");
    deleteDatabaseData.bindValue(":l", "%|" +FilePath+ "/%");

    if(deleteDatabaseData.exec())
    {
        QString repl, atl, reps;
        QStringList s;
        while (deleteDatabaseData.next())
        {
            repl = deleteDatabaseData.value(1).toString();
            deleteFileName=repl.split("|", QString::SkipEmptyParts);

            reps=deleteDatabaseData.value(2).toString();
            s=reps.split(",", QString::SkipEmptyParts);

            repl="";
            reps="";

            for(int i = 0; i<deleteFileName.size(); ++i)
            {
                atl=deleteFileName.at(i);
                if(atl.indexOf(FilePath+add)!=0)
                {
                    repl+="|"+atl;
                    reps+=s.at(i)+",";
                }
            }
            if(repl.count("|")==repl.size())
                repl="";
            else
                repl+="|";

            if(reps.size()!=0)
                reps.resize(reps.size()-1);

            deleteDatabaseLC.prepare("UPDATE users SET lections='"+repl+"', scores='"+reps+"' "
                                     "WHERE id='"+deleteDatabaseData.value(0).toString()+"'");
            deleteDatabaseLC.exec();
        }
    }
}
