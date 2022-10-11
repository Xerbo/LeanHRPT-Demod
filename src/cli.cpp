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

#include "cli.h"
#include "dsp.h"
#include "io/sdr_impl.h"
#include "util/snr_estimator.h"
#include <stdexcept>
#include <iostream>
#include <chrono>

static double desuffix(QString str) {
    QChar suffix = str.right(1)[0];
    if (suffix.isDigit()) {
        return str.toDouble();
    }

    double x = str.chopped(1).toDouble();
    if (suffix == 'G') {
        return x*1000000000.0;
    } else if (suffix == 'M') {
        return x*1000000.0;
    } else if (suffix == 'k') {
        return x*1000.0;
    } else {
        throw std::runtime_error("Unknown suffix");
    }
}

class TUIConstellation {
    public:
        TUIConstellation() {
            std::cout << "\033[2J";
        }
        void work(std::complex<float> *data, size_t n) {
            estimator.push(data, n);

            size_t max = 0;
            memset(grid, 0, 10*20*sizeof(size_t));
            for (size_t i = 0; i < n; i++) {
                int y = clamp((data[i].imag() * 4.0f) + 5.0f,  0.0f, 9.0f);
                int x = clamp((data[i].real() * 8.0f) + 10.0f, 0.0f, 19.0f);
                grid[y][x]++;

                if (grid[y][x] > max) {
                    max = grid[y][x];
                }
            }

            std::cout << "\033[;H";
            for (size_t y = 0; y < 10; y++) {
                for (size_t x = 0; x < 20; x++) {
                    size_t val = grid[y][x];
                    size_t index = (float)val / (float)max * 4.0f;
                    std::cout << characters[index];
                }
                std::cout << "\n";
            }
            std::cout << "SNR: " << estimator.get_snr() << " dB                ";
        }
    private:
        SNREstimator estimator;
        size_t grid[10][20];
        std::vector<std::string> characters = { " ", ".", "-", "+", "#" };
};

void init_parser(QCommandLineParser &parser) {
    parser.setApplicationDescription("LeanHRPT Demod - A high quality, easy to use HRPT demodulator.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({{"d", "downlink"}, "* Demodulator to use, {noaa,meteor,fy3b,fy3c}_hrpt or noaa_gac", "downlink"});
    parser.addOption({{"r", "rate"}, "SDR/RAW ONLY, set sampling rate, supports unit suffixes", "rate"});
    parser.addOption({{"f", "frequency"}, "SDR ONLY, set tuning frequency, supports unit suffixes", "frequency"});
    parser.addOption({{"t", "type"}, "RAW ONLY, data format char/uchar/short/ushort/float", "type"});
    parser.addOption({{"b", "biastee"}, "SDR ONLY, enable bias tee"});
    parser.addOption({{"g", "gain"}, "SDR ONLY, set combined gain", "gain"});
    parser.addOption({{"a", "antenna"}, "SDR ONLY, set antenna", "antenna"});

    parser.addPositionalArgument("source", "IQ source, RAW/WAV/SDR");
    parser.addPositionalArgument("input", "Input filename or device string");
    parser.addPositionalArgument("output", "Output filename");
}

int run_cli(QCommandLineParser &parser) {
    if (parser.positionalArguments().size() != 3) {
        parser.showHelp();
    }
    QString source = parser.positionalArguments()[0].toLower();
    QString input = parser.positionalArguments()[1];
    QString output = parser.positionalArguments()[2];

    QString downlink = parser.value("downlink");
    if (downlink.isEmpty()) {
        parser.showHelp();
    }

    std::shared_ptr<FileReader> file;
    double sample_rate;

    if (source == "wav") {
        file = FileReader::choose_type("wav", input.toStdString());
        sample_rate = file->rate();
    } else if (source == "raw") {
        QString type = parser.value("type");
        if (type.isEmpty()) {
            throw std::runtime_error("RAW source requires -t/--type");
        }

        QString rate = parser.value("rate");
        if (rate.isEmpty() || desuffix(rate) == 0) {
            throw std::runtime_error("RAW source requires -r/--rate");
        }
        sample_rate = desuffix(rate);

        file = FileReader::choose_type(type.toStdString(), input.toStdString());
    } else if (source == "sdr") {
        QString frequency = parser.value("frequency");
        if (frequency.isEmpty() || desuffix(frequency) == 0) {
            throw std::runtime_error("SDR source requires -f/--frequency");
        }
        double _frequency = desuffix(frequency);

        QString rate = parser.value("rate");
        if (rate.isEmpty() || desuffix(rate) == 0) {
            throw std::runtime_error("SDR source requires -r/--rate");
        }
        sample_rate = desuffix(rate);

        QString gain = parser.value("gain");
        if (gain.isEmpty() || gain.toDouble() == 0) {
            throw std::runtime_error("SDR source requires -g/--gain");
        }
        double _gain = gain.toDouble();

        QString antenna = parser.value("antenna");
        if (antenna.isEmpty()) {
            throw std::runtime_error("SDR source requires -a/--antenna");
        }

        file = std::make_shared<SDRSource>(input.toStdString());
        file->set_antenna(antenna.toStdString());
        file->set_frequency(_frequency);
        file->set_rate(sample_rate);
        file->set_gain(antenna.toStdString(), _gain);

        if (parser.isSet("biastee")) {
            if (file->has_biastee()) {
                file->set_biastee(true);
            } else {
                throw std::runtime_error("SDR device does not have biastee");
            }
        }
    } else {
        throw std::runtime_error("Unknown source, must be RAW/WAV/SDR");
    }

    Demodulator *demod = make_demod(downlink.toStdString(), sample_rate, file, output.toStdString());
    TUIConstellation constellation;

    while (demod->is_running()) {
        auto symbols = demod->symbols();
        constellation.work(symbols.data(), 1024);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    demod->stop();
    delete demod;

    return 0;
}
