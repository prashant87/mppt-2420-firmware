/********************************************************************************
 * class        Control and algorithms for application                          *
 *                                                                              *
 * file         Application.h                                                   *
 * author       Ilya Galkin                                                     *
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

/********************************************************************************
 * Class Application
 * 
 * High speed handler   -   2 kHz
 * Low speed handler    -   1 Hz
 * 
 ********************************************************************************/

class Application {
    public:
        struct UserSettings {
            static float thresholdUVLO;
            static float outputVoltage;
        };
    
    static uint16_t dutyBuck;
        
    public:
        static void Init();
        static void UVLO (float reference);

    private:
        static void StartUVLO (float reference);
        static void StartHighSpeedProcessing();
        static void StartLowSpeedProcessing();

};