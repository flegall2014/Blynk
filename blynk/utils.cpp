// Qt:
#include <QCoreApplication>
#include <QDirIterator>
#include <QUrl>
#include <QFontMetrics>
#include <QTooltip>
#include <QFontDataBase>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QComboBox>
#include <QTimeEdit>
#include <QDebug>

// Application:
#include "utils.h"

// Bebas:
QString Utils::sBebasFont = "";

// Bell gothic std black:
QString Utils::sBellGothicStdBlackFont = "";

// Bell gothic std bold:
QString Utils::sBellGothicStdBoldFont = "";

// Bell gothic std bold:
QString Utils::sBellGothicStdLightFont = "";

// Lato regular:
QString Utils::sLatoRegularFont = "";

// Myriad pro:
QString Utils::sMyriadProFont = "";

// Return app dir:
QDir Utils::appDir()
{
    return QDir(QCoreApplication::applicationDirPath());
}

// Return random int in range:
int Utils::randInt(int iLow, int iHigh)
{
    // Random number between low and high
    return qrand() % ((iHigh + 1) - iLow) + iLow;
}

// Random sequence:
QList<int> Utils::randomSequence(int iCount, int iLow, int iHigh)
{
    QList<int> lOut;
    while (lOut.size() < iCount)
    {
        int iValue = randInt(iLow, iHigh);
        if (!lOut.contains(iValue))
            lOut << iValue;
    }
    qSort(lOut.begin(), lOut.end());
    return lOut;
}

// Get files  recursively:
QStringList Utils::files(const QString &srcDir, const QStringList &lImageFilters)
{
    QStringList lFiles;
    if (lImageFilters.isEmpty())
        return QStringList();

    QDirIterator it(srcDir, lImageFilters, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
        lFiles << it.next();

    return lFiles;
}

// Split tooltip:
QString Utils::splitTooltip(const QString &sText, int iWidth)
{
    QString sTmp = sText;
    QFontMetrics fm(QToolTip::font());
    QString result;

    for (;;) {
        int i = 0;
        while (i < sTmp.length()) {
            if (fm.width(sTmp.left(++i + 1)) > iWidth) {
                int j = sTmp.lastIndexOf(' ', i);
                if (j > 0)
                    i = j;
                result += sTmp.left(i);
                result += '\n';
                sTmp = sTmp.mid(i+1);
                break;
            }
        }
        if (i >= sTmp.length())
            break;
    }
    return result + sTmp;
}

// Load fonts:
void Utils::loadFonts()
{
    // Bebas font:
    int id = QFontDatabase::addApplicationFont(":/fonts/BEBAS___.ttf");
    QString sFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug() << id << sFamily;
    sBebasFont = sFamily;

    // Bell gothic std black:
    id = QFontDatabase::addApplicationFont(":/fonts/BellGothicStd-Black.otf");
    sFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug() << id << sFamily;
    sBellGothicStdBlackFont = sFamily;

    // Bell gothic std bold:
    id = QFontDatabase::addApplicationFont(":/fonts/BellGothicStd-Bold.otf");
    sFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug() << id << sFamily;
    sBellGothicStdBoldFont = sFamily;

    // Bell gothic std light:
    id = QFontDatabase::addApplicationFont(":/fonts/BellGothicStd-Light.otf");
    sFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug() << id << sFamily;
    sBellGothicStdLightFont = sFamily;

    // Lato regular:
    id = QFontDatabase::addApplicationFont(":/fonts/Lato-Regular.ttf");
    sFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug() << id << sFamily;
    sLatoRegularFont = sFamily;

    // Myriad pro:
    id = QFontDatabase::addApplicationFont(":/fonts/Myriad-Pro_31655.ttf");
    sFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug() << id << sFamily;
    sMyriadProFont = sFamily;
}

// Set font for target widget:
void Utils::setFontForWidget(QWidget *pRootWidget)
{
    if (pRootWidget)
    {
        QList<QWidget *> lWidgets = pRootWidget->findChildren<QWidget *>();
        foreach (QWidget *pWidget, lWidgets) {
            // Check class name:
            QString sClassName = pWidget->metaObject()->className();

            // Chec object name:
            QString sObjectName = pWidget->objectName();

            // Font:
            QFont font;
            font.setFamily(sLatoRegularFont);
            font.setPixelSize(FONT_18);

            // Set font based on class name and object name:
            if (sClassName.compare("QLABEL", Qt::CaseInsensitive) == 0) {
                QLabel *pLabel = dynamic_cast<QLabel *>(pWidget);
                if (pLabel) {
                    if ((sObjectName == "wBlynkCursorLabel") ||
                        (sObjectName == "wScreenBreakLabel") ||
                        (sObjectName == "wBlueLightReducerLabel"))
                    {
                        font.setFamily(sBebasFont);
                        font.setPixelSize(FONT_20);
                        font.setBold(true);
                    }
                    else
                    if ((sObjectName == "wAboutBlynkLabel") ||
                        (sObjectName == "wNotJustYourEyesLabel"))
                    {
                        font.setFamily(sLatoRegularFont);
                        font.setPixelSize(FONT_22);
                        font.setBold(true);
                    }
                    else
                    if ((sObjectName == "wAboutBlynkText") ||
                        (sObjectName == "wNotJustYourEyesText"))
                    {
                        font.setFamily("Calibri");
                        font.setPixelSize(FONT_15);
                    }
                    else
                    if (sObjectName == "wCopyRightLabel")
                        font.setPixelSize(FONT_12);
                    pLabel->setFont(font);
                }
            }
            else
            if (sClassName.compare("QPUSHBUTTON", Qt::CaseInsensitive) == 0) {
                QPushButton *pButton = dynamic_cast<QPushButton *>(pWidget);
                if (pButton) {
                    if (sObjectName == "wDoneButton") {
                        font.setBold(true);
                        font.setPointSize(FONT_14);
                    }
                    else
                    if (sObjectName == "wAboutDoneButton") {
                        font.setBold(true);
                        font.setPointSize(FONT_13);
                    }
                    else
                    if (sObjectName == "wVisionAidOverseasButton") {
                        font.setBold(true);
                        font.setPointSize(FONT_13);
                    }
                    pButton->setFont(font);
                }
            }
            else
            if (sClassName.compare("QCHECKBOX", Qt::CaseInsensitive) == 0) {
                QCheckBox *pCheckBox = dynamic_cast<QCheckBox *>(pWidget);
                if (pCheckBox)
                    pCheckBox->setFont(font);
            }
            else
            if (sClassName.compare("QRADIOBUTTON", Qt::CaseInsensitive) == 0) {
                QCheckBox *pCheckBox = dynamic_cast<QCheckBox *>(pWidget);
                if (pCheckBox)
                    pCheckBox->setFont(font);
            }
            else
            if (sClassName.contains("SLIDER", Qt::CaseInsensitive)) {
                QSlider *pSlider = dynamic_cast<QSlider *>(pWidget);
                if (pSlider)
                    pSlider->setFont(font);
            }
            else
            if (sClassName.compare("QCOMBOBOX", Qt::CaseInsensitive) == 0) {
                QComboBox *pCombo = dynamic_cast<QComboBox *>(pWidget);
                if (pCombo) {
                    font.setPixelSize(FONT_16);
                    pCombo->setFont(font);
                }
            }
            else
            if (sClassName.compare("QTIMEEDIT", Qt::CaseInsensitive) == 0) {
                QTimeEdit *pTimeEdit = dynamic_cast<QTimeEdit *>(pWidget);
                if (pTimeEdit) {
                    font.setPixelSize(FONT_18);
                    pTimeEdit->setFont(font);
                }
            }
        }
    }
}
