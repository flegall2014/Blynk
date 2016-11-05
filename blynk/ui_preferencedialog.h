/********************************************************************************
** Form generated from reading UI file 'preferencedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCEDIALOG_H
#define UI_PREFERENCEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "bluelightreducerwidget.h"
#include "customslider.h"
#include "imagewidget.h"

QT_BEGIN_NAMESPACE

class Ui_PreferenceDialog
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_8;
    CustomSlider *wBlynkCursorSlider;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_5;
    QCheckBox *wBlynkCursorEnabled;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *wBlynkRandomCheckbox;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QComboBox *wBlynkPerMinuteValues;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer_6;
    QLabel *wScreenBreakState;
    QCheckBox *wScreenBreakEnabled;
    QHBoxLayout *horizontalLayout_3;
    CustomSlider *wScreenBreakSlider;
    QComboBox *wScreenBreakStrengthCombo;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *pushButton_5;
    QSpacerItem *horizontalSpacer_7;
    QCheckBox *wBlueLightReducerEnabled;
    QHBoxLayout *horizontalLayout;
    BlueLightReducerWidget *wBlueLightReducerWidget;
    QLabel *label_8;
    QTimeEdit *wStartTimeEdit;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *wStartBlynkAtLogin;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *wDoneButton;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_7;
    ImageWidget *splashWidget;

    void setupUi(QDialog *PreferenceDialog)
    {
        if (PreferenceDialog->objectName().isEmpty())
            PreferenceDialog->setObjectName(QStringLiteral("PreferenceDialog"));
        PreferenceDialog->setWindowModality(Qt::NonModal);
        PreferenceDialog->resize(480, 544);
        PreferenceDialog->setMinimumSize(QSize(480, 544));
        PreferenceDialog->setMaximumSize(QSize(480, 544));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/ico-splash.png"), QSize(), QIcon::Normal, QIcon::Off);
        PreferenceDialog->setWindowIcon(icon);
        PreferenceDialog->setStyleSheet(QStringLiteral("QDialog#PreferenceDialog {background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(97, 168, 228, 255))}"));
        PreferenceDialog->setModal(false);
        gridLayout = new QGridLayout(PreferenceDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox_2 = new QGroupBox(PreferenceDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QFont font;
        font.setPointSize(11);
        font.setBold(false);
        font.setWeight(50);
        groupBox_2->setFont(font);
        groupBox_2->setStyleSheet(QLatin1String(" QGroupBox {\n"
" background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
" stop: 0 #E0E0E0, stop: 1 #FFFFFF);\n"
"};\n"
""));
        gridLayout_8 = new QGridLayout(groupBox_2);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        wBlynkCursorSlider = new CustomSlider(groupBox_2);
        wBlynkCursorSlider->setObjectName(QStringLiteral("wBlynkCursorSlider"));

        gridLayout_8->addWidget(wBlynkCursorSlider, 1, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/ico-question.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);

        horizontalLayout_9->addWidget(pushButton);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_5);

        wBlynkCursorEnabled = new QCheckBox(groupBox_2);
        wBlynkCursorEnabled->setObjectName(QStringLiteral("wBlynkCursorEnabled"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        wBlynkCursorEnabled->setFont(font1);

        horizontalLayout_9->addWidget(wBlynkCursorEnabled);


        verticalLayout_3->addLayout(horizontalLayout_9);


        gridLayout_8->addLayout(verticalLayout_3, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        wBlynkRandomCheckbox = new QCheckBox(groupBox_2);
        wBlynkRandomCheckbox->setObjectName(QStringLiteral("wBlynkRandomCheckbox"));
        wBlynkRandomCheckbox->setFont(font1);

        horizontalLayout_2->addWidget(wBlynkRandomCheckbox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);

        horizontalLayout_2->addWidget(label_3);

        wBlynkPerMinuteValues = new QComboBox(groupBox_2);
        wBlynkPerMinuteValues->setObjectName(QStringLiteral("wBlynkPerMinuteValues"));

        horizontalLayout_2->addWidget(wBlynkPerMinuteValues);


        gridLayout_8->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        wBlynkCursorSlider->raise();

        gridLayout->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox_3 = new QGroupBox(PreferenceDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setFont(font);
        groupBox_3->setStyleSheet(QLatin1String(" QGroupBox {\n"
" background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
" stop: 0 #E0E0E0, stop: 1 #FFFFFF);\n"
"}"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        pushButton_4 = new QPushButton(groupBox_3);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setIcon(icon1);

        horizontalLayout_10->addWidget(pushButton_4);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_6);

        wScreenBreakState = new QLabel(groupBox_3);
        wScreenBreakState->setObjectName(QStringLiteral("wScreenBreakState"));
        QFont font2;
        font2.setPointSize(10);
        wScreenBreakState->setFont(font2);

        horizontalLayout_10->addWidget(wScreenBreakState);

        wScreenBreakEnabled = new QCheckBox(groupBox_3);
        wScreenBreakEnabled->setObjectName(QStringLiteral("wScreenBreakEnabled"));
        wScreenBreakEnabled->setFont(font1);

        horizontalLayout_10->addWidget(wScreenBreakEnabled);


        verticalLayout_4->addLayout(horizontalLayout_10);


        gridLayout_3->addLayout(verticalLayout_4, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        wScreenBreakSlider = new CustomSlider(groupBox_3);
        wScreenBreakSlider->setObjectName(QStringLiteral("wScreenBreakSlider"));

        horizontalLayout_3->addWidget(wScreenBreakSlider);

        wScreenBreakStrengthCombo = new QComboBox(groupBox_3);
        wScreenBreakStrengthCombo->setObjectName(QStringLiteral("wScreenBreakStrengthCombo"));

        horizontalLayout_3->addWidget(wScreenBreakStrengthCombo);


        gridLayout_3->addLayout(horizontalLayout_3, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox_3, 2, 0, 1, 1);

        groupBox_4 = new QGroupBox(PreferenceDialog);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setFont(font);
        groupBox_4->setStyleSheet(QLatin1String(" QGroupBox {\n"
" background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
" stop: 0 #E0E0E0, stop: 1 #FFFFFF);\n"
"}"));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        pushButton_5 = new QPushButton(groupBox_4);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setIcon(icon1);

        horizontalLayout_11->addWidget(pushButton_5);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_7);

        wBlueLightReducerEnabled = new QCheckBox(groupBox_4);
        wBlueLightReducerEnabled->setObjectName(QStringLiteral("wBlueLightReducerEnabled"));
        wBlueLightReducerEnabled->setFont(font1);

        horizontalLayout_11->addWidget(wBlueLightReducerEnabled);


        verticalLayout_5->addLayout(horizontalLayout_11);


        gridLayout_4->addLayout(verticalLayout_5, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        wBlueLightReducerWidget = new BlueLightReducerWidget(groupBox_4);
        wBlueLightReducerWidget->setObjectName(QStringLiteral("wBlueLightReducerWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(wBlueLightReducerWidget->sizePolicy().hasHeightForWidth());
        wBlueLightReducerWidget->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(wBlueLightReducerWidget);

        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font1);

        horizontalLayout->addWidget(label_8);

        wStartTimeEdit = new QTimeEdit(groupBox_4);
        wStartTimeEdit->setObjectName(QStringLiteral("wStartTimeEdit"));

        horizontalLayout->addWidget(wStartTimeEdit);


        gridLayout_4->addLayout(horizontalLayout, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox_4, 3, 0, 1, 1);

        groupBox_5 = new QGroupBox(PreferenceDialog);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setFont(font);
        groupBox_5->setStyleSheet(QLatin1String(" QGroupBox {\n"
" background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
" stop: 0 #E0E0E0, stop: 1 #FFFFFF);\n"
"}"));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        wStartBlynkAtLogin = new QCheckBox(groupBox_5);
        wStartBlynkAtLogin->setObjectName(QStringLiteral("wStartBlynkAtLogin"));
        wStartBlynkAtLogin->setFont(font1);

        horizontalLayout_5->addWidget(wStartBlynkAtLogin);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        wDoneButton = new QPushButton(groupBox_5);
        wDoneButton->setObjectName(QStringLiteral("wDoneButton"));
        wDoneButton->setStyleSheet(QStringLiteral(""));

        horizontalLayout_5->addWidget(wDoneButton);


        gridLayout_5->addLayout(horizontalLayout_5, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_5, 4, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget = new QWidget(PreferenceDialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        splashWidget = new ImageWidget(widget);
        splashWidget->setObjectName(QStringLiteral("splashWidget"));
        splashWidget->setStyleSheet(QStringLiteral(""));

        horizontalLayout_7->addWidget(splashWidget);


        gridLayout_2->addLayout(horizontalLayout_7, 1, 0, 1, 1);


        verticalLayout->addWidget(widget);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(PreferenceDialog);

        QMetaObject::connectSlotsByName(PreferenceDialog);
    } // setupUi

    void retranslateUi(QDialog *PreferenceDialog)
    {
        PreferenceDialog->setWindowTitle(QApplication::translate("PreferenceDialog", "Preferences", 0));
        groupBox_2->setTitle(QApplication::translate("PreferenceDialog", "Blynk Cursor", 0));
        pushButton->setText(QString());
        wBlynkCursorEnabled->setText(QApplication::translate("PreferenceDialog", "Disable", 0));
        wBlynkRandomCheckbox->setText(QApplication::translate("PreferenceDialog", "Random", 0));
        label_3->setText(QApplication::translate("PreferenceDialog", "Blinks a minute", 0));
        groupBox_3->setTitle(QApplication::translate("PreferenceDialog", "Screen Break", 0));
        pushButton_4->setText(QString());
        wScreenBreakState->setText(QApplication::translate("PreferenceDialog", "TextLabel", 0));
        wScreenBreakEnabled->setText(QApplication::translate("PreferenceDialog", "Disable", 0));
        wScreenBreakStrengthCombo->clear();
        wScreenBreakStrengthCombo->insertItems(0, QStringList()
         << QApplication::translate("PreferenceDialog", "LIGHT", 0)
         << QApplication::translate("PreferenceDialog", "MEDIUM", 0)
         << QApplication::translate("PreferenceDialog", "STRONG", 0)
        );
        groupBox_4->setTitle(QApplication::translate("PreferenceDialog", "Blue Light Reducer", 0));
        pushButton_5->setText(QString());
        wBlueLightReducerEnabled->setText(QApplication::translate("PreferenceDialog", "Disable", 0));
        label_8->setText(QApplication::translate("PreferenceDialog", "Turn on at", 0));
        groupBox_5->setTitle(QApplication::translate("PreferenceDialog", "Starter", 0));
        wStartBlynkAtLogin->setText(QApplication::translate("PreferenceDialog", "Start Blynk at login", 0));
        wDoneButton->setText(QApplication::translate("PreferenceDialog", "DONE", 0));
    } // retranslateUi

};

namespace Ui {
    class PreferenceDialog: public Ui_PreferenceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCEDIALOG_H
