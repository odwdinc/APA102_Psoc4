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
#include "FastLED.h"
#define RAND16_SEED  1337
uint16_t rand16seed = RAND16_SEED;

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette(CRGBPalette16 &cp)
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    cp = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

uint8_t random8()
{
    //uint8 sample = ReadDieTemp();
    //return 50;
    
    rand16seed = (rand16seed * FASTLED_RAND16_2053) + FASTLED_RAND16_13849;
    // return the sum of the high and low bytes, for better
    //  mixing and non-sequential correlation
    return (uint8_t)(((uint8_t)(rand16seed & 0xFF)) +
                     ((uint8_t)(rand16seed >> 8)));
    //return 0;
}


// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette(CRGBPalette16 &cp)
{
    for( int i = 0; i < 16; i++) {
        cp[i] =  CHSV( random8(), 255, 255);
    }
}


// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette(CRGBPalette16 &cp)
{
    // 'black out' all 16 palette entries...
    fill_solid( cp, 16, CRGB::Black);
    // and set every fourth one to white.
    cp[0] = CRGB::White;
    cp[4] = CRGB::White;
    cp[8] = CRGB::White;
    cp[12] = CRGB::White;
    
}

/* [] END OF FILE */
