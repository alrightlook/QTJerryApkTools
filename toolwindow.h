#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include <QWidget>

namespace Ui {
class ToolWindow;
}

class ToolWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ToolWindow(QWidget *parent = 0);
    ~ToolWindow();

private:
    Ui::ToolWindow *ui;
};

#endif // TOOLWINDOW_H
