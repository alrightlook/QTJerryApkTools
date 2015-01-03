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
#include "qjsonobject.h"

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
    QString content = QString(reply->readAll());
    QVariant statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
    if ( !statusCode.isValid() )
           return;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject resJsonObj = jsonDoc.object();
    QJsonValue loginCode = resJsonObj["code"];
    qDebug()<<loginCode;
    int status = statusCode.toInt();
    QMessageBox::information(this, "Welcome", content);
    if(200 == loginCode.toInt()) {
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

    keyValuePair.insert("account", userName);
    keyValuePair.insert("password", pwd);

    QJsonDocument doc = QJsonDocument::fromVariant(keyValuePair);

    QByteArray postData = doc.toJson();

    QObject::connect(maneger, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));


    maneger->post(request, postData);


}
