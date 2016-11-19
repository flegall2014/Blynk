#ifndef ANIMATEDCURSOR_H
#define ANIMATEDCURSOR_H

// Qt:
#include <QMovie>
#include <QCursor>
#include <QWidget>
#include <QTimer>

class AnimatedCursor : public QObject
{
    Q_OBJECT

public:
    // Constructor:
    AnimatedCursor(QWidget *parent=0);

    // Destructor:
    ~AnimatedCursor();

    // Play:
    void play();

private:
    // Load cursor images:
    void loadCursorImages();

private:
    // Movie:
    QVector<QImage> m_vCursorImages;

    // Timer:
    QTimer m_tTimer;

    // Current image index:
    int m_iCurrentImageIndex;

public slots:
    // Time out:
    void onTimeOut();

signals:
    void nextImage(const QImage &image, bool isLastImage);
    void done();
};

#endif // ANIMATEDCURSOR_H
