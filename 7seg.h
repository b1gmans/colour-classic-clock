void draw7Number(long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
  //- this is a solid coloured 7 seg display
  unsigned int num = abs(n), i, s, t, w, col, h, x, y, si = 0, j = 1, d = 0;
  unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
  unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
  unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
  unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
  unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
  unsigned int y1 = yLoc + x1;                  // starting y location of top side segments
  unsigned int y3 = yLoc + S3 + S4 + 1;         // starting y location of bottom side segments
  // actual x,y locations and direction of all 7 segments 
  unsigned int seg[7][2] = {{x1, yLoc}, {x2, y1}, {x2, y3 + x1}, {x1, (2 * y3) - yLoc}, {0, y3 + x1}, {0, y1}, {x1, y3}}; 
  // segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (*,g,f,e,d,c,b,a)  
  static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 
  unsigned char c, cnt;

  c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
  if (c>10) c= 10;
  if (c<1) c = 1;

  cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
  if (cnt > 10) cnt = 10;
  if (cnt < 1) cnt = 1; 

  d = S2+(3*S3)+2;                              // width of one number

  xLoc += (cnt-1) * d;                          // set starting x at last digit location

  while( cnt > 0) {                             // for cnt number of places

    --cnt;

    if (num > 9) i = num%10;                    // get the last digit 
    else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
    else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
    else i = num;

    num = num/10;                               // trim this digit from the number  

    for (j = 0; j < 7; ++j) {                   // draw all seven segments

      if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
      else col = bC;                            // else use background color

      if (j==0 || j==3 || j==6) {

        w = S2;                                 // Starting width of segment (side)
        x = xLoc + seg[j][0] + cS;              // starting x location
        y = seg[j][1];                          // starting y location
        t = y + S3;                             // maximum thickness of segment
        h = y + S3/2;                           // half way point thickness of segment


        while (y < h) {                         // until y location = half way
          tft.drawFastHLine(x, y, w, col);      // Draw a horizontal segment top 
          ++y;                                  // move the y position by 1
          --x;                                  // move the x position by -1           
          w += 2;                               // make the line wider by 2
          
        }

        while (y < t) {                         //finish drawing horizontal bottom  
          tft.drawFastHLine(x, y, w, col);      // Draw Horizonal segment bottom
          ++y;                                  // keep moving the y or y draw position until t 
          ++x;                                  // move the length or height starting position back the other way.
          w -= 2;                               // move the length or height back the other way
          
        }

      } else {

        w = S4;                                 // Starting height of segment (side)
        x = xLoc + seg[j][0];                   // starting x location
        y = seg[j][1] + cS;                     // starting y location 
        t = x + S3;                             // maximum thickness of segment
        h = x + S3/2;                           // half way point thickness of segment

        while (x < h) {                         // until x location = half way
          tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
          ++x;                                  // move the x position by 1
          --y;                                  // move the y position by -1          
          w += 2;                               // make the line wider by 2
          
        }
        while (x < t) {
          tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
          ++x;                                  // move the x position by 1       
          ++y;                                  // move the length or height starting position back the other way.
          w -= 2;                               // move the length or height back the other way
          
        }
      }       
    }

    xLoc -=d;                                   // move to next digit position
  }
}
void draw7NumberHatch(long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
  // Alan variant, hatch the inside out, by making every other line background
  //- this is a striped colour and black 7 seg display
  unsigned int num = abs(n), i, s, t, w, col, h, x, y, si = 0, j = 1, d = 0;
  unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
  unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
  unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
  unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
  unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
  unsigned int y1 = yLoc + x1;                  // starting y location of top side segments
  unsigned int y3 = yLoc + S3 + S4 + 1;         // starting y location of bottom side segments
  unsigned int count; // used to control hatching
  int chop = 7; //used to control when to chop the hatching
  // actual x,y locations and direction of all 7 segments 
  unsigned int seg[7][2] = {{x1, yLoc}, {x2, y1}, {x2, y3 + x1}, {x1, (2 * y3) - yLoc}, {0, y3 + x1}, {0, y1}, {x1, y3}}; 
  // segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (*,g,f,e,d,c,b,a)  
  static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 
  unsigned char c, cnt;

  c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
  if (c>10) c= 10;
  if (c<1) c = 1;

  cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
  if (cnt > 10) cnt = 10;
  if (cnt < 1) cnt = 1; 

  d = S2+(3*S3)+2;                              // width of one number

  xLoc += (cnt-1) * d;                          // set starting x at last digit location

  while( cnt > 0) {                             // for cnt number of places

    --cnt;

    if (num > 9) i = num%10;                    // get the last digit 
    else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
    else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
    else i = num;

    num = num/10;                               // trim this digit from the number  

    for (j = 0; j < 7; ++j) {                   // draw all seven segments

      if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
      else col = bC;                            // else use background color

      if (j==0 || j==3 || j==6) {

        w = S2;                                 // Starting width of segment (side)
        x = xLoc + seg[j][0] + cS;              // starting x location
        y = seg[j][1];                          // starting y location
        t = y + S3;                             // maximum thickness of segment
        h = y + S3/2;                           // half way point thickness of segment

        count = 0;
        while (y < h) {                         // until y location = half way
          tft.drawFastHLine(x, y, w, col);      // Draw a horizontal segment top 
          ++y;                                  // move the y position by 1
          --x;                                  // move the x position by -1           
          w += 2;                               // make the line wider by 2
          count++; if (col ==fC && count % 15 > chop) col = bC;  // ALAN toggle 
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
        count = 0;
        while (y < t) {                         //finish drawing horizontal bottom  
          tft.drawFastHLine(x, y, w, col);      // Draw Horizonal segment bottom
          ++y;                                  // keep moving the y or y draw position until t 
          ++x;                                  // move the length or height starting position back the other way.
          w -= 2;                               // move the length or height back the other way
           count++; if (col ==fC && count % 15 > chop) col = bC;  // ALAN toggle 
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
      } else {

        w = S4;                                 // Starting height of segment (side)
        x = xLoc + seg[j][0];                   // starting x location
        y = seg[j][1] + cS;                     // starting y location 
        t = x + S3;                             // maximum thickness of segment
        h = x + S3/2;                           // half way point thickness of segment
        count = 0;
        while (x < h) {                         // until x location = half way
          tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
          ++x;                                  // move the x position by 1
          --y;                                  // move the y position by -1          
          w += 2;                               // make the line wider by 2
           count++; if (col ==fC && count % 15 > chop) col = bC; // ALAN toggle 
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
        count = 0;
        while (x < t) {
          tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
          ++x;                                  // move the x position by 1       
          ++y;                                  // move the length or height starting position back the other way.
          w -= 2;                               // move the length or height back the other way
           count++; if (col ==fC && count % 15 > chop) col = bC; // ALAN toggle  
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
      }       
    }

    xLoc -=d;                                   // move to next digit position
  }
}
void draw7NumberHatch2(long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
  // Alan variant, hatch the inside out, by making every other line background
  // this is a another striped colour and black 7 seg display - too similar to above
  unsigned int num = abs(n), i, s, t, w, col, h, x, y, si = 0, j = 1, d = 0;
  unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
  unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
  unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
  unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
  unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
  unsigned int y1 = yLoc + x1;                  // starting y location of top side segments
  unsigned int y3 = yLoc + S3 + S4 + 1;         // starting y location of bottom side segments
  unsigned int count; // used to control hatching
  int chop = 8; //used to control when to chop the hatching
  // actual x,y locations and direction of all 7 segments 
  unsigned int seg[7][2] = {{x1, yLoc}, {x2, y1}, {x2, y3 + x1}, {x1, (2 * y3) - yLoc}, {0, y3 + x1}, {0, y1}, {x1, y3}}; 
  // segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (*,g,f,e,d,c,b,a)  
  static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 
  unsigned char c, cnt;

  c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
  if (c>10) c= 10;
  if (c<1) c = 1;

  cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
  if (cnt > 10) cnt = 10;
  if (cnt < 1) cnt = 1; 

  d = S2+(3*S3)+2;                              // width of one number

  xLoc += (cnt-1) * d;                          // set starting x at last digit location

  while( cnt > 0) {                             // for cnt number of places

    --cnt;

    if (num > 9) i = num%10;                    // get the last digit 
    else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
    else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
    else i = num;

    num = num/10;                               // trim this digit from the number  

    for (j = 0; j < 7; ++j) {                   // draw all seven segments

      if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
      else col = bC;                            // else use background color

      if (j==0 || j==3 || j==6) {

        w = S2;                                 // Starting width of segment (side)
        x = xLoc + seg[j][0] + cS;              // starting x location
        y = seg[j][1];                          // starting y location
        t = y + S3;                             // maximum thickness of segment
        h = y + S3/2;                           // half way point thickness of segment

        count = 0;
        while (y < h) {                         // until y location = half way
          tft.drawFastHLine(x, y, w, col);      // Draw a horizontal segment top 
          ++y;                                  // move the y position by 1
          --x;                                  // move the x position by -1           
          w += 2;                               // make the line wider by 2
          count++; if (col ==fC && count % 15 > chop) col = bC;  // ALAN toggle 
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
        count = 0;
        while (y < t) {                         //finish drawing horizontal bottom  
          if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
          else col = bC;
          count++; if (col ==fC && count % 15 < 15 - chop) col = bC;  // ALAN toggle 
          tft.drawFastHLine(x, y, w, col);      // Draw Horizonal segment bottom
          ++y;                                  // keep moving the y or y draw position until t 
          ++x;                                  // move the length or height starting position back the other way.
          w -= 2;                               // move the length or height back the other way
           
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
      } else {

        w = S4;                                 // Starting height of segment (side)
        x = xLoc + seg[j][0];                   // starting x location
        y = seg[j][1] + cS;                     // starting y location 
        t = x + S3;                             // maximum thickness of segment
        h = x + S3/2;                           // half way point thickness of segment
        count = 0;
        while (x < h) {                         // until x location = half way
          tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
          ++x;                                  // move the x position by 1
          --y;                                  // move the y position by -1          
          w += 2;                               // make the line wider by 2
           count++; if (col ==fC && count % 15 > chop) col = bC; // ALAN toggle 
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
        count = 0;
        while (x < t) {
          if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
          else col = bC;
          count++; if (col ==fC && count % 15 < 15 - chop) col = bC; // ALAN toggle
          tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
          ++x;                                  // move the x position by 1       
          ++y;                                  // move the length or height starting position back the other way.
          w -= 2;                               // move the length or height back the other way
             
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
      }       
    }

    xLoc -=d;                                   // move to next digit position
  }
}
void draw7NumberHatch3(long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
  // Alan variant, hatch the inside out, by making every other line background
  // this fills with white
  // - this is a solid 7 seg display but with a thin white line down the middle of each number. 
  unsigned int num = abs(n), i, s, t, w, col, h, x, y, si = 0, j = 1, d = 0;
  unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
  unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
  unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
  unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
  unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
  unsigned int y1 = yLoc + x1;                  // starting y location of top side segments
  unsigned int y3 = yLoc + S3 + S4 + 1;         // starting y location of bottom side segments
  unsigned int count; // used to control hatching
  int chop = 12; //used to control when to chop the hatching
  // actual x,y locations and direction of all 7 segments 
  unsigned int seg[7][2] = {{x1, yLoc}, {x2, y1}, {x2, y3 + x1}, {x1, (2 * y3) - yLoc}, {0, y3 + x1}, {0, y1}, {x1, y3}}; 
  // segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (*,g,f,e,d,c,b,a)  
  static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 
  unsigned char c, cnt;

  c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
  if (c>10) c= 10;
  if (c<1) c = 1;

  cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
  if (cnt > 10) cnt = 10;
  if (cnt < 1) cnt = 1; 

  d = S2+(3*S3)+2;                              // width of one number

  xLoc += (cnt-1) * d;                          // set starting x at last digit location

  while( cnt > 0) {                             // for cnt number of places

    --cnt;

    if (num > 9) i = num%10;                    // get the last digit 
    else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
    else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
    else i = num;

    num = num/10;                               // trim this digit from the number  

    for (j = 0; j < 7; ++j) {                   // draw all seven segments

      if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
      else col = bC;                            // else use background color

      if (j==0 || j==3 || j==6) {

        w = S2;                                 // Starting width of segment (side)
        x = xLoc + seg[j][0] + cS;              // starting x location
        y = seg[j][1];                          // starting y location
        t = y + S3;                             // maximum thickness of segment
        h = y + S3/2;                           // half way point thickness of segment

        count = 0;
        while (y < h) {                         // until y location = half way
          tft.drawFastHLine(x, y, w, col);      // Draw a horizontal segment top 
          ++y;                                  // move the y position by 1
          --x;                                  // move the x position by -1           
          w += 2;                               // make the line wider by 2
          count++; if (col ==fC && count % 15 > chop) col = TFT_WHITE;  // ALAN toggle 
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
        count = 0;
        while (y < t) {                         //finish drawing horizontal bottom  
          if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
          else col = bC;
          count++; if (col ==fC && count % 15 < 15 - chop) col = TFT_WHITE;  // ALAN toggle 
          tft.drawFastHLine(x, y, w, col);      // Draw Horizonal segment bottom
          ++y;                                  // keep moving the y or y draw position until t 
          ++x;                                  // move the length or height starting position back the other way.
          w -= 2;                               // move the length or height back the other way
           
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
      } else {

        w = S4;                                 // Starting height of segment (side)
        x = xLoc + seg[j][0];                   // starting x location
        y = seg[j][1] + cS;                     // starting y location 
        t = x + S3;                             // maximum thickness of segment
        h = x + S3/2;                           // half way point thickness of segment
        count = 0;
        while (x < h) {                         // until x location = half way
          tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
          ++x;                                  // move the x position by 1
          --y;                                  // move the y position by -1          
          w += 2;                               // make the line wider by 2
           count++; if (col ==fC && count % 15 > chop) col = TFT_WHITE; // ALAN toggle 
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
        count = 0;
        while (x < t) {
          if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
          else col = bC;
          count++; if (col ==fC && count % 15 < 15 - chop) col = TFT_WHITE; // ALAN toggle
          tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
          ++x;                                  // move the x position by 1       
          ++y;                                  // move the length or height starting position back the other way.
          w -= 2;                               // move the length or height back the other way
             
        }
        if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
        else col = bC;
      }       
    }

    xLoc -=d;                                   // move to next digit position
  }
}
void draw7circles(byte n, unsigned int fC, unsigned int bC) {
  // Version to draw circles
  // define 0-9 y & x coordinates for each dot, max 19 pairs - each pair is y,x coords from top left 
  byte coords[10][38] = {{0,1,0,2,0,3,1,0,1,4,2,0,2,1,2,4,3,0,3,2,3,4,4,0,4,3,4,4,5,0,5,4,6,1,6,2,6,3}, //0
                         {0,2,1,1,1,2,2,2,3,2,4,2,5,2,6,1,6,2,6,3}, //1
                         {0,1,0,2,0,3,1,0,1,4,2,4,3,3,4,2,5,1,6,0,6,1,6,2,6,3,6,4}, //2
                         {0,0,0,1,0,2,0,3,0,4,1,4,2,3,3,2,3,3,4,4,5,0,5,4,6,1,6,2,6,3}, //3
                         {0,3,1,2,1,3,2,1,2,3,3,0,3,3,4,0,4,1,4,2,4,3,4,4,5,3,6,3}, //4
                         {0,0,0,1,0,2,0,3,0,4,1,0,2,0,2,1,2,2,2,3,3,4,4,4,5,0,5,4,6,1,6,2,6,3}, //5
                         {0,1,0,2,0,3,1,0,1,4,2,0,3,0,3,1,3,2,3,3,4,0,4,4,5,0,5,4,6,1,6,2,6,3}, //6
                         {0,0,0,1,0,2,0,3,0,4,1,4,2,3,3,2,4,1,5,1,6,1}, //7
                         {0,1,0,2,0,3,1,0,1,4,2,0,2,4,3,1,3,2,3,3,4,0,4,4,5,0,5,4,6,1,6,2,6,3}, //8
                         {0,1,0,2,0,3,1,0,1,4,2,0,2,4,3,1,3,2,3,3,3,4,4,4,5,0,5,4,6,1,6,2,6,3}, //9
                        };
  byte xoff = 0;
  byte yoff = 25;
  byte csiz = 27;
  byte hcsiz = 13; // half of csiz
  byte radius = 11;
  //Alan - add in bg colour here, leave for time being as always black
  tft.fillScreen(TFT_BLACK); //need to blank out first
  for (byte ele = 0 ; ele < 19 ; ele++ ) {
    if (ele > 0 && coords[n][ele*2] == 0 && coords[n][ele*2+1] == 0 ) break; //as soon as we get a blank after first one ("3" uses 0,0)
    tft.fillCircle(xoff+coords[n][ele*2+1]*csiz+hcsiz,yoff+coords[n][ele*2]*csiz+hcsiz,radius,fC);                      
  }
}
