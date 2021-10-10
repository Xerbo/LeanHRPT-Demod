/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qt/qconstellation.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *settingsBox;
    QFormLayout *formLayout;
    QLabel *inputFileLabel;
    QPushButton *inputFile;
    QLabel *fileTypeLabel;
    QComboBox *fileType;
    QLabel *fileFormatLabel;
    QComboBox *fileFormat;
    QLabel *sampleRateLabel;
    QDoubleSpinBox *sampleRate;
    QLabel *outputFileLabel;
    QPushButton *outputFile;
    QPushButton *startButton;
    QSpacerItem *verticalSpacer;
    QConstellation *constellation;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8("logo128.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        settingsBox = new QGroupBox(centralwidget);
        settingsBox->setObjectName(QString::fromUtf8("settingsBox"));
        formLayout = new QFormLayout(settingsBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        inputFileLabel = new QLabel(settingsBox);
        inputFileLabel->setObjectName(QString::fromUtf8("inputFileLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, inputFileLabel);

        inputFile = new QPushButton(settingsBox);
        inputFile->setObjectName(QString::fromUtf8("inputFile"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(inputFile->sizePolicy().hasHeightForWidth());
        inputFile->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::FieldRole, inputFile);

        fileTypeLabel = new QLabel(settingsBox);
        fileTypeLabel->setObjectName(QString::fromUtf8("fileTypeLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, fileTypeLabel);

        fileType = new QComboBox(settingsBox);
        fileType->addItem(QString());
        fileType->addItem(QString());
        fileType->setObjectName(QString::fromUtf8("fileType"));
        sizePolicy.setHeightForWidth(fileType->sizePolicy().hasHeightForWidth());
        fileType->setSizePolicy(sizePolicy);

        formLayout->setWidget(1, QFormLayout::FieldRole, fileType);

        fileFormatLabel = new QLabel(settingsBox);
        fileFormatLabel->setObjectName(QString::fromUtf8("fileFormatLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, fileFormatLabel);

        fileFormat = new QComboBox(settingsBox);
        fileFormat->addItem(QString());
        fileFormat->addItem(QString());
        fileFormat->addItem(QString());
        fileFormat->addItem(QString());
        fileFormat->addItem(QString());
        fileFormat->setObjectName(QString::fromUtf8("fileFormat"));
        fileFormat->setEnabled(false);
        sizePolicy.setHeightForWidth(fileFormat->sizePolicy().hasHeightForWidth());
        fileFormat->setSizePolicy(sizePolicy);

        formLayout->setWidget(2, QFormLayout::FieldRole, fileFormat);

        sampleRateLabel = new QLabel(settingsBox);
        sampleRateLabel->setObjectName(QString::fromUtf8("sampleRateLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, sampleRateLabel);

        sampleRate = new QDoubleSpinBox(settingsBox);
        sampleRate->setObjectName(QString::fromUtf8("sampleRate"));
        sampleRate->setEnabled(false);
        sizePolicy.setHeightForWidth(sampleRate->sizePolicy().hasHeightForWidth());
        sampleRate->setSizePolicy(sizePolicy);
        sampleRate->setMinimum(1.000000000000000);
        sampleRate->setMaximum(10.000000000000000);
        sampleRate->setValue(3.000000000000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, sampleRate);

        outputFileLabel = new QLabel(settingsBox);
        outputFileLabel->setObjectName(QString::fromUtf8("outputFileLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, outputFileLabel);

        outputFile = new QPushButton(settingsBox);
        outputFile->setObjectName(QString::fromUtf8("outputFile"));
        sizePolicy.setHeightForWidth(outputFile->sizePolicy().hasHeightForWidth());
        outputFile->setSizePolicy(sizePolicy);

        formLayout->setWidget(4, QFormLayout::FieldRole, outputFile);

        startButton = new QPushButton(settingsBox);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setEnabled(false);

        formLayout->setWidget(6, QFormLayout::SpanningRole, startButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(5, QFormLayout::LabelRole, verticalSpacer);


        horizontalLayout->addWidget(settingsBox);

        constellation = new QConstellation(centralwidget);
        constellation->setObjectName(QString::fromUtf8("constellation"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(constellation->sizePolicy().hasHeightForWidth());
        constellation->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(constellation);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 30));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "LeanHRPT Demod", nullptr));
        settingsBox->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        inputFileLabel->setText(QCoreApplication::translate("MainWindow", "Input File", nullptr));
        inputFile->setText(QCoreApplication::translate("MainWindow", "Select...", nullptr));
        fileTypeLabel->setText(QCoreApplication::translate("MainWindow", "File Type", nullptr));
        fileType->setItemText(0, QCoreApplication::translate("MainWindow", "wav", nullptr));
        fileType->setItemText(1, QCoreApplication::translate("MainWindow", "raw", nullptr));

        fileFormatLabel->setText(QCoreApplication::translate("MainWindow", "File Format", nullptr));
        fileFormat->setItemText(0, QCoreApplication::translate("MainWindow", "char", nullptr));
        fileFormat->setItemText(1, QCoreApplication::translate("MainWindow", "uchar", nullptr));
        fileFormat->setItemText(2, QCoreApplication::translate("MainWindow", "short", nullptr));
        fileFormat->setItemText(3, QCoreApplication::translate("MainWindow", "ushort", nullptr));
        fileFormat->setItemText(4, QCoreApplication::translate("MainWindow", "float", nullptr));

        sampleRateLabel->setText(QCoreApplication::translate("MainWindow", "Sample Rate (msps)", nullptr));
        outputFileLabel->setText(QCoreApplication::translate("MainWindow", "Output File", nullptr));
        outputFile->setText(QCoreApplication::translate("MainWindow", "Select...", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
