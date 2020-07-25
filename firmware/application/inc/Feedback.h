/********************************************************************************
 * class        Filtering and conversion ADC result                             *
 *                                                                              *
 * file         Feedback.h                                                      *
 * author       @RedCommissary                                                  *
 * date         23.06.2020                                                      *
 *                                                                              *
 ********************************************************************************/

#pragma once

/********************************************************************************
 * Include 
 ********************************************************************************/

#include "Adc.h"
#include "Gpio.h"

#include "FilterWindowMedium.h"

/********************************************************************************
 * Class Feedback
 * 
 * Divider input voltage: 100 kOhm / 5.1 kOhm + 1 = 20.6078431
 * Divider output voltage to 12V: 100 kOhm / 24 kOhm + 1 = 5.1666666
 * Divider output voltage to 24V: 100 kOhm / 12 kOhm + 1 = 9.3333333
 * 
 * Pin control divider - PB14
 * 
 * Gain current sensor: 75 kOhm / (1 kOhm + 1 kOhm) = 37,5
 * Current shunt: 0.004 Ohm
 * 
 ********************************************************************************/

class Feedback {
    public:
        enum class Divider {
            div12V, div24V
        };

        static bool statusOutputDivider;
        constexpr static uint16_t sizeWindow = 20;

    public:
        static void InitGpioDivider() {
            Gpio::Init<14>(GPIOB, Gpio::Mode::output, Gpio::Type::PP);
        }

        static void SetOutputDivider (Divider divider) {
            if (divider == Divider::div12V) { 
                GPIOB->BSRR |= GPIO_BSRR_BR_14; 
                statusOutputDivider = false;
            }
            if (divider == Divider::div24V) {
                GPIOB->BSRR |= GPIO_BSRR_BS_14;
                statusOutputDivider = true;
            }
        }      

        static float GetInputVoltage() {
            Adc::Status::stopInputVoltage = true;
            float inputVoltage = FilterWindowMedium::Compute(Adc::inputVoltage, Adc::sizeBuffer, sizeWindow);
            Adc::Status::stopInputVoltage = false;
            return (inputVoltage * voltageDivInput * sampleStepAdc + staticErrorInputVoltage);
        }

        static float GetInputCurrent() {
            Adc::Status::stopInputCurrent = true;
            float inputCurrent = FilterWindowMedium::Compute(Adc::inputCurrent, Adc::sizeBuffer, sizeWindow);
            Adc::Status::stopInputCurrent = false;
            return (inputCurrent * sampleStepAdc / gainCurrentSensor / currentShunt);
        }

        static float GetOutputVoltage() {
            if (statusOutputDivider) {
                Adc::Status::stopOutputVoltage = true;
                float outputVoltage = FilterWindowMedium::Compute(Adc::outputVoltage, Adc::sizeBuffer, sizeWindow);
                Adc::Status::stopOutputVoltage = false;
                return (outputVoltage * sampleStepAdc * voltageDivOutput24V + staticErrorOutputVoltage);
            } else {
                Adc::Status::stopOutputVoltage = true;
                float outputVoltage = FilterWindowMedium::Compute(Adc::outputVoltage, Adc::sizeBuffer, sizeWindow);
                Adc::Status::stopOutputVoltage = false;
                return (outputVoltage * sampleStepAdc * voltageDivOutput12V + staticErrorOutputVoltage);
            }
        }

        static float GetOutputCurrent() {
            Adc::Status::stopOutputCurrent = true;
            float outputCurrent = FilterWindowMedium::Compute(Adc::outputCurrent, Adc::sizeBuffer, sizeWindow);
            Adc::Status::stopOutputCurrent = false;
            return (outputCurrent * sampleStepAdc / gainCurrentSensor / currentShunt);
        }

    private:
        constexpr static float sampleStepAdc = 0.0008057f;
        constexpr static float gainCurrentSensor = 37.5f;
        constexpr static float currentShunt = 0.004f;
        constexpr static float voltageDivInput = 20.6078431f;
        constexpr static float voltageDivOutput12V = 5.1666666f;
        constexpr static float voltageDivOutput24V = 9.3333333f;
        constexpr static float staticErrorInputVoltage = 0.225f;
        constexpr static float staticErrorOutputVoltage = 0.32f;
};