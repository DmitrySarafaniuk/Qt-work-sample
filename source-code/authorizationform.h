#ifndef AUTHORIZATIONFORM_H
#define AUTHORIZATIONFORM_H

#include <QMainWindow>
#include "authorizationandoutcome.h"
#include "tutorform.h"
#include "studentform.h"

#include <QMessageBox>
#include <QStyle>
#include <QDesktopWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class AuthorizationForm; }
QT_END_NAMESPACE

class AuthorizationForm : public QMainWindow
{
    Q_OBJECT

public:
    AuthorizationForm(QWidget *parent = nullptr);
    ~AuthorizationForm();
    void startDB(void);

protected:
    virtual bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_LogIn_clicked();

private:
    Ui::AuthorizationForm *ui;
    TutorForm *tutorForm = nullptr;
    StudentForm *studentForm = nullptr;

    QString UsersPath;
    bool DBconnect = false;

    void _handleWhatsThisEntry(QWidget * );
};
#endif // AUTHORIZATIONFORM_H
