#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

// Application:
#include <QWidget>

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    // Constructor:
    explicit ImageWidget(QWidget *parent = 0);

    // Set image:
    void setImage(const QString &sImagePath);

    // Set text:
    void setText(const QString &sText, const QColor &color);

protected:
    // Paint event:
    void paintEvent(QPaintEvent *event);

private:
    // Image path:
    QString m_sImagePath;

    // Text:
    QString m_sText;

    // Text color:
    QColor m_textColor;
};

#endif // IMAGEWIDGET_H
