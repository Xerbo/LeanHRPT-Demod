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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    ui = new Ui::MainWindow;
    ui->setupUi(this);

    timer = new QTimer(this);
    QTimer::connect(timer, &QTimer::timeout, this, [this]() {
        if (demod != nullptr && demod->running == false) {
            ui->startButton->setText("Start");
            demod->running = false;
            timer->stop();
        }
        for (size_t i = 0; i < ui->constellation->num_points()/2; i++) {
            ui->constellation->push_sample(demod->symbols[i]);
        }
        ui->constellation->repaint();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (demod != nullptr && demod->running == true) {
        QMessageBox confirm;
        confirm.setText("Are you sure you want to exit? There is currently a file being demodulated.");
        confirm.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        confirm.setDefaultButton(QMessageBox::Cancel);
        confirm.setIcon(QMessageBox::Warning);
        if (confirm.exec() == QMessageBox::Cancel) {
            event->ignore();
            return;
        } else {
            demod->running = false;
        }
    }
}

void MainWindow::on_fileType_textActivated(QString text) {
    ui->fileFormat->setEnabled(text == "raw");
    ui->sampleRate->setEnabled(text == "raw");
}

void MainWindow::on_startButton_clicked() {
    if (ui->startButton->text() == "Start") {
        std::shared_ptr<FileReader> file;
        float samp_rate;

        if (ui->fileType->currentText() == "wav") {
            file = FileReader::choose_type("wav", inputFilename.toStdString());
            samp_rate = file->sample_rate();
        } else {
            file = FileReader::choose_type(ui->fileFormat->currentText().toStdString(), inputFilename.toStdString());
            samp_rate = ui->sampleRate->value()*1e6;
        }

        if (ui->downlink->currentText() == "MetOp HRPT") {
            demod = new QPSKDemodulator(samp_rate, std::move(file), ui->outputFile->text().toStdString());
            ui->constellation->set_lines(true, true);
            ui->constellation->num_points(4096);
        } else {
            demod = new PMDemodulator(samp_rate, std::move(file), ui->outputFile->text().toStdString());
            ui->constellation->set_lines(false, true);
            ui->constellation->num_points(2048);
        }

        timer->start(1000.0f/30.0f);
        ui->startButton->setText("Stop");
    } else {
        QMessageBox confirm;
        confirm.setText("Are you sure you want to cancel?");
        confirm.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        confirm.setDefaultButton(QMessageBox::Cancel);
        confirm.setIcon(QMessageBox::Warning);
        if (confirm.exec() == QMessageBox::Cancel) {
            return;
        }

        demod->running = false;
        timer->stop();
        ui->startButton->setText("Start");
    }
}

void MainWindow::on_inputFile_clicked() {
    inputFilename = QFileDialog::getOpenFileName(this, "Select Input File", "", "Baseband (*.wav *.raw)");
    if (inputFilename.isEmpty()) return;

    ui->inputFile->setText(inputFilename);
    ui->startButton->setEnabled(!outputFilename.isEmpty() && !inputFilename.isEmpty());
}
void MainWindow::on_outputFile_clicked() {
    if (ui->downlink->currentText() == "MetOp HRPT") {
        outputFilename = QFileDialog::getSaveFileName(this, "Select Output File", "", "VCDUs (*.vcdu)");
    } else {
        outputFilename = QFileDialog::getSaveFileName(this, "Select Output File", "", "Binary (*.bin)");
    }
    if (outputFilename.isEmpty()) return;

    ui->outputFile->setText(outputFilename);
    ui->startButton->setEnabled(!outputFilename.isEmpty() && !inputFilename.isEmpty());
}
