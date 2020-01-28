#include "tutorform.h"
#include "ui_tutorform.h"

TutorForm::TutorForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TutorForm)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    setStudentList();
    setFilesView();
}

TutorForm::~TutorForm()
{

}

void TutorForm::on_AddStudent_clicked()
{
    RegistrationOrEdit reg(this);

    reg.setWindowIcon(QIcon(":/RegistrationOrEdit.ico"));
    reg.setModal(true);
    reg.exec();
    setStudentList();
}

void TutorForm::on_UpdateStudent_clicked()
{
    QModelIndex index = ui->studentsList->currentIndex();
    QString PreviousLogin = index.data(Qt::DisplayRole).toString();

    if(PreviousLogin=="")
        QMessageBox::information(this, "Не обран студен", "Вам потрібно вибрати студента для редагування даних");
    else
    {
        RegistrationOrEdit reg(this);

        reg.setWindowIcon(QIcon(":/RegistrationOrEdit.ico"));
        reg.setModal(true);
        reg.setChoosenUser(PreviousLogin);

        reg.exec();
        setStudentList();
    }
}

void TutorForm::on_DeleteStudent_clicked()
{
    QModelIndex index = ui->studentsList->currentIndex();
    QString PreviousLogin = index.data(Qt::DisplayRole).toString();

    if(PreviousLogin=="")
        QMessageBox::information(this, "Не обран студен", "Вам потрібно вибрати студента для його виключення");
    else
    {
        if(!QMessageBox::question(this, "Видалення", "Ви дійсно бажаєте виключити студента "+ PreviousLogin +" з системи?", "Так", "Ні"))
        {
            AuthorizationAndOutcome user(PreviousLogin, "");
            int check = user.deleteUser(user.getLogin());
            if(check == 0)
                QMessageBox::information(this, tr("Помилка"), tr("Невизначена помилка під час спроби видалення"));
            else
                QMessageBox::information(this, tr("Успіх"), tr("Вы успішно виключили студента"));

            setStudentList();
        }
    }
}

void TutorForm::on_exit_clicked()
{
    if(!QMessageBox::question(this, "Вихід", "Ви дійсно бажаєте вийти з системи?", "Так", "Ні"))
    {
        delete StudentListQueryModel;
        delete fileModel;
        delete ui;

        parentWidget()->show();

        this->close();
    }
}

void TutorForm::setStudentList(void)
{
    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);
    QSqlQuery StudentListQuery(UsersDB);
    StudentListQueryModel=new QSqlQueryModel();


    StudentListQuery.prepare("SELECT login FROM users WHERE id>0");
    StudentListQuery.exec();

    StudentListQueryModel->setQuery(StudentListQuery);
    ui->studentsList->setModel(StudentListQueryModel);
}

void TutorForm::setFilesView(void)
{
    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);
    QSqlQuery FilesViewQuery(UsersDB);

    QDir dirpath(QDir::toNativeSeparators(QDir::currentPath() + "/Data/Матеріали"));
    QDir().mkdir(dirpath.path());
    QString mPath = QDir::toNativeSeparators(QDir::currentPath() + "/Data");

    fileModel = new QFileSystemModel(this);

    fileModel->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot );
    fileModel->setRootPath(mPath);
    fileModel->setNameFilters(QStringList() << "*.html");
    fileModel->setNameFilterDisables(false);
    ui->treeView->setModel(fileModel);
    ui->treeView->setRootIndex(fileModel->index(mPath));

    FilesViewQuery.prepare("CREATE TABLE IF NOT EXISTS lectionsconnections (lection CLOB, connections CLOB)");
    FilesViewQuery.exec();
}

void TutorForm::setDB(QString path)
{
    UsersPath = path;
}

void TutorForm::on_AddMaterial_clicked()
{
    QString path = QDir::toNativeSeparators(fileModel->filePath(ui->treeView->currentIndex()));

    MaterialAndTest material;
    int check = material.addMaterial(path, this);
    if(check == 0)
        QMessageBox::information(this, tr("Помилка"), tr("Невизначена помилка під час спроби створити матеріал"));
    else if (check == 1)
        QMessageBox::information(this, tr("Успіх"), tr("Матеріал був успішно створений"));
}

void TutorForm::on_EditName_clicked()
{
    fileModel->setReadOnly(false);
    QString path = QDir::toNativeSeparators(fileModel->filePath(ui->treeView->currentIndex()));

    MaterialAndTest material;
    material.setDB(UsersPath);

    int check = material.changeName(path, this);
    if(check == 0)
        QMessageBox::information(this, tr("Помилка"), tr("Невизначена помилка під час спроби зміни назви матеріалу\n(файл заблокований)"));
    else if (check == 1)
        QMessageBox::information(this, tr("Успіх"), tr("Назва матеріалу була успішно змінена"));

    fileModel->setReadOnly(true);
}

void TutorForm::on_EditMaterial_clicked()
{
    QString path = QDir::toNativeSeparators(fileModel->filePath(ui->treeView->currentIndex()));

    if(path.size() > 5 && path.indexOf(".html", path.size() - 5) > 0)
    {
        QString text = ui->materialEdit->toHtml();

        MaterialAndTest material;
        material.setDB(UsersPath);
        int check = material.changeMaterial(path, text);
        if(check == 0)
            QMessageBox::information(this, tr("Помилка"), tr("Не вдалося відкрити файл"));
        else
            QMessageBox::information(this, tr("Успіх"), tr("Матеріал був успішно змінений"));
    }
    else
        QMessageBox::information(this, tr("Не правильні дані"), tr("Вам необхідно вибрати лекцію"));

}

void TutorForm::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString path = QDir::toNativeSeparators(fileModel->filePath(ui->treeView->currentIndex()));

    if(path.size() > 5 && path.indexOf(".html", path.size() - 5) > 0)
    {
        ui->materialEdit->toHtml();

        MaterialAndTest material;
        material.setDB(UsersPath);

        QString document = material.getMaterial(path);
        if(document == "\v")
            QMessageBox::information(this, tr("Помилка"), tr("Не вдалося відкрити файл"));
        else
            ui->materialEdit->setHtml(document);
    }
    else
        showConnection(path);
}

void TutorForm::on_DeleteMaterial_clicked()
{
    fileModel->setReadOnly(false);

    QString path = QDir::toNativeSeparators(fileModel->filePath(ui->treeView->currentIndex()));
    if(path=="")
        QMessageBox::information(this, tr("Не обран матеріал"), tr("Вам потрібно вибрати матеріал для видалення"));
    else if (!QMessageBox::question(this, "Видалення", "Ви дійсно бажаєте видалити обраний матеріал?", "Так", "Ні"))
    {
        MaterialAndTest material;
        material.setDB(UsersPath);

        int check = material.deleteMaterial(path);
        if(check == 2)
            QMessageBox::information(this, "Константні дані", "Ви не можете видалити цей матеріал");
        else if(check == 0)
            QMessageBox::information(this, tr("Помилка"), tr("Не вдалося видалити файли"));
        else
            QMessageBox::information(this, tr("Успіх"), tr("Матеріали були успішно видалені"));
    }

    fileModel->setReadOnly(true);
}

void TutorForm::on_Connection_clicked()
{
    QString path = QDir::toNativeSeparators(fileModel->filePath(ui->treeView->currentIndex()));
    MaterialAndTest material;
    material.setDB(UsersPath);

    QString check = material.connectTheme(ConnectedLections, path, this);
    if(check != "\v")
        ConnectedLections = check;
    else if(check != "")
        showConnection(QDir::toNativeSeparators(QDir::currentPath()+"/Data/Матеріали/"+ConnectedLections));

}

void TutorForm::on_DeleteConnection_clicked()
{
    QString path = QDir::toNativeSeparators(fileModel->filePath(ui->treeView->currentIndex()));
    MaterialAndTest material;
    material.setDB(UsersPath);

    QString check = material.disconnectTheme(DisconnectedLections, path, this);
    if(check != "\v")
        DisconnectedLections = check;
    else if(check != "")
        showConnection(QDir::toNativeSeparators(QDir::currentPath()+"/Data/Матеріали/"+DisconnectedLections));
}


void TutorForm::showConnection(QString path)
{
    QString current = QDir::toNativeSeparators(QDir::currentPath() + "/Data/Матеріали/");
    int a = path.count(QDir::separator()) - current.count(QDir::separator());
    if(a==1)
    {
        QStringList pathList = path.split(QDir::separator());
        path = pathList.at(pathList.size()-2)+"/"+pathList.at(pathList.size()-1);

        QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);
        QSqlQuery connectionQuery(UsersDB);

        connectionQuery.prepare("SELECT connections FROM lectionsconnections WHERE lection='"+path+"'");
        connectionQuery.exec();
        connectionQuery.first();

        if(connectionQuery.isValid())
        {
            current = connectionQuery.value(0).toString();
            pathList = current.split("|");
            current = "";
            for(int i=0; i<pathList.size(); ++i, current+='\n')
                current+=pathList.at(i);
            ui->materialEdit->setText(current);
        }
        else
            ui->materialEdit->setText("");
    }
    else
        ui->materialEdit->setText("");
}

void TutorForm::on_AddTest_clicked()
{
    QString Path = fileModel->filePath(ui->treeView->currentIndex());
    QString current = QDir::currentPath() + "/Data/Матеріали/";

    if(Path == "" || Path == QDir::currentPath() + "/Data/Матеріали")
        QMessageBox::information(this, "Не обран матеріал", "Вам потрібно вибрати матеріал для додання тесту");
    else
    {
        QStringList neededPath = Path.split("/");
        int a = Path.count("/") - current.count("/");

        if(a == 0)
            Path=neededPath.at(neededPath.size()-1);
        else if(a==1)
            Path=neededPath.at(neededPath.size()-2)+"/"+neededPath.at(neededPath.size()-1);
        else if(a==2)
            Path=neededPath.at(neededPath.size()-3)+"/"+neededPath.at(neededPath.size()-2)+"/"+neededPath.at(neededPath.size()-1);



        testForm = new TestForm(this, UsersPath, Path, 0);

        testForm->setGeometry
        (
            QStyle::alignedRect
            (
                Qt::LeftToRight,
                Qt::AlignCenter,
                testForm->size(),
                qApp->desktop()->availableGeometry()
            )
        );
        testForm->setWindowIcon(QIcon(":/TestForm.ico"));
        testForm->setModal(true);
        testForm->setWindowState(Qt::WindowMaximized);
        testForm->setAttribute(Qt::WA_DeleteOnClose);


        testForm->exec();
        testForm = nullptr;
    }
}

void TutorForm::on_Results_clicked()
{
    QModelIndex index = ui->studentsList->currentIndex();
    QString Login = index.data(Qt::DisplayRole).toString();


    AuthorizationAndOutcome UsersOutcome(Login, "");
    QString outcome = UsersOutcome.getOutcome(Login);
    outcome.replace(".html", "");
    if(outcome=="\v")
        QMessageBox::information(this, "Не обран студент", "Вам необхідно обрати студента, результати якого Ви бажаєте побачити");
    else
        ui->resultsBrowser->setText(outcome);
}

bool TutorForm::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::EnterWhatsThisMode) {
        _handleWhatsThisEntry(QApplication::activeWindow());
        return true;
    }
    return QObject::eventFilter(object, event);
}

void TutorForm::_handleWhatsThisEntry(QWidget * /*sender*/) {

    //QProcess::execute("hh.exe C:/Users/User/Documents/ONPU Subjects/OOPCourseWork/CourseWorkFinal/Help.chm");
    QDesktopServices::openUrl(QUrl::fromLocalFile("Help.chm"));
}
