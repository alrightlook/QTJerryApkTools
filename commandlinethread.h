#ifndef COMMANDLINETHREAD_H
#define COMMANDLINETHREAD_H

#include <QThread>

class CommandLineThread : public QThread
{
    Q_OBJECT
public:
    static const int NOTSET = -1;
    static const int APKDECOMPILE = 0;
    static const int APKCOMPILE = 1;

    explicit CommandLineThread(QObject *parent = 0);
    void SetCommandLine(QString cmd);
    void SetCommandType(int type);
    int GetCommandType();
signals:

protected:
    void run();
public slots:

private:
    QString mCmdLine;
    int mType;

};

#endif // COMMANDLINETHREAD_H
