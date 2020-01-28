#include "authorizationform.h"
#include "ui_authorizationform.h"

AuthorizationForm::AuthorizationForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AuthorizationForm)
{
    this->setWindowIcon(QIcon(":/Authorization.ico"));
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowContextHelpButtonHint);
    qApp->installEventFilter(this);

    startDB();
}

AuthorizationForm::~AuthorizationForm()
{
    QSqlDatabase UsersDB=QSqlDatabase::database(UsersPath);
    UsersDB.close();
    QSqlDatabase::removeDatabase(UsersPath);
    delete ui;
}


void AuthorizationForm::on_LogIn_clicked()
{
    QString Login = ui->Login->currentText();
    QString Password = ui->Password->text();

    if(Password=="")
        QMessageBox::information(this, tr("Пуста форма"), tr("Вам необхідно ввести пароль"));
    else
    {
        AuthorizationAndOutcome user(Login, Password);
        int check = user.login(user.getLogin(), user.getPassword());
        if(check == 0)
            QMessageBox::information(this, tr("Неправильна інформація"), tr("Ви ввели неправильний пароль"));
        else if(check == 1)
        {
            QMessageBox::information(this, tr("Успіх"), tr("Вы успішно ввішли до системи"));
            QString UsersPath = user.getUsersPath();

            QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);

            QSqlQuery qry(UsersDB);
            qry.prepare("SELECT login FROM users WHERE id=0");
            qry.exec();
            qry.first();

            if(user.getLogin()==qry.value(0).toString())
            {
                tutorForm = new TutorForm(this);
                tutorForm->setDB(UsersPath);

                tutorForm->setGeometry
                (
                    QStyle::alignedRect
                    (
                        Qt::LeftToRight,
                        Qt::AlignCenter,
                        tutorForm->size(),
                        qApp->desktop()->availableGeometry()
                    )
                );
                tutorForm->setWindowIcon(QIcon(":/Tutor.ico"));
                tutorForm->setModal(true);
                tutorForm->setWindowState(Qt::WindowMaximized);
                tutorForm->setAttribute(Qt::WA_DeleteOnClose);

                ui->Password->clear();
                this->hide();

                tutorForm->exec();
                tutorForm=nullptr;

                startDB();
            }
            else
            {
                studentForm = new StudentForm(this);
                studentForm->setUsersPath(UsersPath);
                qry.prepare("SELECT id FROM users WHERE login='"+Login+"'");
                qry.exec();

                qry.first();
                studentForm->setStudent(qry.value(0).toInt());

                studentForm->setGeometry
                (
                    QStyle::alignedRect
                    (
                         Qt::LeftToRight,
                         Qt::AlignCenter,
                         studentForm->size(),
                         qApp->desktop()->availableGeometry()
                    )
                );

                studentForm->setWindowIcon(QIcon(":/Studentform.ico"));
                studentForm->setModal(true);
                studentForm->setWindowState(Qt::WindowMaximized);
                studentForm->setAttribute(Qt::WA_DeleteOnClose);

                ui->Password->clear();
                this->hide();

                studentForm->exec();
                studentForm=nullptr;
            }
        }
    }
}

void AuthorizationForm::startDB(void)
{
    ui->Login->clear();

    QDir().mkdir("Data");
    UsersPath = QDir::toNativeSeparators(QDir::currentPath() + "/Data/Users.db");

    if ( !(QFile::exists(UsersPath)) )
    {
        QSqlDatabase UsersDB=QSqlDatabase::addDatabase("QSQLITE");
        UsersDB.setDatabaseName(UsersPath);
        UsersDB.open();

        QSqlQuery qry(UsersDB);
        qry.prepare("CREATE TABLE users (id integer primary key, login text, password text, lections text, scores text, time integer)");
        qry.exec();


        QString Login = "Кунгурцев Олексій Борисович";
        QString AdminPassword = "admin_K1";

        QByteArray PasswordHash (AdminPassword.toStdString().c_str());
        QString CipheredPasswordHash = QCryptographicHash::hash(PasswordHash, QCryptographicHash::Keccak_512).toHex();
        QString lectinos_scores="-1";
        int id=0;
        qry.prepare("INSERT INTO users (id, login, password, lections, scores, time) "
                    "VALUES (:id, :login, :password, :lections, :scores, :time)");

        qry.bindValue(":id", id);
        qry.bindValue(":login", Login);
        qry.bindValue(":password", CipheredPasswordHash);
        qry.bindValue(":lections", lectinos_scores);
        qry.bindValue(":scores", lectinos_scores);
        qry.bindValue(":time", id);
        qry.exec();

        qry.prepare("SELECT login FROM users");
        qry.exec();
        while(qry.next())
            ui->Login->addItem(qry.value(0).toString());

        DBconnect = true;
    }
    else
    {
        QSqlDatabase UsersDB;
        if(!DBconnect)
        {
             UsersDB = QSqlDatabase::addDatabase("QSQLITE");
             DBconnect = true;
        }
        UsersDB.setDatabaseName(UsersPath);
        if(!UsersDB.isOpen())
            UsersDB.open();

        QSqlQuery qry(UsersDB);
        qry.prepare("SELECT login FROM users");
        qry.exec();
        while(qry.next())
            ui->Login->addItem(qry.value(0).toString());
    }
}

bool AuthorizationForm::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::EnterWhatsThisMode) {
        _handleWhatsThisEntry(QApplication::activeWindow());
        return true;
    }
    return QObject::eventFilter(object, event);
}

void AuthorizationForm::_handleWhatsThisEntry(QWidget * /*sender*/) {

    //QProcess::execute("hh.exe C:/Users/User/Documents/ONPU Subjects/OOPCourseWork/CourseWorkFinal/Help.chm");
    QDesktopServices::openUrl(QUrl::fromLocalFile("Help.chm"));
}
