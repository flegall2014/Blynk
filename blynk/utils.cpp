// Qt:
#include <QCoreApplication>
#include <QDirIterator>
#include <QUrl>

// Application:
#include "utils.h"

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
