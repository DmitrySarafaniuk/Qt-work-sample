#ifndef STUDENTFORM_H
#define STUDENTFORM_H

#include <QStringListModel>
#include <QElapsedTimer>

#include "authorizationandoutcome.h"
#include "materialandtest.h"
#include "testandscores.h"
#include "testform.h"

namespace Ui {
class StudentForm;
}

class StudentForm : public QDialog
{
    Q_OBJECT

public:
    explicit StudentForm(QWidget *parent = nullptr);
    ~StudentForm();

    void setUsersPath(QString Path);
    void setStudent(int student);

protected:
    virtual bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_exit_clicked();

    void on_materialsList_doubleClicked(const QModelIndex &index);

    void on_connectedLections_clicked();

    void on_backward_clicked();

    void on_materialsList_clicked(const QModelIndex &index);

    void on_test_clicked();

    void on_getResults_clicked();

private:
    Ui::StudentForm *ui;

    int Student;
    QString UsersPath;

    QStringListModel *fileModel;
    QStringList LectionsList;
    QStringList PathToLections;
    QString CurrPath;
    QString OpenedLection;
    QString PreviousPath;

    int ShowHideConnections = 0;

    QElapsedTimer timer;

    TestForm *testForm = nullptr;
    QString MaterialForTest="";

    void startForm(void);
    void endForm(void);

    void findTheme(QString path);
    void findLections(QString path);
    void findSubjects(QString path);
    void _handleWhatsThisEntry(QWidget * );
};

#endif // STUDENTFORM_H
