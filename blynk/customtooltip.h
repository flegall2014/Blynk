#ifndef CUSTOMTOOLTIP_H
#define CUSTOMTOOLTIP_H

// Qt:
#include <QWidget>

namespace Ui {
class CustomTooltip;
}

class CustomTooltip : public QWidget
{
    Q_OBJECT

public:
    // Constructor:
    explicit CustomTooltip(QWidget *parent = 0);

    // Destructor:
    ~CustomTooltip();

    // Set text:
    void setText(const QString &sTooltipText);

private:
    // UI:
    Ui::CustomTooltip *ui;
};

#endif // CUSTOMTOOLTIP_H
