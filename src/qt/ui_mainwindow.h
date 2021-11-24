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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qt/qconstellation.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionFFT;
    QAction *actionDocumentation;
    QAction *actionIssue_Tracker;
    QAction *actionAbout_LeanHRPT;
    QAction *actionAbout_Qt;
    QAction *actionQuit;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *settingsBox;
    QFormLayout *formLayout;
    QLabel *sourceLabel;
    QComboBox *source;
    QLabel *downlinkLabel;
    QComboBox *downlink;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout_2;
    QLabel *rawInputLabel;
    QPushButton *rawInput;
    QLabel *rawFormatLabel;
    QComboBox *rawFormat;
    QLabel *sampleRateLabel;
    QDoubleSpinBox *sampleRate;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_3;
    QLabel *wavInputLabel;
    QPushButton *wavInput;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout_4;
    QLabel *deviceLabel;
    QLabel *frequencyLabel;
    QComboBox *device;
    QDoubleSpinBox *frequency;
    QLabel *sdrSampleRateLabel;
    QDoubleSpinBox *sdrSampleRate;
    QSlider *gain;
    QLabel *gainLabel;
    QCheckBox *bias;
    QLabel *biasLabel;
    QLabel *outputFileLabel;
    QPushButton *outputFile;
    QPushButton *startButton;
    QSpacerItem *verticalSpacer;
    QConstellation *constellation;
    QMenuBar *menubar;
    QMenu *menuWindows;
    QMenu *menuHelp;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8("logo128.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionFFT = new QAction(MainWindow);
        actionFFT->setObjectName(QString::fromUtf8("actionFFT"));
        actionDocumentation = new QAction(MainWindow);
        actionDocumentation->setObjectName(QString::fromUtf8("actionDocumentation"));
        actionIssue_Tracker = new QAction(MainWindow);
        actionIssue_Tracker->setObjectName(QString::fromUtf8("actionIssue_Tracker"));
        actionAbout_LeanHRPT = new QAction(MainWindow);
        actionAbout_LeanHRPT->setObjectName(QString::fromUtf8("actionAbout_LeanHRPT"));
        actionAbout_Qt = new QAction(MainWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        settingsBox = new QGroupBox(centralwidget);
        settingsBox->setObjectName(QString::fromUtf8("settingsBox"));
        formLayout = new QFormLayout(settingsBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        sourceLabel = new QLabel(settingsBox);
        sourceLabel->setObjectName(QString::fromUtf8("sourceLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, sourceLabel);

        source = new QComboBox(settingsBox);
        source->addItem(QString());
        source->addItem(QString());
        source->addItem(QString());
        source->setObjectName(QString::fromUtf8("source"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(source->sizePolicy().hasHeightForWidth());
        source->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::FieldRole, source);

        downlinkLabel = new QLabel(settingsBox);
        downlinkLabel->setObjectName(QString::fromUtf8("downlinkLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, downlinkLabel);

        downlink = new QComboBox(settingsBox);
        downlink->addItem(QString());
        downlink->addItem(QString());
        downlink->addItem(QString());
        downlink->setObjectName(QString::fromUtf8("downlink"));
        sizePolicy.setHeightForWidth(downlink->sizePolicy().hasHeightForWidth());
        downlink->setSizePolicy(sizePolicy);

        formLayout->setWidget(1, QFormLayout::FieldRole, downlink);

        stackedWidget = new QStackedWidget(settingsBox);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout = new QVBoxLayout(page);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(page);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        formLayout_2 = new QFormLayout(groupBox);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        rawInputLabel = new QLabel(groupBox);
        rawInputLabel->setObjectName(QString::fromUtf8("rawInputLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, rawInputLabel);

        rawInput = new QPushButton(groupBox);
        rawInput->setObjectName(QString::fromUtf8("rawInput"));
        sizePolicy.setHeightForWidth(rawInput->sizePolicy().hasHeightForWidth());
        rawInput->setSizePolicy(sizePolicy);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, rawInput);

        rawFormatLabel = new QLabel(groupBox);
        rawFormatLabel->setObjectName(QString::fromUtf8("rawFormatLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, rawFormatLabel);

        rawFormat = new QComboBox(groupBox);
        rawFormat->addItem(QString());
        rawFormat->addItem(QString());
        rawFormat->addItem(QString());
        rawFormat->addItem(QString());
        rawFormat->addItem(QString());
        rawFormat->setObjectName(QString::fromUtf8("rawFormat"));
        sizePolicy.setHeightForWidth(rawFormat->sizePolicy().hasHeightForWidth());
        rawFormat->setSizePolicy(sizePolicy);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, rawFormat);

        sampleRateLabel = new QLabel(groupBox);
        sampleRateLabel->setObjectName(QString::fromUtf8("sampleRateLabel"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, sampleRateLabel);

        sampleRate = new QDoubleSpinBox(groupBox);
        sampleRate->setObjectName(QString::fromUtf8("sampleRate"));
        sizePolicy.setHeightForWidth(sampleRate->sizePolicy().hasHeightForWidth());
        sampleRate->setSizePolicy(sizePolicy);
        sampleRate->setMinimum(1.000000000000000);
        sampleRate->setMaximum(10.000000000000000);
        sampleRate->setValue(3.000000000000000);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, sampleRate);


        verticalLayout->addWidget(groupBox);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_2 = new QVBoxLayout(page_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox_2 = new QGroupBox(page_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        formLayout_3 = new QFormLayout(groupBox_2);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        wavInputLabel = new QLabel(groupBox_2);
        wavInputLabel->setObjectName(QString::fromUtf8("wavInputLabel"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, wavInputLabel);

        wavInput = new QPushButton(groupBox_2);
        wavInput->setObjectName(QString::fromUtf8("wavInput"));
        sizePolicy.setHeightForWidth(wavInput->sizePolicy().hasHeightForWidth());
        wavInput->setSizePolicy(sizePolicy);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, wavInput);


        verticalLayout_2->addWidget(groupBox_2);

        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        verticalLayout_3 = new QVBoxLayout(page_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        groupBox_3 = new QGroupBox(page_3);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        formLayout_4 = new QFormLayout(groupBox_3);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        deviceLabel = new QLabel(groupBox_3);
        deviceLabel->setObjectName(QString::fromUtf8("deviceLabel"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, deviceLabel);

        frequencyLabel = new QLabel(groupBox_3);
        frequencyLabel->setObjectName(QString::fromUtf8("frequencyLabel"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, frequencyLabel);

        device = new QComboBox(groupBox_3);
        device->setObjectName(QString::fromUtf8("device"));
        sizePolicy.setHeightForWidth(device->sizePolicy().hasHeightForWidth());
        device->setSizePolicy(sizePolicy);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, device);

        frequency = new QDoubleSpinBox(groupBox_3);
        frequency->setObjectName(QString::fromUtf8("frequency"));
        sizePolicy.setHeightForWidth(frequency->sizePolicy().hasHeightForWidth());
        frequency->setSizePolicy(sizePolicy);
        frequency->setMaximum(2000.000000000000000);
        frequency->setValue(1700.000000000000000);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, frequency);

        sdrSampleRateLabel = new QLabel(groupBox_3);
        sdrSampleRateLabel->setObjectName(QString::fromUtf8("sdrSampleRateLabel"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, sdrSampleRateLabel);

        sdrSampleRate = new QDoubleSpinBox(groupBox_3);
        sdrSampleRate->setObjectName(QString::fromUtf8("sdrSampleRate"));
        sizePolicy.setHeightForWidth(sdrSampleRate->sizePolicy().hasHeightForWidth());
        sdrSampleRate->setSizePolicy(sizePolicy);
        sdrSampleRate->setMinimum(1.000000000000000);
        sdrSampleRate->setMaximum(10.000000000000000);
        sdrSampleRate->setValue(3.000000000000000);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, sdrSampleRate);

        gain = new QSlider(groupBox_3);
        gain->setObjectName(QString::fromUtf8("gain"));
        gain->setEnabled(false);
        gain->setMaximum(40);
        gain->setOrientation(Qt::Horizontal);

        formLayout_4->setWidget(3, QFormLayout::FieldRole, gain);

        gainLabel = new QLabel(groupBox_3);
        gainLabel->setObjectName(QString::fromUtf8("gainLabel"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, gainLabel);

        bias = new QCheckBox(groupBox_3);
        bias->setObjectName(QString::fromUtf8("bias"));
        bias->setEnabled(false);

        formLayout_4->setWidget(4, QFormLayout::FieldRole, bias);

        biasLabel = new QLabel(groupBox_3);
        biasLabel->setObjectName(QString::fromUtf8("biasLabel"));

        formLayout_4->setWidget(4, QFormLayout::LabelRole, biasLabel);


        verticalLayout_3->addWidget(groupBox_3);

        stackedWidget->addWidget(page_3);

        formLayout->setWidget(2, QFormLayout::SpanningRole, stackedWidget);

        outputFileLabel = new QLabel(settingsBox);
        outputFileLabel->setObjectName(QString::fromUtf8("outputFileLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, outputFileLabel);

        outputFile = new QPushButton(settingsBox);
        outputFile->setObjectName(QString::fromUtf8("outputFile"));
        sizePolicy.setHeightForWidth(outputFile->sizePolicy().hasHeightForWidth());
        outputFile->setSizePolicy(sizePolicy);

        formLayout->setWidget(3, QFormLayout::FieldRole, outputFile);

        startButton = new QPushButton(settingsBox);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setEnabled(false);

        formLayout->setWidget(5, QFormLayout::SpanningRole, startButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(4, QFormLayout::FieldRole, verticalSpacer);


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
        menuWindows = new QMenu(menubar);
        menuWindows->setObjectName(QString::fromUtf8("menuWindows"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuWindows->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuWindows->addAction(actionFFT);
        menuHelp->addAction(actionDocumentation);
        menuHelp->addAction(actionIssue_Tracker);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout_LeanHRPT);
        menuHelp->addAction(actionAbout_Qt);
        menuFile->addAction(actionQuit);

        retranslateUi(MainWindow);
        QObject::connect(source, SIGNAL(activated(int)), stackedWidget, SLOT(setCurrentIndex(int)));

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "LeanHRPT Demod", nullptr));
        actionFFT->setText(QCoreApplication::translate("MainWindow", "FFT", nullptr));
        actionDocumentation->setText(QCoreApplication::translate("MainWindow", "Documentation", nullptr));
        actionIssue_Tracker->setText(QCoreApplication::translate("MainWindow", "Issue Tracker", nullptr));
        actionAbout_LeanHRPT->setText(QCoreApplication::translate("MainWindow", "About LeanHRPT", nullptr));
        actionAbout_Qt->setText(QCoreApplication::translate("MainWindow", "About Qt", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        settingsBox->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        sourceLabel->setText(QCoreApplication::translate("MainWindow", "Source", nullptr));
        source->setItemText(0, QCoreApplication::translate("MainWindow", "raw", nullptr));
        source->setItemText(1, QCoreApplication::translate("MainWindow", "WAV", nullptr));
        source->setItemText(2, QCoreApplication::translate("MainWindow", "SDR", nullptr));

        downlinkLabel->setText(QCoreApplication::translate("MainWindow", "Downlink", nullptr));
        downlink->setItemText(0, QCoreApplication::translate("MainWindow", "NOAA/Meteor HRPT", nullptr));
        downlink->setItemText(1, QCoreApplication::translate("MainWindow", "MetOp HRPT", nullptr));
        downlink->setItemText(2, QCoreApplication::translate("MainWindow", "FengYun HRPT", nullptr));

        groupBox->setTitle(QCoreApplication::translate("MainWindow", "raw", nullptr));
        rawInputLabel->setText(QCoreApplication::translate("MainWindow", "Input File", nullptr));
        rawInput->setText(QCoreApplication::translate("MainWindow", "Select...", nullptr));
        rawFormatLabel->setText(QCoreApplication::translate("MainWindow", "Format", nullptr));
        rawFormat->setItemText(0, QCoreApplication::translate("MainWindow", "char", nullptr));
        rawFormat->setItemText(1, QCoreApplication::translate("MainWindow", "uchar", nullptr));
        rawFormat->setItemText(2, QCoreApplication::translate("MainWindow", "short", nullptr));
        rawFormat->setItemText(3, QCoreApplication::translate("MainWindow", "ushort", nullptr));
        rawFormat->setItemText(4, QCoreApplication::translate("MainWindow", "float", nullptr));

        sampleRateLabel->setText(QCoreApplication::translate("MainWindow", "Sample Rate", nullptr));
        sampleRate->setSuffix(QCoreApplication::translate("MainWindow", " msps", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "WAV", nullptr));
        wavInputLabel->setText(QCoreApplication::translate("MainWindow", "Input File", nullptr));
        wavInput->setText(QCoreApplication::translate("MainWindow", "Select...", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "SDR", nullptr));
        deviceLabel->setText(QCoreApplication::translate("MainWindow", "Device", nullptr));
        frequencyLabel->setText(QCoreApplication::translate("MainWindow", "Frequency", nullptr));
        frequency->setSuffix(QCoreApplication::translate("MainWindow", " MHz", nullptr));
        sdrSampleRateLabel->setText(QCoreApplication::translate("MainWindow", "Sample Rate", nullptr));
        sdrSampleRate->setSuffix(QCoreApplication::translate("MainWindow", " msps", nullptr));
        gainLabel->setText(QCoreApplication::translate("MainWindow", "Gain", nullptr));
        bias->setText(QString());
        biasLabel->setText(QCoreApplication::translate("MainWindow", "Enable Bias Tee", nullptr));
        outputFileLabel->setText(QCoreApplication::translate("MainWindow", "Output File", nullptr));
        outputFile->setText(QCoreApplication::translate("MainWindow", "Select...", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        menuWindows->setTitle(QCoreApplication::translate("MainWindow", "Windows", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
