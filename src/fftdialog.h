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

#ifndef FFTDIALOG_H
#define FFTDIALOG_H

#include <QDialog>

#include "qt/ui_fftdialog.h"
#include "fft.h"
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class FFTDialog; }
QT_END_NAMESPACE

class FFTDialog : public QDialog {
    Q_OBJECT
    public:
        FFTDialog(QWidget *parent = nullptr);
        ~FFTDialog();
        void load_data(const std::complex<float> *data) {
            fft.work(data, fft_data.data());
            ui->spectrum->load_data(fft_data.data(), 2048);
            ui->waterfall->load(fft_data.data(), 2048);
            update();
        }
        void update() {
            float range = ui->verticalSlider_2->value();
            float offset = ui->verticalSlider->value();
            ui->spectrum->d_min = offset;
            ui->spectrum->d_max = offset+range;
            ui->spectrum->repaint();
            ui->waterfall->d_min = offset;
            ui->waterfall->d_max = offset+range;
            ui->waterfall->repaint();
        }

    private:
        Ui::FFTDialog *ui;
        FFT fft;
        std::vector<float> fft_data;
    private slots:
        void on_verticalSlider_valueChanged() {
            update();
        }
        void on_verticalSlider_2_valueChanged() {
            update();
        }
};

#endif