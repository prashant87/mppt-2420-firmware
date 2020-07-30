/********************************************************************************
 * Include 
 ********************************************************************************/

#include "Application.h"

/********************************************************************************
 * Variables 
 ********************************************************************************/

float Application::inputVoltage = 0.0f;
float Application::inputCurrent = 0.0f;
float Application::outputVoltage = 0.0f;
float Application::outputCurrent = 0.0f;
uint16_t Application::dutyBuck = 0;

PidController pidVoltageMode;
PidController pidCurrentMode;

Battery UserBattery;

/********************************************************************************
 * Class Application
 ********************************************************************************/

void Application::Init() {
    UserBattery.SetParameters(Battery::TypeBattery::SLA, _1S, 2.0f);
    Application::outputVoltage = UserBattery.GetVoltage();
    Application::outputCurrent = UserBattery.GetCurrent();

    Feedback::InitGpioDivider();
    if (Application::outputVoltage > 16.0f) {
        Feedback::SetOutputDivider(Feedback::Divider::div24V);
    } else {
        Feedback::SetOutputDivider(Feedback::Divider::div12V);
    }


    Application::StartLowSpeedProcessing();
    Application::StartHighSpeedProcessing();
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
 * High speed processing #1
 ********************************************************************************/

void sTim3::handler (void) {
    TIM3->SR &= ~TIM_SR_UIF;

    float resultPID = 0.0f;

    float outputVoltage = Feedback::GetOutputVoltage();
    float outputCurrent = Feedback::GetOutputCurrent();

    if (outputVoltage < (Application::outputVoltage - 0.2f)) {
        pidCurrentMode
            .SetReference(Application::outputCurrent)
            .SetSaturation(-40000, 40000)
            .SetFeedback(outputCurrent, 0.0002)
            .SetCoefficient(10, 0, 0, 0, 0)
            .Compute();
        resultPID = pidCurrentMode.Get();
    } else {
        pidVoltageMode
            .SetReference(Application::outputVoltage)
            .SetSaturation(-40000, 40000)
            .SetFeedback(outputVoltage, 0.0002)
            .SetCoefficient(50, 0, 0, 0, 0)
            .Compute();
        resultPID = pidVoltageMode.Get();
    }

    Application::dutyBuck += resultPID;
    Hrpwm::SetDuty(Application::dutyBuck);
}

/********************************************************************************
 * Low speed processing #1
 ********************************************************************************/

void sTim2::handler (void) {
    TIM2->SR &= ~TIM_SR_UIF;
    Led::Toggle(Led::Color::yellow);
}