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

#ifndef QT_QWATERFALL_H
#define QT_QWATERFALL_H

#include <QPixmap>
#include <QPainter>
#include <cstring>
#include <cmath>

inline QColor lerp(QColor a, QColor b, double n) {
    return QColor::fromRgbF(
        a.redF()  *(1.0-n) + b.redF()  *n,
        a.greenF()*(1.0-n) + b.greenF()*n,
        a.blueF() *(1.0-n) + b.blueF() *n
    );
}

// A basic constellation widget
class QWaterfall : public QWidget {
    public:
        QWaterfall([[maybe_unused]] QWidget *parent) : waterfall(2048, 200) {
            waterfall.fill(QColor(0, 0, 0));
        }
        ~QWaterfall() {

        }
        void load(const float *data, size_t n) {
            d_data = data;
            d_data_n = n;
            is_good = true;
        }
        float d_min = -70.0f;
        float d_max = -20.0f;
    private:
        float lut[5][3] = {
            { 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x50 },
            { 0x1E, 0x90, 0xFF },
            { 0xAD, 0xD8, 0xE6 },
            { 0xFF, 0xFF, 0xFF }
        };
        std::vector<QColor> gradient = {
            QColor(0x00, 0x00, 0x00),
            QColor(0x00, 0x00, 0x50),
            QColor(0x1E, 0x90, 0xFF),
            QColor(0xAD, 0xD8, 0xE6),
            QColor(0xFF, 0xFF, 0xFF)
        };
        const float *d_data = nullptr;
        size_t d_data_n = 0;
        QPixmap waterfall;
        bool is_good = false;

        virtual void paintEvent([[maybe_unused]] QPaintEvent *event) override {
            if (d_data != nullptr) {
                QPainter waterfall_painter(&waterfall);
                for (size_t i = 0; i < 2048; i++) {
                    float val = (d_data[i] - d_min) / (d_max-d_min) * (float)gradient.size();
                    val = std::max(std::min<float>(gradient.size(), val), 0.0f);

                    QColor a = gradient[(int)floorf(val)];
                    QColor b = gradient[(int)ceilf(val)];

                    waterfall_painter.setPen(lerp(a, b, fmodf(val, 1.0f)));
                    waterfall_painter.drawPoint(i, 0);
                }
            }

            // Draw waterfall
            QPainter painter(this);
            painter.drawPixmap(0, 0, waterfall.scaled(width(), height()));

            if (is_good) {
                waterfall.scroll(0, 1, waterfall.rect());
                is_good = false;
            }
        }
};

#endif
