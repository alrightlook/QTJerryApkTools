#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include <QWidget>
#include <qdir.h>
#include "commandlinethread.h"
#include "commandprocess.h"
#include "qprogressdialog.h"
#include "qurl.h"
#include "QVariantMap"
#include "QNetworkReply"

namespace Ui {
class ToolWindow;
}

class ToolWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ToolWindow(QWidget *parent = 0);
    void setUser(QString user);
    ~ToolWindow();

private slots:
    void on_pushButton_2_clicked();
    void onCmdThreadFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onCmdThreadStart();

    void replyFinished(QNetworkReply* reply);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

protected:
    void closeEvent(QCloseEvent *event);
    void postOperation(QUrl url, QVariantMap params);

private:
    QString mUser;
    CommandProcess mCmdProc;
    QString mOpenAPKFilePath;
    QString mCurrentDir;
    QString mChooseIconPath;
    QString mChooseCornerPath;
    QString mApkIconPath;
    QString mApkDecomplePath;
    QString mAndroidManifestPath;
    QDir mApkDecompileDir;


    QPixmap chooseIconPixmap;
    QPixmap chooseCornerPixmap;

    QProgressDialog mProgressDialog;
    Ui::ToolWindow *ui;
};

#endif // TOOLWINDOW_H
