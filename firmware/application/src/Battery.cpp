/********************************************************************************
 * Include
 ********************************************************************************/

#include "Battery.h"

/********************************************************************************
 * Class Battery
 ********************************************************************************/

void Battery::SetParameters(TypeBattery type, uint8_t cell, float current) {
    Parameters.typeBattery = type;
    Parameters.numberOfCells = cell;
    Parameters.chargeCurrent = current;
}

float Battery::GetVoltage() {
    float voltageCell = 0.0f;

    switch (Parameters.typeBattery) {
        case TypeBattery::SLA:
            voltageCell = VoltageCell::SLA;
            break;
        case TypeBattery::AGM:
            voltageCell = VoltageCell::AGM;
            break;
        case TypeBattery::GEL:
            voltageCell = VoltageCell::GEL;
            break;
        case TypeBattery::Lithium:
            voltageCell = VoltageCell::Lithium;
            break;
        case TypeBattery::LiPo:
            voltageCell = VoltageCell::LiPo;
            break;
        case TypeBattery::LiFePO:
            voltageCell = VoltageCell::LiFePO;
            break;
        case TypeBattery::LTO:
            voltageCell = VoltageCell::LTO;
            break;
    }

    float outputVoltage = Parameters.numberOfCells * voltageCell;
    return (outputVoltage);
}

float Battery::GetCurrent() {
    return Parameters.chargeCurrent;
}