#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    parseXMLDocument();
    numQuestionAsked = 0;
    inEasyQuestions = false;
    inMediumQuestions = false;
    inHardQuestions = false;
    indexCurrentQuestion = -1;

    ui->pushButtonA->setEnabled(false);
    ui->pushButtonB->setEnabled(false);
    ui->pushButtonC->setEnabled(false);
    ui->pushButtonD->setEnabled(false);

    styleCorrect = "font: 10pt \"MS Shell Dlg 2\"; color: white; background-color: green;";
    styleIncorrect = "font: 10pt \"MS Shell Dlg 2\"; color: white; background-color: red;";
    styleClearButtons = "font: 10pt \"MS Shell Dlg 2\"; color: white;";
    styleSumsNormal = "font: 12pt \"MS Shell Dlg 2\"; color: white; border: 2px solid yellow;";
    styleSumsSure = "font: 12pt \"MS Shell Dlg 2\"; color: yellow; border: 2px solid yellow;";
    styleSumsClearNormal = "font: 12pt \"MS Shell Dlg 2\"; color: white;";
    styleSumsClearSure = "font: 12pt \"MS Shell Dlg 2\"; color: yellow;";

    ui->pushButtonNextQuestion->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseXMLDocument()
{
    QDomDocument xmlBOM;
    QFile xmlFile(QDir::currentPath() + "/XML_folder/questionsDB.xml");
    if (!xmlFile.open(QIODevice::ReadOnly ))
    {
        QMessageBox::critical(this, "Database file", "Error while loading database file.");
        return;
    }
    xmlBOM.setContent(&xmlFile);
    xmlFile.close();

    QDomElement root=xmlBOM.documentElement();

    QDomElement questionsGroup=root.firstChildElement();

    while (!questionsGroup.isNull()){
        QDomElement question = questionsGroup.firstChildElement();
        QVector<QDomElement> answers;
        while(!question.isNull()){
            QString textQuestion = question.firstChild().toElement().text();
            QDomElement answerA = question.childNodes().at(1).toElement();
            QDomElement answerB = question.childNodes().at(2).toElement();
            QDomElement answerC = question.childNodes().at(3).toElement();
            QDomElement answerD = question.childNodes().at(4).toElement();
            answers.push_back(answerA);
            answers.push_back(answerB);
            answers.push_back(answerC);
            answers.push_back(answerD);

            if (questionsGroup.tagName() == "easyQuestions"){
                easyQuestions.push_back(Question(textQuestion, answers));
            } else if (questionsGroup.tagName() == "mediumQuestions"){
                mediumQuestions.push_back(Question(textQuestion, answers));
            } else {
                hardQuestions.push_back(Question(textQuestion, answers));
            }
            answers.clear();
            question = question.nextSiblingElement();
        }
        questionsGroup = questionsGroup.nextSiblingElement();
    }
}

void MainWindow::loadQuestion()
{
    ui->pushButtonA->setStyleSheet(styleClearButtons);
    ui->pushButtonB->setStyleSheet(styleClearButtons);
    ui->pushButtonC->setStyleSheet(styleClearButtons);
    ui->pushButtonD->setStyleSheet(styleClearButtons);

    if (numQuestionAsked <= 5) { //easy
        inEasyQuestions = true;
        inMediumQuestions = false;
        inHardQuestions = false;
        int index;
        do {
            index =  QRandomGenerator::global()->bounded(0, easyQuestions.size());
        } while(usedEasy[index]);
        indexCurrentQuestion = index;

        ui->labelQuestionText->setText(easyQuestions[index].getText());
        ui->pushButtonA->setText(easyQuestions[index].getAnswers()[0].getText());
        ui->pushButtonB->setText(easyQuestions[index].getAnswers()[1].getText());
        ui->pushButtonC->setText(easyQuestions[index].getAnswers()[2].getText());
        ui->pushButtonD->setText(easyQuestions[index].getAnswers()[3].getText());
        usedEasy[index] = true;
        putBordersOnSums();
        numQuestionAsked++;

    } else if (numQuestionAsked <= 10) { //medium

        inEasyQuestions = false;
        inMediumQuestions = true;
        inHardQuestions = false;
        int index;
        do {
            index =  QRandomGenerator::global()->bounded(0, mediumQuestions.size());
        } while(usedMedium[index]);
        indexCurrentQuestion = index;

        ui->labelQuestionText->setText(mediumQuestions[index].getText());
        ui->pushButtonA->setText(mediumQuestions[index].getAnswers()[0].getText());
        ui->pushButtonB->setText(mediumQuestions[index].getAnswers()[1].getText());
        ui->pushButtonC->setText(mediumQuestions[index].getAnswers()[2].getText());
        ui->pushButtonD->setText(mediumQuestions[index].getAnswers()[3].getText());
        usedMedium[index] = true;
        putBordersOnSums();
        numQuestionAsked++;

    } else { //hard
        inEasyQuestions = false;
        inMediumQuestions = false;
        inHardQuestions = true;
        int index;
        do {
            index =  QRandomGenerator::global()->bounded(0, hardQuestions.size());
        } while(usedHard[index]);
        indexCurrentQuestion = index;

        ui->labelQuestionText->setText(hardQuestions[index].getText());
        ui->pushButtonA->setText(hardQuestions[index].getAnswers()[0].getText());
        ui->pushButtonB->setText(hardQuestions[index].getAnswers()[1].getText());
        ui->pushButtonC->setText(hardQuestions[index].getAnswers()[2].getText());
        ui->pushButtonD->setText(hardQuestions[index].getAnswers()[3].getText());
        usedHard[index] = true;
        putBordersOnSums();
        numQuestionAsked++;

    }
    ui->pushButtonNextQuestion->setEnabled(false);
}

void MainWindow::colorRightWhenWrongAnswer()
{
    int indexRight;
    if (inEasyQuestions){
        indexRight = easyQuestions[indexCurrentQuestion].getCorrect();
    } else if (inMediumQuestions){
        indexRight = mediumQuestions[indexCurrentQuestion].getCorrect();
    } else {
        indexRight = hardQuestions[indexCurrentQuestion].getCorrect();
    }
    switch (indexRight) {
        case 0: ui->pushButtonA->setStyleSheet(styleCorrect); break;
        case 1: ui->pushButtonB->setStyleSheet(styleCorrect); break;
        case 2: ui->pushButtonC->setStyleSheet(styleCorrect); break;
        case 3: ui->pushButtonD->setStyleSheet(styleCorrect); break;
    }
}

void MainWindow::putBordersOnSums()
{
    switch (numQuestionAsked) {
        case 1: ui->labelSum100->setStyleSheet(styleSumsNormal); break;
    case 2: ui->labelSum200->setStyleSheet(styleSumsNormal); ui->labelSum100->setStyleSheet(styleSumsClearNormal); break;
    case 3: ui->labelSum300->setStyleSheet(styleSumsNormal); ui->labelSum200->setStyleSheet(styleSumsClearNormal); break;
    case 4: ui->labelSum400->setStyleSheet(styleSumsNormal); ui->labelSum300->setStyleSheet(styleSumsClearNormal); break;
    case 5: ui->labelSum500->setStyleSheet(styleSumsSure); ui->labelSum400->setStyleSheet(styleSumsClearNormal); break;
    case 6: ui->labelSum1000->setStyleSheet(styleSumsNormal); ui->labelSum500->setStyleSheet(styleSumsClearSure); break;
    case 7: ui->labelSum1500->setStyleSheet(styleSumsNormal); ui->labelSum1000->setStyleSheet(styleSumsClearNormal); break;
    case 8: ui->labelSum2000->setStyleSheet(styleSumsNormal); ui->labelSum1500->setStyleSheet(styleSumsClearNormal); break;
    case 9: ui->labelSum2500->setStyleSheet(styleSumsNormal); ui->labelSum2000->setStyleSheet(styleSumsClearNormal); break;
    case 10: ui->labelSum5000->setStyleSheet(styleSumsSure);  ui->labelSum2500->setStyleSheet(styleSumsClearNormal);break;
    case 11: ui->labelSum10000->setStyleSheet(styleSumsNormal); ui->labelSum5000->setStyleSheet(styleSumsClearSure); break;
    case 12: ui->labelSum20000->setStyleSheet(styleSumsNormal); ui->labelSum10000->setStyleSheet(styleSumsClearNormal); break;
    case 13: ui->labelSum50000->setStyleSheet(styleSumsNormal);  ui->labelSum20000->setStyleSheet(styleSumsClearNormal); break;
    case 14: ui->labelSum100000->setStyleSheet(styleSumsNormal); ui->labelSum50000->setStyleSheet(styleSumsClearNormal); break;
    case 15: ui->labelSum200000->setStyleSheet(styleSumsSure); ui->labelSum100000->setStyleSheet(styleSumsClearNormal); break;
    }
}

void MainWindow::clearBordersOnSums()
{
    ui->labelSum100->setStyleSheet(styleSumsClearNormal);
 ui->labelSum100->setStyleSheet(styleSumsClearNormal);
ui->labelSum200->setStyleSheet(styleSumsClearNormal);
ui->labelSum300->setStyleSheet(styleSumsClearNormal);
ui->labelSum400->setStyleSheet(styleSumsClearNormal);
ui->labelSum500->setStyleSheet(styleSumsClearSure);
ui->labelSum1000->setStyleSheet(styleSumsClearNormal);
ui->labelSum1500->setStyleSheet(styleSumsClearNormal);
ui->labelSum2000->setStyleSheet(styleSumsClearNormal);
ui->labelSum2500->setStyleSheet(styleSumsClearNormal);
ui->labelSum5000->setStyleSheet(styleSumsClearSure);
ui->labelSum10000->setStyleSheet(styleSumsClearNormal);
ui->labelSum20000->setStyleSheet(styleSumsClearNormal);
ui->labelSum50000->setStyleSheet(styleSumsClearNormal);
ui->labelSum100000->setStyleSheet(styleSumsClearNormal);
ui->labelSum200000->setStyleSheet(styleSumsClearSure);
}


void MainWindow::on_pushButtonStartGame_clicked()
{
    clearBordersOnSums();
    for(int i = 0; i < easyQuestions.size(); i++){
        usedEasy.push_back(false);
    }
    for(int i = 0; i < mediumQuestions.size(); i++){
        usedMedium.push_back(false);
    }
    for(int i = 0; i < hardQuestions.size(); i++){
        usedHard.push_back(false);
    }

    ui->pushButtonA->setEnabled(true);
    ui->pushButtonB->setEnabled(true);
    ui->pushButtonC->setEnabled(true);
    ui->pushButtonD->setEnabled(true);
    numQuestionAsked = 1;
    loadQuestion();
}


void MainWindow::on_pushButtonA_clicked()
{
    if(inEasyQuestions == true){
        if(easyQuestions[indexCurrentQuestion].getAnswers()[0].getCorrect()){
            ui->pushButtonA->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonA->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    } else if (inMediumQuestions) {
        if(mediumQuestions[indexCurrentQuestion].getAnswers()[0].getCorrect()){
            ui->pushButtonA->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonA->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    } else {
        if(hardQuestions[indexCurrentQuestion].getAnswers()[0].getCorrect()){
            ui->pushButtonA->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonA->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    }
}

void MainWindow::on_pushButtonB_clicked()
{
    if(inEasyQuestions == true){
        if(easyQuestions[indexCurrentQuestion].getAnswers()[1].getCorrect()){
            ui->pushButtonB->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonB->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    } else if (inMediumQuestions) {
        if(mediumQuestions[indexCurrentQuestion].getAnswers()[1].getCorrect()){
            ui->pushButtonB->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonB->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    } else {
        if(hardQuestions[indexCurrentQuestion].getAnswers()[1].getCorrect()){
            ui->pushButtonB->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonB->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    }
}

void MainWindow::on_pushButtonC_clicked()
{
    if(inEasyQuestions == true){
        if(easyQuestions[indexCurrentQuestion].getAnswers()[2].getCorrect()){
            ui->pushButtonC->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonC->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    } else if (inMediumQuestions){
        if(mediumQuestions[indexCurrentQuestion].getAnswers()[2].getCorrect()){
            ui->pushButtonC->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonC->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    } else {
        if(hardQuestions[indexCurrentQuestion].getAnswers()[2].getCorrect()){
            ui->pushButtonC->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonC->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    }
}

void MainWindow::on_pushButtonD_clicked()
{
    if(inEasyQuestions == true){
        if(easyQuestions[indexCurrentQuestion].getAnswers()[3].getCorrect()){
            ui->pushButtonD->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonD->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    } else if (inMediumQuestions){
        if(mediumQuestions[indexCurrentQuestion].getAnswers()[3].getCorrect()){
            ui->pushButtonD->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonD->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    } else {
        if(hardQuestions[indexCurrentQuestion].getAnswers()[3].getCorrect()){
            ui->pushButtonD->setStyleSheet(styleCorrect);
            ui->pushButtonNextQuestion->setEnabled(true);
        } else {
            ui->pushButtonD->setStyleSheet(styleIncorrect);
            colorRightWhenWrongAnswer();
            ui->pushButtonNextQuestion->setEnabled(false);
        }
    }
}



void MainWindow::on_pushButtonNextQuestion_clicked()
{
    loadQuestion();
}

