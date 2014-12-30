#ifndef COMMANDPROCESS_H
#define COMMANDPROCESS_H

#include <QProcess>

class CommandProcess : public QProcess
{
    Q_OBJECT
public:
    const static int NOTSET = -1;
    const static int APKDECOMPILE = 0;
    const static int APKBUILD     = 1;
    explicit CommandProcess(QObject *parent = 0);

    void setType(int type);
    int getType();
signals:

private:
    int mType;
public slots:

};

#endif // COMMANDPROCESS_H
