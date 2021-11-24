/*
 * LeanHRPT Demod
 * Copyright (C) 2021 Xerbo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDesktopServices>
#include <QMessageBox>

#include "qt/ui_mainwindow.h"
#include "dsp.h"
#include "fftdialog.h"

#define ABOUT_TEXT "<h2>LeanHRPT Decode</h2>\
An easy to use HRPT demodulator<br/>\
Licensed under GPL-3.0."

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    private:
        Ui::MainWindow *ui;
        FFTDialog *fft;

        bool isDemodulating = false;
        void closeEvent(QCloseEvent *event);

        Demodulator *demod;
        QTimer *timer;

        QString wavFilename;
        QString rawFilename;
        QString outputFilename;
    private slots:
        void on_startButton_clicked();

        void on_wavInput_clicked();
        void on_rawInput_clicked();
        void on_outputFile_clicked();

        void on_source_textActivated(const QString &text);
        void on_gain_valueChanged(int value);
        void on_bias_toggled(bool state) { demod->file->set_biastee(state); }
        void on_antenna_textActivated(const QString &text) { demod->file->set_antenna(text.toStdString()); }

        // menuFile
        void on_actionQuit_triggered() { QApplication::quit(); }
        // menuWindows
        void on_actionFFT_triggered() { fft->show(); }
        // menuHelp
        void on_actionDocumentation_triggered()  { QDesktopServices::openUrl(QUrl("https://github.com/Xerbo/LeanHRPT-Decode/wiki")); };
        void on_actionIssue_Tracker_triggered()  { QDesktopServices::openUrl(QUrl("https://github.com/Xerbo/LeanHRPT-Decode/issues")); };
        void on_actionAbout_LeanHRPT_triggered() { QMessageBox::about(this, "About LeanHRPT", QString("%1\nVersion: %2").arg(ABOUT_TEXT).arg(VERSION)); };
        void on_actionAbout_Qt_triggered()       { QMessageBox::aboutQt(this, "About Qt"); };
};

#endif // MAINWINDOW_H
