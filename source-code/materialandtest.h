#ifndef MATERIALANDTEST_H
#define MATERIALANDTEST_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QString>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QDebug>

class MaterialAndTest
{
public:
    MaterialAndTest();
    ~MaterialAndTest();

    void setDB(QString UP);
    int addMaterial(QString FilePath, QWidget* Father);
    int changeName(QString FilePath, QWidget* Father);
    int deleteMaterial(QString FilePath);

    int changeMaterial(QString Path, QString Text);
    QString getMaterial(QString Path);

    QString connectTheme(QString ConnectedLection, QString FilePath, QWidget *Father);
    QString disconnectTheme(QString DisconnectedLection, QString FilePath, QWidget *Father);

private:
    QString UsersPath;
    void databaseManipulation(QString FilePath, QString newName, int a);
    void deleteDatabaseData(QString FilePath, QStringList deleteFileName, int a);
};

#endif // MATERIALANDTEST_H
