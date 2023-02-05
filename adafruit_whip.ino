//Code by Erin St. Blaine for Adafruit.com

#include <FastLED.h>

#define LED_PIN    6
#define COLOR_ORDER GRB
#define NUM_LEDS 48 //48x5 steps pixels for Justice's scarf, 39x7 steps in Matt's scarf

int HUE = 0;
int SATURATION = 255;
int BRIGHTNESS = 255;
int STEPS = 5;  

const byte BUTTON_PIN  =  9;
uint8_t gHue = 0; // rotating "base color" used by confetti

CRGB leds[NUM_LEDS];
TBlendType    currentBlending;
CRGBPalette16 currentPalette;

//BUTTON SETUP STUFF
byte prevKeyState = HIGH;         // button is active low

unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = 80;    // 80 * 25 = 2000 ms
byte longKeyPressCount = 0;

int ledMode = 0;  //FIRST ACTIVE MODE

//ADDITIONAL COLOR PALETTES
//Uncomment to activate a color palette

//Murica
/*const TProgmemPalette16 Murica_p PROGMEM =
{
    CRGB::Red,
    CRGB::Red, // 'white' is too bright compared to red and blue
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
*/

// Gradient palette "bhw1_06_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_06.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.
DEFINE_GRADIENT_PALETTE( AquaMagenta_gp ) {
    0, 184,  1,128,
  160,   1,193,182,
  219, 153,227,190,
  255, 255,255,255};

/*
DEFINE_GRADIENT_PALETTE( AquaMan ) {
    0, 0,125,255,
   38, 0,255,255,
  128, 255,255,255,
  217, 0,255,255,
  255, 0,125,255};
*/

/*
DEFINE_GRADIENT_PALETTE( RedFish_gp ) {
    0, 255,0,0,
   64, 238,155,174,
  128, 255,0,0,
  191, 238,155,174,
  255, 255,0,0};
*/



// Gradient palette "bhw1_15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
/*
DEFINE_GRADIENT_PALETTE( BlueFish_gp ) {
    0,   1,  8, 87,
   71,  23,195,130,
  122, 186,248,233,
  168,  23,195,130,
  255,   1,  8, 87};
*/


// Gradient palette "bhw4_005_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_005.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( WhiteRainbow_gp ) {
    0, 244,246,108,
   76, 244,111,105,
  145, 182,241,255,
  226,  74, 96,127,
  255,  74, 96,127};

/*
DEFINE_GRADIENT_PALETTE( warm_gp ) {
    0, 255,  0,  0,
  127, 255, 125, 0,
  255, 255,  0,  0};
*/


// Gradient palette "bhw1_justducky_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_justducky.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.
//yellow night test pattern
/*
DEFINE_GRADIENT_PALETTE( justducky_gp ) {
    0,  47, 28,  2,
   76, 229, 73,  1,
  163, 255,255,  0,
  255, 229, 73,  1};
*/

// Gradient palette "sky_20_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-20.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.
// yellow night test pattern...yellow and white
/*
DEFINE_GRADIENT_PALETTE( sky_20_gp ) {
    0, 255,250,160,
   76, 255,227, 32,
  165, 244,146, 16,
  255, 188,119, 21};
*/

// Gradient palette "bhw1_07_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_07.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.
// yellow night test pattern...yellow with some orange
/*
DEFINE_GRADIENT_PALETTE( bhw1_07_gp ) {
    0, 232, 65,  1,
  255, 229,227,  1};
*/

  // Gradient palette "Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/lb/misc/tn/Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
/*
DEFINE_GRADIENT_PALETTE( Yellow_gp ) {
    0, 255,173,  5,
   53, 252,139,  3,
  127, 255,180,  8,
  193, 255,152,  3,
  255, 252,136,  3};
*/
// Gradient palette "Sundance_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/lb/misc/tn/Sundance_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
/*
DEFINE_GRADIENT_PALETTE( Sundance_Yellow_gp ) {
    0, 255,146,  9,
   53, 239,104,  4,
  114, 252,149, 12,
  193, 249,122,  4,
  255, 237,107,  4};
*/

// Gradient palette "Lemon_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/lb/misc/tn/Lemon_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( Lemon_Yellow_gp ) {
    0, 255,173,  3,
   73, 239,139,  4,
  135, 252,180,  6,
  206, 247,152,  4,
  255, 244,124,  5};

// Gradient palette "bhw1_pinkpurple_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_pinkpurple.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
// Palette for Candy Girl
/*
DEFINE_GRADIENT_PALETTE( bhw1_pinkpurple_gp ) {
    0, 201, 10,135,
   58, 255,136,255,
   96, 237, 29,192,
  127,  91,  1,153,
  198, 161, 79,219,
  255,  34,  1,128};
*/

// Gradient palette "bhw1_HIMpink_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_HIMpink.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw1_HIMpink_gp ) {
    0, 222, 59,133,
   20, 255, 12,255,
   66, 217, 13, 75,
  122, 197, 30,145,
  163, 244,135,203,
  209, 197, 30,145,
  255, 217, 13, 75};

// Gradient palette "bhw1_purplered_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_purplered.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.
DEFINE_GRADIENT_PALETTE( PurpleRed_gp ) {
    0, 255,  0,  0,
  255, 107,  1,205};

// Gradient palette "bhw1_26_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_26.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw1_26_gp ) {
    0, 107,  1,205,
   35, 255,255,255,
   73, 107,  1,205,
  107,  10,149,210,
  130, 255,255,255,
  153,  10,149,210,
  170,  27,175,119,
  198,  53,203, 56,
  207, 132,229,135,
  219, 255,255,255,
  231, 132,229,135,
  252,  53,203, 56,
  255,  53,203, 56};

// Gradient palette "bhw4_029_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_029.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( MardiGras_gp ) {
    0,  75,  0,128, //Purple
   42,  75,  0,128,
   43,   0,184, 36, //Aqua Green
   84,   0,184, 36,
   85, 224,205,  4, //Yellow
  127, 224,205,  4,
  128,  75,  0,128, //Purple
  172,  75,  0,128,
  173,   0,184, 36, //Aqua Green
  215,   0,184, 36,
  216, 224,205,  4, //Yellow
  255, 224,205,  4};





//------------------SETUP------------------
void setup()  
{
  delay( 3000 ); // power-up safety delay
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentBlending = LINEARBLEND;
 
}
  
#define NUM_MODES 10
//------------------MAIN LOOP------------------
void loop() {
    switch (ledMode) {
       case 999: break;
       case 0: currentPalette = PartyColors_p; rainbow(); break;
       case 1: currentPalette = LavaColors_p; rainbow(); break;  
       case 2: currentPalette = ForestColors_p; rainbow(); break;
       case 3: currentPalette = OceanColors_p; rainbow(); break;                    
       case 4: currentPalette = Rainbow_gp; rainbow(); break;
       case 5: currentPalette = PurpleRed_gp; rainbow(); break;
       case 6: currentPalette = Lemon_Yellow_gp; rainbow(); break;
       case 7: currentPalette = AquaMagenta_gp; rainbow(); break;
       case 8: currentPalette = WhiteRainbow_gp; rainbow(); break;
       case 9: currentPalette = bhw1_HIMpink_gp; rainbowfade(); break;
       case 10: currentPalette = MardiGras_gp; rainbow(); break;
       case 888: ledMode=0; break;
       } 
       
        // key management section
    if (millis() - keyPrevMillis >= keySampleIntervalMs) {
        keyPrevMillis = millis();
        
        byte currKeyState = digitalRead(BUTTON_PIN);
        
        if ((prevKeyState == HIGH) && (currKeyState == LOW)) {
            keyPress();
        }
        else if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
            keyRelease();
        }
        else if (currKeyState == LOW) {
            longKeyPressCount++;
        }
        
        prevKeyState = currKeyState;
    }
       
   EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow    
   FastLED.show(); 
   FastLED.delay(20); 

}     


//BUTTON CONTROL STUFF
// called when button is kept pressed for less than 2 seconds
void shortKeyPress() {
    //Serial.println("short");
    ledMode++;
    if (ledMode > NUM_MODES){
    ledMode=0; }
}


// called when button is kept pressed for more than 2 seconds
void longKeyPress() {
    //Serial.println("long");
    ledMode=888;
}


// called when key goes from not pressed to pressed
void keyPress() {
    //Serial.println("key press");
    longKeyPressCount = 0;
}


// called when key goes from pressed to not pressed
void keyRelease() {
    //Serial.println("key release");
    if (longKeyPressCount >= longKeyPressCountMax) {
        longKeyPress();
    }
    else {
        shortKeyPress();
    }


    // other code goes here
}

     
// SOLID ----------------------------------------------------
void solid()
{
 
  fill_solid(leds, NUM_LEDS, CHSV( HUE, SATURATION, BRIGHTNESS));     
  FastLED.show();
   
   }
   

void rainbowfade()
{
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 0; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  FastLED.delay(20);
  
  }

void rainbow()
{
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  FastLED.delay(20);
  
  }

//this bit is in every palette mode, needs to be in there just once
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{ 
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex += STEPS;
  }
}



void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
    FastLED.show(); 
}
