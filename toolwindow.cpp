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
    env += ";D:\\tools\\";
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

void ToolWindow::on_pushButton_2_clicked()
{
     mOpenAPKFilePath = QFileDialog::getOpenFileName(this, "Open Apk File", "", "All apk Files(*.apk)");
     QString apktoolCmd = "apktool d -f " + mOpenAPKFilePath + " " + mApkDecomplePath;
     apktoolCmd.replace(QString("("), QString("\\("));
     apktoolCmd.replace(QString(")"), QString("\\)"));
     qDebug() << "The apktool Cmd line is:" + apktoolCmd;

     mCmdProc.setType(CommandProcess::APKDECOMPILE);
     system("echo off");

     mCmdProc.start("apktool.bat", QStringList()<<"d"<<"-f"<<mOpenAPKFilePath<<mApkDecomplePath);
}

void ToolWindow::on_pushButton_clicked()
{
    if (ui->PackageNameEdit->text() != "") {

            ParseManifest pm(mAndroidManifestPath);
            pm.setPackageName(ui->PackageNameEdit->text());
            mCmdProc.setType(CommandProcess::APKBUILD);
            QString apkSavedPath = QFileDialog::getSaveFileName(this, "Build Apk", "", "All apk Files(*.apk)");
            QString apktoolBuildCmd = "apktool b " + mApkDecomplePath + " " + apkSavedPath;
            qDebug() << "The build cmd is:" + apktoolBuildCmd;
            mCmdProc.start("apktool.bat", QStringList()<<"b"<<mApkDecomplePath<<apkSavedPath);

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
