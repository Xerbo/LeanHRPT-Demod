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
#include "io/sdr_impl.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    ui = new Ui::MainWindow;
    ui->setupUi(this);

    timer = new QTimer(this);
    QTimer::connect(timer, &QTimer::timeout, this, [this]() {
        if (!demod->is_running()) {
            demod->stop();
            delete demod;

            isDemodulating = false;
            ui->startButton->setText("Start");
            timer->stop();
        } else {
            std::vector<complex> &symbols = demod->symbols();
            for (size_t i = 0; i < ui->constellation->num_points()/2; i++) {
                ui->constellation->push_sample(symbols[i]);
            }

            ui->constellation->repaint();
            if (fft->isVisible()) {
                fft->load_data(demod->freq().data());
            }
        }
    });

    fft = new FFTDialog(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (isDemodulating) {
        QMessageBox confirm;
        confirm.setText("Are you sure you want to exit? There is currently a file being demodulated.");
        confirm.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        confirm.setDefaultButton(QMessageBox::Cancel);
        confirm.setIcon(QMessageBox::Warning);
        if (confirm.exec() == QMessageBox::Cancel) {
            event->ignore();
            return;
        } else {
            demod->stop();
        }
    }
}

void MainWindow::on_startButton_clicked() {
    if (ui->startButton->text() == "Start") {
        std::shared_ptr<FileReader> file;
        double samp_rate;

        if (ui->source->currentText() == "raw") {
            file = FileReader::choose_type(ui->rawFormat->currentText().toStdString(), rawFilename.toStdString());
            samp_rate = ui->sampleRate->value()*1e6;
        } else if (ui->source->currentText() == "WAV") {
            file = FileReader::choose_type("wav", wavFilename.toStdString());
            samp_rate = file->rate();
        } else {
            std::string device = ui->device->currentText().toStdString();
            file = std::make_shared<SDRSource>(device, ui->frequency->value()*1e6, ui->sdrSampleRate->value()*1e6);
            // Exact sample rate of the SDR, not the target
            samp_rate = file->rate();

            // Gain configuration
            SoapySDR::Range range = file->gain_range();
            ui->gain->setRange(range.minimum(), range.maximum());
            ui->gain->setValue(0);
            file->set_gain(0);

            ui->bias->setEnabled(file->has_biastee());

            ui->antenna->clear();
            for (const auto &antenna : file->antennas()) {
                ui->antenna->addItem(QString::fromStdString(antenna));
            }
            QString antenna = QString::fromStdString(file->antenna());
            ui->antenna->setCurrentIndex(ui->antenna->findText(antenna));
            ui->antenna->setEnabled(true);
        }

        if (ui->downlink->currentText() == "MetOp HRPT") {
            demod = new MetopDemodulator(samp_rate, std::move(file), ui->outputFile->text().toStdString());
            ui->constellation->set_lines(true, true);
            ui->constellation->num_points(4096);
        } else if (ui->downlink->currentText() == "FengYun HRPT") {
            demod = new FengyunDemodulator(samp_rate, std::move(file), ui->outputFile->text().toStdString());
            ui->constellation->set_lines(true, true);
            ui->constellation->num_points(4096);
        } else{
            demod = new PMDemodulator(samp_rate, std::move(file), ui->outputFile->text().toStdString());
            ui->constellation->set_lines(false, true);
            ui->constellation->num_points(2048);
        }

        isDemodulating = true;
        timer->start(1000.0f/30.0f);
        ui->startButton->setText("Stop");
        ui->gain->setEnabled(true);
    } else {
        QMessageBox confirm;
        confirm.setText("Are you sure you want to cancel?");
        confirm.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        confirm.setDefaultButton(QMessageBox::Cancel);
        confirm.setIcon(QMessageBox::Warning);
        if (confirm.exec() == QMessageBox::Cancel) {
            return;
        }

        demod->stop();
        delete demod;

        isDemodulating = false;
        timer->stop();
        ui->startButton->setText("Start");
        ui->gain->setEnabled(false);
        ui->bias->setEnabled(false);
        ui->antenna->setEnabled(false);
    }
}

void MainWindow::on_source_textActivated(const QString &text) {
    if (text == "WAV") { 
        ui->startButton->setEnabled(!outputFilename.isEmpty() && !wavFilename.isEmpty());
    } else if (text == "raw") {
        ui->startButton->setEnabled(!outputFilename.isEmpty() && !rawFilename.isEmpty());
    } else { // SDR
        ui->device->clear();
        SoapySDR::KwargsList results = SoapySDR::Device::enumerate();
        SoapySDR::Kwargs::iterator it;

        for (size_t i = 0; i < results.size(); i++) {
            std::string driver;
            std::string label;
            for(it = results[i].begin(); it != results[i].end(); it++){
                if (it->first == "driver") driver = it->second;
                if (it->first == "label") label = it->second;
            }

            if (driver != "audio") {
                ui->device->addItem(QString::fromStdString("driver=" + driver));
            }
        }

        ui->startButton->setEnabled(!outputFilename.isEmpty() && !ui->device->currentText().isEmpty());
    }
}

void MainWindow::on_gain_valueChanged(int value) {
    if (isDemodulating) {
        demod->file->set_gain(value);
    }
}

void MainWindow::on_wavInput_clicked() {
    QString _inputFilename = QFileDialog::getOpenFileName(this, "Select Input File", "", "Baseband (*.wav)");
    if (_inputFilename.isEmpty()) return;

    wavFilename = _inputFilename;
    ui->wavInput->setText(wavFilename);
    ui->startButton->setEnabled(!outputFilename.isEmpty() && !wavFilename.isEmpty());
}
void MainWindow::on_rawInput_clicked() {
    QString _inputFilename = QFileDialog::getOpenFileName(this, "Select Input File", "", "Baseband (*.bin *.raw)");
    if (_inputFilename.isEmpty()) return;

    rawFilename = _inputFilename;
    ui->rawInput->setText(rawFilename);
    ui->startButton->setEnabled(!outputFilename.isEmpty() && !rawFilename.isEmpty());
}
void MainWindow::on_outputFile_clicked() {
    QString _outputFilename;
    if (ui->downlink->currentText() != "NOAA/Meteor HRPT") {
        _outputFilename = QFileDialog::getSaveFileName(this, "Select Output File", "", "VCDUs (*.vcdu)");
    } else {
        _outputFilename = QFileDialog::getSaveFileName(this, "Select Output File", "", "Binary (*.bin)");
    }
    if (_outputFilename.isEmpty()) return;

    outputFilename = _outputFilename;
    ui->outputFile->setText(outputFilename);

    if (ui->source->currentText() == "WAV") { 
        ui->startButton->setEnabled(!outputFilename.isEmpty() && !wavFilename.isEmpty());
    } else if (ui->source->currentText() == "raw") {
        ui->startButton->setEnabled(!outputFilename.isEmpty() && !rawFilename.isEmpty());
    } else { // SDR
        ui->startButton->setEnabled(!outputFilename.isEmpty() && !ui->device->currentText().isEmpty());
    }
}
