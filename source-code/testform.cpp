#include "testform.h"
#include "ui_testform.h"
#include <random>

TestForm::TestForm(QWidget *parent, QString UsersPath, QString Material, int isStudent) :
    QDialog(parent), UsersPath(UsersPath), Material(Material), isStudent(isStudent),
    ui(new Ui::TestForm)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    allCheckBoxes = this->findChildren<QCheckBox *>();
    allTextEdit = this->findChildren<QTextEdit *>();

    startForm();
    if(isStudent == 0)
        ui->questionsChoice->repaint();
    else
    {
        ui->questionValue->hide();
        ui->questionsChoice->hide();
        ui->addQuestion->hide();
        ui->deleteQuestion->hide();
        ui->editQuestion->hide();
        ui->editAnswers->hide();
        ui->saveAll->hide();
    }
}

TestForm::~TestForm()
{

}

void TestForm::on_exit_clicked()
{
    if(!QMessageBox::question(this, "Вихід", "Ви дійсно бажаєте вийти з редактору тестів?", "Так", "Ні"))
    {
        QuestionsQuantity.resize(0);
        on_nextQuestion_clicked();

        delete StudentTest;
        delete test;
        delete ui;

        this->close();
    }
}


void TestForm::on_answer_1_textChanged()
{
    int i = 1;
    showHide(i);
}

void TestForm::on_answer_2_textChanged()
{
    int i = 2;
    showHide(i);
}

void TestForm::on_answer_3_textChanged()
{
    int i = 3;
    showHide(i);
}

void TestForm::on_answer_4_textChanged()
{
    int i = 4;
    showHide(i);
}

void TestForm::on_answer_5_textChanged()
{
    int i = 5;
    showHide(i);
}

void TestForm::on_answer_6_textChanged()
{
    int i = 6;
    showHide(i);
}

void TestForm::on_answer_7_textChanged()
{
    int i = 7;
    showHide(i);
}

void TestForm::on_answer_8_textChanged()
{
    int i = 8;
    showHide(i);
}

void TestForm::on_answer_9_textChanged()
{
    int i = 9;
    showHide(i);
}

void TestForm::on_answer_10_textChanged()
{
    int i = 10;

    QString text = allTextEdit.at(i)->toPlainText().simplified();
    text = text.trimmed();

    if(text!="" && lastAnswer < 10)
    {
        ++lastAnswer;
        allCheckBoxes.at(lastAnswer-1)->show();
        allTextEdit.at(lastAnswer)->show();
    }
    else if(lastAnswer > i &&
            allTextEdit.at(lastAnswer-1)->toPlainText().simplified().trimmed() == "")
    {
        allCheckBoxes.at(lastAnswer-1)->hide();
        allTextEdit.at(lastAnswer)->hide();
        --lastAnswer;
    }
}

void TestForm::showHide(int i)
{
    if(isStudent==0)
    {
        QString text = allTextEdit.at(i)->toPlainText().simplified().trimmed();

        if(text!="" && i == lastAnswer && lastAnswer+1 < allTextEdit.size() && allTextEdit.at(lastAnswer+1)->isHidden())
        {
            ++lastAnswer;
            allCheckBoxes.at(lastAnswer-1)->show();
            allTextEdit.at(lastAnswer)->show();
        }
        else if(lastAnswer > i && lastAnswer-1 >= 0 &&
                allTextEdit.at(lastAnswer-1)->toPlainText().simplified().trimmed() == "")
        {
            allCheckBoxes.at(lastAnswer-1)->hide();
            allTextEdit.at(lastAnswer)->hide();
            --lastAnswer;
        }
    }
}

void TestForm::startForm(void)
{
    TestAndScores t(UsersPath);

    if(isStudent == 0)
    {
        lastAnswer = 1;
        QSqlDatabase questions =  QSqlDatabase::database(UsersPath);
        QSqlQuery questionsQuery(questions);

        questionsQuery.prepare("SELECT question FROM tests WHERE material='"+Material+"'");
        questionsQuery.exec();
        while(questionsQuery.next())
            ui->questionsChoice->addItem(questionsQuery.value(0).toString());


        for (QCheckBox *checkBox: allCheckBoxes)
              checkBox->hide();

        for (QTextEdit *textEdit: allTextEdit)
              textEdit->hide();

        allTextEdit.at(lastAnswer-1)->show();

        allCheckBoxes.at(lastAnswer-1)->show();
        allTextEdit.at(lastAnswer)->show();
    }
    else
    {
        for (QCheckBox *checkBox: allCheckBoxes)
              checkBox->hide();

        for (QTextEdit *textEdit: allTextEdit)
        {
              textEdit->hide();
              textEdit->setReadOnly(true);
        }
        StudentTest=new TestAndScores(UsersPath);
        test = StudentTest->getTest(Material);

        while(test->next())
            StudentTest->falseAnswerScore+=test->value(4).toInt();

        test->last();
        int c = test->value(0).toInt()+1;
        for(int i = 0; i<c; ++i)
            QuestionsQuantity.push_back(i);

        lastAnswer = 0;

        c = getRand(0, QuestionsQuantity.size()-1);

        lastAnswer = QuestionsQuantity.at(c);
        QuestionsQuantity.erase(QuestionsQuantity.begin() + c);
        c = lastAnswer;

        test->first();
        int i = -1;
        while(++i<c)
            test->next();


        QString question = test->value(1).toString();

        allTextEdit.at(0)->setText(question);
        allTextEdit.at(0)->show();

        QStringList allAnswers;
        allAnswers = test->value(5).toString().split("\v", QString::SkipEmptyParts);

        i=0;
        while(allAnswers.size()>0)
        {
            c = getRand(0, allAnswers.size()-1);

            allCheckBoxes.at(i)->setChecked(false);
            allCheckBoxes.at(i)->show();

            ++i;
            allTextEdit.at(i)->setPlainText(allAnswers.at(c));
            allTextEdit.at(i)->show();

            allAnswers.erase(allAnswers.begin()+c);
        }
    }
}

void TestForm::endForm(void)
{
    while(ui->questionsChoice->count()>1)
        ui->questionsChoice->removeItem(ui->questionsChoice->count()-1);


    for (QCheckBox *checkBox: allCheckBoxes)
          checkBox->setChecked(false);

    for (QTextEdit *textEdit: allTextEdit)
          textEdit->clear();

    ui->questionValue->setValue(1);
}

void TestForm::on_saveAll_clicked()
{
    QString question = allTextEdit.at(0)->toPlainText();
    QStringList rightAnswers, falseAnswers, allAnswers;

    if(allTextEdit.at(0)->toPlainText().simplified().trimmed() == "")
        QMessageBox::information(this, "Немає питання", "Вам потрібно заповнити поле \"Питання\"");
    else
    {
        for (int i = 1; i<allTextEdit.size(); ++i)
            if(!(allTextEdit.at(i)->isHidden()) &&
                 allTextEdit.at(i)->toPlainText().simplified().trimmed() != "")
            {
                if(allCheckBoxes.at(i-1)->checkState())
                    rightAnswers<<allTextEdit.at(i)->toPlainText();
                else
                    falseAnswers<<allTextEdit.at(i)->toPlainText();

                allAnswers<<allTextEdit.at(i)->toPlainText();
            }


        rightAnswers.removeDuplicates();
        QStringList l;
        for(int i = 0; i<rightAnswers.size(); ++i)
            l << rightAnswers.at(i).toUpper();

        for(int i = 0; i<l.size(); ++i)
        {
            QString duplicate = l.at(i);
            for(int j = i+1; j<l.size(); ++j)
                if(l.at(j)==duplicate)
                {
                    rightAnswers.erase(rightAnswers.begin()+j);
                    l.erase(l.begin()+j);
                    --j;
                }
        }
        l.clear();

        falseAnswers.removeDuplicates();
        for(int i = 0; i<falseAnswers.size(); ++i)
            l << falseAnswers.at(i).toUpper();

        for(int i = 0; i<l.size(); ++i)
        {
            QString duplicate = l.at(i);
            for(int j = i+1; j<l.size(); ++j)
                if(l.at(j)==duplicate)
                {
                    falseAnswers.erase(falseAnswers.begin()+j);
                    l.erase(l.begin()+j);
                    --j;
                }
        }
        l.clear();

        for(int i = 0; i<allAnswers.size(); ++i)
            if(!(rightAnswers.contains(allAnswers.at(i))) && !(falseAnswers.contains(allAnswers.at(i))))
            {
                allAnswers.erase(allAnswers.begin()+i);
                --i;
            }

        if(allAnswers.size() < 2)
            QMessageBox::information(this, "Недостатньо відповідей", "Вам необхідно вказати хочаб 2 відповіді, що не дублюються");
        else
        {
            if(!rightAnswers.size())
                QMessageBox::information(this, "Немає відповіді", "Вам необхідно вказати хочаб 1 правильну відповідь");
            else
            {
                int value = 1;
                for(int i = 0; i<rightAnswers.size() && value; ++i)
                {
                    QString r = rightAnswers.at(i);
                    r = r.toUpper();
                    for(int j = 0; j<falseAnswers.size(); ++j)
                    {
                        QString f = falseAnswers.at(j);
                        f = f.toUpper();

                        if(r == f)
                        {
                            value = 0;
                            break;
                        }
                    }
                }


                if(!value)
                    QMessageBox::information(this, "Дублювання", "Правильні та неправильні відповіді дублюються");
                else
                {
                    int value = ui->questionValue->value();

                    TestAndScores test(UsersPath);
                    int check = -2;

                    if(ui->questionsChoice->currentIndex() == 0)
                        check = test.addQuestion(Material, question, rightAnswers, falseAnswers, value, allAnswers);

                    if(check == 0)
                        QMessageBox::information(this, "Помилка", "Під час редагування тесту виникла невизначена помилка");
                    else if(check == 1)
                    {
                        QMessageBox::information(this, "Успіх", "Питання було успішно добавлене");
                        indexDoesntChange = true;
                        endForm();
                        startForm();
                        indexDoesntChange = false;
                    }
                    else if(check == -2)
                    {
                        QString oldQuestion = ui->questionsChoice->currentText();
                        check = test.changeQuestion(oldQuestion, Material, question, rightAnswers, falseAnswers, value, allAnswers);
                        if(check == 0)
                            QMessageBox::information(this, "Помилка", "Під час редагування тесту виникла невизначена помилка");
                        else if(check == 2)
                            QMessageBox::information(this, "Наявне питання", "Ви намагаєтесь змінити питання на таке, яке вже є в тесті");
                        else if(check == 1)
                        {
                            QMessageBox::information(this, "Успіх", "Тест був успішно відредагован");
                            indexDoesntChange = true;
                            endForm();
                            startForm();
                            indexDoesntChange = false;
                        }
                    }
                    else
                        QMessageBox::information(this, "Наявне питання", "Таке питання вже є в цьому тесті.\nЯкщо ви бажаєте редагувати питання, виберіть його в полі зі списком");
                }
            }
        }
    }
}

void TestForm::on_questionsChoice_currentIndexChanged(const QString &arg1)
{
    if(ui->questionsChoice->currentIndex() != 0)
        setChoosenQuestion(arg1);
}

void TestForm::setChoosenQuestion(QString question)
{
    if(!indexDoesntChange)
    {
        QSqlDatabase testsData =  QSqlDatabase::database(UsersPath);
        QSqlQuery testsDataQuery(testsData);

        testsDataQuery.prepare("SELECT * FROM tests WHERE material='"+Material+"' and question='"+question+"'");
        testsDataQuery.exec();
        testsDataQuery.first();

        QStringList listRightAnswers = testsDataQuery.value(3).toString().split("\v");
        QStringList listFalseAnswers = testsDataQuery.value(4).toString().split("\v");
        QStringList listAllAnswerts = testsDataQuery.value(6).toString().split("\v");
        int value = testsDataQuery.value(5).toInt();


        for (QCheckBox *checkBox: allCheckBoxes)
        {
              checkBox->hide();
              checkBox->setChecked(false);
        }

        for (QTextEdit *textEdit: allTextEdit)
        {
              textEdit->hide();
              textEdit->clear();
        }


        ui->questionEdit->setText(question);
        ui->questionEdit->show();

        ui->questionValue->setValue(value);
        value = 1;

        for(; value<listAllAnswerts.size(); ++value)
        {
            allTextEdit.at(value)->setText(listAllAnswerts.at(value));
            if(listRightAnswers.contains(listAllAnswerts.at(value)))
                allCheckBoxes.at(value - 1)->setChecked(true);

            allTextEdit.at(value)->show();
            allCheckBoxes.at(value - 1)->show();
        }

        allCheckBoxes.at(value - 2)->setChecked(false);
    }
}

void TestForm::on_addQuestion_clicked()
{
    QString question = allTextEdit.at(0)->toPlainText();
    QStringList rightAnswers, falseAnswers, allAnswers;

    if(allTextEdit.at(0)->toPlainText().simplified().trimmed() == "")
        QMessageBox::information(this, "Немає питання", "Вам потрібно заповнити поле \"Питання\"");
    else
    {
        for (int i = 1; i<allTextEdit.size(); ++i)
            if(!(allTextEdit.at(i)->isHidden()) &&
                 allTextEdit.at(i)->toPlainText().simplified().trimmed() != "")
            {
                if(allCheckBoxes.at(i-1)->checkState())
                    rightAnswers<<allTextEdit.at(i)->toPlainText();
                else
                    falseAnswers<<allTextEdit.at(i)->toPlainText();

                allAnswers<<allTextEdit.at(i)->toPlainText();
            }


        rightAnswers.removeDuplicates();
        QStringList l;
        for(int i = 0; i<rightAnswers.size(); ++i)
            l << rightAnswers.at(i).toUpper();

        for(int i = 0; i<l.size(); ++i)
        {
            QString duplicate = l.at(i);
            for(int j = i+1; j<l.size(); ++j)
                if(l.at(j)==duplicate)
                {
                    rightAnswers.erase(rightAnswers.begin()+j);
                    l.erase(l.begin()+j);
                    --j;
                }
        }
        l.clear();

        falseAnswers.removeDuplicates();
        for(int i = 0; i<falseAnswers.size(); ++i)
            l << falseAnswers.at(i).toUpper();

        for(int i = 0; i<l.size(); ++i)
        {
            QString duplicate = l.at(i);
            for(int j = i+1; j<l.size(); ++j)
                if(l.at(j)==duplicate)
                {
                    falseAnswers.erase(falseAnswers.begin()+j);
                    l.erase(l.begin()+j);
                    --j;
                }
        }
        l.clear();

        for(int i = 0; i<allAnswers.size(); ++i)
            if(!(rightAnswers.contains(allAnswers.at(i))) && !(falseAnswers.contains(allAnswers.at(i))))
            {
                allAnswers.erase(allAnswers.begin()+i);
                --i;
            }

        if(allAnswers.size() < 2)
            QMessageBox::information(this, "Недостатньо відповідей", "Вам необхідно вказати хочаб 2 відповіді, що не дублюються");
        else
        {
            if(!rightAnswers.size())
                QMessageBox::information(this, "Немає відповіді", "Вам необхідно вказати хочаб 1 правильну відповідь");
            else
            {
                int value = 1;
                for(int i = 0; i<rightAnswers.size() && value; ++i)
                {
                    QString r = rightAnswers.at(i);
                    r = r.toUpper();
                    for(int j = 0; j<falseAnswers.size(); ++j)
                    {
                        QString f = falseAnswers.at(j);
                        f = f.toUpper();

                        if(r == f)
                        {
                            value = 0;
                            break;
                        }
                    }
                }


                if(!value)
                    QMessageBox::information(this, "Дублювання", "Правильні та неправильні відповіді дублюються");
                else
                {
                    int value = ui->questionValue->value();

                    TestAndScores test(UsersPath);
                    int check = test.addQuestion(Material, question, rightAnswers, falseAnswers, value, allAnswers);

                    if(check == 0)
                        QMessageBox::information(this, "Помилка", "Під час редагування тесту виникла невизначена помилка");
                    else if(check == 1)
                    {
                        QMessageBox::information(this, "Успіх", "Питання було успішно добавлене");
                        indexDoesntChange = true;
                        endForm();
                        startForm();
                        indexDoesntChange = false;
                    }
                    else
                        QMessageBox::information(this, "Наявне питання", "Таке питання вже є в цьому тесті.\nЯкщо ви бажаєте редагувати питання, виберіть його в полі зі списком");
                }
            }
        }
    }
}

void TestForm::on_editQuestion_clicked()
{
    if(ui->questionsChoice->currentIndex()!=0)
    {
        QString oldQuestion = ui->questionsChoice->currentText();
        QString question = allTextEdit.at(0)->toPlainText();
        int value = ui->questionValue->value();

        if(allTextEdit.at(0)->toPlainText().simplified().trimmed() == "")
            QMessageBox::information(this, "Немає питання", "Вам потрібно заповнити поле \"Питання\"");
        else
        {
            TestAndScores test(UsersPath);
            int check = test.changeOnlyQuestion(oldQuestion, Material, question, value);
            if(check == 2)
                QMessageBox::information(this, "Наявне питання", "Ви намагаєтесь змінити питання на таке, яке вже є в тесті");
            else if(check == 0)
                QMessageBox::information(this, "Помилка", "Під час редагування питання виникла невизначена помилка");
            else if(check == 1)
            {
                QMessageBox::information(this, "Успіх", "Питання було успішно відредаговане");
                endForm();
                startForm();
            }
        }
    }
    else
        QMessageBox::information(this, "Не обрано питання", "Вам необхідно вказати питання в полі зі списком задля його редагування");
}

void TestForm::on_editAnswers_clicked()
{
    QString question = allTextEdit.at(0)->toPlainText();
    QStringList rightAnswers, falseAnswers, allAnswers;

    if(allTextEdit.at(0)->toPlainText().simplified().trimmed() == "")
        QMessageBox::information(this, "Немає питання", "Вам потрібно заповнити поле \"Питання\"");
    else
    {
        for (int i = 1; i<allTextEdit.size(); ++i)
            if(!(allTextEdit.at(i)->isHidden()) &&
                 allTextEdit.at(i)->toPlainText().simplified().trimmed() != "")
            {
                if(allCheckBoxes.at(i-1)->checkState())
                    rightAnswers<<allTextEdit.at(i)->toPlainText();
                else
                    falseAnswers<<allTextEdit.at(i)->toPlainText();

                allAnswers<<allTextEdit.at(i)->toPlainText();
            }


        rightAnswers.removeDuplicates();
        QStringList l;
        for(int i = 0; i<rightAnswers.size(); ++i)
            l << rightAnswers.at(i).toUpper();

        for(int i = 0; i<l.size(); ++i)
        {
            QString duplicate = l.at(i);
            for(int j = i+1; j<l.size(); ++j)
                if(l.at(j)==duplicate)
                {
                    rightAnswers.erase(rightAnswers.begin()+j);
                    l.erase(l.begin()+j);
                    --j;
                }
        }
        l.clear();

        falseAnswers.removeDuplicates();
        for(int i = 0; i<falseAnswers.size(); ++i)
            l << falseAnswers.at(i).toUpper();

        for(int i = 0; i<l.size(); ++i)
        {
            QString duplicate = l.at(i);
            for(int j = i+1; j<l.size(); ++j)
                if(l.at(j)==duplicate)
                {
                    falseAnswers.erase(falseAnswers.begin()+j);
                    l.erase(l.begin()+j);
                    --j;
                }
        }
        l.clear();

        for(int i = 0; i<allAnswers.size(); ++i)
            if(!(rightAnswers.contains(allAnswers.at(i))) && !(falseAnswers.contains(allAnswers.at(i))))
            {
                allAnswers.erase(allAnswers.begin()+i);
                --i;
            }

        if(allAnswers.size() < 2)
            QMessageBox::information(this, "Недостатньо відповідей", "Вам необхідно вказати хочаб 2 відповіді, що не дублюються");
        else
        {
            if(!rightAnswers.size())
                QMessageBox::information(this, "Немає відповіді", "Вам необхідно вказати хочаб 1 правильну відповідь");
            else
            {
                int value = 1;
                for(int i = 0; i<rightAnswers.size() && value; ++i)
                {
                    QString r = rightAnswers.at(i);
                    r = r.toUpper();
                    for(int j = 0; j<falseAnswers.size(); ++j)
                    {
                        QString f = falseAnswers.at(j);
                        f = f.toUpper();

                        if(r == f)
                        {
                            value = 0;
                            break;
                        }
                    }
                }


                if(!value)
                    QMessageBox::information(this, "Дублювання", "Правильні та неправильні відповіді дублюються");
                else
                {
                    TestAndScores test(UsersPath);
                    QString oldQuestion = ui->questionsChoice->currentText();

                    int check = test.changeOnlyAnswers(Material, oldQuestion, rightAnswers, falseAnswers, allAnswers);
                    if(check == 0)
                        QMessageBox::information(this, "Помилка", "Під час редагування відповідей виникла невизначена помилка");
                    else if(check == 2)
                        QMessageBox::information(this, "Наявне питання", "Ви намагаєтесь змінити питання на таке, яке вже є в тесті");
                    else if(check == 1)
                    {
                        QMessageBox::information(this, "Успіх", "Тест був успішно відредагован");
                        indexDoesntChange = true;
                        endForm();
                        startForm();
                        indexDoesntChange = false;
                    }
                }
            }
        }
    }
}

void TestForm::on_nextQuestion_clicked()
{
    if(isStudent == 0)
    {
        int i = ui->questionsChoice->currentIndex();
        int c = ui->questionsChoice->count();
        i = (i+1)%c;
        QString next = ui->questionsChoice->itemText(i);
        ui->questionsChoice->setCurrentText(next);
    }
    else
    {
        QStringList rightAnswers = test->value(2).toString().split("\v", QString::SkipEmptyParts);
        QStringList allAnswers = test->value(5).toString().split("\v", QString::SkipEmptyParts);

        int answerValue = test->value(4).toInt();
        int score=-1;
        QString answer;
        for(int i = 0; i<allAnswers.size(); ++i)
        {
            answer = allTextEdit.at(i+1)->toPlainText();
            if((allCheckBoxes.at(i)->isChecked() && !(rightAnswers.contains(answer))) ||
                    (rightAnswers.contains(answer) && !(allCheckBoxes.at(i)->isChecked())))
            {
                score = 0;
                break;
            }
        }
        if(score==-1)
            score=answerValue;

        StudentTest->rightAnswerScore+=score;

        if(QuestionsQuantity.size())
        {
            int c = getRand(0, QuestionsQuantity.size()-1);
            lastAnswer = QuestionsQuantity.at(c);
            QuestionsQuantity.erase(QuestionsQuantity.begin() + c);
            c = lastAnswer;

            test->first();
            int i = -1;
            while(++i<c)
                test->next();


            for (QCheckBox *checkBox: allCheckBoxes)
                  checkBox->hide();

            for (QTextEdit *textEdit: allTextEdit)
                  textEdit->hide();


            QString question = test->value(1).toString();
            allTextEdit.at(0)->setText(question);
            allTextEdit.at(0)->show();


            allAnswers = test->value(5).toString().split("\v", QString::SkipEmptyParts);


            i=0;
            while(allAnswers.size()>0)
            {
                c = getRand(0, allAnswers.size()-1);

                allCheckBoxes.at(i)->setChecked(false);
                allCheckBoxes.at(i)->show();

                ++i;
                allTextEdit.at(i)->setPlainText(allAnswers.at(c));
                allTextEdit.at(i)->show();

                allAnswers.erase(allAnswers.begin()+c);
            }
        }
        else
        {
            QString scores = QString::number(StudentTest->rightAnswerScore)+"/"+QString::number(StudentTest->falseAnswerScore);
            QMessageBox::information(this, "Оцінка", "Ваша оцінка: "+scores);

            StudentTest->setScore(Material, isStudent, scores);
            this->close();
        }
    }
}

void TestForm::on_deleteQuestion_clicked()
{
    if(ui->questionsChoice->currentIndex()==0)
        QMessageBox::information(this, "Константна форма", "Ви не можете видалити цей запис");
    else
    {
        QString question = ui->questionsChoice->currentText();
        TestAndScores deleteTest(UsersPath);
        int check = deleteTest.deleteQuestion(Material, question);
        if(!check)
            QMessageBox::information(this, "Помилка", "Під час видалення питання виникла невизначена помилка");
        else
            QMessageBox::information(this, "Успіх", "Питання було успішно видалено");

        endForm();
        startForm();
    }
}

int TestForm::getRand(int min, int max){
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    std::mt19937 gen(ms);
    std::uniform_int_distribution<> uid(min, max);
    return uid(gen);
}

bool TestForm::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::EnterWhatsThisMode) {
        _handleWhatsThisEntry(QApplication::activeWindow());
        return true;
    }
    return QObject::eventFilter(object, event);
}

void TestForm::_handleWhatsThisEntry(QWidget * /*sender*/) {

    //QProcess::execute("hh.exe C:/Users/User/Documents/ONPU Subjects/OOPCourseWork/CourseWorkFinal/Help.chm");
    QDesktopServices::openUrl(QUrl::fromLocalFile("Help.chm"));
}
