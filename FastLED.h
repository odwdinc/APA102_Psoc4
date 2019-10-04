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


#ifndef __INC_FASTLED_H
#define __INC_FASTLED_H
    
    
#ifdef __cplusplus
extern "C" {
#endif
    #include "project.h"

    #define delay(ms)(CyDelay(ms))
    #define delay_us(us)(CyDelayUs(us))

#ifdef __cplusplus
}
#endif

#define FL_PROGMEM
#define FL_PGM_READ_BYTE_NEAR(x)  (*((const  uint8_t*)(x)))
#define FL_PGM_READ_WORD_NEAR(x)  (*((const uint16_t*)(x)))
#define FL_PGM_READ_DWORD_NEAR(x) (*((const uint32_t*)(x)))
#define FL_ALIGN_PROGMEM  __attribute__ ((aligned (4)))

    // System includes, these have in-built protection,
// and therefore don't have to be wrapped
#include <stdio.h>		// snprintf()
#include <stdlib.h>		// realloc(), malloc(), free()
#include <getopt.h>		// getopt()	
#include <stdint.h>   

#include "lib8.h"

#include "pixeltypes.h"

#include "color.h"
#include "colorutils.h"
#include "colorpalettes.h"
#include "ledStrip.h"


// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette(CRGBPalette16 &cp);
// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette(CRGBPalette16 &cp);
// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette(CRGBPalette16 &cp);
// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};



/* [] END OF FILE */
#endif