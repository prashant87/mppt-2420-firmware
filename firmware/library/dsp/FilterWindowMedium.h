/********************************************************************************
 * class        DSP window filter with medium samling                           *
 *                                                                              *
 * file         FilterWindowMedium.h                                            *
 * author       Ilya Galkin                                                     *
 * date         03.06.2020                                                      *
 *                                                                              *
 ********************************************************************************/

/********************************************************************************
 * Include 
 ********************************************************************************/

#include <cmath>

#include "Adc.h"

/********************************************************************************
 * Class Filter
 ********************************************************************************/

class FilterWindowMedium {
    public:
        static float FilterCompute (uint16_t array[], uint8_t size) {
            float sumResult = 0;

            for (uint8_t i = 0; i < size; i++) {
                sumResult = sumResult + array[i];
            }
            
            float result = sumResult / size;
            return result;
        }
};