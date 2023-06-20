//####################################################################################################
// Setup
//####################################################################################################
void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);  
  pinMode(0, INPUT); //in built button - as an alternate
  pinMode(button2, INPUT);  // using D2, internal led, need to be low
  digitalWrite(button2, LOW);
  pinMode(SR_DATA, OUTPUT);
  pinMode(SR_SRCLK, OUTPUT);
  pinMode(SR_RCLK, OUTPUT);

  // Set all Chip Selects off - ie high
  // Set all outputs to zero
  digitalWrite(SR_RCLK, LOW);
  // Shift out the bits
  shiftOut(SR_DATA, SR_SRCLK, MSBFIRST, 255);
  digitalWrite(SR_RCLK, HIGH);

  pinMode(SD_CS, OUTPUT);
  // Set all chip selects high to avoid bus contention during initialisation of each peripheral
  //digitalWrite(22, HIGH); // Touch controller chip select (if used)
  //digitalWrite(15, HIGH); // TFT screen chip select
  digitalWrite( SD_CS, HIGH); // SD card chip select
  // setup the 165 inut SR's:
  //pinMode(load, OUTPUT);
  //pinMode(clockEnablePin, OUTPUT);
  //pinMode(clockIn, OUTPUT);
  //pinMode(dataIn, INPUT);
  Serial.print("Ver: ");
  Serial.println(versionno);
  // initialsie all TFT's
  outgoing = 0; // enable all digits
  write_sr();
  tft.init(); // whats the difference begin or init?
  
  // Added for new jped lib V0.08:
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
  //End new add V0.08
  
  //outgoing = 255; // reset all digits
  //write_sr();
  //initialse and sort out the displays:
  outgoing = 0; // enable all digits
  write_sr();
  tft.setRotation(0);  // portrait
  tft.fillScreen(0);
  
  //Retro Electro Logo
  //V0.08 remove old:drawArrayJpeg(retroElectro, sizeof(retroElectro), 0, 0);
  TJpgDec.drawJpg(0, 0, retroElectro, sizeof(retroElectro)); //V0.08 add new
  
  Wire.begin(27,16); //SDA 27 and SCL 16 for the new hw I designed.
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(00,44,13,5,19,04,19);
  //WiFi.mode(WIFI_MODE_STA);
  foundSDcard = true; // Assume we have one, unless:
  sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  //if (!SD.begin(SD_CS, sdSPI)){
  if (!SD.begin(SD_CS, sdSPI,14000000)){ //default is 4mhz speeded up to 16Mhz works, 18 works, 20 doesnt. On new board reduced to 14Mhz V4.12
    Serial.println("Card Mount Failed");
    foundSDcard = false;
    outgoing = 127-1; //
    write_sr();
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    while (true) {
      tft.setCursor(0, 0, 4);  
      tft.fillScreen(TFT_BLACK);
      tft.println("SD Card");
      tft.println("Mount Err.");
      tft.println("Stopping!");
      delay(60000);
      ESP.restart();
    }
  }
  uint8_t cardType = SD.cardType();

  if (foundSDcard == true && cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    foundSDcard = false;
    outgoing = 127-1; //
    write_sr();
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    while (true) {
      tft.setCursor(0, 0, 4);  
      tft.fillScreen(TFT_BLACK);
      tft.println("SD Card");
       tft.println("Not found");
      tft.println("Stopping!");
      delay(60000);
      ESP.restart();
    }
    //return;
  }
  if (foundSDcard == true) {
    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
      Serial.println("MMC");
    } else if (cardType == CARD_SD) {
      Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
      Serial.println("SDHC");
    } else {
      Serial.println("UNKNOWN");
    }
  
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
  }
  //now read the parameters from the config.txt file
  Serial.print("Trying to Open config.txt JSON file:");
  Serial.println(conffilenametwo);
  // First open or create if not exists
  if (foundSDcard == false) {
    // start SPIFFS
    if (!SPIFFS.begin(true)) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  }
  File file3 = (foundSDcard == true) ? SD.open(conffilenametwo): SPIFFS.open(conffilenametwo);
    if(!file3){
      Serial.println("File not found, attempt to create");
      File file4 = (foundSDcard == true) ? SD.open(conffilenametwo, FILE_WRITE): SPIFFS.open(conffilenametwo, FILE_WRITE);
      if(!file4){
        Serial.println("There was an error opening the file for writing"); 
      } else {
        file4.close(); 
        // set up defaults
        config.displayMode = 0;
        config.displayFont = 0;
        config.display12hrFormat = false;
        for (int i = 0; i<30;i++) {
          config.fontList[i] = 255; // Set all to off
        }
        //config.splashScreenTime = 4000;
        //Write the file away
        saveAjConfig(true);      
      }
    }
    else {
      Serial.println("File found, Content:");
      //here is where we unpack the json.

      DynamicJsonDocument doc(5120);

      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, file3);
      if (error) {
        Serial.println(F("Failed to read file, using default configuration"));
        config.wifi_ssid[0] = '\0';
      }
      else {
        // Copy values from the JsonDocument to the Config
        config.displayMode = doc["displayMode"] | 0;
        config.displayFont = doc["displayFont"] | 0;
        for (int i = 0; i<30;i++) {
            config.fontList[i] =doc["fontList"][i] |255; // Default all to off
        }
        strcpy(config.wifi_ssid,doc["wifi_ssid"]);
        strcpy(config.wifi_pass,doc["wifi_pass"]);
        config.display12hrFormat = doc["display12hrFormat"]|false; 
        config.quietStart = doc["quietStart"]|0;
        config.quietEnd = doc["quietEnd"]|0;
        config.DSTadjust = doc["DSTadjust"]|1; // adjustment in Day light Saving Time
        config.TZadjust = doc["TZadjust"]|0;
        config.tempSensorInstalled = doc["tempSensorInstalled"]|false;
        config.tempInFahrenheit = doc["tempInFahrenheit"]|false;
        config.col_bw = doc["col_bw"]|0;
        config.tempOffset = doc["tempOffset"]|0;
        config.hourChime = doc["hourChime"]|false;
        config.hourChimeTune = doc["hourChimeTune"]|0;

        for (int i = 0; i<3;i++) {
          config.alarmOnOff[i] = doc["alarmOnOff"][i]|false;
          config.alarmHour[i] = doc["alarmHour"][i]|0;
          config.alarmMin[i] = doc["alarmMin"][i]|0;
          config.alarmTune[i] = doc["alarmTune"][i]|0;
          config.alarmDays[i] = doc["alarmDays"][i]|0; 
        }  
  
        for (int i = 0; i<20;i++) {
            config.displayList[i] = doc["displayList"][i] |255; // Default all to off
            config.displayTime[i] = doc["displayTime"][i] |0; // Default 0 secs
            //config.displayTime[i] = 7; //alan hard code fix temp - need remove
        }
      }  
      if (config.displayList[0] == 255) { //set up something default
          config.displayList[0] = 0;
          config.displayList[1] = 1;
          config.displayList[2] = 2;
          config.displayTime[0] = 10;
          config.displayTime[1] = 10;
          config.displayTime[2] = 10;
      }
       if (config.displayTime[0] == 0) config.displayTime[0] = 10;
       
      // Now print em all
      for (int i = 0; i<30;i++) {
        Serial.print("fontList[") ;
        Serial.print(i) ;
        Serial.print("]:") ;
        Serial.println(config.fontList[i]); 
      }
      for (int i = 0; i<20;i++) {
        Serial.print("displayList[") ;
        Serial.print(i) ;
        Serial.print("]:") ;
        Serial.print(config.displayList[i]); 
        Serial.print("  displayTime:") ;
        Serial.println(config.displayTime[i]); 
      }
      Serial.print("wifi_ssid:") ;
      Serial.println(config.wifi_ssid) ;
      Serial.print("wifi_pass:") ;
      Serial.println(config.wifi_pass) ;
      Serial.print("displayMode:") ;
      Serial.println(config.displayMode) ;
      Serial.print("displayFont:") ;
      Serial.println(config.displayFont) ;
      Serial.print("Display 12 Hr Format:") ;
      Serial.println(config.display12hrFormat) ;
      Serial.print("Quiet Start:") ;
      Serial.println(config.quietStart) ;
      Serial.print("Quiet End:") ;
      Serial.println(config.quietEnd) ;
      Serial.print("DST Adjust:") ;
      Serial.println(config.DSTadjust) ;
      Serial.print("TZ Adjust:") ;
      Serial.println(config.TZadjust) ;
      Serial.print("tempSensorInstalled:") ;
      Serial.println(config.tempSensorInstalled) ;
      Serial.print("tempInFahrenheight:") ;
      Serial.println(config.tempInFahrenheit) ;
      Serial.print("tempOffset:") ;
      Serial.println(config.tempOffset);
      Serial.print("HourChime:") ;
      Serial.println(config.hourChime);
      Serial.print("ChimeTune:") ;
      Serial.println(config.hourChimeTune);

      for (int i = 0; i<3;i++) {
          Serial.print("Alarm:") ;
          Serial.println(i) ;
          Serial.print("alarmOnOff:") ;
          Serial.println(config.alarmOnOff[i]);
          Serial.print("alarmHour:") ;
          Serial.println(config.alarmHour[i]);
          Serial.print("alarmMin:") ;
          Serial.println(config.alarmMin[i]); 
          Serial.print("alarmTune:") ;
          Serial.println(config.alarmTune[i]);
          Serial.print("alarmDays Sun-Sat:") ;
          for (int j = 0; j<7;j++) {
            Serial.print(bitRead(config.alarmDays[i],j));
            Serial.print(" ");
          }
          Serial.println(" ");
          
        }  
      
      file3.close();
      Serial.println("Json Content Ends:");
    }      
  
  currFont = config.displayFont; // need to set this otherwise get issues.
  /*outgoing = 0; // enable all digits
  write_sr();
  tft.setRotation(0);  // portrait
  tft.fillScreen(0);
*/
  /*
  //Retro Electro Logo
  //V0.08 remove old:drawArrayJpeg(retroElectro, sizeof(retroElectro), 0, 0);
  TJpgDec.drawJpg(0, 0, retroElectro, sizeof(retroElectro)); //V0.08 add new
  */

  
  delay(2000); //Delay so can see retro electro screen
  
  displayTime(); // get RTC clock time initially,
  if (Min > 59 ) Min = 0; // fix e.g. if no RTC module
  if (Hour > 23 ) Hour = 0; // fix if no RTC
  byte tmpHour = Hour;
  // If 12 hour display Format, then modify hour. 
  if (config.display12hrFormat == true && Hour >= 12 ) tmpHour = Hour - 12;
  displayValues[5] = tmpHour / 10;
  displayValues[4] = tmpHour % 10;
  displayValues[2] = Min / 10;
  displayValues[1] = Min % 10;
  
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 95, 2); 
  //tft.println("       By"); 
  //  tft.print("   A. Jones");
  //drawArrayJpeg(retroElectro, sizeof(retroElectro), 0, 158);
  outgoing = 255; // disable
  write_sr();

  
  //Display Params
  outgoing = 127-1; //
  write_sr();
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0, 4);  
  tft.fillScreen(TFT_BLACK);
  tft.print("Ver:");
  tft.println(versionno);
  if (foundSDcard == false) {
     tft.println("NO SD");
     tft.println("CARD");
     //tft.println("");
  } else {
    tft.println("SD CARD");
     tft.println("OK");
  }
  tft.print("Wifi:");
  tft.println((config.wifi_ssid[0] != '\0') ? "Yes":"No");
  
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
    tft.println("3-Col Cyc");  
    break;
    case 4:
    tft.println("4-List Cyc");  
    break;
  } 
  tft.print("Font:");
  //tft.println("(Start)");
  //tft.println("");
  tft.println(config.displayFont);
  tft.print("Col/B&W");
  //tft.println(config.col_bw);
  switch (config.col_bw) {
    case 0:
    tft.println("0-All");  
    break;
    case 1:
    tft.println("1-Colour");  
    break;
    case 2:
    tft.println("2-B&W");  
    break;  
  }
  
  outgoing = 127-2; //
  write_sr();
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0, 4);  
  tft.fillScreen(TFT_BLACK);
  if (config.display12hrFormat) tft.println("12H"); else tft.println("24H");
  tft.println("Sleep:");
  if (config.quietStart < 10) tft.print("0");
  tft.print(config.quietStart);
  tft.print(":00-");
  if (config.quietEnd < 10) tft.print("0");
  tft.print(config.quietEnd);
  tft.println(":00");
  tft.println("DST Adj:");
  tft.print(config.DSTadjust); 
  switch (config.DSTadjust) {
    case 0: tft.println("-None"); break; 
    case 1: tft.println("-UK/EU"); break; 
    case 2: tft.println("-USA/Can"); break; 
  }
  tft.print("TZ Hr Adj:");
  tft.println(config.TZadjust); 
  tft.println("");
  tft.print(displayValues[5]);
  tft.print(displayValues[4]);
  tft.print(":");
  tft.print(displayValues[2]);
  tft.print(displayValues[1]);
  tft.print(" ");
  switch (WeekDay) {
    case 0:
    tft.println("Sun"); 
    break;
    case 1:
    tft.println("Mon"); 
    break;
    case 2:
    tft.println("Tue"); 
    break;
    case 3:
    tft.println("Wed"); 
    break;
    case 4:
    tft.println("Thu"); 
    break;
    case 5:
    tft.println("Fri"); 
    break;
    case 6:
    tft.println("Sat"); 
    break;
  }  
  tft.print(MonthDay);
  tft.print("-");
  tft.print(Month);
  tft.print("-");
  tft.println(Year);
  
  
  outgoing = 127-4; //
  write_sr();
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0, 4);  
  tft.fillScreen(TFT_BLACK);
  
  tft.print("DST22: ");
  if (config.tempSensorInstalled) tft.println("Yes"); else tft.println("No");
  tft.print("Temp: ");
  if (config.tempInFahrenheit) tft.println("F"); else tft.println("C");
  tft.print("Offset: ");
  tft.println(config.tempOffset);
  
  //Show Alarm settings on a screen each
  for (int a = 0;a<3;a++){
    outgoing = 127;
    bitClear(outgoing,3+a);
    write_sr();
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, 0, 4);  
    tft.fillScreen(TFT_BLACK);
    tft.print("Alarm ");
    tft.print(a+1);
    tft.print(": ");
    if (config.alarmOnOff[a]) tft.println("On"); else tft.println("Off");
    tft.print("Time: ");
    tft.print(config.alarmHour[a]);
    tft.print(":");
    tft.println(config.alarmMin[a]);
    tft.println("Days:");
    //tft.println("SMTWTFS");
    for (int i = 0;i<7;i++){
      switch (i) {
        case 0: if (bitRead(config.alarmDays[a],i)) tft.print("S"); else tft.print(" "); break;
        case 1: if (bitRead(config.alarmDays[a],i)) tft.print("M"); else tft.print(" "); break;
        case 2: if (bitRead(config.alarmDays[a],i)) tft.print("T"); else tft.print(" "); break;
        case 3: if (bitRead(config.alarmDays[a],i)) tft.print("W"); else tft.print(" "); break;
        case 4: if (bitRead(config.alarmDays[a],i)) tft.print("T"); else tft.print(" "); break;
        case 5: if (bitRead(config.alarmDays[a],i)) tft.print("F"); else tft.print(" "); break;
        case 6: if (bitRead(config.alarmDays[a],i)) tft.print("S"); else tft.print(" "); break;
      }
    }
    tft.println("");
    tft.print("Tune: ");
    tft.println(config.alarmTune[a]);
    if (config.alarmTune[a] < 16 ) tft.println(songName[config.alarmTune[a]]);
  }
  outgoing = 127-64; // rhs display
  write_sr();
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0, 4);  
  tft.fillScreen(TFT_BLACK);
  tft.print("Hr.Chime:");
  if (config.hourChime) tft.println("On"); else tft.println("Off");
  tft.print("Tune:");
  tft.println(config.hourChimeTune);
  if (config.hourChimeTune < 16 ) tft.println(songName[config.hourChimeTune]);
  
  delay(2000);
  while (digitalRead(button) == 0) { //keep displaying if adv button held down
    delay(5000);
  }
  while (digitalRead(0) == 0) { //keep displaying if board int button held down
    delay(5000);
  }
  //outgoing = 0; //enable all
  //write_sr();
  //tft.fillScreen(0);
  if (config.wifi_ssid[0] != '\0') {
     //Display Params
    outgoing = 127-64; // rhs display
    write_sr();
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, 0, 4);  
    tft.fillScreen(TFT_BLACK);
    //tft.print("Ver:");
    //tft.println(versionno);
    //tft.println("");
    tft.println("Wifi");
    tft.println("Connecting");
    tft.println("Please");
    tft.println("Wait");
    tft.println(config.wifi_ssid);
    // Wi-Fi connection
    wifiSetup();
    if (WiFi.status() == WL_CONNECTED) tft.println("Connected");
    else tft.println("NOT Connected");
  }   
  while (digitalRead(button) == 0) { //keep displaying if adv button held down
    delay(5000);
  }
  while (digitalRead(0) == 0) { //keep displaying if board int button held down
    delay(5000);
  }
  delay(500);
  /*
  //Now play a tune - could be a while.
  cutShort = false; //play short versions if true
  song[random(16)]();
  cutShort = false; 
  */
  
/*
  // Initialise the Sending for ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    //return;
  }
  esp_now_register_recv_cb(data_receive);
  esp_now_register_send_cb(data_sent);
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;     
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    //return;
  }
  */
  /*V4.15  move further down
  // If we are the sender, send a message to any of our receivers
  if (config.senderOrReceiver == 0) {
    Serial.println("Sender: Broadcasting to Receivers:");
    message.gameId = config.gameId;
    message.senderOrReceiver = config.senderOrReceiver;
    message.messageType = 0;
    message.newCurrFont = currFont;  
    /*sendermac1;  //dont thing we need to use the mac address alan?
    sendermac2;
    sendermac3;
    sendermac4;
    sendermac5;*//*
    esp_err_t outcome = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
    if (outcome == ESP_OK) {
      Serial.println("Mesage sent successfully!");
    }
    else {
      Serial.println("Error sending the message");
    }
  }
  */

  //Check SD card to see if we have a firmware update to do
  Serial.println(F("Looking for Firmware update file:firmware.bin"));
  File firmware =  SD.open("/firmware.bin");
  if (firmware) {
    Serial.println(F("found!"));
    Serial.println(F("Try to update!"));

    Update.onProgress(progressCallBack);

    Update.begin(firmware.size(), U_FLASH);
    Update.writeStream(firmware);
    if (Update.end()){
      Serial.println(F("Update finished!"));
    }else{
      Serial.println(F("Update error!"));
      Serial.println(Update.getError());
    }

    firmware.close();

    if (SD.rename("/firmware.bin", "/firmware.bak")){
      Serial.println(F("Firmware rename succesfully!"));
    }else{
      Serial.println(F("Firmware rename error!"));
    }
    delay(2000);

    ESP.restart();
  } else{
    Serial.println(F("No Firmware update"));
  }

  // Now do the splash screen
  if (foundSDcard == true ) { // only if we have sd card
    // Display the last image on all screens initially - this is the tail of the fathom, for example 
    /*sprintf(filename,"/splash%d.jpg",6); // gives us splash6.jpg
      outgoing = 128;
      write_sr();
      drawSdJpeg(filename, 0,0);     // This draws a jpeg pulled off the SD Card
      outgoing = 127; // disable all digits
      write_sr();
      */

    /* no need for splash screens  
    // Display the 0-6 splash screen images - these are 7 images splash0.jpg to splash6.jpg - idea is to have one splashed
    //on each of the 7 displays at startup
    
    for (int k = 0; k < 7; k++) { // now loop round splash 0-6
      //digitalWrite(CS_1, LOW); // Chip Select to low to Enable
      
      sprintf(filename,"/splash%d.jpg",k); // gives us splash0.jpg to splash6.jpg
      outgoing = 127;
      bitClear(outgoing,k);
      write_sr();
      //drawSdJpeg(filename, 0,0);     // This draws a jpeg pulled off the SD Cardh
      // New decoder:
      //const char *name = filename;
      jpeg.open(filename, myOpen, myClose, myRead, mySeek, JPEGDraw);
      jpeg.setPixelType(RGB565_BIG_ENDIAN);
      //Can be one of RGB565_LITTLE_ENDIAN (the default), RGB565_BIG_ENDIAN, 
      //EIGHT_BIT_GRAYSCALE, FOUR_BIT_DITHERED, TWO_BIT_DITHERED, or ONE_BIT_DITHERED. This MUST be called after open() 
      jpeg.decode(0, 0, 0);
      jpeg.close();
      // end new decoder
      write_sr();
    }
    */
    /* V4.14 remove flash
    //alan - try a flash here using bit 7 on outgoing - works - i like it - keep it in!  Btw Bit 7 is switching on and off the LCD backlights
    for (int k = 0; k < 3; k++) {
        outgoing = 128;
        write_sr();
        delay(120);
        outgoing = 0;
        write_sr();
        delay(120);
      }
    //delay(config.splashScreenTime);
    */
    
    //Alan here do tests for the comma font files, if we have these, then set the comma flag. 
    //use array fontCommas[21]
    for (int i = 0 ; i <21 ; i++) { //loop round 21 fonts 0-20
      fontCommas[i] = true; //assume true until we dont find a comma font file
      for (int j = 0 ; j < 1 ; j++) {
        sprintf(filename,"/%dc.jpg",i*10+j); 
        if (!SD.exists(filename)) {
          fontCommas[i] = false; 
          j=10; //exit this loop  
        }
      }
    }
    
  } // end found SD card
  
  displayTime(); // get RTC clock time initially, need it set for first run through main loop.
  
  Serial.print("Main initialisation done. Ver: ");
  Serial.println(versionno);

/* no data in demo
  //Now wait for some score data - this is better than a fixed delay as all screens will show splash until get some data
  Serial.println("waiting for score data.");
  while (getScoreData() !=0 ) {
    checkSetup();
    delay(200);
  }
  */
  delay(1000);
  
  //now blank the screens
  blankAll();
  //blankTriggered = false;  
  Serial.println("got initial score data.");
  /*
  //V4.15  Moved this from further up - its here so all receivers should have booted by now
  // If we are the sender, send a message to any of our receivers
  if (config.senderOrReceiver == 0) {
    Serial.println("Sender: Broadcasting to Receivers:");
    message.gameId = config.gameId;
    message.senderOrReceiver = config.senderOrReceiver;
    message.messageType = 0;
    message.newCurrFont = currFont;  
    /*sendermac1;  //dont thing we need to use the mac address alan?
    sendermac2;
    sendermac3;
    sendermac4;
    sendermac5;*//*
    esp_err_t outcome = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
    if (outcome == ESP_OK) {
      Serial.println("Mesage sent successfully!");
    }
    else {
      Serial.println("Error sending the message");
    }
    
  }*/
  // DHT Temp sensor stuff
  if (config.tempSensorInstalled) {
    Serial.println(F("Start DHT"));
    dht.begin();
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
  
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      //Alan - set flag to missing
    }
  }
  saveAjConfig(false); // Save config - Alan added here to save me having manually go into setup which is disabled.
  
  masterRoot = SD.open("/splash screens");
  File topentry;
  //Choose a top level folder at random
  byte rnd = random(15);
  for (byte l = 0; l< rnd; l++) {
    topentry = masterRoot.openNextFile();
    if (! topentry) {
      masterRoot.rewindDirectory();  
    }
  }
  while (true) {
    topentry = masterRoot.openNextFile();
    if (topentry.isDirectory()) {
      break;
    }
  }
  root = SD.open(topentry.name());
  Serial.print("Start Dir:" );
  Serial.println(topentry.name());
  
  displayListTimer =0;
  displayListCounter = 19; //set to last one, so scrolls round to first 
  Serial.print("Setup complete, ver: ");
  Serial.println(versionno);
}
