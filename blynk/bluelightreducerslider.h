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
};

#endif // BLUELIGHTREDUCERSLIDER_H
