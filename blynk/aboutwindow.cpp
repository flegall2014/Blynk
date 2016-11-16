// Application:
#include "aboutwindow.h"
#include "ui_aboutwindow.h"

// Constructor:
AboutWindow::AboutWindow(const QString &sTitle, QWidget *parent) :
    CustomWindow(sTitle, parent),
    ui(new Ui::AboutWindow)
{
    // Setup UI:
    ui->setupUi(this);
}
