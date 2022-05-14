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

#ifndef IO_SDR_IMPL_H
#define IO_SDR_IMPL_H

#include "reader.h"

#include <SoapySDR/Device.hpp>
#include <SoapySDR/Types.hpp>
#include <SoapySDR/Formats.hpp>

class SDRSource : public FileReader {
    public:
        SDRSource(std::string device, double freq, double rate) {
            sdr = SoapySDR::Device::make(device);
            if (sdr == nullptr) {
                throw std::invalid_argument("Could not open SDR");
            }

            set_frequency(freq);
            set_rate(rate);

            stream = sdr->setupStream(SOAPY_SDR_RX, SOAPY_SDR_CF32);
            if (stream == nullptr) {
                throw std::invalid_argument("Could not open RX stream");
            }
            sdr->activateStream(stream);
        }
        ~SDRSource() {
            sdr->deactivateStream(stream);
	        sdr->closeStream(stream);
            SoapySDR::Device::unmake(sdr);
        }

        size_t read_samples(std::complex<float> *ptr, size_t n) {
            void *buffs[] = { ptr };
            int flags;
            long long timeNs;

            int elements = sdr->readStream(stream, buffs, n, flags, timeNs, 1e5);
            if (elements < 0) return 0;
            return elements;
        }

        // Sample rate control
        SoapySDR::RangeList rate_range() {
            return sdr->getSampleRateRange(SOAPY_SDR_RX, 0);
        }
        double rate() {
            return sdr->getSampleRate(SOAPY_SDR_RX, 0);
        }
        void set_rate(double rate) {
            sdr->setSampleRate(SOAPY_SDR_RX, 0, rate);
        }

        // Frequency control
        SoapySDR::RangeList frequency_range() {
            return sdr->getFrequencyRange(SOAPY_SDR_RX, 0);
        }
        double frequency() {
            return sdr->getFrequency(SOAPY_SDR_RX, 0);
        }
        void set_frequency(double frequency) {
            sdr->setFrequency(SOAPY_SDR_RX, 0, frequency);
        }

        // Gain control
        SoapySDR::Range gain_range(const std::string &name) {
            return sdr->getGainRange(SOAPY_SDR_RX, 0, name);
        }
        double gain() {
            return sdr->getGain(SOAPY_SDR_RX, 0);
        }
        void set_gain(const std::string &name, double gain) {
            sdr->setGain(SOAPY_SDR_RX, 0, name, gain);
        }
        std::vector<std::string> get_gains() {
            return sdr->listGains(SOAPY_SDR_RX, 0);
        }

        // Bias tee control
        bool has_biastee() {
            for (const auto &setting : sdr->getSettingInfo()) {
                if (setting.key == "biastee" || setting.key == "biasT_ctrl" || setting.key == "bias_tx") {
                    bias_name = setting.key;
                    return true;
                }
            }

            return false;
        }
        void set_biastee(double enabled) { sdr->writeSetting(bias_name, enabled ? "true" : "false"); }

        // Antenna control
        std::vector<std::string> antennas() {
            return sdr->listAntennas(SOAPY_SDR_RX, 0);
        }
        void set_antenna(const std::string &antenna) {
            return sdr->setAntenna(SOAPY_SDR_RX, 0, antenna);
        }
        std::string antenna() {
            return sdr->getAntenna(SOAPY_SDR_RX, 0);
        }
    private:
        SoapySDR::Device *sdr;
        SoapySDR::Stream *stream;

        std::string bias_name;
};

#endif
