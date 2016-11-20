#include <QFile>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>

// Application:
#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#define VISION_AIDS_OVERSEA_URL "http://getblynk.org/visionaid-overseas-blynk"

// Constructor:
AboutWindow::AboutWindow(const QString &sTitle, QWidget *parent) :
    CustomWindow(sTitle, parent),
    ui(new Ui::AboutWindow)
{
    // Setup UI:
    ui->setupUi(this);

    // Set image:
    ui->wLogoArea->setImage(":/icons/ico-blynklogo.png");

    // Vision aid overseas:
    connect(ui->wVisionAidOverseasButton, &QPushButton::clicked, this, &AboutWindow::onShowVisionAidOverseas);

    // Done:
    connect(ui->wAboutDoneButton, &QPushButton::clicked, this, &AboutWindow::onDone);
}

// Done:
void AboutWindow::onDone()
{
    onCloseButtonClicked();
}

// Show vision aid overseas dialog:
void AboutWindow::onShowVisionAidOverseas()
{
    QDesktopServices::openUrl(QUrl(VISION_AIDS_OVERSEA_URL));
}
