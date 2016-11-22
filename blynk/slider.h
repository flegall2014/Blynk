#ifndef SLIDER_H
#define SLIDER_H

#include <QWidget>

namespace Ui {
class Slider;
}

class Slider : public QWidget
{
    Q_OBJECT

public:
    // Constructor:
    explicit Slider(QWidget *parent = 0);

    // Destructor:
    ~Slider();

    // Set major ticks labels:
    void setMajorTickLabels(const QStringList &lTickLabels);

    // Set minor ticks:
    void setMinorTicks(int nMinor);

    // Set range:
    void setRange(int iMinValue, int iMaxValue);

    // Set value:
    void setValue(int iValue);

    // Set page step:
    void setPageStep(int iPageStep);

    // Set single step:
    void setSingleStep(int iSingleStep);

protected:
    // Paint event:
    virtual void paintEvent(QPaintEvent *event);

private:
    // UI:
    Ui::Slider *ui;

    // Tick labels:
    QStringList m_lTickLabels;

    // Number of minor ticks:
    int m_nMinorTicks;

    // Major tick color:
    QColor m_majorTickColor;

    // Minor tick color:
    QColor m_minorTickColor;

    // Horizontal margin:
    int m_iHorizontalMargin;

    // Label area height:
    int m_iLabelAreaHeight;

signals:
    // Value changed:
    void valueChanged(int iValue);
};

#endif // SLIDER_H
