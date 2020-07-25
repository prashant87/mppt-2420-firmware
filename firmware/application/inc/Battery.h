/********************************************************************************
 * class        Settings type battery and charge algorithms                     *
 *                                                                              *
 * file         Battery.h                                                       *
 * author       @RedCommissary                                                  *
 * date         26.07.2020                                                      *
 *                                                                              *
 ********************************************************************************/

#pragma once

/********************************************************************************
 * Include
 ********************************************************************************/

#include <stdint.h>

/********************************************************************************
 * Defines
 ********************************************************************************/

#define _1S     (uint8_t)1U
#define _2S     (uint8_t)2U
#define _3S     (uint8_t)3U
#define _4S     (uint8_t)4U
#define _5S     (uint8_t)5U
#define _6S     (uint8_t)6U

/********************************************************************************
 * Class Battery
 * 
 * Supported batteries:
 *      - Pb                    -   14.4V
 *      - AGM                   -   14.4V
 *      - Gel                   -   14.4V
 *      - Lithium Ion           -   4.2V
 *      - Lithium Polymer       -   4.2V
 *      - LiFePO4               -   3.4V
 *      - Lithium Titanate      -   2.4V
 * 
 ********************************************************************************/

class Battery {
    public:
        enum class TypeBattery {
            SLA, AGM, GEL, Lithium, LiPo, LiFePO, LTO
        };

    public:
        static void SetParameters(TypeBattery type, uint8_t cell, float current);
        static float GetVoltage();
        static float GetCurrent();

    private:
        static TypeBattery typeBattery;
        static float voltageCell;
        static uint8_t numberOfCells;
        static float chargeCurrent;
};