#include "toolwindow.h"
#include "ui_toolwindow.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include <cstdlib>
#include "qdebug.h"
#include "commandlinethread.h"
#include "parsemanifest.h"

ToolWindow::ToolWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolWindow)
{
    ui->setupUi(this);
    ui->LabelPreview->setFixedSize(512, 512);
    mCurrentDir = QDir::currentPath();
    qDebug() << mCurrentDir;
    mApkDecomplePath = mCurrentDir + "/Decomplie";


    QObject::connect(&mCmdThread, SIGNAL(started()), this, SLOT(onCmdThreadStart()));
    QObject::connect(&mCmdThread, SIGNAL(finished()), this, SLOT(onCmdThreadFinished()));


    qDebug() << mApkDecomplePath;
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

void ToolWindow::onCmdThreadFinished()
{
    qDebug()<<"The commandLine Thread Finished";
    if(mCmdThread.GetCommandType() == CommandLineThread::APKDECOMPILE) {

        QString manifestPath = mApkDecomplePath + "/AndroidManifest.xml";
        qDebug()<<"The androidmanivest path is:" + manifestPath;
        ParseManifest pm(manifestPath);
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

     mCmdThread.SetCommandType(CommandLineThread::APKDECOMPILE);
     mCmdThread.SetCommandLine(apktoolCmd);
     mCmdThread.start();
}
