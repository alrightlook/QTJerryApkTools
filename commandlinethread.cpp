#include "commandlinethread.h"
#include "qthread.h"
#include <cstdlib>
#include "qdebug.h"

CommandLineThread::CommandLineThread(QObject *parent) :
    QThread(parent)
{
    mType = NOTSET;
}

void CommandLineThread::run()
{
    qDebug()<<"The cmd line is:" + mCmdLine;
    system(mCmdLine.toStdString().c_str());
}

int CommandLineThread::GetCommandType()
{
    return mType;
}


void CommandLineThread::SetCommandType(int type)
{
    mType = type;
}

void CommandLineThread::SetCommandLine(QString cmd)
{
    mCmdLine = cmd;
}
