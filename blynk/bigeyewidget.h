#ifndef BIGEYEWIDGET_H
#define BIGEYEWIDGET_H

// Qt:
#include <QLabel>
class QMovie;

class BigEyeWidget : public QLabel
{
    Q_OBJECT

public:
    // Constructor:
    explicit BigEyeWidget(const QString &sMoviePath, QWidget *parent = 0);

    // Set opacity:
    void setOpacity(int iOpacity);

protected:
    // Handle paint events:
    void paintEvent(QPaintEvent *event);

private:
    // Opacity:
    double m_iOpacity;

    // Movie:
    QMovie *m_pBigEyeMovie;

public slots:
    // Frame changed:
    void onFrameChanged(int iFrameIndex);
};

#endif // BIGEYEWIDGET_H
