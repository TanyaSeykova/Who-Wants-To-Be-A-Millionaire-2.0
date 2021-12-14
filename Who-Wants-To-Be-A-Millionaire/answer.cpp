#include "answer.h"

Answer::Answer(QString _text, QString _percentage, bool _correct) : text(_text), percentage(_percentage), correct(_correct){
}

const QString &Answer::getText() const
{
    return text;
}

const QString &Answer::getPercentage() const
{
    return percentage;
}

bool Answer::getCorrect() const
{
    return correct;
}

