/********************************************************************************
 * class        Settings type battery and charge algorithms                     *
 *                                                                              *
 * file         Battery.h                                                       *
 * author       @RedCommissary                                                  *
 * date         26.07.2020                                                      *
 *                                                                              *
 ********************************************************************************/

/********************************************************************************
 * Include
 ********************************************************************************/

#include "Battery.h"

/********************************************************************************
 * Class Battery
 ********************************************************************************/

void Battery::SetParameters (TypeBattery type, uint8_t cell, float current) {
    Battery::typeBattery = type;
    Battery::numberOfCells = cell;
    Battery::chargeCurrent = current;
}

float Battery::GetVoltage() {
    float voltageCell = 0.0f;

    switch (Battery::typeBattery) {
        case Battery::TypeBattery::SLA:
            voltageCell = 14.4f;
            break;
        case Battery::TypeBattery::AGM:
            voltageCell = 14.4f;
            break;
        case Battery::TypeBattery::GEL:
            voltageCell = 14.4f;
            break;
        case Battery::TypeBattery::Lithium:
            voltageCell = 4.2f;
            break;
        case Battery::TypeBattery::LiPo:
            voltageCell = 4.2f;
            break;
        case Battery::TypeBattery::LiFePO:
            voltageCell = 3.4f;
            break;
        case Battery::TypeBattery::LTO:
            voltageCell = 2.4f;
            break;
    }

    float outputVoltage = Battery::numberOfCells * voltageCell;
    return (outputVoltage);
}

float Battery::GetCurrent() {
    return chargeCurrent;
}