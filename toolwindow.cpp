#include "toolwindow.h"
#include "ui_toolwindow.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include <cstdlib>
#include "qdebug.h"
#include "commandlinethread.h"
#include "parsemanifest.h"
#include "QProcessEnvironment"
#include "qpainter.h"
#include "stdlib.h"

ToolWindow::ToolWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolWindow)
{
    ui->setupUi(this);
    ui->LabelPreview->setFixedSize(512, 512);
    mCurrentDir = QDir::currentPath();
    qDebug() << mCurrentDir;
    mApkDecomplePath = mCurrentDir + "/Decomplie";


    QObject::connect(&mCmdProc, SIGNAL(started()), this, SLOT(onCmdThreadStart()));
    QObject::connect(&mCmdProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onCmdThreadFinished(int, QProcess::ExitStatus)));

    QString env = getenv("PATH");
    qDebug () <<"Get evn is:" + env;
    env += ":/usr/local/bin/";
    qDebug() << "env new is:" + env;
    qputenv("PATH",QByteArray().append(env));
    qDebug() << mApkDecomplePath;
    env = getenv("PATH");
    qDebug()<<"The set env reslut is:" + env;
    mApkDecompileDir.setPath(mApkDecomplePath);
    if(!mApkDecompileDir.exists()) {
        mApkDecompileDir.mkpath(".");
    }
    this->setLayout(ui->MainLayout);
}

void ToolWindow::closeEvent(QCloseEvent *event)
{
    if (mApkDecompileDir.exists()) {
        mApkDecompileDir.removeRecursively();
    }
}

void ToolWindow::setUser(QString user)
{
    mUser = user;
}

void ToolWindow::onCmdThreadFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug()<<"The commandLine Thread Finished" + exitCode;
    if(mCmdProc.getType() == CommandProcess::APKDECOMPILE) {

        QString manifestPath = mApkDecomplePath + "/AndroidManifest.xml";
        mAndroidManifestPath = manifestPath;
        qDebug()<<"The androidmanivest path is:" + manifestPath;
        ParseManifest pm(manifestPath);
        ui->PackageNameEdit->setText(pm.getPackageName());
        mApkIconPath = pm.getIconPath();
    }
    mProgressDialog.close();
}

void ToolWindow::onCmdThreadStart()
{
    qDebug()<<"The commandLine Thread Started";
    mProgressDialog.setWindowTitle("Processing...");
    mProgressDialog.show();
}

ToolWindow::~ToolWindow()
{
    delete ui;
}


void ToolWindow::replyFinished(QNetworkReply* reply)
{
    qDebug()<<"reply called";
    QString content = QString(reply->readAll());
    QVariant statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
    if ( !statusCode.isValid() )
           return;

    qDebug()<<"checkout 1";
    QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject resJsonObj = jsonDoc.object();
    QJsonValue loginCode = resJsonObj["code"];
    qDebug()<<loginCode;
    int status = statusCode.toInt();
    QMessageBox::information(this, "Welcome", content);
    if(0 == loginCode.toInt()) {
        ParseManifest pm(mAndroidManifestPath);
        pm.setPackageName(ui->PackageNameEdit->text());
        mCmdProc.setType(CommandProcess::APKBUILD);
        QString apkSavedPath = QFileDialog::getSaveFileName(this, "Build Apk", "", "All apk Files(*.apk)");
        QString apktoolBuildCmd = "apktool b " + mApkDecomplePath + " " + apkSavedPath;
        qDebug() << "The build cmd is:" + apktoolBuildCmd;
        mCmdProc.start("apktool", QStringList()<<"b"<<mApkDecomplePath<<apkSavedPath);
    }

}


void ToolWindow::on_pushButton_2_clicked()
{
     mOpenAPKFilePath = QFileDialog::getOpenFileName(this, "Open Apk File", "", "All apk Files(*.apk)");
     QString apktoolCmd = "apktool d -f " + mOpenAPKFilePath + " " + mApkDecomplePath;
     apktoolCmd.replace(QString("("), QString("\\("));
     apktoolCmd.replace(QString(")"), QString("\\)"));
     qDebug() << "The apktool Cmd line is:" + apktoolCmd;

     mCmdProc.setType(CommandProcess::APKDECOMPILE);
     mCmdProc.start("apktool", QStringList()<<"d"<<"-f"<<mOpenAPKFilePath<<mApkDecomplePath);
}

void ToolWindow::postOperation(QUrl url, QVariantMap params)
{
    qDebug()<<url.toString();
    QNetworkAccessManager* maneger = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QObject::connect(maneger, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    QJsonDocument doc = QJsonDocument::fromVariant(params);

    QByteArray postData = doc.toJson();

    maneger->post(request, postData);
}

void ToolWindow::on_pushButton_clicked()
{
    QString inputPackageName = ui->PackageNameEdit->text();

    if (inputPackageName != "") {
        QVariantMap params;
        params.insert("account", mUser);
        params.insert("pakcagename", inputPackageName);
        QString apkSavedPath = QFileDialog::getSaveFileName(this, "Build Apk", "", "All apk Files(*.apk)");
        QString apktoolBuildCmd = "apktool b " + mApkDecomplePath + " " + apkSavedPath;
        params.insert("command", apktoolBuildCmd);
        postOperation(QUrl("http://127.0.0.1:3000/operation"), params);

    }
}

void ToolWindow::on_pushButton_3_clicked()
{
    mChooseIconPath = QFileDialog::getOpenFileName(this, "Choose Icon", "", "PNG files(*.png)");
    qDebug() << "The image path :" + mChooseIconPath;
    chooseIconPixmap.load(mChooseIconPath);
    ui->LabelPreview->setPixmap(chooseIconPixmap);
}

void ToolWindow::on_pushButton_4_clicked()
{
    mChooseCornerPath = QFileDialog::getOpenFileName(this, "Choose corner", "", "PNG files(*.png)");
    chooseCornerPixmap.load(mChooseCornerPath);
    QPainter painter;
    painter.begin(&chooseIconPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0,0, chooseIconPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(0,0, chooseCornerPixmap);
    painter.save();
    ui->LabelPreview->setPixmap(chooseIconPixmap);
}

void ToolWindow::on_pushButton_5_clicked()
{
    QString srcIconPath = mApkDecomplePath;
    QString tmpIconPath = mApkIconPath;
    srcIconPath = mApkDecomplePath + tmpIconPath.replace("@drawable/", "/res/drawable/") + ".png";
    qDebug()<<srcIconPath;
    chooseIconPixmap.save(srcIconPath, "PNG");
    tmpIconPath = mApkIconPath;
    srcIconPath = mApkDecomplePath + tmpIconPath.replace("@drawable/", "/res/drawable-hdpi/") + ".png";
    qDebug()<<srcIconPath;
    chooseIconPixmap.save(srcIconPath, "PNG");
    tmpIconPath = mApkIconPath;
    srcIconPath = mApkDecomplePath + tmpIconPath.replace("@drawable/", "/res/drawable-ldpi/")+ ".png";
    qDebug()<<srcIconPath;
    chooseIconPixmap.save(srcIconPath, "PNG");
    tmpIconPath = mApkIconPath;
    srcIconPath = mApkDecomplePath + tmpIconPath.replace("@drawable/", "/res/drawable-mdpi/") + ".png";
    qDebug()<<srcIconPath;
    chooseIconPixmap.save(srcIconPath, "PNG");
    tmpIconPath = mApkIconPath;
    srcIconPath = mApkDecomplePath + tmpIconPath.replace("@drawable/", "/res/drawable-xhdpi/")+ ".png";
    qDebug()<<srcIconPath;
    chooseIconPixmap.save(srcIconPath, "PNG");
    tmpIconPath = mApkIconPath;
    srcIconPath = mApkDecomplePath + tmpIconPath.replace("@drawable/", "/res/drawable-xxhdpi/")+ ".png";
    qDebug()<<srcIconPath;
    chooseIconPixmap.save(srcIconPath, "PNG");


}
