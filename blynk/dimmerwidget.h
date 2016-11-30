#ifndef DIMMERWIDGET_H
#define DIMMERWIDGET_H

// Qt:
#include <QDialog>

// Application:
#include "dimmerwidget.h"
#include "parameters.h"

class AnimatedImage;
class Parameters;
class Monitor;

namespace Ui {
class DimmerWidget;
}

class DimmerWidget : public QDialog
{
    Q_OBJECT

public:
    // Constructor:
    explicit DimmerWidget(QWidget *parent = 0);

    // Destructor:
    ~DimmerWidget();

    // Set temperature:
    bool setTemperature(int iTemperature);

    // Return strength:
    const Parameters::Strength &strength() const;

    // Play cursor:
    void playCursor();

    // Play animated image:
    void playAnimatedImage(const Parameters::Strength &eStrength);

    // Set parameters:
    void setParameters(Parameters *pParameters);

    // Return monitor:
    Monitor *monitor();

protected:
    // Paint event:
    virtual void paintEvent(QPaintEvent *event);

private:
    // UI:
    Ui::DimmerWidget *ui;

    // Animated cursor:
    AnimatedImage *m_pAnimatedCursor;

    // Animated image:
    AnimatedImage *m_pAnimatedImage;

    // Current cursor image:
    QImage m_currentCursorImage;

    // Current animated image:
    QImage m_currentAnimatedImage;

    // Cursor done:
    bool m_bCursorDone;

    // Animated image done:
    bool m_bAnimatedImageDone;

    // Parameters:
    Parameters *m_pParameters;

    // Monitor:
    Monitor *m_pMonitor;

    // Support gamma ramp?
    bool m_bSupportGammaRamp;

public slots:
    // Next image available:
    void onNextCursorImageAvailable(const QImage &image);

    // Next animated image available:
    void onNextAnimatedImageAvailable(const QImage &image);

    // Cursor done:
    void onCursorDone();

    // Animated image done:
    void onAnimatedImageDone();
};

#endif // DIMMERWIDGET_H
