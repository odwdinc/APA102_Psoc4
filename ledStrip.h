/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/


#ifndef __INC_LEDStrip_H
#define __INC_LEDStrip_H
    
    
#include <stdio.h>		// snprintf()
#include <stdlib.h>		// realloc(), malloc(), free()
#include <getopt.h>		// getopt()	
#include <stdint.h>   
#include "FastLED.h"
    
#ifdef __cplusplus
extern "C" {
#endif
    #define LED_DataPin(st)(LED_DATA_Write(st))
    #define LED_ClockPin(st)(LED_CLK_Write(st))
    #define T0H() CyDelayFreq(2857142u); // 350ns
    #define T1H() CyDelayFreq(1428571u); // 700ns
    #define TLD() CyDelayFreq(1666666u); // 600ns
    #define RLL() CyDelayFreq(166666u); // 6ms
#ifdef __cplusplus
}
#endif



class LEDStrip {
    public:
    CRGB *Leds;
    uint16_t PIXEL_COUNT;
    LEDStrip(CRGB *leds);
    void spi_write(uint8_t spi_data);
    void start_frame();
    void end_frame();
    void show();
};
#endif

/* [] END OF FILE */
