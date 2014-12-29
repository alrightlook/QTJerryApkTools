#include "parsemanifest.h"
#include "qfile.h"
#include <QDomDocument>
#include "QDomNodeList"
#include "qdebug.h"

const QString ParseManifest::AndroidNameSpace = "http://schemas.android.com/apk/res/android";

ParseManifest::ParseManifest(QString filename)
{
    QFile* xmlFile = new QFile(filename);
    xmlFile->open(QFile::ReadWrite | QFile::Text);
    QDomDocument xmlDoc;
    xmlDoc.setContent(xmlFile, true);
    xmlFile->close();

    QDomNodeList manifestNode = xmlDoc.elementsByTagName("manifest");
    qDebug()<< manifestNode.length();
    QDomNode packageNode = manifestNode.at(0).attributes().namedItem("package");
    qDebug() << "The package Name is:" << packageNode.nodeValue();
    mPackageName = packageNode.nodeValue();
}

ParseManifest::~ParseManifest()
{

}


QString ParseManifest::getPackageName()
{

}

