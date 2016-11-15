// Qt:
#include <QDebug>

// Application:
#include "parameters.h"

// Constructor:
Parameters::Parameters(QObject *parent) : QObject(parent)
{
    // Define parameters name:
    defineParameterName();

    // Define parameters default values:
    defineParameterDefaultValues();
}

// Set parameter:
void Parameters::setParameter(const Parameter &parameter, const QString &value)
{
    m_parameters[parameter] = value;
    emit parameterChanged(parameter);
}

// Return parameter:
QString Parameters::parameter(const Parameter &parameter) const
{
    return m_parameters[parameter];
}

// Return parameter name:
QString Parameters::parameterName(const Parameter &parameter) const
{
    return m_parameterName[parameter];
}

// Return parameter key:
Parameters::Parameter Parameters::parameterKey(const QString &sParameterName) const
{
    for (QMap<int, QString>::const_iterator it=m_parameterName.begin(); it!=m_parameterName.end(); ++it)
        if (it.value() == sParameterName)
            return (Parameters::Parameter)it.key();
    return Parameters::UNDEFINED;
}

// Show:
void Parameters::debug()
{
    for (QMap<int, QString>::iterator it=m_parameters.begin(); it!=m_parameters.end(); ++it)
        qDebug() << parameterName((Parameter)it.key()) << " : " << parameter((Parameter)it.key());
}

// Serialize:
CXMLNode Parameters::serialize()
{
    qDebug() << "*** SAVE PARAMETERS ***";
    CXMLNode xRootNode("Parameters");
    for (QMap<int, QString>::iterator it=m_parameters.begin(); it!=m_parameters.end(); ++it)
    {
        QString sParameterName = parameterName((Parameter)it.key());
        QString sParameterValue = it.value();

        CXMLNode xNode("Parameter");
        xNode.setTag(sParameterName);
        xNode.setValue(sParameterValue);
        xRootNode.addNode(xNode);
    }

    return xRootNode;
}

// Deserialize:
void Parameters::deserialize(const CXMLNode &xRootNode)
{
    qDebug() << "*** LOAD PARAMETERS ***";
    foreach (CXMLNode node, xRootNode.nodes())
    {
        QString sTagName = node.tag();
        Parameter parameter = parameterKey(sTagName);
        if (parameter != Parameters::UNDEFINED)
            m_parameters[parameter] = node.value();
    }
}

// Define parameters name:
void Parameters::defineParameterName()
{
    m_parameterName[BLYNK_CURSOR_REGULARITY] = "blynkCursorRegularity";
    m_parameterName[BLYNK_CURSOR_ENABLED] = "blynkCursorEnabled";
    m_parameterName[BLYNK_CURSOR_RANDOM_MODE] = "blynkCursorRandomMode";
    m_parameterName[BLYNK_PER_MINUTE_REGULAR] = "blynkPerMinuteRegular";
    m_parameterName[BLYNK_PER_MINUTE_RANDOM] = "blynkPerMinuteRandom";
    m_parameterName[BLYNK_CURSOR_MIN] = "blynkCursorMin";
    m_parameterName[BLYNK_CURSOR_MAX] = "blynkCursorMax";
    m_parameterName[BLYNK_PER_MINUTE_RANGE] = "blynkPerMinuteRange";
    m_parameterName[SCREEN_BREAK_STATE] = "enabled";
    m_parameterName[SCREEN_BREAK_STRENGTH] = "screenBreakStrength";
    m_parameterName[SCREEN_BREAK_MIN] = "screenBreakMin";
    m_parameterName[SCREEN_BREAK_MAX] = "screenBreakMax";
    m_parameterName[SCREEN_BREAK_REGULARITY] = "screenBreakRegularity";
    m_parameterName[BLUE_LIGHT_REDUCER_ENABLED] = "blueLightReducerEnabled";
    m_parameterName[BLUE_LIGHT_REDUCER_STRENGTH] = "blueLightReducerStrength";
    m_parameterName[BLUE_LIGHT_REDUCER_START_TIME] = "blueLightReducerStartTime";
    m_parameterName[START_BLYNK_AT_LOGIN] = "startBlynkAtLogin";
    m_parameterName[LIGHT_TEMPERATURE] = "lightTemperature";
    m_parameterName[MEDIUM_TEMPERATURE] = "mediumTemperature";
    m_parameterName[STRONG_TEMPERATURE] = "strongTemperature";
}

// Define parameter default value:
void Parameters::defineParameterDefaultValues()
{
    setParameter(BLYNK_CURSOR_REGULARITY, "4");
    setParameter(BLYNK_CURSOR_ENABLED, "1");
    setParameter(BLYNK_CURSOR_RANDOM_MODE, "0");
    setParameter(BLYNK_PER_MINUTE_REGULAR, "2");
    setParameter(BLYNK_CURSOR_MIN, "2");
    setParameter(BLYNK_CURSOR_MAX, "10");
    setParameter(BLYNK_PER_MINUTE_RANDOM, "8");
    setParameter(BLYNK_PER_MINUTE_RANGE, "6, 15");
    setParameter(SCREEN_BREAK_STATE, SCREEN_BREAK_ENABLED);
    setParameter(SCREEN_BREAK_STRENGTH, "1");
    setParameter(SCREEN_BREAK_MIN, "20");
    setParameter(SCREEN_BREAK_MAX, "100");
    setParameter(SCREEN_BREAK_REGULARITY, "30");
    setParameter(BLUE_LIGHT_REDUCER_ENABLED, "1");
    setParameter(BLUE_LIGHT_REDUCER_STRENGTH, "1");
    setParameter(BLUE_LIGHT_REDUCER_START_TIME, "09:00:00");
    setParameter(START_BLYNK_AT_LOGIN, "0");
    setParameter(LIGHT_TEMPERATURE, "4000");
    setParameter(MEDIUM_TEMPERATURE, "3000");
    setParameter(STRONG_TEMPERATURE, "2700");
}
