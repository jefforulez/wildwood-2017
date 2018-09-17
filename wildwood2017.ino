
#include <SD.h>
#include <SPI.h>

// requires https://github.com/adafruit/Adafruit_VS1053_Library/releases
#include <Adafruit_VS1053.h>

// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)

// Feather ESP8266
#define VS1053_CS      16     // VS1053 chip select pin (output)
#define VS1053_DCS     15     // VS1053 Data/command select pin (output)
#define CARDCS          2     // Card chip select pin
#define VS1053_DREQ     0     // VS1053 Data request, ideally an Interrupt pin

#define TRIGGER_PIN   4

const static char* TRIGGER_AUDIO_FILE_00 = "TRAMCAR0.OGG" ;
const static char* TRIGGER_AUDIO_FILE_01 = "TRAMCAR1.OGG" ;

Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer( VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS ) ;

void setup() {
    Serial.begin( 115200 ) ;

    while ( ! Serial ) { delay(1) ; }

    Serial.println(F( "Looking for VS1053: " )) ;

    if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F( "Not Found" )) ;
     while (42) { delay( 10 ) ; } ;
    }

    // from https://github.com/adafruit/Adafruit_VS1053_Library/blob/master/examples/feather_player/feather_player.ino
    musicPlayer.useInterrupt( VS1053_FILEPLAYER_PIN_INT ) ;  // DREQ int

    Serial.println(F( "Found" )) ;
    musicPlayer.sineTest( 0x44, 1000 ) ;
    delay( 500 ) ;

    Serial.print( "Looking for SD Card: " ) ;
    if ( ! SD.begin( CARDCS ) ) {
        Serial.println( F( "Not Found" ) ) ;
        while (42) {
            static int delayMS = 0 ;
            delayMS += 1000 ;
            delay( delayMS ) ;
            musicPlayer.sineTest( 0x24, 500 ) ;
        }
    }
    Serial.println( "Found" ) ;
    musicPlayer.sineTest( 0x44, 500 ) ;

    pinMode( TRIGGER_PIN, INPUT ) ;
    delay( 100 ) ;
}

void loop() {

    int val = 0 ;

    while (42) {
        val = digitalRead( TRIGGER_PIN ) ;

        if ( val == HIGH ) {
            Serial.println( "TRIGGER_PIN is HIGH" ) ;

            musicPlayer.setVolume( 3, 3 ) ;

            if ( random(2) == 0 ) {
                musicPlayer.playFullFile( TRIGGER_AUDIO_FILE_00 ) ;
            }
            else {
                musicPlayer.playFullFile( TRIGGER_AUDIO_FILE_01 ) ;
            }
        }

        delay( 100 ) ;
    }
}
