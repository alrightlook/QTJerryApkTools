#include "commandprocess.h"

CommandProcess::CommandProcess(QObject *parent) :
    QProcess(parent)
{
    mType = NOTSET;
}

void CommandProcess::setType(int type)
{
    mType = type;
}


int CommandProcess::getType()
{
    return mType;
}
