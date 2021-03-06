#ifndef PARAMETERS_H
#define PARAMETERS_H

// Qt:
#include <QObject>
#include <QMap>
#include <QVariant>

// Application:
#include <cxmlnode.h>

#define PARAMETER(x) m_parameters.parameter(x)

#define BLYNK_CURSOR_ENABLED "Enabled"
#define BLYNK_CURSOR_DISABLED "Disabled"
#define BLYNK_CURSOR_DISABLED_FOR_ONE_HOUR "Disabled For One Hour"
#define BLYNK_CURSOR_DISABLED_FOR_THREE_HOURS "Disabled For Three Hours"
#define BLYNK_CURSOR_DISABLED_UNTIL_TOMORROW "Disabled Until Tomorrow"

#define SCREEN_BREAK_ENABLED "Enabled"
#define SCREEN_BREAK_DISABLED "Disabled"
#define SCREEN_BREAK_DISABLED_FOR_ONE_HOUR "Disabled For One Hour"
#define SCREEN_BREAK_DISABLED_FOR_THREE_HOURS "Disabled For Three Hours"
#define SCREEN_BREAK_DISABLED_UNTIL_TOMORROW "Disabled Until Tomorrow"

#define BLUELIGHTREDUCER_ENABLED "Enabled"
#define BLUELIGHTREDUCER_DISABLED "Disabled"
#define BLUELIGHTREDUCER_DISABLED_FOR_ONE_HOUR "Disabled For One Hour"
#define BLUELIGHTREDUCER_DISABLED_FOR_THREE_HOURS "Disabled For Three Hours"
#define BLUELIGHTREDUCER_DISABLED_UNTIL_TOMORROW "Disabled Until Tomorrow"

#define ON "1"
#define OFF "0"

class Parameters : public QObject
{
    Q_OBJECT

public:
    enum Parameter {
        UNDEFINED=0,
        BLYNK_CURSOR_REGULARITY,
        BLYNK_CURSOR_STATE,
        BLYNK_CURSOR_RANDOM_MODE,
        BLYNK_PER_MINUTE_RANDOM,
        BLYNK_CURSOR_MIN,
        BLYNK_CURSOR_MAX,
        BLYNK_PER_MINUTE_RANGE,
        BLYNK_REGULARITY_RANGE,
        SCREEN_BREAK_REGULARITY,
        SCREEN_BREAK_STATE,
        SCREEN_BREAK_STRENGTH,
        SCREEN_BREAK_MIN,
        SCREEN_BREAK_MAX,
        BLUELIGHTREDUCER_STATE,
        BLUE_LIGHT_REDUCER_STRENGTH,
        BLUE_LIGHT_REDUCER_START_TIME,
        BLUE_LIGHT_REDUCER_ALWAYS_ON,
        START_BLYNK_AT_LOGIN,
        LIGHT_TEMPERATURE,
        MEDIUM_TEMPERATURE,
        STRONG_TEMPERATURE
    };

    enum Strength {LIGHT=0, MEDIUM, STRONG};

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

    // Return parameters count:
    int count() const;

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
