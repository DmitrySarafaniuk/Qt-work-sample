#ifndef TUTORFORM_H
#define TUTORFORM_H

#include <QFileSystemModel>

#include "authorizationandoutcome.h"
#include "registrationoredit.h"
#include "materialandtest.h"
#include "testform.h"

namespace Ui {
class TutorForm;
}

class TutorForm : public QDialog
{
    Q_OBJECT

public:
    explicit TutorForm(QWidget *parent = nullptr);
    ~TutorForm();
    void setDB(QString path);
    void setStudentList(void);
    void setFilesView(void);

protected:
    virtual bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_exit_clicked();

    void on_AddStudent_clicked();

    void on_UpdateStudent_clicked();

    void on_DeleteStudent_clicked();

    void on_AddMaterial_clicked();

    void on_EditName_clicked();

    void on_EditMaterial_clicked();

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_DeleteMaterial_clicked();

    void on_Connection_clicked();

    void on_DeleteConnection_clicked();

    void on_AddTest_clicked();

    void on_Results_clicked();

private:
    Ui::TutorForm *ui;
    QString UsersPath;
    QString ConnectedLections="";
    QString DisconnectedLections="";

    QFileSystemModel *fileModel;
    QSqlQueryModel *StudentListQueryModel;

    TestForm *testForm = nullptr;

    void showConnection(QString path);
    void _handleWhatsThisEntry(QWidget * );
};

#endif // TUTORFORM_H
