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

#define DATA_PIN LED_DATA_Write
#define CLOCK_PIN LED_CLK_Write
#define UPDATES_PER_SECOND 25u

#define NUM_LEDS    60u
#define PS 11u 

/*Logical Size of Em_EEPROM*/
#define LOGICAL_EEPROM_SIZE     PS
#define LOGICAL_EEPROM_START    0u
#define ASCII_P                 0x50
#define ASCII_ZERO              0x30

/*location of reset counter in Em_EEPROM*/
#define Address_LOCATION    1u
#define Address_SIZE        1u

/* EEPROM storage in work flash, this is defined in Em_EEPROM.c*/
const uint8_t Em_EEPROM_em_EepromStorage[Em_EEPROM_PHYSICAL_SIZE]
__ALIGNED(CY_FLASH_SIZEOF_ROW) = {0u};

union _scratchpad {
    struct {
        uint8_t address;                    // 1 byts 
        uint8_t delaytime;                  // 1 byts   
        uint16_t StripSize;                 // 2 byts   
        TBlendType Blending;                // 1 byts   
        uint8_t motion_speed;               // 1 byts  
        uint8_t brightness;                 // 1 byts
        uint8_t ClorPaletteNum;             // 1 byts
        uint8_t PaletteIndex;               // 1 byts
        //TProgmemRGBPalette16 ClorPalette;   // 64 byts  
        uint8_t marker;                     // 1 byts   
    };
    uint8_t data_raw[PS];
    /*RAM arrays for holding EEPROM read and write data*/
    uint8_t eepromArray[PS];
};

union _scratchpad scratchpad;
struct CRGB leds[400];

static uint8_t LastAddress = SCB_I2C_SLAVE_ADDRESS;
static uint8_t LastClorPaletteNum = 250;
uint16_t DelelyIndex = 0;
CRGBPalette16 currentPalette;
//static uint8_t startIndex = 0;
void ChangePalette();
uint16 i;


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    for( i = 0; i < scratchpad.StripSize; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, scratchpad.brightness, scratchpad.Blending);
        colorIndex += 3;
    }
}

int main(void)
{

    //currentPalette  = RainbowColors_p;
   
        
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    Em_EEPROM_Init((uint32_t)Em_EEPROM_em_EepromStorage);
    Em_EEPROM_Read(LOGICAL_EEPROM_START, scratchpad.eepromArray, LOGICAL_EEPROM_SIZE);
    
       /*If first byte of EEPROM isn't 'P' the write data from array*/
    if( (scratchpad.marker != ASCII_P) )
    {
        /*Write inital data to EEPROM*/
        scratchpad.marker = ASCII_P;        
        scratchpad.address = SCB_I2C_SLAVE_ADDRESS;
        scratchpad.StripSize = NUM_LEDS;
        scratchpad.Blending = LINEARBLEND; 
        scratchpad.motion_speed = 2;
        scratchpad.brightness = 250;
        scratchpad.ClorPaletteNum = 1;
        //scratchpad.RunPalette = 1;
        scratchpad.PaletteIndex = 0x42;
        scratchpad.delaytime = (1000/UPDATES_PER_SECOND);
        ChangePalette();
        //for( i = 0; i < 16; i++) {
        //    scratchpad.ClorPalette[i]  = RainbowColors_p[i]; 
        //}
        Em_EEPROM_Write(LOGICAL_EEPROM_START,scratchpad.eepromArray, LOGICAL_EEPROM_SIZE);
        Em_EEPROM_Read(LOGICAL_EEPROM_START, scratchpad.eepromArray, LOGICAL_EEPROM_SIZE);
    }
    
  
    LEDStrip s1 = LEDStrip(leds);
    s1.PIXEL_COUNT = scratchpad.StripSize;
    ChangePalette();
    
    /* Initialize write buffer before call SCB_Start */ 
    SCB_I2CSlaveInitWriteBuf((uint8 *)scratchpad.data_raw, 8u);
    
    /* Initialize read buffer before call SCB_Start */ 
    SCB_I2CSlaveInitReadBuf((uint8 *) scratchpad.data_raw, PS);
    SCB_Start();
    
    SCB_I2CSlaveSetAddress(scratchpad.address);
    LastAddress = scratchpad.address;
    DelelyIndex = scratchpad.delaytime;
    for(;;)
    {   
        // Place your application code here. 
        uint8_t status = SCB_I2CSlaveStatus();
        if(0u != (status & SCB_I2C_SSTAT_WR_CMPLT)){ //Slave write transfer complete. 
            if(0u == (status & SCB_I2C_SSTAT_WR_ERR)){ //Slave captured error on the bus while write transfer. 

                s1.PIXEL_COUNT = scratchpad.StripSize;
                DelelyIndex = scratchpad.delaytime;
                if(LastAddress != scratchpad.address){
                    SCB_I2CSlaveSetAddress(scratchpad.address);
                    LastAddress = scratchpad.address;                   
                }
                ChangePalette();
                Em_EEPROM_Write(LOGICAL_EEPROM_START,scratchpad.eepromArray, LOGICAL_EEPROM_SIZE);
            }
            SCB_I2CSlaveClearWriteBuf(); 
            SCB_I2CSlaveClearWriteStatus();
        }else if(0u != (status & SCB_I2C_SSTAT_RD_CMPLT)){ //Slave read transfer complete. 
            // Clean-up status and buffer pointer  
            SCB_I2CSlaveClearReadBuf();
            SCB_I2CSlaveClearReadStatus();
        }else if(0u == (status & SCB_I2C_SSTAT_WR_BUSY)){
            CyDelayUs(500);
            if(DelelyIndex-- <= 1){
                DelelyIndex = scratchpad.delaytime;   
                
                
                //if(scratchpad.RunPalette == 1){
                    scratchpad.PaletteIndex = scratchpad.PaletteIndex + scratchpad.motion_speed; /* motion speed */
                //}
                    
                
                FillLEDsFromPaletteColors(scratchpad.PaletteIndex);
                s1.show();
                
            }
            
       }
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}
// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    
    currentPalette = CRGBPalette16(
                                   green,  green,  CRGB::Black,  CRGB::Black,
                                   purple, purple, CRGB::Black,  CRGB::Black,
                                   green,  green,  CRGB::Black,  CRGB::Black,
                                   purple, purple, CRGB::Black,  CRGB::Black );
}

void ChangePalette()
{
    if(LastClorPaletteNum != scratchpad.ClorPaletteNum){
        
        
        switch(scratchpad.ClorPaletteNum){     
            case 1:
                currentPalette = RainbowColors_p;         
                scratchpad.Blending = LINEARBLEND;
                break;  
            case 2:
                currentPalette = RainbowStripeColors_p;   
                scratchpad.Blending = NOBLEND;
                break;
               
            case 3: 
                currentPalette = RainbowStripeColors_p;   
                scratchpad.Blending = LINEARBLEND;
                break;

            case 4:
                SetupBlackAndWhiteStripedPalette();       
                scratchpad.Blending = NOBLEND;
                break;
            case 5:
                SetupBlackAndWhiteStripedPalette();       
                scratchpad.Blending = LINEARBLEND;
                break;
           
            case 6:
                currentPalette = CloudColors_p;           
                scratchpad.Blending = LINEARBLEND;
                break;
            case 7:
                currentPalette = PartyColors_p;           
                scratchpad.Blending = LINEARBLEND;
                break;
             
            case 8:
                currentPalette = myRedWhiteBluePalette_p; 
                scratchpad.Blending = NOBLEND;
                break;
            case 9:
                currentPalette = myRedWhiteBluePalette_p; 
                scratchpad.Blending = LINEARBLEND;
                break;
                
            /*
            case 10:
                SetupPurpleAndGreenPalette();            
                scratchpad.Blending = LINEARBLEND;
                break;
          
            case 11:
                SetupTotallyRandomPalette(currentPalette);              
                scratchpad.Blending = LINEARBLEND;
                break;
              */
            
            default:
               // currentPalette = scratchpad.ClorPalette;
                LastClorPaletteNum = 0;
                scratchpad.ClorPaletteNum = 0;
                break;
        }
        
        //for( i = 0; i < 16; i++) {
        //memcpy( &(scratchpad.ClorPalette[0]), &(currentPalette.entries[0]), sizeof(scratchpad.ClorPalette));
            //scratchpad.ClorPalette[i]  = currentPalette[i]; 
        //}
    }
}

/* [] END OF FILE */
