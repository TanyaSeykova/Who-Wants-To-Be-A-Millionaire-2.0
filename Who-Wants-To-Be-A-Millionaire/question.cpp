#include "question.h"

Question::Question(const QString& _text, const QVector<QDomElement>& answersElements)
{
    text = _text;
    QString ansText = "", percentage = "";
    bool correct = false;
    for(int i = 0; i < answersElements.size(); i++){
        ansText = answersElements[i].text();
        percentage = answersElements[i].attribute("audience");
        if (answersElements[i].attribute("correct") == "yes"){
            correct = true;
        } else {
            correct = false;
        }
        answers.push_back(Answer(ansText, percentage, correct));
    }
}

const QString &Question::getText() const
{
    return text;
}

const QVector<Answer> &Question::getAnswers() const
{
    return answers;
}

int Question::getCorrect()
{
    int index = 0;
    for (index = 0; index < answers.length(); index++){
        if (answers[index].getCorrect()) return index;
    }
    return index;
}
