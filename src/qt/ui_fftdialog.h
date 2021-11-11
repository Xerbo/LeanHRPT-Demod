/********************************************************************************
** Form generated from reading UI file 'fftdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FFTDIALOG_H
#define UI_FFTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include "qspectrum.h"
#include "qwaterfall.h"

QT_BEGIN_NAMESPACE

class Ui_FFTDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *sink;
    QSpectrum *spectrum;
    QWaterfall *waterfall;
    QVBoxLayout *settings;
    QLabel *label;
    QSlider *verticalSlider_2;
    QLabel *label_2;
    QSlider *verticalSlider;

    void setupUi(QDialog *FFTDialog)
    {
        if (FFTDialog->objectName().isEmpty())
            FFTDialog->setObjectName(QString::fromUtf8("FFTDialog"));
        FFTDialog->resize(698, 457);
        QIcon icon;
        icon.addFile(QString::fromUtf8("logo128.png"), QSize(), QIcon::Normal, QIcon::Off);
        FFTDialog->setWindowIcon(icon);
        horizontalLayout = new QHBoxLayout(FFTDialog);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        sink = new QVBoxLayout();
        sink->setSpacing(0);
        sink->setObjectName(QString::fromUtf8("sink"));
        spectrum = new QSpectrum(FFTDialog);
        spectrum->setObjectName(QString::fromUtf8("spectrum"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(spectrum->sizePolicy().hasHeightForWidth());
        spectrum->setSizePolicy(sizePolicy);

        sink->addWidget(spectrum);

        waterfall = new QWaterfall(FFTDialog);
        waterfall->setObjectName(QString::fromUtf8("waterfall"));
        sizePolicy.setHeightForWidth(waterfall->sizePolicy().hasHeightForWidth());
        waterfall->setSizePolicy(sizePolicy);

        sink->addWidget(waterfall);


        horizontalLayout->addLayout(sink);

        settings = new QVBoxLayout();
        settings->setObjectName(QString::fromUtf8("settings"));
        settings->setContentsMargins(6, 6, 6, 6);
        label = new QLabel(FFTDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        settings->addWidget(label);

        verticalSlider_2 = new QSlider(FFTDialog);
        verticalSlider_2->setObjectName(QString::fromUtf8("verticalSlider_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(verticalSlider_2->sizePolicy().hasHeightForWidth());
        verticalSlider_2->setSizePolicy(sizePolicy1);
        verticalSlider_2->setMinimum(10);
        verticalSlider_2->setMaximum(60);
        verticalSlider_2->setValue(50);
        verticalSlider_2->setOrientation(Qt::Vertical);
        verticalSlider_2->setTickPosition(QSlider::TicksBothSides);

        settings->addWidget(verticalSlider_2);

        label_2 = new QLabel(FFTDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        settings->addWidget(label_2);

        verticalSlider = new QSlider(FFTDialog);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        sizePolicy1.setHeightForWidth(verticalSlider->sizePolicy().hasHeightForWidth());
        verticalSlider->setSizePolicy(sizePolicy1);
        verticalSlider->setMinimum(-100);
        verticalSlider->setMaximum(0);
        verticalSlider->setValue(-70);
        verticalSlider->setOrientation(Qt::Vertical);
        verticalSlider->setTickPosition(QSlider::TicksBothSides);

        settings->addWidget(verticalSlider);


        horizontalLayout->addLayout(settings);


        retranslateUi(FFTDialog);

        QMetaObject::connectSlotsByName(FFTDialog);
    } // setupUi

    void retranslateUi(QDialog *FFTDialog)
    {
        FFTDialog->setWindowTitle(QCoreApplication::translate("FFTDialog", "FFT", nullptr));
        label->setText(QCoreApplication::translate("FFTDialog", "Range", nullptr));
        label_2->setText(QCoreApplication::translate("FFTDialog", "Offset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FFTDialog: public Ui_FFTDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FFTDIALOG_H
