#ifndef TESTFORM_H
#define TESTFORM_H

#include <QDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QDebug>

#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QDesktopServices>

#include "testandscores.h"

namespace Ui {
class TestForm;
}

class TestForm : public QDialog
{
    Q_OBJECT

public:
    explicit TestForm(QWidget *parent = nullptr,
                      QString UsersPath="", QString Material="", int isStudent=0);
    ~TestForm();

protected:
    virtual bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_exit_clicked();

    void on_answer_1_textChanged();

    void on_answer_2_textChanged();

    void on_answer_3_textChanged();

    void on_answer_4_textChanged();

    void on_answer_5_textChanged();

    void on_answer_6_textChanged();

    void on_answer_7_textChanged();

    void on_answer_8_textChanged();

    void on_answer_9_textChanged();

    void on_answer_10_textChanged();


    void on_saveAll_clicked();

    void on_questionsChoice_currentIndexChanged(const QString &arg1);

    void on_addQuestion_clicked();

    void on_editQuestion_clicked();

    void on_editAnswers_clicked();

    void on_nextQuestion_clicked();

    void on_deleteQuestion_clicked();

private:
    QList<QTextEdit *> allTextEdit;
    QList<QCheckBox *> allCheckBoxes;

    const QString UsersPath;
    const QString Material;
    const int isStudent;
    int lastAnswer;

    Ui::TestForm *ui;

    bool indexDoesntChange = false;

    TestAndScores *StudentTest;
    QSqlQuery *test;

    QVector<int> QuestionsQuantity;

    void showHide(int i);
    void startForm(void);
    void endForm(void);
    void setChoosenQuestion(QString question);
    int getRand(int min, int max);
    void _handleWhatsThisEntry(QWidget * );
};

#endif // TESTFORM_H
