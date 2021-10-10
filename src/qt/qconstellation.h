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

#ifndef QT_QCONSTELLATION_H
#define QT_QCONSTELLATION_H

#include <QPainter>
#include <QResizeEvent>
#include <complex>
#include <vector>
#include "util/snr_estimator.h"

#define NUM_SYMBOLS 2048

// A basic constellation widget
class QConstellation : public QWidget {
    public:
        explicit QConstellation([[maybe_unused]] QWidget *parent)
            : symbols(NUM_SYMBOLS) { }

        void push_sample(std::complex<float> symbol) {
            symbols[n++] = symbol;
            if (n == NUM_SYMBOLS) n = 0;
        }
    private:
        SNREstimator snr_est;
        std::vector<std::complex<float>> symbols;
        size_t n = 0;

        virtual void resizeEvent(QResizeEvent *event) {
            event->accept();
            if (event->size().width() < event->size().height()) {
                QWidget::resize(event->size().width(), event->size().width());
            } else {
                QWidget::resize(event->size().height(), event->size().height());
            }
        }

        virtual void paintEvent([[maybe_unused]] QPaintEvent* p) override {
            QPainter painter(this);
            painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0));

            QPen pen;
            pen.setWidthF(width()/100.0f);
            pen.setColor(QColor(255, 255, 70));
            pen.setCapStyle(Qt::RoundCap);
            painter.setPen(pen);
            for (std::complex<float> &symbol : symbols) {
                painter.drawPoint(
                    (symbol.real()+1.0f) * width()/2,
                    (symbol.imag()+1.0f) * height()/2
                );
            }

            painter.setPen(QColor(70, 70, 70));
            double snr = snr_est.get_snr(symbols.data(), NUM_SYMBOLS);
            painter.drawText(5, height()-105, 100, 100, Qt::AlignBottom, QString("SNR: %1 dB").arg(QString::number(snr, 'f', 2)));
            painter.drawLine(width()/2, 0, width()/2, height());
        }
};

#endif
