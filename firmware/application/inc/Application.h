/********************************************************************************
 * class        Control and algorithms for application                          *
 *                                                                              *
 * file         Application.h                                                   *
 * author       @RedCommissary                                                  *
 * date         23.06.2020                                                      *
 *                                                                              *
 ********************************************************************************/

#pragma once

/********************************************************************************
 * Include 
 ********************************************************************************/

#include "startupF334.h"

#include "Led.h"
#include "Adc.h"
#include "Hrpwm.h"

#include "Pid.h"

#include "Feedback.h"
#include "Battery.h"

/********************************************************************************
 * Class Application
 * 
 * High speed handler   -   2 kHz
 * Low speed handler    -   1 Hz
 * 
 ********************************************************************************/

class Application {
    public:   
        static uint16_t dutyBuck;
        static float outputVoltage;
        static float outputCurrent;
        
    public:
        static void Init();

    private:
        static void StartHighSpeedProcessing();
        static void StartLowSpeedProcessing();
};