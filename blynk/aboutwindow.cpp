#include <QFile>
#include <QTextStream>

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

    // Set image:
    ui->logoWidget->setImage(":/icons/blynk-logo.jpg");

    // Load HTML contents:
    load(":/html/About Blynk.htm");
}

// Load:
void AboutWindow::load(const QString &sFilePath)
{
    QFile f(sFilePath);
    if (f.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&f);
        QString sHtmlContents = in.readAll();
        f.close();
        ui->textEdit->setHtml(sHtmlContents);
    }
}
