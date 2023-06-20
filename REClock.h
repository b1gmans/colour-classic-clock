// Clock stuff now

#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}


int Hour;
int wHour;
int Min;
int Sec;
int WeekDay;
int MonthDay;
int Month;
int Year;
int LastHour =99;
int LastMin =99;
int LastSec =99;
bool InDST=false;  // 
// end clock stuff

// NTP stuff.
unsigned long lastNTPtime; // last time we got NTP time

//int DSTadjust; // adjustment in Day light Saving Time
//int TZadjust; // adjustment in timezone
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

// end NTP stuff

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (0=Sunday, 6=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

boolean IsDST()
{
    /* This now handles:
    DSTadjust = 0 - no adjustment
              = 1 - UK/EU - Last Sunday March and Last Sunday Oct
              = 2 - USA/Can Second Sunday March and 1st Sunday Nov
    */
    if (config.DSTadjust == 0) return false; // no adjustment
    else if (config.DSTadjust == 1) {  // UK / EU
      //January, february, and november are out.
      if (Month < 3 || Month > 10) { return false; }
      //April to September are in
      if (Month > 3 && Month < 10) { return true; }
  
      // find last sun in mar and oct - quickest way I've found to do it
      // last sunday of march
      int lastMarSunday =  (31 - (5* (Year+2000) /4 + 4) % 7);
      //last sunday of october
      int lastOctSunday = (31 - (5 * (Year+2000) /4 + 1) % 7);
          
      //In march, we are BST if is the last sunday in the month
      if (Month == 3) { 
        
        if( MonthDay > lastMarSunday)
          return true;
        if( MonthDay < lastMarSunday)
          return false;
        
        if (Hour < 1)
          return false;
                
        return true; 
    
      }
      //In October we must be before the last sunday to be bst.
      //That means the previous sunday must be before the 1st.
      if (Month == 10) { 
  
        if( MonthDay < lastOctSunday)
          return true;
        if( MonthDay > lastOctSunday)
          return false;  
        
        if (Hour >= 1)
          return false;
          
        return true;  
      }
    }  
    else if (config.DSTadjust == 2) {  // USA / CAN 
      //January, february, and dec are out.
      if (Month < 3 || Month > 11) { return false; }
      //April to September are in
      if (Month > 3 && Month < 11) { return true; }
  
      int previousSunday = MonthDay - WeekDay; // Weekday 0 sunday 6 sat
      
      //In march, we are DST if our previous sunday was on or after the 8th.
      if (Month == 3) { 
        // if we're on the actual day then need to look at the time, 02:00 goes to 03:00
        if (previousSunday == MonthDay && previousSunday <=14 && previousSunday >=8) {
          if (Hour < 2) return false; 
          else return true; 
        }
        else return previousSunday >= 8;
      }
      
      //In november we must be before the first sunday to be dst.
        //That means the previous sunday must be before the 1st.
      if (Month == 11) { 
        // if we're on the actual day then need to look at the time 02:00 goes to 01:00
        if (previousSunday == MonthDay && previousSunday <=7) {
          if (Hour >=1) return false; 
          else return true; 
        }
        else return previousSunday <= 0;
      }
    }
    return false; // if not any of the above  
}
void displayTime(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,&year);
  Hour = hour;
  Month = month;
  MonthDay = dayOfMonth;
  Min = minute;
  Sec = second;
  WeekDay = dayOfWeek;
  MonthDay = dayOfMonth;
  Month = month;
  Year = year;
  InDST =  IsDST(); 
  if ( InDST ) { // v1.06 do BST adjustment here - the DS3231 should always have time in GMT/UTC or local,non DST-adjusted, time
    Hour = Hour + 1; 
    //Now handle all the rollovers that can happen
    if (Hour ==24 ) {
      Hour = 0;
      WeekDay++;
      if (WeekDay == 7) WeekDay = 0;
      MonthDay++;
      // now roll monmys - no need for jan, feb, dec and times dont changee in those months.
      if ((Month == 3 && MonthDay == 32 ) || 
          (Month == 4 && MonthDay == 31 ) ||
          (Month == 5 && MonthDay == 32 ) || 
          (Month == 6 && MonthDay == 31 ) || 
          (Month == 7 && MonthDay == 32 ) || 
          (Month == 8 && MonthDay == 32 ) || 
          (Month == 9 && MonthDay == 31 ) || 
          (Month == 10 && MonthDay == 32 ) || 
          (Month == 11 && MonthDay == 31 ) ){
        MonthDay = 1;
        Month++;
      }  
    } 
  }
  /*
  // send it to the serial monitor
  Serial.print(Hour, DEC);// not hour
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10){
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10){
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 0:
    Serial.println("Sunday");
    break;
  case 1:
    Serial.println("Monday");
    break;
  case 2:
    Serial.println("Tuesday");
    break;
  case 3:
    Serial.println("Wednesday");
    break;
  case 4:
    Serial.println("Thursday");
    break;
  case 5:
    Serial.println("Friday");
    break;
  case 6:
    Serial.println("Saturday");
    break;
  case 7:
    Serial.println("Sunday");
    break;  
  }
  */
}



void getNTPtime() {
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  //v1.06 if ( IsDST() ) timeClient.setTimeOffset(DSTadjust*3600);  // adjust TZ if needed
  //v1.06 else timeClient.setTimeOffset(0);
  // V2.0 timeClient.setTimeOffset(0); // v1.06 always save time as GMT time, adjust in display above.byte errorcount = 0;
  timeClient.setTimeOffset(config.TZadjust*3600); //V2.0 Now we need base time to be adjusted by the new TZadjust
  byte errorcount = 0;
  while(!timeClient.update() && errorcount < 2) {//was 15 but was freezing too long
    timeClient.forceUpdate();
    Serial.print(".");
    errorcount++;
    //delay(1000);
  }
  if (errorcount >=2 ) {  return;} // looks like we dropped off wifi,  
  
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  Year = dayStamp.substring(2,4).toInt();
  Month= dayStamp.substring(5,7).toInt();
  MonthDay= dayStamp.substring(8,10).toInt();
  Hour= timeStamp.substring(0,2).toInt(); 
  //Serial.print("New hour");Serial.println(Hour);
  Min= timeStamp.substring(3,5).toInt();
  Sec= timeStamp.substring(6,8).toInt();

  // v2.02 now get the day of the week from NTP time
  WeekDay = timeClient.getDay();
  Serial.print("day of week: ");
  Serial.println(WeekDay);
  // now set into RTC module
  setDS3231time(Sec, Min, Hour, WeekDay, MonthDay, Month, Year);
}  
