#ifndef ANSWER_H
#define ANSWER_H

#include <QString>
#include <QtXml>

class Answer
{

public:
    Answer(QString _text, QString _percentage, bool _correct);
    const QString &getText() const;

    const QString &getPercentage() const;

    bool getCorrect() const;

private:
    QString text;
    QString percentage;
    bool correct;

};

#endif // ANSWER_H
