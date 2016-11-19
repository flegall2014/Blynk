#ifndef BLRLIB_H
#define BLRLIB_H
#include "blrlib_global.h"
#include <QColor>

class BLRLIBSHARED_EXPORT Blrlib
{
public:
    // Return color for temperature:
    static QColor colorForTemperature(int iTemperature);
    static QColor colorForTemperature1(int kelvin);
};

#endif // BLRLIB_H
