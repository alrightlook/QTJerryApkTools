#ifndef PARSEMANIFEST_H
#define PARSEMANIFEST_H

#include <QtCore>
#include <QDomDocument>

class ParseManifest
{
public:
    ParseManifest(QString filename);
    ~ParseManifest();
    QString getPackageName();
    QString getIconPath();
    void setPackageName(QString packagename);
private:
    QString mIconPath;
    QString mPackageName;
    QString mAndroidManifestPath;
    QDomDocument xmlDoc;
    static const QString AndroidNameSpace;
};

#endif // PARSEMANIFEST_H
