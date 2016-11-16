#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

// Qt:
#include <QWidget>

namespace Ui {
class CustomSlider;
}

class CustomSlider : public QWidget
{
    Q_OBJECT

public:
    // Constructor:
    explicit CustomSlider(QWidget *parent = 0);

    // Destructor:
    ~CustomSlider();

    // Set range:
    void setRange(int iMin, int iMax);

    // Set value:
    void setValue(int iValue);

    // Set title:
    void setTitle(const QString &sTitle);

    // Set orientation:
    void setOrientation(const Qt::Orientation &orientation);

    // Return value:
    int value() const;

private:
    // UI:
    Ui::CustomSlider *ui;

public slots:
    // Slider value changed:
    void onSliderValueChanged(int iValue);

signals:
    // Value changed:
    void valueChanged(int iValue);
};

#endif // CUSTOMSLIDER_H
