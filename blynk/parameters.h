#ifndef PARAMETERS_H
#define PARAMETERS_H

// Qt:
#include <QObject>
#include <QMap>
#include <QVariant>

// Application:
#include <cxmlnode.h>

#define PARAMETER(x) m_parameters.parameter(x)

#define SCREEN_BREAK_ENABLED "Enabled"
#define SCREEN_BREAK_DISABLED "Disabled"
#define SCREEN_BREAK_DISABLED_FOR_ONE_HOUR "Disabled For One Hour"
#define SCREEN_BREAK_DISABLED_FOR_THREE_HOURS "Disabled For Three Hours"
#define SCREEN_BREAK_DISABLED_UNTIL_TOMORROW "Disabled Until Tomorrow"

class Parameters : public QObject
{
    Q_OBJECT

public:
    enum Parameter {
        UNDEFINED=0,
        BLYNK_CURSOR_REGULARITY,
        BLYNK_CURSOR_ENABLED,
        BLYNK_CURSOR_RANDOM_MODE,
        BLYNK_PER_MINUTE_REGULAR,
        BLYNK_PER_MINUTE_RANDOM,
        BLYNK_CURSOR_MIN,
        BLYNK_CURSOR_MAX,
        BLYNK_PER_MINUTE_RANGE,
        SCREEN_BREAK_REGULARITY,
        SCREEN_BREAK_STATE,
        SCREEN_BREAK_STRENGTH,
        SCREEN_BREAK_MIN,
        SCREEN_BREAK_MAX,
        BLUE_LIGHT_REDUCER_ENABLED,
        BLUE_LIGHT_REDUCER_STRENGTH,
        BLUE_LIGHT_REDUCER_START_TIME,
        START_BLYNK_AT_LOGIN,
        LIGHT_TEMPERATURE,
        MEDIUM_TEMPERATURE,
        STRONG_TEMPERATURE
    };

    enum Strength {NO_STRENGTH=0, LIGHT, MEDIUM, STRONG};

    // Constructor:
    Parameters(QObject *parent=0);

    // Set parameter:
    void setParameter(const Parameter &parameter, const QString &value);

    // Return parameter:
    QString parameter(const Parameter &parameter) const;

    // Return parameter name:
    QString parameterName(const Parameter &parameter) const;

    // Return parameter key:
    Parameter parameterKey(const QString &sParameterName) const;

    // Show parameters:
    void debug();

    // Serialize:
    CXMLNode serialize();

    // Deserialize:
    void deserialize(const CXMLNode &xRootNode);

private:
    // Define parameters name:
    void defineParameterName();

    // Define parameters default values:
    void defineParameterDefaultValues();

private:
    // Parameters:
    QMap<int, QString> m_parameters;
    QMap<int, QString> m_parameterName;

signals:
    // Parameter changed:
    void parameterChanged(const Parameter &parameter);
};

#endif // PARAMETERS_H
