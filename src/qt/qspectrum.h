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

#ifndef QT_QSPECTRUM_H
#define QT_QSPECTRUM_H

#include <QWidget>
#include <QPainter>

#include <vector>
#include <cmath>    

class QSpectrum : public QWidget {
    public:
        QSpectrum([[maybe_unused]] QWidget *parent) { }

        void load_data(const float *data, size_t n) {
            if (!std::isfinite(data[1024])) return;
            fft.resize(n);

            for (size_t i = 0; i < n; i++) {
                fft[i] = data[i]*d_alpha + fft[i]*(1.0f-d_alpha);
            }
        }

        float d_alpha = 0.2f;
        float d_min = -70.0f;
        float d_max = -20.0f;
    private:
        std::vector<float> fft;

        virtual void paintEvent([[maybe_unused]] QPaintEvent* p) override {
            QPainter painter(this);
            painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0));

            for (size_t i = 0; i < 20; i++) {
                float db = i*-10.0f;
                float y = (db - d_min) / (d_max-d_min) * (float)height();
                
                painter.setPen(QColor(60,60,60));
                painter.drawLine(0, height() - y, width(), height() - y);
                painter.setPen(QColor(255,255,255));
                painter.drawText(3, height() - y, width(), height(), Qt::AlignTop, QString("%1db").arg(db));
            }
            painter.setPen(QColor(60,60,60));
            painter.drawLine(width()/2, 0, width()/2, height());

            if (fft.size() == 0) return;

            std::vector<QPoint> points(width());
            for (size_t i = 0; i < points.size(); i++) {
                size_t x = (double)i * (double)fft.size()/(double)width();
                points[i] = QPoint(i, height() - (fft[x]-d_min) * height()/(d_max-d_min));
            }

            painter.setPen(QColor(255,255,255));
            painter.drawPolyline(points.data(), points.size());
        }
};

#endif
