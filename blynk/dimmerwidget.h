#ifndef DIMMERWIDGET_H
#define DIMMERWIDGET_H

// Qt:
#include <QDialog>

// Application:
#include "dimmerwidget.h"
#include "parameters.h"

class QMovie;
class AnimatedCursor;
class Parameters;

namespace Ui {
class DimmerWidget;
}

class DimmerWidget : public QDialog
{
    Q_OBJECT

public:
    // Constructor:
    explicit DimmerWidget(const QString &sMoviePath, QWidget *parent = 0);

    // Destructor:
    ~DimmerWidget();

    // Set strength:
    bool setStrength(const Parameters::Strength &eStrength);

    // Set color:
    bool setColor(const QColor &startColor, const QColor &stopColor);

    // Set temperature:
    bool setTemperature(int iTemperature);

    // Return strength:
    const Parameters::Strength &strength() const;

    // Play cursor:
    void playCursor();

    // Play big eye:
    void playBigEye(const Parameters::Strength &eStrength);

    // Set parameters:
    void setParameters(Parameters *pParameters);

protected:
    // Paint event:
    virtual void paintEvent(QPaintEvent *event);

private:
    // UI:
    Ui::DimmerWidget *ui;

    // Movie:
    QMovie *m_pBigEyeMovie;

    // Animated cursor:
    AnimatedCursor *m_pAnimatedCursor;

    // Current cursor image:
    QImage m_currentCursorImage;

    // Done:
    bool m_bDone;

    // Strength:
    Parameters::Strength m_eStrength;

    // Parameters:
    Parameters *m_pParameters;

    // Start color:
    QColor m_startColor;

    // Stop color:
    QColor m_stopColor;

public slots:
    // Next image available:
    void onNextImageAvailable(const QImage &image);

    // Done:
    void onDone();

    // Frame changed:
    void onFrameChanged(int iFrameIndex);
};

#endif // DIMMERWIDGET_H
