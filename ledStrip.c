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

#include "ledStrip.h"


LEDStrip::LEDStrip(CRGB *leds){
    Leds = leds;
}

void LEDStrip::spi_write(uint8_t spi_data) {
	for (uint8_t i = 0; i < 8; i++) {
		if (spi_data & 0x80) {
            LED_DataPin(1);
		} else {
            LED_DataPin(0);
		}
        LED_ClockPin(1);
		spi_data = spi_data << 1;
        LED_ClockPin(0);
	}
   
    
}

void LEDStrip::start_frame() {
	spi_write(0x00);
	spi_write(0x00);
	spi_write(0x00);
	spi_write(0x00);
}
void LEDStrip::end_frame() {
	spi_write(0xff);
	spi_write(0xff);
	spi_write(0xff);
	spi_write(0xff);
}

void LEDStrip::show(){
      
    start_frame();
	for (uint8_t i = 0; i < PIXEL_COUNT; i++) {
		spi_write(0xff);
		spi_write(Leds[i].b);
		spi_write(Leds[i].g);
		spi_write(Leds[i].r);
	}
	end_frame();
    
};

/* [] END OF FILE */
