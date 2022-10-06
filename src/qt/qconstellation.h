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
#include "util/math.h"

#define NUM_SYMBOLS 2048

// A basic constellation widget
class QConstellation : public QWidget {
    public:
        explicit QConstellation([[maybe_unused]] QWidget *parent) { }

        void push_sample(std::complex<float> symbol) {
            symbols[n++] = symbol;
            if (n == symbols.size()) n = 0;
        }

        void set_lines(bool x, bool y) {
            xline = x;
            yline = y;
            
            // reset SNR variables
            snrmax = 0;
            snravg = 0;
            snrtotal = 0;
            snrstep = 0;
            snrR = 120;
            snrG = 120;
            snrB = 120;
        }
        void num_points(size_t x) { symbols.resize(x); }
        size_t num_points() { return symbols.size(); }
    private:
        SNREstimator snr_est;
        std::vector<std::complex<float>> symbols;
        size_t n = 0;
        bool xline = false;
        bool yline = false;
        double snr = 0;
        double snrmax = 0;
        double snravg = 0;
        double snrtotal = 0;
        int snrstep = 0;
        int snrR = 120;
        int snrG = 120;
        int snrB = 120;
        int style = 0;
        virtual void resizeEvent(QResizeEvent *event) {
            event->accept();
            if (event->size().width() < event->size().height()) {
                QWidget::resize(event->size().width(), event->size().width());
            } else {
                QWidget::resize(event->size().height(), event->size().height());
            }
        }

        virtual void paintEvent([[maybe_unused]] QPaintEvent* p) override {
            snr = snr_est.get_snr(symbols.data(), symbols.size());
            if ((snr < 0)) {
            	snr = 0;
            }
            if (snrstep == 0){
            	snr = 0;
            	snrstep += 1;
            }
            if (snr > snrmax){
                snrmax = snr;
            }
            if (snr > 0){
            	snrstep += 1;
            	snrtotal = (snrtotal+snr); 
            	snravg = (snrtotal/snrstep);
	        snrR = clamp((100-snr*7)*2.55,0,255);
                snrG = clamp((snr*6.6)*2.55,0,255);
                snrB = 0;
            }
            if (snr == 0){
                snrR = 120;
                snrG = 120;
                snrB = 120;
            }
            
            if (width() > 10){
		    // Constellation background
		    QPainter painter(this);
		    painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0));

		    // Constellation symbols
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
		    
		    // Constellation divider lines
		    painter.setPen(QColor(50, 50, 50));
		    if (yline) painter.drawLine(width()/2, 0, width()/2, height());
		    if (xline) painter.drawLine(0, height()/2, width(), height()/2);
		    
		    if ((width() > 50) && (style > 0)){
			    
			    if (style == 1){
			        // SNR meter outline
			        painter.fillRect(0 +width()*0.25, 0, width()*0.5, height()*0.25, QColor(snrR, snrG, snrB));
			        // SNR meter background
			        painter.fillRect(width()*0.02+1 +width()*0.25, height()*0.02+1, width()*0.46, height()*0.21, QColor(snrR*0.2+180, snrG*0.2+180, snrB*0.2+180));
			        
       			        painter.setPen(QColor(0, 0, 0));
			    }
			    if (style == 2){
			        // SNR meter outline
			        painter.fillRect(0 +width()*0.25, 0, width()*0.5, height()*0.25, QColor(255, 255, 70));
			        // SNR meter background
			        painter.fillRect(width()*0.005+1 +width()*0.25, height()*0.005+1, width()*0.49, height()*0.24, QColor(0,0,0));
			        
       			        painter.setPen(QColor(255, 255, 70));
			    }

			    // Current SNR readout:
			    QFont font = painter.font();
			    if (style == 1){
				    font.setPointSize(font.pointSize() * height()/80);
				    painter.setFont(font);
				    painter.drawText(width()*0.02+1 +width()*0.25, height()*0.02+1 -height()*0.03, width()*0.46, height()*0.21, Qt::AlignCenter, QString("%1").arg(QString::number(snr, 'f', 2)));
		            }
			    if (style == 2){
				    font.setPointSize(font.pointSize() * height()/75);
				    painter.setFont(font);
				    painter.drawText(width()*0.02 +width()*0.25, height()*0.02+1 , width()*0.46, height()*0.21, Qt::AlignCenter, QString("%1").arg(QString::number(snr, 'f', 2)));
		            }
	 
			    if ((width() > 100) && (style == 1)){           
				    // Max SNR readout:
				    font.setPointSize(font.pointSize()/3);
				    painter.setFont(font);
				    painter.drawText(width()*0.02+1 -width()*0.13 +width()*0.25, height()*0.02+1 +height()*0.05, width()*0.46, height()*0.21, Qt::AlignCenter, QString("%1").arg(QString::number(snrmax, 'f', 2)));
				    
				    // Avg SNR readout:
				    painter.drawText(width()*0.02+1 +width()*0.13 +width()*0.25, height()*0.02+1 +height()*0.05, width()*0.46, height()*0.21, Qt::AlignCenter, QString("%1").arg(QString::number(snravg, 'f', 2)));
			    
				    if (width() > 200){
					    // Max SNR label:
					    font.setPointSize(font.pointSize()/1.5);
					    painter.setFont(font);
					    painter.drawText(width()*0.02+1 -width()*0.13 +width()*0.25, height()*0.02+1 +height()*0.08, width()*0.46, height()*0.21, Qt::AlignCenter, QString("max"));
					    
					    // Avg SNR label:
					    painter.drawText(width()*0.02+1 +width()*0.13 +width()*0.25, height()*0.02+1 +height()*0.08, width()*0.46, height()*0.21, Qt::AlignCenter, QString("avg"));
					    
					    // dB label:
					    painter.drawText(width()*0.02+1 +width()*0.25, height()*0.02+1 +height()*0.07, width()*0.46, height()*0.21, Qt::AlignCenter, QString("dB\nSNR"));
				    }
			    }
		    }
		    if (style == 0){
         	        painter.setPen(QColor(70, 70, 70));
		    	painter.drawText(5, height()-105, 100, 100, Qt::AlignBottom, QString("SNR: %1 dB").arg(QString::number(snr, 'f', 2)));
		    }
            }
            
            
        }
        virtual void mousePressEvent(QMouseEvent *E){
            (void)E;
            style += 1;
            if (style == 3){
                style = 0;
            }
            QWidget::update();
        }
};

#endif
