
/* ColourClassis
 * DATE       VERSION DESCRIPTION
 * ========== ======= ====================================================================================================================
 *                  
* 20/01/2023  0.0     Cut from V4.15 of the live project. This is a demo version that shows off all the fonts and splash screens
* 13/02/2023  0.01    Starting to demo up how a clock could work
* 15/02/2023  0.02    Took the list cycle from V4.16, also added RTC and clock stuff. 
* 16/02/2023  0.03    Major work on this all day yesterday, nearly there. Now working on buzzer sounds and alarms
* 18/02/2023  0.05    splash0 on its own added.
* 20/02/2023  0.06    colour or b/w image processing
* 23/02/2023  0.07    Lots of image files, too slow - so split into separate directories, had to code for that. Also picks random top
*                     starting directory.
* 25/02/2023  0.08    try new jpeg decoder lib from bodmer                    
* 08/03/2023  0.09    Move config display to before wifi connection, error if no SD card.
* 20/03/2023  1.00    1st commercial Release
* 23/03/2023  1.01    Show 2 diff messages for Card Mount fail and SD read fail, restart after 6 secs if fails
*                     also show Time,Day of Week, date on startup screen 2
* 25/03/2023  1.02    Add: Temp Offset paramemter -10 to +10.  Hour Chime option.                    
* 11/04/2023  1.03    Wifi reconnect, no reboot, so just carry on,  temp offset -20 to +20, "+" button will skip to next cycle item.
* 20/06/2023  1.04    Increase wi-fi SSID from 20 to 40 characters
*/
const char versionno[] = "V1.04";
//#define DEBUG true   // true or false for debug output to serial, and maybe also to TFT??
#include <SPIFFS.h> // For talking to the file system if not using SD card
#include "Update.h"
#include <SPI.h> // for SD card
#include <FS.h> // for sd card
#include <SD.h> // SD card
//#include "SdFat.h"
//SdFat SD;
#include <TFT_eSPI.h>
#include <WiFi.h>
#include "Wire.h"
#include <NTPClient.h>
//#define WIFI_SSID "BTWholeHome-N7S"  // just for testing until I add these as params
//#define WIFI_PASS "6MgLQhEReRGw"
TFT_eSPI tft = TFT_eSPI();
#include <ArduinoJson.h>
File root;
File masterRoot;
#include <TJpg_Decoder.h> //newer faster bodmer version 
// Return the minimum of two values a and b
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))
#include "7seg.h" //7segment line drawing
#include <esp_now.h>
#include "WiFi.h"
#include "retrologo.h"
#include <pgmspace.h>

// v0.08 remove:JPEGDEC jpeg;

SPIClass sdSPI(HSPI);
#define SD_MISO     13
#define SD_MOSI     15
#define SD_SCLK     17
#define SD_CS       14

// Pins for the 7 Chip selects, via a 595 shift register
#define SR_DATA     5
#define SR_SRCLK    32 //19 used by tft driver
#define SR_RCLK     19 // i used in error, now change tft to 21
byte outgoing; // data to output SR
//byte button = 0; //Use on-board esp32s button.
byte button = 12; //Use dedicated button.
byte button2 = 2; //Use one connected to led, 
//int runMode = 0; // Runmode 0 = normal, 1 = setup - move lower
byte subMode; // used in Setup
byte displayValues[7] = {15,15,15,15,15,15,15};//initialise all at 15 = blank
byte currDisplayValues[7] = {15,15,15,15,15,15,15};//initialise all at 15 = blank
long lastGotTime = 0; //when did we get the data last?
char filename[10]; // for jpeg file reads from SD card
byte currFont; // Which display font to use
byte currFontList; // V4.14
bool fontCommas[21]; // Used to hold at startup whihc fonts have commas and whihc dont.
/* 0-20 are the 21 different jpeg file series
 * 21-30 are the 7seg patterns in 10 colours
 * 31-40 are the numberHatch 7seg patterns in 10 colours
 * 41-50 are the numberhatch2 7seg patterns in 10 colours
 * 51-60 are the numberhatch3 7seg patterns in 10 colours
 */
 byte currColour = 0; //pointer to array below
 unsigned int colours[10] = {TFT_WHITE, //white note use special TFT colours
                    TFT_RED, //red
                    TFT_GREEN, //green
                    TFT_BLUE, //blue
                    TFT_YELLOW, //yellow
                    TFT_ORANGE, //TFT_CYAN, //cyan
                    TFT_DARKGREEN, //TFT_MAGENTA, //magenta
                    TFT_MAGENTA, //TFT_PINK, //pink
                    TFT_DARKGREY, //TFT_MAROON, //maroon
                    TFT_CYAN};//TFT_PURPLE}; //purple
byte imageFontColonColours[21] = {5,0,5,7,0,4,0,1,0,9,0,0,0,0,3,4,1,0,0,5,3};              
bool foundSDcard = false; // flag to determin if we have an SD card or not. V2.06
bool refSuScreen = true; // flag to tell setup mode when to refresh the screen so it doesnt flicker V4.14
bool changeList = false; //flag used to determine if we're maintaining the CycleList
byte listCounter; // counter for where we are in the CycleList when maintaining it.
byte dayCounter; //counter for days in Setup Sun-Sat
unsigned long toggleColonTime;
bool colonUpdateNeeded = true;
bool colonClearNeeded = true;
bool changedFlag = false; //flag for when we change display mode in Main Loop - added for this clock new code.
byte displayListCounter = 0;
bool doneQuiet = false; //flag used switching in and out of quietTime.
int runMode = 0; // Runmode 0 = normal, 1 = setup
unsigned long displayListTimer;
// Json config file stuff
const String conffilenametwo = "/IPSconfig.txt";
struct Config {
  int displayMode;
  int displayFont;
  char wifi_ssid[40]; //increase to 40 from 20
  char wifi_pass[20];
  int fontList[30]; //V4.14 add in font list
  bool display12hrFormat;
  int quietStart;
  int quietEnd;
  int DSTadjust; // adjustment in Day light Saving Time
  int TZadjust; // adjustment in timezone
  int displayList[20];
  int displayTime[20];
  bool tempSensorInstalled;
  bool tempInFahrenheit;
  bool alarmOnOff[3];
  byte alarmHour[3];
  byte alarmMin[3];
  byte alarmTune[3];
  byte alarmDays[3]; //bitwise bit 1 = sun, bit 7 = Sat
  byte col_bw;  //Colour or bw images flag = 0 = both, 1 = colour, 2 = b/w
  int tempOffset; //-10 to +10 - V1.03 increase to -20 to +20
  bool hourChime; 
  byte hourChimeTune;
};
Config config; 
//Config oldConfig;
// end json config file stuff 

#include "REClock.h"
//clock vars:
unsigned long lastCheckTime;
unsigned long lastMove;
int lastMinute = 99;
unsigned long lastWifiConnectTry; // 

byte checkButton(); //define here before songs.
#include "songs.h" //Buzzer sounds
#include "mydht.h"

// firmware update procedure - Alan make this talk to the TFT displays, once working
void progressCallBack(size_t currSize, size_t totalSize) {
  Serial.printf("CALLBACK:  Update process at %d of %d bytes...\n", currSize, totalSize);
  outgoing = 63; // 7th digit from left
  write_sr();
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0, 4);  
  tft.fillScreen(TFT_BLACK);    
  tft.println("Firmware");
  tft.println("Update");
  tft.println(currSize);
  tft.println("of");
  tft.println(totalSize);
  tft.println("bytes");
}
// Wi-Fi Connection
void wifiSetup() {
    //DO NOT TOUCH
    //  This is here to force the ESP32 to reset the WiFi and initialise correctly.
    Serial.print("Initial WIFI status = ");
    Serial.println(WiFi.getMode());
    WiFi.disconnect(true);
    delay(100);
    Serial.print("After disconnect WIFI status = ");
    Serial.println(WiFi.getMode());
    delay(100);
    WiFi.mode(WIFI_STA);
    delay(100);
    Serial.print("New WIFI status = ");
    Serial.println(WiFi.getMode());
    // End silly stuff !!!

   // Connect to provided SSID and PSWD
    WiFi.begin(config.wifi_ssid, config.wifi_pass);

  
  // Set WIFI module to STA mode
  //WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Connecting to %s ", config.wifi_ssid);
  //WiFi.begin(WIFI_SSID, WIFI_PASS);
  byte retries=0;
  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //WiFi.disconnect(true);
    delay(500);
    //WiFi.begin(WIFI_SSID, WIFI_PASS);
    retries +=1;
    if ( retries == 40 ) {
        Serial.println();
        Serial.print("Connot connect ");
        //delay(5000);
        //ESP.restart();
        return;
    }
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) //ALAn for use with new TJpg_Decoder.h library V0.08
{
  /*Serial.print("tft_output:x=");
  Serial.print(x);
  Serial.print("tft_output:y=");
  Serial.print(y);
  Serial.print("tft_output:w=");
  Serial.print(w);
  Serial.print("tft_output:h=");
  Serial.println(h);*/
  
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}
#include "setup.h"
int delayT = 2400;
//####################################################################################################
// Main loop
//####################################################################################################
void loop() {
  // Wifi Reconnection
  if (WiFi.status() != WL_CONNECTED && config.wifi_ssid[0] != '\0' && millis() - lastWifiConnectTry > 15 * 60 * 1000) {
    Serial.println("Dropped off the wifi, attempting recall to wifiSetup()...");
    wifiSetup();
    lastWifiConnectTry = millis();
  }
  
  //RTC and NTP Time stuff first
  // Go get NTP time if we're connected to Wifi
  if ( WiFi.status() == WL_CONNECTED /*&& RTC_INSTALLED*/) {
    if (millis() < lastCheckTime ) {
      lastNTPtime = 0;  // Handle over flow of Millis every 50 days or so
      lastCheckTime = 0;
      lastMove = 0;
    }
    if (millis() - lastNTPtime > 1000 * 60) { //check every minute
      lastNTPtime = millis();
      Serial.println("Getting NTP time");
      getNTPtime();
      displayTime(); // get RTC clock time, to ensure Hour is set correctly as GetNTPtime messes it up!
    }
  }
  //END RTC NTP Time Stuff


  //is it time to change 
  if (millis() - displayListTimer > config.displayTime[displayListCounter] * 1000 ) {
    displayListTimer = millis();
    displayListCounter = (displayListCounter + 1) % 20;
    changedFlag = true;
    if (config.displayList[displayListCounter] == 255) displayListCounter = 0; // restart as soon as hit a 255 one.
    while (config.displayTime[displayListCounter] == 0 ) {
      displayListCounter = (displayListCounter + 1) % 20;   
    }
    Serial.print("Change List to :");  
    Serial.print(displayListCounter); 
    Serial.print(" runMode:");   
    Serial.print(runMode);   
    Serial.println(":runMode:");   
   
  }

  //Alan add in quiet time here
  if (quietTime() ) {
    if (!doneQuiet) {
      doneQuiet = true;
      // switch displays off, by turning backlight off.
      outgoing = 128; //Disable Backlight to displays
      write_sr(); // switch off
    }
  }
  else {
    if (doneQuiet) {
      doneQuiet = false;
      changedFlag = true; //force a display.
      // switch display back on
      outgoing = 127; //ok to switch backlight back on and redisplay prev score
      write_sr();
    }  
    if (runMode != 1) {
      switch (config.displayList[displayListCounter]) { // Now do whatever we need  
        case 0:   
        do_pictures(); break;
        case 1:
        do_time(); break;
        case 2:
        do_date(); break;
        case 3:
        if (config.tempSensorInstalled) do_temp(); else do_time(); break;  //do an alternate if not installed
        case 4:
        if (config.tempSensorInstalled) do_humidity();else do_date();  break; //do an alternate if not installed
        case 5:
        do_blank(); break;
      }
    } else { //in setup - now to do
    }
  }
  // Alarm Processing - need to get time, so repeat the code from the Clock:
  if (millis() - lastCheckTime > 500 * 1) {  // every 0.5 second - no faster as the RTC chip / board can fail if hit too fast, returns a duff value
    //if (!quietTime() ) Serial.println("Not Quiet Time"); else Serial.println("IT IS IT IS Quiet Time");
    lastCheckTime = millis();
    //Serial.println("Getting battery backup RTC module Time");
    displayTime(); // get RTC clock time
  
    if (Min > 59 ) Min = 0; // fix e.g. if no RTC module
    if (Hour > 23 ) Hour = 0; // fix if no RTC
    
    for (int i = 0;i < 3;i++) {
      if (config.alarmOnOff[i]) {
        if (config.alarmHour[i] == Hour && config.alarmMin[i] == Min && Sec < 6 &&
            bitRead(config.alarmDays[i],WeekDay) == 1) {
           cutShort = false; //play short versions if true
           Serial.print("Alarm ");
           Serial.print(i+1);
           Serial.print(" triggered, ");
           Serial.print("Song: ");
           Serial.print(config.alarmTune[i]);
           Serial.print(": ");
           Serial.println(songName[config.alarmTune[i]]);
           outgoing = 127; //ok to switch backlight back on
           write_sr();
           doneQuiet = false;
           changedFlag = true; //force a display.
           do_time(); // display time, even if in quietTime
           song[config.alarmTune[i]]();
        }
      }
    }
    //Hour Chime check - only run it if on the hour nd flag set and not quiet time.
    if (Min == 0 && Sec < 6 && config.hourChime && !quietTime() ) {
       cutShort = true; //play short versions if true  
       song[config.hourChimeTune]();
    }
    
  }
  //Now test for button presses,
  checkSetup();
  
} //end main loop
void do_blank() {
  if (changedFlag) {
    changedFlag = false;
    blankAll();
    outgoing = 128; //Disable Backlight to displays
    write_sr(); // switch off
  }
}     
void do_humidity() {
  // DHT Temp sensor stuff
  if (config.tempSensorInstalled) {
     if (changedFlag) {
      changedFlag = false;
      //dht.begin();
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      //float h = dht.readHumidity();
      float t;
      t = dht.readHumidity();
      
    
      // Check if any reads failed and exit early (to try again).
      if ( !isnan(t))  {
        Serial.print("Humidity="); 
        Serial.println(t); 
        // ALAN - Cut it up and display it here.
        //If over 100, or if over 10 handle 3,2,1 digits.   
        
        displayValues[6] = 15;
        displayValues[5] = t / 100; 
        if (displayValues[5] == 0) displayValues[5] = 15;
        displayValues[4] = t / 10; 
        if (displayValues[4] > 9) displayValues[4] = displayValues[4] - 10;
        if (displayValues[4] == 0) displayValues[4] = 15;
        displayValues[3] = int(t) % 10;
        displayValues[2] = 15;
        displayValues[1] = 15;
        displayValues[0] = 15;
        
        for (int i = 6; i>=0 ; i--) {
          currDisplayValues[i] = displayValues[i];
          bool comma = false;
          //if ( i == 5 || i == 3 ) comma = true;
          displayDigit(i, comma); //update the display
        }
        
        outgoing = 127-16; // 3rd from right
        write_sr();
        tft.setTextSize(1);
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
        tft.setCursor(0, 80, 4);  
        tft.fillScreen(TFT_BLACK);
        tft.println("Humidity"); 
        //tft.println(""); 
        tft.setTextSize(2);
        tft.print("%"); 
      }
      else {
        outgoing = 127-8; // Middle
        write_sr();
        tft.setTextSize(1);
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
        tft.setCursor(0, 0, 4);  
        tft.fillScreen(TFT_BLACK);
        tft.println("Cannot");
        tft.println("read");
        tft.println("HUM");
      }
    }
  } else {
        outgoing = 127-8; // Middle
        write_sr();
        tft.setTextSize(1);
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
        tft.setCursor(0, 0, 4);  
        tft.fillScreen(TFT_BLACK);
        tft.println("Humidity");
        tft.println("Sensor");
        tft.println("Not"); 
        tft.println("Installed");      
  }  
}
void do_temp() {
  // DHT Temp sensor stuff
  if (config.tempSensorInstalled) {
     if (changedFlag) {
      changedFlag = false;
      //dht.begin();
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      //float h = dht.readHumidity();
      float t;
      if (config.tempInFahrenheit) t = dht.readTemperature(true);// Read temperature as Fahrenheit (isFahrenheit = true)
      else t = dht.readTemperature();// Read temperature as Celsius (the default)
      
      // Check if any reads failed and exit early (to try again).
      if ( !isnan(t))  {
        t = t + config.tempOffset; //apply offset
        Serial.print("Temp="); 
        Serial.println(t); 
        // ALAN - Cut it up and display it here.
        //If over 100, or if over 10 handle 3,2,1 digits.   
        
        displayValues[6] = 15;
        displayValues[5] = t / 100; 
        if (displayValues[5] == 0) displayValues[5] = 15;
        displayValues[4] = t / 10; 
        if (displayValues[4] > 9) displayValues[4] = displayValues[4] - 10;
        if (displayValues[4] == 0) displayValues[4] = 15;
        displayValues[3] = int(t) % 10;
        displayValues[2] = 15;
        displayValues[1] = 15;
        displayValues[0] = 15;
        
        for (int i = 6; i>=0 ; i--) {
          currDisplayValues[i] = displayValues[i];
          bool comma = false;
          //if ( i == 5 || i == 3 ) comma = true;
          displayDigit(i, comma); //update the display
        }
        
        outgoing = 127-16; // 3rd from right
        write_sr();
        tft.setTextSize(1);
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
        tft.setCursor(0, 80, 4);  
        tft.fillScreen(TFT_BLACK);
        tft.println("Temp");
        //tft.println("");
        tft.setTextSize(2);
        if (config.tempInFahrenheit) tft.print("F"); else tft.print("C");
      }
      else {
        outgoing = 127-8; // Middle
        write_sr();
        tft.setTextSize(1);
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
        tft.setCursor(0, 0, 4);  
        tft.fillScreen(TFT_BLACK);
        tft.println("Cannot");
        tft.println("read");
        tft.println("TEMP");
      }
    }
  } else {
        outgoing = 127-8; // Middle
        write_sr();
        tft.setTextSize(1);
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
        tft.setCursor(0, 0, 4);  
        tft.fillScreen(TFT_BLACK);
        tft.println("Temp");
        tft.println("Sensor");
        tft.println("Not"); 
        tft.println("Installed");      
  }
}
void do_pictures() {
  //Only display if we have changed
  if (changedFlag) {
    changedFlag = false;
    File entry;
    File topentry;
    
    // Here we move the pointer on a bit more at random
    byte rnd = random(30);
    for (byte l = 0; l< rnd; l++) {
      entry = root.openNextFile();
      //Serial.print("Skipping over: ");
      //Serial.println(entry.name());
      if (! entry) {
        Serial.print("Start Dir changing" );
        //root.rewindDirectory();  
        while (true) {
          topentry = masterRoot.openNextFile();
          if (! topentry) {
            masterRoot.rewindDirectory();  
          }
          if (topentry.isDirectory()) {
            break;
          }
        }
        Serial.print("Start Dir changed:" );
        Serial.println(topentry.name());
        root = SD.open(topentry.name());
        
        
        entry = root.openNextFile(); 
      } 
    }
    //config.col_bw = 0; //just testing
    while (true) {
      entry = root.openNextFile();
      Serial.println(entry.name());
      if (! entry) {
        Serial.print("Start Directory changing" );
        //root.rewindDirectory();  
        while (true) {
          topentry = masterRoot.openNextFile();
          if (! topentry) {
            masterRoot.rewindDirectory();  
          }
          if (topentry.isDirectory()) {
            break;
          }
        }
        Serial.print("Start Dir changed:" );
        Serial.println(topentry.name());
        root = SD.open(topentry.name());        
        entry = root.openNextFile(); 
      } else if (entry.isDirectory() && (
           config.col_bw == 0 || //Col and b&w
           (config.col_bw == 1 && String(entry.name()).substring(strlen(entry.name())-3,strlen(entry.name())) != "_BW") || //Col only
           (config.col_bw == 2 && String(entry.name()).substring(strlen(entry.name())-3,strlen(entry.name())) == "_BW") //B&w only
        )) break;
    }
    //Serial.print(entry.name());
    char filename[100];
    if (entry.isDirectory()) {
       //Serial.println("/");
       //Serial.println(entry.name());
       //printDirectory(entry, numTabs + 1);
       // Alan - only get certain ones we need
       //sprintf(filename,"%s",entry.name());
       
       // If splash1.jpg missing, then assume we only have splash0.jpg to be repeated across all 7 screens
       bool oneMissing = false;
       int aloop = 7;
       sprintf(filename,"%s/splash%d.jpg",entry.name(),1);
       if (!SD.exists(filename)) {
        oneMissing = true; 
        aloop = 1;
       }
       blankAll();
       for (int k = 0; k < aloop; k++) { // now loop round splash 0-6
        //digitalWrite(CS_1, LOW); // Chip Select to low to Enable
        outgoing = 127;
        bitClear(outgoing,k);
        int y =k;
        if (oneMissing) {
          y = 0;
          outgoing = 0; //all displays
          Serial.println("only splash0");
        }
        write_sr();
        //sprintf(filename,"");
        sprintf(filename,"%s/splash%d.jpg",entry.name(),y); // gives us splash0.jpg to splash6.jpg
        //Serial.println(filename);
        // V0.08 - new jepg decoder starts
        //uint16_t w = 0, h = 0;
        //TJpgDec.getSdJpgSize(&w, &h, filename);
        //Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);
        // Draw the image, top left at 0,0
        TJpgDec.drawSdJpg(0, 0, filename);
         // V0.08 - new jepg decoder ends
        write_sr();
       }
       //delay(delayT);
       
    } 
    entry.close();  
    
  }
}
void do_time() {
  
  if (millis() - lastCheckTime > 500 * 1) {  // every 0.5 second - no faster as the RTC chip / board can fail if hit too fast, returns a duff value
    //if (!quietTime() ) Serial.println("Not Quiet Time"); else Serial.println("IT IS IT IS Quiet Time");
    lastCheckTime = millis();
    //Serial.println("Getting battery backup RTC module Time");
    displayTime(); // get RTC clock time
  }
  if (Min > 59 ) Min = 0; // fix e.g. if no RTC module
  if (Hour > 23 ) Hour = 0; // fix if no RTC
  byte tmpHour = Hour;
  // If 12 hour display Format, then modify hour. 
  if (config.display12hrFormat == true && Hour >= 12 ) tmpHour = Hour - 12;
  if (changedFlag) {
    //FIRST - Handle any font changes
    if (config.displayMode > 0) { //Change font if cycle, random or cycle colours
      if (config.displayMode == 2) {
        if (foundSDcard == true) currFont = random(61); //Random 0,60
        else currFont = random(40)+21; // make sure 21-60
      }
      else if (config.displayMode < 4) currFont++;  // Dont increment if ListCycle, Mode 4 or ListRandom, Mode 5 V4.16
      
      if (currFont > 60 && config.displayMode == 1) {
         if (foundSDcard == true) currFont = 0; //Cycle all
         else currFont = 21; 
      }
      if (currFont > 60 && config.displayMode == 3) currFont = 51; //Cycle 7 Seg colour
      else  if (currFont == 51 && config.displayMode == 3) currFont = 41; //Cycle 7 Seg colour
      else  if (currFont == 41 && config.displayMode == 3) currFont = 31; //Cycle 7 Seg colour
      else  if (currFont == 31 && config.displayMode == 3) currFont = 21; //Cycle 7 Seg colour 
      
      // V4.14 - List Cycle
      if (config.displayMode == 4) { //ListCycle
        currFontList++; //Cycle the pointer
        if (currFontList >= 30) currFontList = 0; //restart if get to end of 30 long array
        if (config.fontList[currFontList] == 255 ) currFontList = 0; //restart once hit a 255;
        if (config.fontList[currFontList] <= 60 ) currFont = config.fontList[currFontList]; 
        if (foundSDcard == false && currFont < 21) currFont = 21; //Force it to display something.
      } else
      if (config.displayMode == 5) { //ListRandom
        currFontList = random(30); // pick one at random
        while ( config.fontList[currFontList] == 255) {
           currFontList = random(30); // pick another at random  
        }
        if (config.fontList[currFontList] <= 60 ) currFont = config.fontList[currFontList]; 
        if (foundSDcard == false && currFont < 21) currFont = 21; //Force it to display something.
      }
      blankAll(); // need to blank as well - for the transition from jpeg to rendered as rendered doesnt refresh whole screen
      Serial.print("Change Font = ");
      Serial.println(currFont);
      currColour = imageFontColonColours[currFont]; // Array to try match colon font to the image - hard coded at moment!!
    }
  }
  if ( Min != lastMinute || changedFlag) { //Only do if changed or time has changed.
    changedFlag = false;
    lastMinute = Min;
    Serial.println("Checking Clock");
    Serial.print("Hour: ");
    Serial.print(tmpHour);
    Serial.print(" Min: ");
    Serial.println(Min);
    displayValues[6] = 15;
    displayValues[5] = tmpHour / 10;
    displayValues[4] = tmpHour % 10;
    displayValues[3] = 14; //blank until I get a Colon
    displayValues[2] = Min / 10;
    displayValues[1] = Min % 10;
    displayValues[0] = 15;
    for (int i = 6; i>=0 ; i--) {
      currDisplayValues[i] = displayValues[i];
      if (i != 3) displayDigit(i, false); //update the display
    }
  }
  // Colon flashing - Ooh err!
  if (millis() - toggleColonTime  < 500) {
    if (colonUpdateNeeded) {
      displayColon(3);
      colonUpdateNeeded = false;
      colonClearNeeded = true;
    }
  }
  else if (millis() - toggleColonTime  < 1000) {
    if (colonClearNeeded) {
      clearColon(3);
      colonClearNeeded = false;
      colonUpdateNeeded = true;
    }
  } else { toggleColonTime = millis();}
}
void do_date() {
  //Only display if we have changed
  if (changedFlag) {
    changedFlag = false;
    displayValues[5] = MonthDay / 10;
    displayValues[4] = MonthDay % 10;
    displayValues[3] = Month / 10;
    displayValues[2] = Month % 10;
    displayValues[1] = Year / 10;
    displayValues[0] = Year % 10;
    displayValues[6] = 15;
    
    for (int i = 6; i>=0 ; i--) {
      currDisplayValues[i] = displayValues[i];
      bool comma = false;
      if ( i == 4 || i == 2 ) comma = true;
      displayDigit(i, comma); //update the display
    }
    outgoing = 126; // 1st display
    write_sr();
    tft.setTextSize(1);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    //tft.fillScreen(TFT_BLACK);
    tft.setCursor(40, 100, 4);  
    tft.print("DATE");
    //delay(2000);
  }  
}


void displayDigit(int digit, bool comma){
  //Serial.print("Display Digit:");  
  //Serial.print(digit); 
  //setup the digit to write to.
  outgoing = 127;
  bitClear(outgoing,6-digit); //clear the digit we need to enable it. 6-digit as they are in reverse order
  write_sr(); 

  //now handle blanks
  if (currDisplayValues[digit] > 9) {
    tft.fillScreen(0); //blank out
    outgoing = 127; // disable all digits
    write_sr();
    return;
  }
  
  //Alan - here do the display - whatever that is.  
  if (currFont < 21 ) {
    //doing a jpeg display
    //Alan here add in the comma font stuff
    if (fontCommas[currFont] == true && comma == true){
      sprintf(filename,"/%dc.jpg",currFont*10+currDisplayValues[digit]); // gives us 0c.jpg to nnNc.jpg  
    } else sprintf(filename,"/%d.jpg",currFont*10+currDisplayValues[digit]); // gives us 0.jpg to nnN.jpg nn is font and N number
    // V0.08 - new jepg decoder starts
    //uint16_t w = 0, h = 0;
    //TJpgDec.getSdJpgSize(&w, &h, filename);
    //Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);
    // Draw the image, top left at 0,0
    TJpgDec.drawSdJpg(0, 0, filename);
    // V0.08 - new jepg decoder ends
  }
  else if (currFont < 31 ){
    //doing 7number - this is a solid coloured 7 seg display
    tft.fillScreen(TFT_BLACK);
    currColour = currFont-21; 
    draw7Number(currDisplayValues[digit],0,0,12, colours[currColour] , TFT_BLACK,1);
    if (comma) {
      tft.fillCircle(123,210,10,colours[currColour]);
      tft.fillTriangle(113,210,133,213,108,238,colours[currColour]);
    }  
  }
  else if (currFont < 41 ){
    //doing 7numberHatch - this is a striped colour and black 7 seg display
    tft.fillScreen(TFT_BLACK);
    currColour = currFont-31;
    draw7NumberHatch(currDisplayValues[digit],0,0,12, colours[currColour] , TFT_BLACK,1);
    if (comma) {
      tft.fillCircle(123,210,10,colours[currColour]);
      tft.fillTriangle(113,210,133,213,108,238,colours[currColour]);
    }  
  }
  else if (currFont < 51 ){
    //doing 7numberHatch3 - this is a solid 7 seg display but with a thin white line down the middle of each number. 
    tft.fillScreen(TFT_BLACK);
    currColour = currFont-41;
    draw7NumberHatch3(currDisplayValues[digit],0,0,12, colours[currColour] , TFT_BLACK,1);
    if (comma) {
      tft.fillCircle(123,210,10,colours[currColour]);
      tft.fillTriangle(113,210,133,213,108,238,colours[currColour]);
    }  
  }
  else if (currFont < 61 ){
    //doing circles
    currColour = currFont-51;
    draw7circles(currDisplayValues[digit], colours[currColour] , TFT_BLACK);
    if (comma) {
      tft.fillCircle(123,210,10,colours[currColour]);
      tft.fillTriangle(113,210,133,213,108,238,colours[currColour]);
    }  
  }
  else {
    // catch all here alan
  }
  //Disable all digits
  outgoing = 127; // disable all digits
  write_sr();
}
void displayColon(int digit){
  outgoing = 127;
  bitClear(outgoing,6-digit); //clear the digit we need to enable it. 6-digit as they are in reverse order
  write_sr(); 
  if (currFont < 21 ) {
    //doing a jpeg display
    //Alan here add in the comma font stuff
    sprintf(filename,"/%dcolon.jpg",currFont*10); // gives us 0colon.jpg to nncolon.jpg nn is font
    // V0.08 - new jepg decoder starts
    //uint16_t w = 0, h = 0;
    //TJpgDec.getSdJpgSize(&w, &h, filename);
    //Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);
    // Draw the image, top left at 0,0
    TJpgDec.drawSdJpg(0, 0, filename);
    // V0.08 - new jepg decoder ends
  }
  else {
    tft.fillScreen(TFT_BLACK);
    //currColour = 0; 
    //draw7Number(3,0,0,8, colours[currColour] , TFT_BLACK,1);
    tft.fillRect(60,100,13,26,colours[currColour]);
    tft.fillRect(60,140,13,26,colours[currColour]);
  }
  outgoing = 127; // disable all digits
  write_sr();
}  
void clearColon(int digit){
  outgoing = 127;
  bitClear(outgoing,6-digit); //clear the digit we need to enable it. 6-digit as they are in reverse order
  write_sr(); 
  tft.fillScreen(TFT_BLACK);
  outgoing = 127; // disable all digits
  write_sr();   
}

void write_sr() {
      digitalWrite(SR_RCLK, LOW);
      shiftOut(SR_DATA, SR_SRCLK, MSBFIRST, outgoing);
      digitalWrite(SR_RCLK, HIGH);       
}   
void blankAll() {
  //delay(500);
  outgoing = 128; // enable all digits, but switch off backlight
  write_sr();
  tft.fillScreen(TFT_BLACK);
  outgoing = 255; // disable all digits and no backlight on. //4.07 change
  write_sr();  
  for (int i = 0; i<7 ; i++) { 
    currDisplayValues[i] = 255; //force them all to be re-drawn
  }
  //delay(500); //alan just see whats going on here.
}


void checkSetup() {
  //Now test for button presses, do different things depending on runmode
  //Serial.println("Runmode=");
  //Serial.println(runMode);
  int ck = checkButton();
  if (runMode != 1 && ck >= 2) {
    runMode = 1; 
    subMode = 0;
    refSuScreen = true; //Set screen refresh flag
    // Save the config that might change
    //oldConfig.displayMode = config.displayMode;
    //oldConfig.displayFont =config.displayFont;
    Serial.println("Start Setup");
  }
  else if (runMode != 1 && ck == 1) {
    Serial.println("skip to next setting"); 
    displayListTimer = 0; //zeroises this so will skip next time around
    delay(500); // debounce
  }
  //RunMode 1 processing - Setup
  if (runMode == 1) doSetUp();
}
byte checkButton() {
  //if button is pressed, return 1 for short press, 2 for long press, 0 for no button
  long btnStartTime = 0;  
  //Serial.println("Button read");
  //Now we have 2 buttons, button 1 will generate a Long press =2 or a very long press = 3
  //Button2 will generate short =1 o
  if (digitalRead(button) == 0 ){
    Serial.print("Button 2 Press : ");
    btnStartTime = millis();
    while (digitalRead(button) == 0 ) {
    }
    if (millis() - btnStartTime > 1500) { //no need for 3 secs now, 1.5 sec should do.
      Serial.println("Really Long");
      return 3;   
    }  
    else {
      Serial.println("Long");
      return 2;
    }
  } else if (digitalRead(button2) != 0 ){ 
      Serial.print("Button 2 Press : Short");
      delay(200); //add this in to slow it down
      return 1; 
  } else if (digitalRead(0) == 0 ){ //also read the inbuit button
    Serial.print("Button Press : ");
    btnStartTime = millis();
    while (digitalRead(0) == 0 ) {
      
    }
    if (millis() - btnStartTime > 3000) {
      Serial.println("Really Long");
      return 3;   
    }  
    else if (millis() - btnStartTime > 300) {
      Serial.println("Long");
      return 2; 
    } else {
      Serial.println("Short");
      return 1;
    }
  }
  else return 0;
}
void saveAjConfig(bool audit) {
  
  Serial.print("Saving Json Config to file"); 
  Serial.println(conffilenametwo); 
  File file = (foundSDcard == true) ? SD.open(conffilenametwo, FILE_WRITE): SPIFFS.open(conffilenametwo, FILE_WRITE);
  if(!file){
    Serial.println("There was an error opening the file for writing"); 
  } 
  else {
    //Now write the JSON config file
    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    DynamicJsonDocument doc(5120);
  
    // Set the values in the document
    doc["wifi_ssid"] = config.wifi_ssid; 
    doc["wifi_pass"] = config.wifi_pass; 
    doc["displayMode"] = config.displayMode;
    doc["displayFont"] = config.displayFont;
    doc["display12hrFormat"] = config.display12hrFormat;
    doc["quietStart"] = config.quietStart;
    doc["quietEnd"] = config.quietEnd;
    doc["DSTadjust Comment"] = "0=non,1=UK/Europe,2=USA/Canada";
    doc["DSTadjust"] = config.DSTadjust; 
    doc["TZadjust"] = config.TZadjust;
    doc["tempSensorInstalled"] = config.tempSensorInstalled;
    doc["tempInFahrenheit"] = config.tempInFahrenheit;
    doc["col_bw Comment"] = "0=all images, 1=colour images, 2=B&W images";
    doc["col_bw"] = config.col_bw;
    doc["tempOffset"] = config.tempOffset;
    doc["hourChime"] = config.hourChime;
    doc["hourChimeTune"] = config.hourChimeTune;

    
    JsonArray alarmOnOff = doc.createNestedArray("alarmOnOff");
    JsonArray alarmMin = doc.createNestedArray("alarmMin");
    JsonArray alarmHour = doc.createNestedArray("alarmHour");
    JsonArray alarmTune = doc.createNestedArray("alarmTune"); 
    JsonArray alarmDays = doc.createNestedArray("alarmDays"); 
    for (int i = 0; i<3;i++) {
      alarmOnOff.add(config.alarmOnOff[i]);
      alarmMin.add(config.alarmMin[i]);
      alarmHour.add(config.alarmHour[i]);
      alarmTune.add(config.alarmTune[i]);
      alarmDays.add(config.alarmDays[i]);
    }  
        
    doc["Font List Comment"] = "List of fonts,0-60, 255 to end list";
    JsonArray fontList = doc.createNestedArray("fontList"); 
    for (int i=0;i<30;i++){
      fontList.add(config.fontList[i]); 
    }  
    doc["displayList Comment"] = "0=Images, 1=Time, 2=Date, 3=Temp, 4=Humidity,5=blank, 255 to end list";
    JsonArray displayList = doc.createNestedArray("displayList"); 
    for (int i=0;i<20;i++){
      displayList.add(config.displayList[i]); 
    }  
    doc["displayTime Comment"] = "Time in seconds for each List Item to be displayed";
    JsonArray displayTime = doc.createNestedArray("displayTime"); 
    for (int i=0;i<20;i++){
      displayTime.add(config.displayTime[i]); 
    }  
    
    // Serialize JSON to file
    if (serializeJsonPretty(doc, file) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    // Close the file
    file.close();
    }
    // Now let's print the whole file as a debug if audit flag on
     // Open file for reading
    if (audit) {
      file = (foundSDcard == true) ? SD.open(conffilenametwo): SPIFFS.open(conffilenametwo); 
      if (!file) {
        Serial.println(F("Failed to read json file"));      
      }
      else {
        Serial.println("Json file contents:");
        while (file.available()) {
      
        // Extract each characters by one by one
          Serial.print((char)file.read());
        }
        Serial.println("Json file contents End");
        // Close the file
        file.close();
      }
    }
}

void doSetUp() {
    outgoing = 63; // only enable rhs digit
    write_sr();
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, 0, 4);  
    if ( refSuScreen){
      tft.fillScreen(TFT_BLACK);
      refSuScreen = false;
    }
    
    if (subMode == 0) {
      tft.println("Setup");
      tft.print("Ver:");
      tft.println(versionno);
      tft.println("<press>");
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1 || btn == 2)  { //long or short press
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 10; 
        refSuScreen = true;
        listCounter = 0;
      }
      else if (btn ==3){
        runMode = 0; 
        blankAll();
        Serial.println("Exit Setup");
        refSuScreen = true;  
      }
    } 
    else if (subMode == 10){ //Alarm Processing
      tft.print("Alarm ");
      tft.println(listCounter+1);
      tft.println("");
      switch (config.alarmOnOff[listCounter]) {
        case false:
        tft.println("Off");  
        break;
        case true:
        tft.println("On");  
        break;
      }
      //Show current time
      tft.println("");
      tft.print(config.alarmHour[listCounter]);
      tft.print(":");
      tft.println(config.alarmMin[listCounter]);
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.alarmOnOff[listCounter] = !config.alarmOnOff[listCounter]; //toggle
        refSuScreen = true;
      } else if (btn == 2) { //long press
        if (listCounter == 2 ) { //Exit if at end of array
          Serial.print("Exit submode :");Serial.println(subMode);
          subMode = 15;   
          changeList = false;
          saveAjConfig(false);
        } else listCounter++; 
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
        saveAjConfig(false);
      } 
    } else if (subMode == 15){ //Hour Chime Processing
      tft.print("Hour Chime ");
      tft.println("");
      switch (config.hourChime) {
        case false:
        tft.println("Off");  
        break;
        case true:
        tft.println("On");  
        break;
      }
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.hourChime = !config.hourChime; //toggle
        refSuScreen = true;
      } else if (btn == 2) { //long press
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 20;   
        changeList = false;
        saveAjConfig(false);
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
        saveAjConfig(false);
      } 
    } else if (subMode == 20){ //Ask if change Alarms
      tft.println("Change");
      tft.println("Alarm");
      tft.println("Settings?");
      tft.println("");
      switch (changeList) {
        case false:
        tft.println("No");  
        break;
        case true:
        tft.println("Yes");
        break;
      }
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        changeList = !changeList;
        refSuScreen = true;
      } else if (btn >= 2) { //either longer press
        Serial.print("Exit submode :");Serial.println(subMode); 
        if (changeList) {
          subMode = 21; // go to list maintain
          listCounter = 0;
        }
        else subMode = 30; //Got to next
        refSuScreen = true;
      }
    } else if (subMode == 21){ // Hour - note we need to go through all 3 alarms, hH,MM,tune,weekdays
      tft.print("Alarm ");
      tft.println(listCounter+1);
      tft.println("Hour");
      tft.println("");
      tft.println(config.alarmHour[listCounter]);
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.alarmHour[listCounter]++;
        if (config.alarmHour[listCounter] > 23) config.alarmHour[listCounter] = 0; 
        refSuScreen = true;
      } else if (btn == 2) { //long 
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 22; 
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      } 
    } else if (subMode == 22){ // Minute
      tft.print("Alarm ");
      tft.println(listCounter+1);
      tft.println("Minute");
      tft.println("");
      tft.println(config.alarmMin[listCounter]);
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.alarmMin[listCounter]++;
        if (config.alarmMin[listCounter] > 59) config.alarmMin[listCounter] = 0; 
        refSuScreen = true;
      } else if (btn == 2) { //either longer press
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 23; 
        changeList = false;
        refSuScreen = true;
      } else if (btn == 3) { //really long press
        config.alarmMin[listCounter] = 0;
        changeList = false;
        refSuScreen = true;
      }
    } else if (subMode == 23){ // Tune
      tft.print("Alarm ");
      tft.println(listCounter+1);
      tft.println("Tune");
      tft.println("");
      tft.println(config.alarmTune[listCounter]);
      if (config.alarmTune[listCounter] < 16 ) tft.println(songName[config.alarmTune[listCounter]]);
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.alarmTune[listCounter]++;
        if (config.alarmTune[listCounter] > 15) config.alarmTune[listCounter] = 0; 
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 24; 
        changeList = false;
        refSuScreen = true;
        dayCounter = 0;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      }  
    } else if (subMode == 24){ // Days of Week
      tft.print("Alarm ");
      tft.println(listCounter+1);
      tft.println("Day:");
      tft.println(" ");
      switch (dayCounter) {
        case 0: tft.println("Sun"); break; 
        case 1: tft.println("Mon"); break; 
        case 2: tft.println("Tue"); break; 
        case 3: tft.println("Wed"); break; 
        case 4: tft.println("Thu"); break; 
        case 5: tft.println("Fri"); break; 
        case 6: tft.println("Sat"); break; 
      }
      if (bitRead(config.alarmDays[listCounter],dayCounter)) tft.println("On");
      else tft.println("Off");
      
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        if (bitRead(config.alarmDays[listCounter],dayCounter)) bitClear(config.alarmDays[listCounter],dayCounter);
        else bitSet(config.alarmDays[listCounter],dayCounter);
        refSuScreen = true;
      } else if (btn == 2) { //long press
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 24; 
        changeList = false;
        refSuScreen = true;
        dayCounter++;
        if (dayCounter > 6) {
          listCounter++;
          subMode = 21; //back to next hour
          if (listCounter >2) subMode = 25; //on to next group of settings
        }
      }
      else if (btn ==3){
        subMode = 25; //Jump to next section
        refSuScreen = true;  
      }  
    } else if (subMode == 25){ // Tune
      tft.print("Hr.Chime ");
      tft.println("Tune");
      tft.println("");
      tft.println(config.hourChimeTune);
      if (config.hourChimeTune < 16 ) tft.println(songName[config.hourChimeTune]);
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.hourChimeTune++;
        if (config.hourChimeTune > 15) config.hourChimeTune = 0; 
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 30; 
        changeList = false;
        refSuScreen = true;
        dayCounter = 0;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      }  
    } else if (subMode == 30){ // Night/Quiet Start Time in hours
      tft.println("Night");
      tft.println("Start");
      tft.println("");
      tft.println(config.quietStart);
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.quietStart++;
        if (config.quietStart > 23) config.quietStart = 0; 
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 31; 
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      } 
    } else if (subMode == 31){ // Night/Quiet End Time in hours
      tft.println("Night");
      tft.println("End");
      tft.println("");
      tft.println(config.quietEnd);
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.quietEnd++;
        if (config.quietEnd > 23) config.quietEnd = 0; 
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 40; 
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      } 
    } else if (subMode == 40){ // DST Adjust 0,1,2
      tft.println("DST");
      tft.println("Adj");
      tft.println("");
      tft.println(config.DSTadjust);
      switch (config.DSTadjust) {
        case 0: tft.println("None"); break; 
        case 1: tft.println("UK/EU"); break; 
        case 2: tft.println("USA/Can"); break; 
      }
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.DSTadjust++;
        if (config.DSTadjust > 2) config.DSTadjust = 0; 
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 50; 
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      } 
    } else if (subMode == 50){ // TZ Adjust
      tft.println("TZ");
      tft.println("Adj");
      tft.println("");
      tft.println(config.TZadjust);
      
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.TZadjust++;
        if (config.TZadjust > 12) config.TZadjust = -12; 
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 60; 
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      } 
    } else if (subMode == 60){ // temp Sensor
      tft.println("Temp &");
      tft.println("Humidity");
      tft.println("Sensor");
      tft.println("Present");
      tft.println(" ");
      //tft.println(config.tempSensorInstalled);
      if (config.tempSensorInstalled) tft.println("Yes"); else tft.println("No");
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.tempSensorInstalled = !config.tempSensorInstalled;
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        if (config.tempSensorInstalled) subMode = 70; else subMode = 80;
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      } 
    } else if (subMode == 70){ // F or C
      tft.println("Temp in");
      tft.println("Fahrenheit");
      tft.println("or");
      tft.println("Centigrade");
      tft.println(" ");
      if (config.tempInFahrenheit)  tft.println("Fahrenheit"); else tft.println("Centigrade");
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.tempInFahrenheit = !config.tempInFahrenheit;
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 75;
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      } 
    } 
    else if (subMode == 75){ // Temp Offset
      tft.println("Temp");
      tft.println("offset");
      tft.println("");
      tft.println(config.tempOffset);
      
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.tempOffset++;
        if (config.tempOffset > 20) config.tempOffset = -20; 
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 80; 
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      } 
    }
    else if (subMode == 80){ // 12 / 24 hr time
      tft.println("24 or 12");
      tft.println("Hr time");
      tft.println(" ");
      if (config.display12hrFormat)  tft.println("12"); else tft.println("24");
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.display12hrFormat = !config.display12hrFormat;
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 90;
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 90; //jump to display Mode
        refSuScreen = true;  
      } 
    } else if (subMode == 90){ //Display Mode
      tft.println("Display");
      tft.println("Mode");
      tft.println("");
      switch (config.displayMode) {
        case 0:
        tft.println("0-Single");  
        break;
        case 1:
        tft.println("1-Cycle All");  
        break;
        case 2:
        tft.println("2-Random");  
        break;
        case 3:
        tft.println("3-7 Seg Colour Cycle");  
        break;
        case 4:
        tft.println("4-List Cycle");  
        break;
        case 5:
        tft.println("5-List Random");  
        break;
      }
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.displayMode++;
        /*if (config.displayMode > 3) config.displayMode = 0;  //V4.14c - temp disable List Cycle*/
        if (config.displayMode > 5) config.displayMode = 0; //V4.16 ListCycle back in as an option, add ListRandom too
        refSuScreen = true;
      } else if (btn == 2) { //long
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 100; 
        if (config.displayMode == 0 ) {
          //subMode++; // Skip subMode 4 if displayMode = 0, ie only showing 1 fixed font, so dont need time interval
          //setup for next section
          currFont = config.displayFont;
          blankAll(); 
          refSuScreen = true;
        } else if (config.displayMode >3 ) { //The 2 List Modes - jump out to subMode 110
          Serial.print("Exit submode :");Serial.println(subMode);
          subMode = 110;  
          changeList = false;
        }
        refSuScreen = true;
      }
      else if (btn ==3){
        subMode = 255; //Jump to end
        refSuScreen = true;  
      } 
    }
    //Alan Add in submode 110 - ask if change list yes no - if no back to sub3, if yes to mode 21
    else if (subMode == 110){ //Maintain Cycle/Random List
      tft.println("Maintain");
      tft.println("FONT");
      tft.println("List");
      tft.println("");
      switch (changeList) {
        case false:
        tft.println("No");  
        break;
        case true:
        tft.println("Yes");
        break;
      }
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        changeList = !changeList;
        refSuScreen = true;
      } else if (btn >= 2) { //either longer press
        Serial.print("Exit submode :");Serial.println(subMode); 
        if (changeList) {
          subMode = 111; // go to list maintain
          listCounter = 0;
          if (config.fontList[0] < 61) currFont = config.fontList[0]; //Set font to display first in list, if it's there
        }
        else subMode = 100; //Got to next
        refSuScreen = true;
      }
      
    } else if (subMode == 111){ //FONT List Maintenance
      tft.print("List #");
      tft.println(listCounter);
      tft.println("");
      if (config.fontList[listCounter] == 255) tft.println("<END>");
      else tft.println(config.fontList[listCounter]);
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.fontList[listCounter]++;
        if (config.fontList[listCounter] >= 255) config.fontList[listCounter] = 0;
        if (config.fontList[listCounter] > 60) config.fontList[listCounter] = 255;
        if (foundSDcard == false && config.fontList[listCounter] <21) config.fontList[listCounter] = 21; 
        currFont = config.fontList[listCounter];
        blankAll(); 
        refSuScreen = true;
      } else if (btn == 2) { //long press
        if (listCounter == 29 || config.fontList[listCounter] == 255 ) { //Exit if at end of array, or we have chosen end
          if (config.fontList[0] == 255) config.fontList[0] = 21; //need to have at least 1 entry in the list
          Serial.print("Exit submode :");Serial.println(subMode);
          subMode = 100;   
        } else listCounter++; 
        refSuScreen = true;
      } else if (btn == 3) { //really long press
         config.fontList[listCounter] = 255; //reset to beginning, quickway to get to <END>
         blankAll();
         refSuScreen = true;
      }
    } else if (subMode == 100){ //Font (Pattern)
      //outgoing = 128+63; // only enable rhs digit
      //write_sr();
      if (foundSDcard == false && config.displayFont <21) config.displayFont =21; 
      
      if (config.displayMode == 3 && config.displayFont < 21) {
        config.displayFont = 21; //needs to be 21 or more
        currFont = config.displayFont; 
        blankAll(); 
      }
      //tft.setTextSize(1);
      //tft.fillScreen(TFT_BLACK);
      //tft.setCursor(0, 0, 4);  
      tft.println("Font");
      tft.println("(Start)");
      tft.println("");
      tft.println(config.displayFont);
      tft.println("");
      if (config.displayFont < 21 ) tft.println("jpeg");
      else tft.println("7 Segment");
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.displayFont++;
        if (config.displayFont > 60) config.displayFont = 0;
        if (foundSDcard == false && config.displayFont <21) config.displayFont =21; 
        currFont = config.displayFont;
        blankAll(); 
        refSuScreen = true;
      } else if (btn == 2) { //long press
        Serial.print("Exit submode :");Serial.println(subMode);
        subMode = 120; 
        refSuScreen = true;
      } else if (btn == 3) { //really long press
         config.displayFont = 0; //reset to beginning
         blankAll();
         refSuScreen = true;
      }
    } else if (subMode == 120){ //Display List
      tft.println("Maintain");
      tft.println("Display");
      tft.println("List");
      tft.println("");
      switch (changeList) {
        case false:
        tft.println("No");  
        break;
        case true:
        tft.println("Yes");
        break;
      }
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        changeList = !changeList;
        refSuScreen = true;
      } else if (btn >= 2) { //either longer press
        Serial.print("Exit submode :");Serial.println(subMode); 
        if (changeList) {
          subMode = 121; // go to list maintain
          listCounter = 0;
        }
        else subMode = 130; //Got to next
        refSuScreen = true;
      }
    } else if (subMode == 121){ //Display List Maintenance
      tft.print("List #");
      tft.println(listCounter);
      tft.println("");
      switch (config.displayList[listCounter]) {
        case 0: tft.print("0-Images"); break;
        case 1: tft.print("1-Time"); break;
        case 2: tft.print("2-Date"); break;
        case 3: tft.print("3-Temp"); break;
        case 4: tft.print("4-Humid"); break;
        case 5: tft.print("5-Blank"); break;
        case 255: tft.print("<END>"); break;
      }
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.displayList[listCounter]++;
        if (config.displayList[listCounter] >= 255) config.displayList[listCounter] = 0;
        if (config.displayList[listCounter] > 5) config.displayList[listCounter] = 255; //Only 6 at mo 0-5
        refSuScreen = true;
      } else if (btn == 2) { //long press
        if (listCounter == 19 || config.displayList[listCounter] == 255 ) { //Exit if at end of array, or we have chosen end
          if (config.displayList[0] == 255) config.displayList[0] = 1; //need to have at least 1 entry in the list
          Serial.print("Exit submode :");Serial.println(subMode);
          subMode = 130;   
        } else listCounter++; 
        refSuScreen = true;
      } else if (btn == 3) { //really long press
         subMode = 130;
         refSuScreen = true;
      } 
    }
    else if (subMode == 130){ //Display List Times
      tft.println("Maintain");
      tft.println("Display");
      tft.println("Time");
      tft.println("");
      switch (changeList) {
        case false:
        tft.println("No");  
        break;
        case true:
        tft.println("Yes");
        break;
      }
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        changeList = !changeList;
        refSuScreen = true;
      } else if (btn >= 2) { //either longer press
        Serial.print("Exit submode :");Serial.println(subMode); 
        if (changeList) {
          subMode = 131; // go to list maintain
          listCounter = 0;
        }
        else subMode = 255; //Got to next
        refSuScreen = true;
      }
    } else if (subMode == 131){ //Display List TimeMaintenance
      tft.print("List #");
      tft.println(listCounter);
      tft.println("");
      switch (config.displayList[listCounter]) {
        case 0: tft.print("0-Images"); break;
        case 1: tft.print("1-Time"); break;
        case 2: tft.print("2-Date"); break;
        case 3: tft.print("3-Temp"); break;
        case 4: tft.print("4-Humid"); break;
        case 5: tft.print("5-Blank"); break;
        case 255: tft.print("<END>"); break;
      }
      tft.println("");
      tft.println(config.displayTime[listCounter]);
      outgoing = 127; // disable all digits
      write_sr();
      byte btn = checkButton();
      if (btn == 1) { //short press
        config.displayTime[listCounter]++;
        if (config.displayTime[listCounter] >= 600) config.displayTime[listCounter] = 0; // 10 mins max
        if (config.displayTime[0] == 0) config.displayTime[0] = 1; //need to have time in first entry
        switch (config.displayTime[listCounter]) {
           case 11: config.displayTime[listCounter] = 20; break;  
           case 21: config.displayTime[listCounter] = 30; break;
           case 31: config.displayTime[listCounter] = 60; break;
           case 61: config.displayTime[listCounter] = 120; break;
           case 121: config.displayTime[listCounter] = 300; break;
           case 301: config.displayTime[listCounter] = 600; break;
        } // allow 0-10s then 20,30,60,120,300,600s
        refSuScreen = true;
      } else if (btn == 2) { //long press
        if (listCounter == 19 || config.displayList[listCounter] == 255 ) { //Exit if at end of array, or we get to end of entries in use
          Serial.print("Exit submode :");Serial.println(subMode);
          subMode = 255;   
        } else listCounter++; 
        refSuScreen = true;
      } else if (btn == 3) { //really long press
         subMode = 255;
         refSuScreen = true;
      } 
    }
    else if (subMode == 255){ //Exit
      tft.println("Press Key");
      tft.println("to exit");
      tft.println("");  
      outgoing = 127; // disable all digits
      write_sr();
      if (checkButton() >0) { //any press to get out
        runMode = 0; 
        blankAll();
        Serial.println("Exit Setup");
        refSuScreen = true;
        // now check if need to save config
        Serial.println("Saving Config"); 
        saveAjConfig(true);
      }
    } 
}

boolean quietTime() {
  if (config.quietEnd > config.quietStart) { //e.g. 21:00 - 23:00
    if (Hour > config.quietStart  && Hour < config.quietEnd ) return true;
    else if (Hour == config.quietStart  && Hour < config.quietEnd && Min > 0 ) return true;
    else return false;    // if we go quiet on the hour, let the clock show the hour, not prev hour and 59 mins
  }
  else if (config.quietEnd < config.quietStart) { // e.g. 21:00 - 06:00
    if (Hour > config.quietStart  ||  Hour < config.quietEnd ) return true;
    else if (Hour == config.quietStart && Min > 0) return true;
    else return false;  // if we go quiet on the hour, let the clock show the hour, not prev hour and 59 mins
  }
  else return false; // End and Start are equal, so no quiet time anyway
}
