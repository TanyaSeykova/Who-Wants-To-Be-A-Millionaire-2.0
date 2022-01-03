#ifndef QUESTION_H
#define QUESTION_H
#include "answer.h"

#include <QtXml>
#include <QVector>

class Question
{
public:
    Question(const QString& _text, const QVector<QDomElement>& answersElements);

    const QString &getText() const;

    const QVector<Answer> &getAnswers() const;
    int getCorrect();
private:
    QVector<Answer> answers;
    QString text;

};

#endif // QUESTION_H
