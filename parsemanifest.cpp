#include "parsemanifest.h"
#include "qfile.h"
#include <QDomDocument>
#include "QDomNodeList"
#include "qdebug.h"

const QString ParseManifest::AndroidNameSpace = "http://schemas.android.com/apk/res/android";

ParseManifest::ParseManifest(QString filename)
{
    mAndroidManifestPath = filename;
    QFile* xmlFile = new QFile(filename);
    xmlFile->open(QFile::ReadWrite | QFile::Text);
    xmlDoc.setContent(xmlFile);
    qDebug()<<xmlDoc.toString();
    xmlFile->close();

    QDomNodeList manifestNode = xmlDoc.elementsByTagName("manifest");
    QDomNodeList applicationNode = xmlDoc.elementsByTagName("application");
    qDebug()<< manifestNode.length();
    QDomNode packageNode = manifestNode.at(0).attributes().namedItem("package");
    qDebug() << "The package Name is:" << packageNode.nodeValue();
    qDebug()<<xmlDoc.namespaceURI();
    mPackageName = packageNode.nodeValue();
    QDomNode iconNode = applicationNode.at(0).attributes().namedItem("android:icon");

    mIconPath = iconNode.nodeValue();
    qDebug()<<"The icon name is:" + mIconPath;
}

ParseManifest::~ParseManifest()
{

}

QString ParseManifest::getIconPath()
{
    return mIconPath;
}

void ParseManifest::setPackageName(QString packagename)
{
    QDomNodeList manifestNode = xmlDoc.elementsByTagName("manifest");
    manifestNode.at(0).attributes().namedItem("package").setNodeValue(packagename);

    QFile savedFile(mAndroidManifestPath);
    savedFile.open(QFile::ReadWrite | QFile::Text);
    QTextStream stream( &savedFile);
    qDebug() << xmlDoc.toString();
    stream << xmlDoc.toString();
    savedFile.close();
}


QString ParseManifest::getPackageName()
{
    return mPackageName;
}

