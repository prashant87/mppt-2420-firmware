/********************************************************************************
 * class        Filtering and conversion ADC result                             *
 *                                                                              *
 * file         Feedback.h                                                      *
 * author       Ilya Galkin                                                     *
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
            float inputVoltage = FilterWindowMedium::Compute(Adc::inputVoltage, Adc::sizeBuffer);
            return (voltageDivInput * sampleStepAdc * inputVoltage + staticErrorInputVoltage);
        }

        static float GetInputCurrent() {
            return (Adc::inputCurrent[0] * sampleStepAdc / gainCurrentSensor / currentShunt);
        }

        static float GetOutputVoltage() {
            if (statusOutputDivider) {
                float outputVoltage = FilterWindowMedium::Compute(Adc::outputVoltage, Adc::sizeBuffer);
                return (voltageDivOutput24V * sampleStepAdc * outputVoltage);
            } else {
                float outputVoltage = FilterWindowMedium::Compute(Adc::outputVoltage, Adc::sizeBuffer);
                return (voltageDivOutput12V * sampleStepAdc * outputVoltage);
            }
        }

        static float GetOutputCurrent() {
            return (Adc::outputCurrent[0] * sampleStepAdc / gainCurrentSensor / currentShunt);
        }

    private:
        constexpr static float sampleStepAdc = 0.000809f;
        constexpr static float gainCurrentSensor = 37.5f;
        constexpr static float currentShunt = 0.004f;
        constexpr static float voltageDivInput = 20.6078431f;
        constexpr static float voltageDivOutput12V = 5.1666666f;
        constexpr static float voltageDivOutput24V = 9.3333333f;
        constexpr static float staticErrorInputVoltage = 0.225f;
};