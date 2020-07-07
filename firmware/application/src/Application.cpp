/********************************************************************************
 * class        Control and algorithms for application                          *
 *                                                                              *
 * file         Application.cpp                                                 *
 * author       Ilya Galkin                                                     *
 * date         23.06.2020                                                      *
 *                                                                              *
 ********************************************************************************/

/********************************************************************************
 * Include 
 ********************************************************************************/

#include "Application.h"

/********************************************************************************
 * Variables 
 ********************************************************************************/

namespace Status {
    struct Flags {
        static bool errorUVLO;
    };
} 

float Application::UserSettings::thresholdUVLO = 0.0f;
float Application::UserSettings::outputVoltage = 0.0f;
bool Status::Flags::errorUVLO = false;
uint16_t Application::dutyBuck = 0;
bool Feedback::statusOutputDivider = false;

PidController pidVoltageMode;

/********************************************************************************
 * Class Application
 ********************************************************************************/

void Application::Init() {
    Application::UserSettings::thresholdUVLO = 14.5f;
    Application::UserSettings::outputVoltage = 12.0f;
    Feedback::SetOutputDivider(Feedback::Divider::div12V);

    Application::StartLowSpeedProcessing();
    Application::StartHighSpeedProcessing();

    Led::On(Led::Color::yellow);
}

void Application::StartHighSpeedProcessing() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;     

    TIM3->PSC = 36-1;
    TIM3->ARR = 500;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1  |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM3_IRQn);
}

void Application::StartLowSpeedProcessing() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;     

    TIM2->PSC = 36000-1;
    TIM2->ARR = 1000;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1  |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM2_IRQn);
}

/********************************************************************************
 * Application
 ********************************************************************************/

void sTim3::handler (void) {
    TIM3->SR &= ~TIM_SR_UIF;

    float outputVoltage = Feedback::GetOutputVoltage();

    pidVoltageMode
        .SetReference(Application::UserSettings::outputVoltage)
        .SetSaturation(-40000, 40000)
        .SetFeedback(outputVoltage, 0.0002)
        .SetCoefficient(100,0,0,0,0)
        .Compute();

    Application::dutyBuck += pidVoltageMode.Get();

    Hrpwm::SetDuty(Application::dutyBuck);
}

void sTim2::handler (void) {
    TIM2->SR &= ~TIM_SR_UIF;
}