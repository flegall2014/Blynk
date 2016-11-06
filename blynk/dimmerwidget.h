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

    // Set RGB:
    void setRGB(unsigned short iMinRed, unsigned short iMaxRed,
        unsigned short iMinGreen, unsigned short iMaxGreen,
        unsigned short iMinBlue, unsigned short iMaxBlue);

    // Set strength:
    void setStrength(const Parameters::Strength &eStrength);

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

    // Min red:
    unsigned short m_iMinRed;

    // Max red:
    unsigned short m_iMaxRed;

    // Min green:
    unsigned short m_iMinGreen;

    // Max green:
    unsigned short m_iMaxGreen;

    // Min blue:
    unsigned short m_iMinBlue;

    // Max blue:
    unsigned short m_iMaxBlue;

public slots:
    // Next image available:
    void onNextImageAvailable(const QImage &image);

    // Done:
    void onDone();

    // Frame changed:
    void onFrameChanged(int iFrameIndex);

    // Update UI:
    void updateUI();
};

#endif // DIMMERWIDGET_H
