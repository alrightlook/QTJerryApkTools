#include "toolwindow.h"
#include "ui_toolwindow.h"

ToolWindow::ToolWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolWindow)
{
    ui->setupUi(this);
    ui->LabelPreview->setFixedSize(512, 512);
    this->setLayout(ui->MainLayout);
}

ToolWindow::~ToolWindow()
{
    delete ui;
}
