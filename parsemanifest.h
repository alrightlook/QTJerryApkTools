#ifndef PARSEMANIFEST_H
#define PARSEMANIFEST_H

#include <QtCore>

class ParseManifest
{
public:
    ParseManifest(QString filename);
    ~ParseManifest();
    QString getPackageName();
private:
    QString mPackageName;
    static const QString AndroidNameSpace;
};

#endif // PARSEMANIFEST_H
