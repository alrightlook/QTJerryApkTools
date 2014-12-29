#include "dialog.h"
#include "ui_dialog.h"
#include "toolwindow.h"
#include "qnetworkaccessmanager.h"
#include "qurl.h"
#include "qnetworkrequest.h"
#include "qnetworkreply.h"
#include "qdebug.h"
#include "qurlquery.h"
#include "qmessagebox.h"
#include "qjsondocument.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setLayout(ui->MainLoginLayout);
    this->setFixedSize(300, 200);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::replyFinished(QNetworkReply* reply)
{
    QVariant statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
    if ( !statusCode.isValid() )
           return;

    int status = statusCode.toInt();

    if(200 == status) {
        QMessageBox::information(this, "Welcome", "Hey! Welcome you son of a ...");
        this->close();
        ToolWindow* tw = new ToolWindow();
        tw->show();
    }

}

void Dialog::on_pushButton_clicked()
{
    QNetworkAccessManager* maneger = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("http://127.0.0.1:3000/users/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QVariantMap keyValuePair;


    QString userName = ui->userName->text();
    QString pwd = ui->userPassword->text();

    keyValuePair.insert("username", userName);
    keyValuePair.insert("password", pwd);

    QJsonDocument doc = QJsonDocument::fromVariant(keyValuePair);

    QByteArray postData = doc.toJson();

    QObject::connect(maneger, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));


    maneger->post(request, postData);


}
