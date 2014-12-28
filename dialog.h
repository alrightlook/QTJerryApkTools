#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <qnetworkreply.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void replyFinished(QNetworkReply* reply);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
