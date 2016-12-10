#ifndef BLUELIGHTREDUCERSLIDER_H
#define BLUELIGHTREDUCERSLIDER_H

// Qt:
#include <QSlider>
class PaintEvent;

class BlueLightReducerSlider : public QSlider
{
public:
    // Constructor:
    BlueLightReducerSlider(QWidget *parent=NULL);

protected:
    // Paint event:
    virtual void paintEvent(QPaintEvent *event);

    // Mouse press event:
    virtual void mousePressEvent(QMouseEvent *event);

    // Resize event:
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // BLUELIGHTREDUCERSLIDER_H
