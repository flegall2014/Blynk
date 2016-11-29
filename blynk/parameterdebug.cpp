#include "parameterdebug.h"
#include "parameters.h"

// Constructor:
ParameterDebug::ParameterDebug(QObject *parent) : QAbstractListModel(parent),
    m_pParameters(NULL)
{

}

// Set parameters:
void ParameterDebug::setParameters(Parameters *pParameters)
{
    beginResetModel();
    m_pParameters = pParameters;
    endResetModel();
}

// Row count:
int ParameterDebug::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_pParameters ? m_pParameters->count() : 0;
}

// Data:
QVariant ParameterDebug::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if ((index.row() < 0) || (index.row() > (rowCount()-1)))
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        Parameters::Parameter eParam = (Parameters::Parameter)index.row();
        QString sParameterName = m_pParameters->parameterName(eParam);
        QString sParameterValue = m_pParameters->parameter(eParam);
        QString sLabel = QString("%1 : %2").arg(sParameterName).arg(sParameterValue);
        return sLabel;
    }
    return QVariant();
}

// Reset model:
void ParameterDebug::resetModel()
{
    beginResetModel();
    endResetModel();
}
