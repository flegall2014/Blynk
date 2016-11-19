#ifndef ANIMATEDIMAGE_H
#define ANIMATEDIMAGE_H

// Qt:
#include <QMovie>
#include <QCursor>
#include <QWidget>
#include <QTimer>
#include <QDir>

class AnimatedImage : public QObject
{
    Q_OBJECT

public:
    // Constructor:
    AnimatedImage(QWidget *parent=0);

    // Destructor:
    ~AnimatedImage();

    // Play:
    void play();

    // Load cursor images:
    void loadCursorImages(const QDir &imgDir);

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

#endif // ANIMATEDIMAGE_H