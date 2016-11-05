/********************************************************************************
** Form generated from reading UI file 'dimmerwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIMMERWIDGET_H
#define UI_DIMMERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DimmerWidget
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *movieArea;

    void setupUi(QWidget *DimmerWidget)
    {
        if (DimmerWidget->objectName().isEmpty())
            DimmerWidget->setObjectName(QStringLiteral("DimmerWidget"));
        DimmerWidget->resize(512, 512);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DimmerWidget->sizePolicy().hasHeightForWidth());
        DimmerWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(DimmerWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        movieArea = new QLabel(DimmerWidget);
        movieArea->setObjectName(QStringLiteral("movieArea"));
        movieArea->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(movieArea);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(DimmerWidget);

        QMetaObject::connectSlotsByName(DimmerWidget);
    } // setupUi

    void retranslateUi(QWidget *DimmerWidget)
    {
        DimmerWidget->setWindowTitle(QApplication::translate("DimmerWidget", "Form", 0));
        movieArea->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DimmerWidget: public Ui_DimmerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIMMERWIDGET_H
