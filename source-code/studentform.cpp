#include "studentform.h"
#include "ui_studentform.h"

StudentForm::StudentForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentForm)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    startForm();
}

StudentForm::~StudentForm()
{

}

void StudentForm::on_exit_clicked()
{
    if(!QMessageBox::question(this, "Вихід", "Ви дійсно бажаєте вийти з системи?", "Так", "Ні"))
    {
        endForm();
        parentWidget()->show();

        this->close();
    }
}

void StudentForm::on_materialsList_doubleClicked(const QModelIndex &index)
{
    QString c = QDir::currentPath() + "/Data/Матеріали/";
    int a = CurrPath.count("/")-c.count("/");

    QStringList SplitList;

    QString path = ui->materialsList->currentIndex().data().toString();
    if(a == 0)
        findTheme(path);
    else if(a==1)
        findLections(path);
    else if(a==2)
    {
        QSqlDatabase Lections = QSqlDatabase::database(UsersPath);
        QSqlQuery lectionsQuery(Lections);

        if(path.count("|")==0)
        {
            OpenedLection = path;
            path = CurrPath+path+".html";
        }
        else
        {
            path.replace("| ","/");
            for(int i=0; i<PathToLections.size(); ++i)
                if(PathToLections[i].indexOf(path)!=-1)
                {
                    OpenedLection = PathToLections[i];
                    path = QDir::currentPath()+"/Data/Матеріали/"+PathToLections[i];
                    break;
                }
        }

        MaterialAndTest lection;
        QString text = lection.getMaterial(path);
        if(text!="\v")
            ui->materialBrowser->setHtml(text);
        else
            QMessageBox::information(this, "Помилка", "Помилка під час спроби відкрити файл");

        SplitList = path.split("/", QString::SkipEmptyParts);
        path=SplitList.at(SplitList.size()-3)+"/"+SplitList.at(SplitList.size()-2)+"/"+SplitList.at(SplitList.size()-1);

        QString lections_passed="";
        lectionsQuery.prepare("SELECT lections, scores FROM users WHERE id=:id");
        lectionsQuery.bindValue(":id", Student);
        lectionsQuery.exec();

        lectionsQuery.first();
        text = lectionsQuery.value(0).toString();
        SplitList = text.split("|", QString::SkipEmptyParts);

        QString scores = lectionsQuery.value(1).toString();
        if(SplitList.indexOf(path)==-1)
        {
            if(text=="")
            {
                lections_passed+="|"+path+"|";
                scores += "-1";
            }
            else
            {
                lections_passed+=text;
                lections_passed+=path+"|";
                scores += ",-1";
            }

            lectionsQuery.prepare("UPDATE users SET lections=:l, scores=:s "
                                  "WHERE id=:id");
            lectionsQuery.bindValue(":l", lections_passed);
            lectionsQuery.bindValue(":s", scores);
            lectionsQuery.bindValue(":id", Student);
            lectionsQuery.exec();
        }


        lectionsQuery.prepare("SELECT question FROM tests WHERE material='"+path+"'");
        lectionsQuery.exec();
        lectionsQuery.first();
        if(lectionsQuery.value(0).toString() != "")
            ui->test->show();
    }
}

void StudentForm::on_connectedLections_clicked()
{
    QSqlDatabase connectedLections = QSqlDatabase::database(UsersPath);
    QSqlQuery connectedQuery(connectedLections);

    QStringList lpath = QDir::toNativeSeparators(CurrPath).split(QDir::separator(), QString::SkipEmptyParts);

    QString npath="";

    QString current = (QDir::currentPath() + "/Data/Матеріали/");
    int a = CurrPath.count(current.at(current.size()-1)) - current.count(current.at(current.size()-1));

    if (a==2)
    {
        npath=lpath.at(lpath.size()-2)+"/"+lpath.at(lpath.size()-1);
        if(++ShowHideConnections%2)
        {
            PathToLections.clear();
            connectedQuery.prepare("SELECT connections FROM lectionsconnections WHERE lection = '"+npath+"'");
            connectedQuery.exec();
            connectedQuery.first();


            QString check_connections = connectedQuery.value(0).toString();
            if(check_connections!="")
            {
                QStringList SplitList, SplitConnection = check_connections.split("|", QString::SkipEmptyParts);
                for(int i = 0, s = SplitConnection.size(); i<s; ++i)
                {
                    QString path = QDir::toNativeSeparators(QDir::currentPath()+"/Data/Матеріали/"+SplitConnection.at(i));
                    QDirIterator it(path, QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);

                    while (it.hasNext())
                    {
                        it.next();
                        path = it.filePath();


                        SplitList = path.split("/", QString::SkipEmptyParts);

                        int s = SplitList.size();

                        QString current = SplitList.at(s-3)+"/"+SplitList.at(s-2)+"/"+SplitList.at(s-1);
                        PathToLections<<current;
                        current = SplitList.at(s-2)+"| "+SplitList.at(s-1);
                        current.replace(".html", "");

                        LectionsList<<current;
                    }
                }
                fileModel->setStringList(LectionsList);
                ui->materialsList->update();
            }
            else
                QMessageBox::information(this, "Немає зв'язків", "У цієї теми зараз відсутні зв'язки");
        }
        else
        {
            if(OpenedLection.count("/")!=0)
                ui->materialBrowser->clear();
            LectionsList.erase(LectionsList.end()-PathToLections.size(),
                               LectionsList.end());
            fileModel->setStringList(LectionsList);
            ui->materialsList->update();
        }
    }
    else
        QMessageBox::information(this, "Неправильне місцезнаходження", "Вам потрібно перейти до розділу з лекціями,\nщоб виконати цю операцію");
}

void StudentForm::on_backward_clicked()
{
    QString c = QDir::currentPath() + "/Data/Матеріали/";
    int a = PreviousPath.count("/")-c.count("/");

    ShowHideConnections = 0;
    if(a == 0)
    {
        LectionsList.clear();
        CurrPath = c;
        findSubjects(CurrPath);
        fileModel->setStringList(LectionsList);

        ui->materialsList->update();
        ui->materialBrowser->clear();
    }
    else if(a == 1)
    {
        QStringList newPath = PreviousPath.split("/", QString::SkipEmptyParts);
        CurrPath = c;

        findTheme(newPath.at(newPath.size()-1));

        ui->materialBrowser->clear();
    }
    else if (a == 2)
    {
        QStringList newPath = PreviousPath.split("/", QString::SkipEmptyParts);
        CurrPath = c+newPath.at(newPath.size()-2)+"/";

        findLections(newPath.at(newPath.size()-1));

        ui->materialBrowser->clear();
    }
    else
        QMessageBox::information(this, "Недоступна операція", "З цієї позиції ця операція недоступна");
}

void StudentForm::setUsersPath(QString Path)
{
    UsersPath = Path;
}

void StudentForm::setStudent(int student)
{
    Student = student;
}

void StudentForm::startForm(void)
{
    ui->test->hide();
    timer.start();

    CurrPath = QDir::currentPath() + "/Data/Матеріали";

    QDir dirpath(QDir::toNativeSeparators(CurrPath));
    if(!dirpath.exists())
        QDir().mkdir(dirpath.path());
    CurrPath+="/";

    fileModel = new QStringListModel(this);

    findSubjects(CurrPath);

    fileModel->setStringList(LectionsList);
    ui->materialsList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->materialsList->setModel(fileModel);
}

void StudentForm::endForm(void)
{
    long long time = timer.elapsed();
    time = time / 600;

    QSqlDatabase UsersDB = QSqlDatabase::database(UsersPath);
    QSqlQuery timeQuery(UsersDB);

    timeQuery.prepare("SELECT time FROM users WHERE id=:id");
    timeQuery.bindValue(":id", Student);
    timeQuery.exec();

    timeQuery.first();
    time = time + timeQuery.value(0).toInt();

    timeQuery.prepare("UPDATE users SET time=:t WHERE id=:id");
    timeQuery.bindValue(":t", time);
    timeQuery.bindValue(":id", Student);
    timeQuery.exec();

    delete fileModel;
    delete ui;
}

void StudentForm::findSubjects(QString path)
{
    ui->test->hide();

    PreviousPath = QDir::currentPath() + "/Data/Матеріали";
    QStringList SplitList;
    QDirIterator it((QDir::toNativeSeparators(path)),
                    QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);
    QString mPath;
    while (it.hasNext())
    {
        it.next();
        mPath = it.filePath();


        SplitList = mPath.split("/", QString::SkipEmptyParts);

        int s = SplitList.size();

        QString current = SplitList.at(s-3);

        if(LectionsList.indexOf(SplitList.at(s-3))==-1)
           LectionsList<<current;
    }
}

void StudentForm::findTheme(QString path)
{
    ui->test->hide();

    LectionsList.clear();
    PreviousPath = CurrPath;
    CurrPath+=path+"/";

    QStringList SplitList;

    QDirIterator it((QDir::toNativeSeparators(CurrPath)),
                    QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        path = it.filePath();

        SplitList = path.split("/", QString::SkipEmptyParts);
        int s = SplitList.size();

        QString current = SplitList.at(s-2);

        if(LectionsList.indexOf(SplitList.at(s-2))==-1)
           LectionsList<<current;
    }
    fileModel->setStringList(LectionsList);
    ui->materialsList->update();
}

void StudentForm::findLections(QString path)
{
    ui->test->hide();

    LectionsList.clear();
    PreviousPath = CurrPath;
    CurrPath+=path+"/";

    QStringList SplitList;

    QDirIterator it((QDir::toNativeSeparators(CurrPath)),
                    QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        path = it.filePath();


        SplitList = path.split("/", QString::SkipEmptyParts);

        int s = SplitList.size();

        QString current = SplitList.at(s-1);
        current.replace(".html", "");

        if(LectionsList.indexOf(SplitList.at(s-1))==-1)
           LectionsList<<current;
    }
    fileModel->setStringList(LectionsList);
    ui->materialsList->update();
}

void StudentForm::on_materialsList_clicked(const QModelIndex &index)
{
    TestAndScores checkTest(UsersPath);

    QString MaterialName=index.data().toString();
    QString path;
    if(MaterialName.count("|"))
    {
        MaterialName.replace("| ","/");
        path=PreviousPath+"/";
    }
    else
        path = CurrPath+"/";

    path += MaterialName;
    path.replace("//","/");

    QString c = QDir::currentPath() + "/Data/Матеріали/";
    int a = path.count("/")-c.count("/");

    QStringList pathParts = path.split("/", QString::SkipEmptyParts);
    if(a == 0)
        path = pathParts.at(pathParts.size()-1);
    else if(a == 1)
        path = pathParts.at(pathParts.size()-2)+"/"+pathParts.at(pathParts.size()-1);
    else if(a == 2)
        path = pathParts.at(pathParts.size()-3)+"/"+pathParts.at(pathParts.size()-2)+"/"+pathParts.at(pathParts.size()-1)+".html";

    MaterialForTest = path;
    int check = checkTest.checkTestAvaiability(path, Student);
    if(check)
        ui->test->show();
    else
        ui->test->hide();
}

void StudentForm::on_test_clicked()
{
    testForm = new TestForm(this, UsersPath, MaterialForTest, Student);

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
    testForm->setModal(false);
    testForm->setWindowState(Qt::WindowMaximized);
    testForm->setAttribute(Qt::WA_DeleteOnClose);


    testForm->exec();
    testForm = nullptr;
    ui->test->hide();
}

void StudentForm::on_getResults_clicked()
{
    QSqlDatabase Outcome = QSqlDatabase::database(UsersPath);
    QSqlQuery outcomeQuery(Outcome);
    outcomeQuery.prepare("SELECT login FROM users WHERE id='"+QString::number(Student)+"'");
    outcomeQuery.exec();
    outcomeQuery.first();

    AuthorizationAndOutcome UsersOutcome(outcomeQuery.value(0).toString(), "");
    QString outcome = UsersOutcome.getOutcome(outcomeQuery.value(0).toString());
    outcome.replace(".html", "");

    QStringList outcomeList = outcome.split("\n\n");

    if(outcomeList.at(0) == "Цей студент ще не пройшов жоден тест")
        ui->materialBrowser->setText("Ви ще не пройшли жоден тест");
    else
        ui->materialBrowser->setText(outcomeList.at(0));
}

bool StudentForm::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::EnterWhatsThisMode) {
        _handleWhatsThisEntry(QApplication::activeWindow());
        return true;
    }
    return QObject::eventFilter(object, event);
}

void StudentForm::_handleWhatsThisEntry(QWidget * /*sender*/) {

    //QProcess::execute("hh.exe C:/Users/User/Documents/ONPU Subjects/OOPCourseWork/CourseWorkFinal/Help.chm");
    QDesktopServices::openUrl(QUrl::fromLocalFile("Help.chm"));
}
