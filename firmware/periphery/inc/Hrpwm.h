/********************************************************************************
 * project     Class for initialization and work with High Resolution PWM       *
 *                                                                              *
 * file        Hrpwm.h                                                          *
 * author      Ilya Galkin                                                      *
 * date        13.05.2020                                                       *
 *                                                                              *
 ********************************************************************************/

#pragma once

/********************************************************************************
 * Include 
 ********************************************************************************/

#include "stm32f3xx.h"
#include "Gpio.h"

/********************************************************************************
 * Class HRPWM
 * 
 * Dead time - 104 ns
 * Complementary output
 * Channel A
 * 
 ********************************************************************************/

class Hrpwm {
    public:
        static void Init (void);
        static void SetDuty (uint16_t duty);

    private:
        static void InitGpio (void);

    private:
        static const uint16_t periodHrpwm = 45000;     // Fsw = 100 kHz
};

/********************************* END OF FILE **********************************/