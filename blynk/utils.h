#ifndef UTILS_H
#define UTILS_H

// Qt:
#include <QDir>
#include <QList>
#include <QFont>

#define FONT_12 12
#define FONT_13 13
#define FONT_14 14
#define FONT_15 15
#define FONT_16 16
#define FONT_17 17
#define FONT_18 18
#define FONT_19 19
#define FONT_20 20
#define FONT_21 21
#define FONT_22 22
#define FONT_23 23
#define FONT_24 24

class Utils
{
public:
    enum WidgetType {LABEL=0, BUTTON, COMBO, SLIDER};

    // Return application dir:
    static QDir appDir();

    // Return random int in range:
    static int randInt(int iLow, int iHigh);

    // Generate random sequence:
    static QList<int> randomSequence(int iCount, int iLow, int iHigh);

    // Return files:
    static QStringList files(const QString &srcDir, const QStringList &lImageFilters);

    // Split tooltip:
    static QString splitTooltip(const QString &sText, int iWidth);

    // Set font for target widget:
    static void setFontForWidget(QWidget *pRootWidget);

    // Load fonts:
    static void loadFonts();

    // Open-SANS regular:
    static QString sOpenSansRegular;
};

#endif // UTILS_H
