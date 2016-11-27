#ifndef PARAMETERDEBUG_H
#define PARAMETERDEBUG_H
#include <QAbstractListModel>
class Parameters;

class ParameterDebug : public QAbstractListModel
{
public:
    // Constructor:
    ParameterDebug(QObject *parent=0);

    // Set parameters:
    void setParameters(Parameters *pParameters);

    // Row count:
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;

    // Data:
    virtual QVariant data(const QModelIndex &index, int role) const;

    // Reset model:
    void resetModel();

private:
    // Parameters:
    Parameters *m_pParameters;
};

#endif // PARAMETERDEBUG_H
