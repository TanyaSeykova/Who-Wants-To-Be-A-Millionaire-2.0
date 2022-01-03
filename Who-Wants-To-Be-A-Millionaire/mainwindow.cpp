#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <QFont>
#include <QGraphicsTextItem>
#include <QDebug>

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

    //new setEnabled false and setVisible false
    ui->pushButton5050->setEnabled(false);
    ui->pushButtonCallAFriend->setEnabled(false);
    ui->pushButtonAskTheAudience->setEnabled(false);
    ui->audienceBox->setVisible(false);
    ui->callerBox->setVisible(false);

    styleCorrect = "font: 10pt \"MS Shell Dlg 2\"; color: white; background-color: green;";
    styleIncorrect = "font: 10pt \"MS Shell Dlg 2\"; color: white; background-color: red;";
    styleClearButtons = "font: 10pt \"MS Shell Dlg 2\"; color: white;";
    styleSumsNormal = "font: 12pt \"MS Shell Dlg 2\"; color: white; border: 2px solid yellow;";
    styleSumsSure = "font: 12pt \"MS Shell Dlg 2\"; color: yellow; border: 2px solid yellow;";
    styleSumsClearNormal = "font: 12pt \"MS Shell Dlg 2\"; color: white;";
    styleSumsClearSure = "font: 12pt \"MS Shell Dlg 2\"; color: yellow;";
    //new style for used Hint
    styleHintUsed = "font: 10pt \"MS Shell Dlg 2\"; color: white; background-color: maroon;";

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
//new added visible
void MainWindow::setABCDVisible(){
    ui->pushButtonA->setVisible(true);
    ui->pushButtonB->setVisible(true);
    ui->pushButtonC->setVisible(true);
    ui->pushButtonD->setVisible(true);
}
//new added policy
void MainWindow::setABCDHidePolicy() {
    QSizePolicy a_retain = ui->pushButtonA->sizePolicy();
    a_retain.setRetainSizeWhenHidden(true);
    ui->pushButtonA->setSizePolicy(a_retain);

    QSizePolicy b_retain = ui->pushButtonB->sizePolicy();
    b_retain.setRetainSizeWhenHidden(true);
    ui->pushButtonB->setSizePolicy(b_retain);

    QSizePolicy c_retain = ui->pushButtonC->sizePolicy();
    c_retain.setRetainSizeWhenHidden(true);
    ui->pushButtonC->setSizePolicy(c_retain);

    QSizePolicy d_retain = ui->pushButtonD->sizePolicy();
    d_retain.setRetainSizeWhenHidden(true);
    ui->pushButtonD->setSizePolicy(d_retain);

}
void MainWindow::loadQuestion()
{
    setABCDVisible();
    ui->pushButtonA->setStyleSheet(styleClearButtons);
    ui->pushButtonB->setStyleSheet(styleClearButtons);
    ui->pushButtonC->setStyleSheet(styleClearButtons);
    ui->pushButtonD->setStyleSheet(styleClearButtons);
    ui->audienceBox->setVisible(false);
    ui->callerBox->setVisible(false);
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
    //new added set enabled shtoto inache pak mojesh da natisnesh vernoto i da produljij igrata
    ui->pushButtonA->setEnabled(false);
    ui->pushButtonB->setEnabled(false);
    ui->pushButtonC->setEnabled(false);
    ui->pushButtonD->setEnabled(false);
    ui->pushButton5050->setEnabled(false);
    ui->pushButtonCallAFriend->setEnabled(false);
    ui->pushButtonAskTheAudience->setEnabled(false);
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
    //new emptying
    usedEasy.erase(usedEasy.begin(),usedEasy.end());
    usedMedium.erase(usedMedium.begin(),usedMedium.end());
    usedHard.erase(usedHard.begin(),usedHard.end());
    //^^
    for(int i = 0; i < easyQuestions.size(); i++){
        usedEasy.push_back(false);
    }
    for(int i = 0; i < mediumQuestions.size(); i++){
        usedMedium.push_back(false);
    }
    for(int i = 0; i < hardQuestions.size(); i++){
        usedHard.push_back(false);
    }
    //yuh
    setABCDVisible();
    setABCDHidePolicy();
    //^^^
    ui->pushButtonA->setEnabled(true);
    ui->pushButtonB->setEnabled(true);
    ui->pushButtonC->setEnabled(true);
    ui->pushButtonD->setEnabled(true);
    //joker
    ui->audienceBox->setVisible(false);
    ui->callerBox->setVisible(false);
    ui->pushButton5050->setEnabled(true);
    ui->pushButtonCallAFriend->setEnabled(true);
    ui->pushButtonAskTheAudience->setEnabled(true);
    ui->pushButton5050->setStyleSheet(styleClearButtons);
    ui->pushButtonCallAFriend->setStyleSheet(styleClearButtons);
    ui->pushButtonAskTheAudience->setStyleSheet(styleClearButtons);
    //^^^
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
//new 5050button click
void MainWindow::on_pushButton5050_clicked()
{
    ui->pushButton5050->setEnabled(false);
    ui->pushButton5050->setStyleSheet(styleHintUsed);
    int correctIndex;
    int highestPercentIndex;
    QString maxPercent = "0";
    QVector<Answer> currQuestion;
    if(inEasyQuestions == true) {
        currQuestion = easyQuestions[indexCurrentQuestion].getAnswers();
        correctIndex = easyQuestions[indexCurrentQuestion].getCorrect();
    } else if (inMediumQuestions) {
        currQuestion = mediumQuestions[indexCurrentQuestion].getAnswers();
        correctIndex = mediumQuestions[indexCurrentQuestion].getCorrect();
    } else {
        currQuestion = hardQuestions[indexCurrentQuestion].getAnswers();
        correctIndex = hardQuestions[indexCurrentQuestion].getCorrect();
    }
    for(int i=0; i<currQuestion.size(); i++) {
        if (i != correctIndex) {
            QString currPercent = currQuestion[i].getPercentage();
            if ( currPercent > maxPercent) {
               highestPercentIndex = i;
               maxPercent = currPercent;
            }
        }
    }
    for(int i=0;i<currQuestion.size();i++) {
        if (i != correctIndex && i != highestPercentIndex) {
            switch (i) {
                case 0: ui->pushButtonA->setVisible(false); break;
                case 1: ui->pushButtonB->setVisible(false); break;
                case 2: ui->pushButtonC->setVisible(false); break;
                case 3: ui->pushButtonD->setVisible(false); break;
            }
        }
    }
}

QString MainWindow::setCorrectAnswer(Question currQuestion) {
    int correctIndex = currQuestion.getCorrect();
    switch(correctIndex) {
        case 0: return "A";
        case 1: return "B";
        case 2: return "C";
        case 3: return "D";
    }
    return "Error";
}
void MainWindow::on_pushButtonCallAFriend_clicked()
{
    ui->pushButtonCallAFriend->setEnabled(false);
    ui->pushButtonCallAFriend->setStyleSheet(styleHintUsed);
    ui->callerBox->setReadOnly(true);
    ui->callerBox->setAlignment(Qt::AlignCenter);
    ui->callerBox->adjustSize();
    ui->callerBox->setVisible(true);
    QString correctAnswer = "";
    if (inEasyQuestions == true) {
        correctAnswer = setCorrectAnswer(easyQuestions[indexCurrentQuestion]);
        QString callerText = "I'm pretty sure that the correct answer is " + correctAnswer;
        ui->callerBox->setText(callerText);
    } else if (inMediumQuestions == true) {
        correctAnswer = setCorrectAnswer(mediumQuestions[indexCurrentQuestion]);
        QString callerText = "Not sure. I've heard somewhere that " + correctAnswer + " is the correct answer ";
        ui->callerBox->setText(callerText);
    } else {
        correctAnswer = setCorrectAnswer(hardQuestions[indexCurrentQuestion]);
        int highestPercentIndex;
        QString maxPercent = "0";
        for(int i=0; i<hardQuestions[indexCurrentQuestion].getAnswers().size(); i++) {
            if (i != hardQuestions[indexCurrentQuestion].getCorrect()) {
                QString currPercent =  hardQuestions[indexCurrentQuestion].getAnswers()[i].getPercentage();
                if ( currPercent > maxPercent) {
                   highestPercentIndex = i;
                   maxPercent = currPercent;
                }
            }
        }
        QString secondAnswer = "";
        switch(highestPercentIndex) {
            case 0: secondAnswer = "A"; break;
            case 1: secondAnswer = "B"; break;
            case 2: secondAnswer = "C"; break;
            case 3: secondAnswer = "D"; break;
        }
        QString callerText = "That is a tough one. If I had to choose I'd choose either " + correctAnswer + " or " + secondAnswer;
        ui->callerBox->setText(callerText);
    }
}


void MainWindow::on_pushButtonAskTheAudience_clicked()
{
    ui->pushButtonAskTheAudience->setEnabled(false);
    ui->pushButtonAskTheAudience->setStyleSheet(styleHintUsed);
    ui->audienceBox->setVisible(true);

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsItemGroup *rectGroup = new QGraphicsItemGroup;
    int rectX = 0, rectY = 0;
    int percentNumber;
    QString currPercent = "";
    QVector<Answer> currQuestion;
    if(inEasyQuestions == true) {
        currQuestion = easyQuestions[indexCurrentQuestion].getAnswers();
    } else if (inMediumQuestions) {
        currQuestion = mediumQuestions[indexCurrentQuestion].getAnswers();
    } else {
        currQuestion = hardQuestions[indexCurrentQuestion].getAnswers();
    }
    for(int i=0;i<currQuestion.size();i++) {
        percentNumber = 0;
        currPercent = currQuestion[i].getPercentage();
        int j = currPercent.size() - 1;
        while(j >= 0) {
            if(currPercent[j] == '%') {
                j--;
                continue;
            }
            percentNumber += (currPercent[j].unicode()-'0')*(pow(10,currPercent.size() - 2 - j));
            j--;
        }

        double height = 120*(double)(percentNumber*0.01);

        QGraphicsRectItem *rect = new QGraphicsRectItem(rectX,rectY,35,height);
        rect->setBrush(QBrush(Qt::red));
        rect->setPen(QPen(Qt::red));
        rectGroup->addToGroup(rect);

        QGraphicsTextItem *percentText = new QGraphicsTextItem();
        percentText->setDefaultTextColor(Qt::white);
        percentText->setFont(QFont("MS Shell Dlg 2",12));
        percentText->setPlainText(currPercent);
        percentText->setPos(rectX,height+10);
        rectGroup->addToGroup(percentText);

        QGraphicsTextItem *answerText = new QGraphicsTextItem();
        answerText->setDefaultTextColor(Qt::white);
        answerText->setFont(QFont("MS Shell Dlg 2",12));
        answerText->setPos(rectX+8,rectY-25);
        switch(i) {
            case 0: answerText->setPlainText("A"); break;
            case 1: answerText->setPlainText("B"); break;
            case 2: answerText->setPlainText("C"); break;
            case 3: answerText->setPlainText("D"); break;
        }
        rectGroup->addToGroup(answerText);

        rectX += 70;
    }
    rectGroup->mapToScene(0,0);
    scene->addItem(rectGroup);
    ui->audienceBox->setScene(scene);

}

