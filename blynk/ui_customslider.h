/********************************************************************************
** Form generated from reading UI file 'customslider.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMSLIDER_H
#define UI_CUSTOMSLIDER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomSlider
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSlider *wSlider;
    QLabel *wSliderValue;
    QLabel *wSliderTitle;

    void setupUi(QWidget *CustomSlider)
    {
        if (CustomSlider->objectName().isEmpty())
            CustomSlider->setObjectName(QStringLiteral("CustomSlider"));
        CustomSlider->resize(400, 300);
        gridLayout = new QGridLayout(CustomSlider);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        wSlider = new QSlider(CustomSlider);
        wSlider->setObjectName(QStringLiteral("wSlider"));
        wSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(wSlider);

        wSliderValue = new QLabel(CustomSlider);
        wSliderValue->setObjectName(QStringLiteral("wSliderValue"));
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        wSliderValue->setFont(font);

        horizontalLayout->addWidget(wSliderValue);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        wSliderTitle = new QLabel(CustomSlider);
        wSliderTitle->setObjectName(QStringLiteral("wSliderTitle"));
        wSliderTitle->setFont(font);

        gridLayout->addWidget(wSliderTitle, 0, 0, 1, 1);


        retranslateUi(CustomSlider);

        QMetaObject::connectSlotsByName(CustomSlider);
    } // setupUi

    void retranslateUi(QWidget *CustomSlider)
    {
        CustomSlider->setWindowTitle(QApplication::translate("CustomSlider", "Form", 0));
        wSliderValue->setText(QApplication::translate("CustomSlider", "TextLabel", 0));
        wSliderTitle->setText(QApplication::translate("CustomSlider", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class CustomSlider: public Ui_CustomSlider {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMSLIDER_H
