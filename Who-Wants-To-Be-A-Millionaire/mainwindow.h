#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml>
#include <QFile>
#include "question.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonStartGame_clicked();

    void on_pushButtonA_clicked();
    void on_pushButtonB_clicked();
    void on_pushButtonC_clicked();
    void on_pushButtonD_clicked();
    void on_pushButtonNextQuestion_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Question> easyQuestions;
    QVector<Question> mediumQuestions;
    QVector<Question> hardQuestions;
    QVector<bool> usedEasy, usedMedium, usedHard;
    void parseXMLDocument();
    void loadQuestion();
    void colorRightWhenWrongAnswer();
    void putBordersOnSums();
    void clearBordersOnSums();
    int numQuestionAsked;
    int indexCurrentQuestion;
    bool inEasyQuestions, inMediumQuestions, inHardQuestions;
    QString styleCorrect, styleIncorrect, styleClearButtons, styleSumsNormal, styleSumsSure, styleSumsClearNormal, styleSumsClearSure;
};
#endif // MAINWINDOW_H
