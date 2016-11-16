#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H
#include "customwindow.h"

namespace Ui {
class AboutWindow;
}

class AboutWindow : public CustomWindow
{
public:
    // Constructor:
    AboutWindow(const QString &sTitle, QWidget *parent=0);

private:
    Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_H
