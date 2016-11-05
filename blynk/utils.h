#ifndef UTILS_H
#define UTILS_H
#include <QDir>
#include <QList>

class Utils
{
public:
    // Return application dir:
    static QDir appDir();

    // Return random int in range:
    static int randInt(int iLow, int iHigh);

    // Generate random sequence:
    static QList<int> randomSequence(int iCount, int iLow, int iHigh);

    // Return files:
    static QStringList files(const QString &srcDir, const QStringList &lImageFilters);
};

#endif // UTILS_H
