#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include <QWidget>
#include <qdir.h>
#include "commandlinethread.h"
#include "qprogressdialog.h"

namespace Ui {
class ToolWindow;
}

class ToolWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ToolWindow(QWidget *parent = 0);
    ~ToolWindow();

private slots:
    void on_pushButton_2_clicked();
    void onCmdThreadFinished();
    void onCmdThreadStart();

protected:
    void closeEvent(QCloseEvent *event);

private:
    CommandLineThread mCmdThread;
    QString mOpenAPKFilePath;
    QString mCurrentDir;
    QString mApkDecomplePath;
    QDir mApkDecompileDir;
    QProgressDialog mProgressDialog;
    Ui::ToolWindow *ui;
};

#endif // TOOLWINDOW_H
