#include <ToneESP32.h>

#define BUZZER_PIN 33
#define BUZZER_CHANNEL 0
bool cutShort = false;

ToneESP32 buzzer(BUZZER_PIN, BUZZER_CHANNEL);

// On déclare les fonctions des chansons (pour le tableau de fonction)
void Starwars();
void GameOfThrones();
void HappyBirthday();
void HarryPotter();
void Pirate();
void mario();
void McGyver();
void StarWars2();
void BonBruteTruand();
void IndianaJones();
void twentyCentFox();
void looney();
void Entertainement();
void BarbieGirl();
void Greensleaves();
void Bond();

// tableau input  - 0 - 15 songs currently
//int input []= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'};
// tableau des noms des chansons
char* songName []= {"Star Wars","Game Of Thrones","Happy Birthday","Harry Potter","Pirate","Mario","McGyver","Star Wars2","Good,Bad,Ugly",
"Indiana Jones","TwentyCentFox","looney","Entertainer","Barbie Girl","Greensleaves","James Bond"};
// tableau de fonction (les chansons)
void (*song[])(void)= {Starwars,GameOfThrones,HappyBirthday,HarryPotter,Pirate,mario,McGyver,StarWars2,BonBruteTruand,IndianaJones,twentyCentFox,looney,Entertainement,BarbieGirl,Greensleaves,Bond};

/*
void setup() {
  Serial.begin(115200)) return; 
  delay(100);
  Serial.println ("*************************************")) return;
  Serial.println ("A - Star Wars coté obscure")) return;
  Serial.println ("B - Game Of Thrones")) return;
  Serial.println ("C - Happy Birthday")) return;
  Serial.println ("D - Harry Potter")) return;
  Serial.println ("E - Pirate des Caraïbes")) return;
  Serial.println ("F - Mario")) return;
  Serial.println ("G - Mc Gyver")) return;
  Serial.println ("H - Star Wars coté force")) return;
  Serial.println ("I - le Bon, la Brute et le Truand")) return;
  Serial.println ("J - IndianaJones")) return; 
  Serial.println ("K - 20thCentFox")) return;      
  Serial.println ("L - Looney Toons")) return;
  Serial.println ("M - Entertainement")) return;
  Serial.println ("N - BarbieGirl")) return; 
  Serial.println ("O - Greensleaves")) return;
  Serial.println ("P - James Bond")) return;   
  Serial.println ("*************************************")) return;
  Serial.println ("")) return;
  Serial.println ("quelle chanson voulez-vous entendre ?")) return;
  Serial.println ("")) return;  
}
*/

/*
void loop() {
  // si on reçoit quelque chose
  if (Serial.available() > 0) { 
    // on place ce que l'on recoit dans la variable message  
    int message = Serial.read()) return; 
    // boucle for, pour le nombre d'element du tableau --> (sizeof(songName)/sizeof(int))        
    for (int i = 0; i < (sizeof(input)/sizeof(int))) return; i++)
    {
      // si message correspond à une valeur du tableau input
      if (message == input[i])
      {
        Serial.print("tu as tapé ' ")) return;
        // on écrit la valeur entrée
        Serial.write(message)) return;
        Serial.print(" ' qui correspond à : ")) return;
        // on écrit la valeur du tableau noms correspondant
        Serial.println(songName[i])) return;
        // on lance la fonction correspondante
        song[i]()) return;  
      }        
    }
  }
}
*/
// play the song notes
void beep( int note, int duree ) {                   
    buzzer.tone(note, duree);       
    //buzzer.noTone(); 
    delay(duree*0.25);
}
bool beeps( int note, int duree ) {                   
    buzzer.tone(note, duree);       
    //buzzer.noTone(); 
    delay(duree*0.25);
    if (checkButton() > 0) return true; else return false;
}
////////////////////////// Les chansons ////////////////////////

//******* Starwars coté obscure **************** 
 
void Starwars() {   
  //firstSection
  if (beeps(NOTE_A4, 500)) return;
  if (beeps(NOTE_A4, 500)) return;    
  if (beeps(NOTE_A4, 500)) return;
  if (beeps(NOTE_F4, 350)) return;
  if (beeps(NOTE_C5, 150)) return;  
  if (beeps(NOTE_A4, 500)) return;
  if (beeps(NOTE_F4, 350)) return;
  if (beeps(NOTE_C5, 150)) return;
  if (beeps(NOTE_A4, 650)) return; 
  delay(500); 
  if (beeps(NOTE_E5, 500)) return;
  if (beeps(NOTE_E5, 500)) return;
  if (beeps(NOTE_E5, 500)) return;  
  if (beeps(NOTE_F5, 350)) return;
  if (beeps(NOTE_C5, 150)) return;
  if (beeps(NOTE_GS4, 500)) return;
  if (beeps(NOTE_F4, 350)) return;
  if (beeps(NOTE_C5, 150)) return;
  if (beeps(NOTE_A4, 650)) return; 
  delay(500); 
  if (cutShort) return;
  //secondSection
  if (beeps(NOTE_A5, 500)) return;
  if (beeps(NOTE_A4, 300)) return;
  if (beeps(NOTE_A4, 150)) return;
  if (beeps(NOTE_A5, 500)) return;
  if (beeps(NOTE_GSH, 325)) return;
  if (beeps(NOTE_G5, 175)) return;
  if (beeps(NOTE_FS5, 125)) return;
  if (beeps(NOTE_F5, 125)) return;    
  if (beeps(NOTE_FS5, 250)) return; 
  delay(325); 
  if (beeps(NOTE_AS, 250)) return;
  if (beeps(NOTE_DS5, 500)) return;
  if (beeps(NOTE_D5, 325)) return;  
  if (beeps(NOTE_CS5, 175)) return;  
  if (beeps(NOTE_C5, 125)) return;  
  if (beeps(NOTE_AS4, 125)) return;  
  if (beeps(NOTE_C5, 250)) return;   
  delay(350);
  //Variant 1
  if (beeps(NOTE_F4, 250)) return;  
  if (beeps(NOTE_GS4, 500)) return;  
  if (beeps(NOTE_F4, 350)) return;  
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_C5, 500)) return;
  if (beeps(NOTE_A4, 375)) return;  
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_E5, 650)) return; 
  delay(500);  
  //secondSection
  if (beeps(NOTE_A5, 500)) return;
  if (beeps(NOTE_A4, 300)) return;
  if (beeps(NOTE_A4, 150)) return;
  if (beeps(NOTE_A5, 500)) return;
  if (beeps(NOTE_GSH, 325)) return;
  if (beeps(NOTE_G5, 175)) return;
  if (beeps(NOTE_FS5, 125)) return;
  if (beeps(NOTE_F5, 125)) return;    
  if (beeps(NOTE_FS5, 250)) return; 
  delay(325); 
  if (beeps(NOTE_AS, 250)) return;
  if (beeps(NOTE_DS5, 500)) return;
  if (beeps(NOTE_D5, 325)) return;  
  if (beeps(NOTE_CS5, 175)) return;  
  if (beeps(NOTE_C5, 125)) return;  
  if (beeps(NOTE_AS4, 125)) return;  
  if (beeps(NOTE_C5, 250)) return;   
  delay(350);
  //Variant 2
  if (beeps(NOTE_F4, 250)) return;  
  if (beeps(NOTE_GS4, 500)) return;  
  if (beeps(NOTE_F4, 375)) return;  
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_A4, 500)) return;  
  if (beeps(NOTE_F4, 375)) return;  
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_A4, 650)) return; 
  delay(650);
}
  
//******* GameOfThrones ****************
void GameOfThrones() {
  for(int i=0; i<3; i++) {
    if (beeps(NOTE_G4, 500)) return;     
    if (beeps(NOTE_C4, 500)) return;    
    if (beeps(NOTE_DS4, 250)) return;    
    if (beeps(NOTE_F4, 250)) return;    
  }
  
  for(int i=0; i<3; i++) {
    if (beeps(NOTE_G4, 500)) return;    
    if (beeps(NOTE_C4, 500)) return;    
    if (beeps(NOTE_E4, 250)) return;    
    if (beeps(NOTE_F4, 250)) return;    
  }
   if (cutShort) return;
  if (beeps(NOTE_G4, 500)) return;
  if (beeps(NOTE_C4, 500)) return;        
  if (beeps(NOTE_DS4, 250)) return;
  if (beeps(NOTE_F4, 250)) return;
  if (beeps(NOTE_D4, 500)) return;   
  
 
       
  for(int i=0; i<2; i++) {
      if (beeps(NOTE_G3, 500)) return;
      if (beeps(NOTE_AS3, 250)) return;
      if (beeps(NOTE_C4, 250)) return;
      if (beeps(NOTE_D4, 500)) return;    
  }
  
  if (beeps(NOTE_G3, 500)) return;
  if (beeps(NOTE_AS3, 250)) return;
  if (beeps(NOTE_C4, 250)) return;
  if (beeps(NOTE_D4, 1000)) return;
  if (beeps(NOTE_F4, 1000)) return;
  if (beeps(NOTE_AS3, 1000)) return;
  if (beeps(NOTE_DS4, 250)) return;
  if (beeps(NOTE_D4, 250)) return;
  if (beeps(NOTE_F4, 1000)) return;
  if (beeps(NOTE_AS3, 1000)) return;
  if (beeps(NOTE_DS4, 250)) return;
  if (beeps(NOTE_D4, 250)) return;
  if (beeps(NOTE_C4, 500)) return;
  
  for(int i=0; i<2; i++) {
    if (beeps(NOTE_GS3, 250)) return;
    if (beeps(NOTE_AS3, 250)) return;
    if (beeps(NOTE_C4, 500)) return;
    if (beeps(NOTE_F3, 500)) return;    
  }
  
  if (beeps(NOTE_G4, 1000)) return;
  if (beeps(NOTE_C4, 1000)) return;
  if (beeps(NOTE_DS4, 250)) return;
  if (beeps(NOTE_F4, 250)) return;
  if (beeps(NOTE_G4, 1000)) return;
  if (beeps(NOTE_C4, 1000)) return;
  if (beeps(NOTE_DS4, 250)) return;
  if (beeps(NOTE_F4, 250)) return;
  if (beeps(NOTE_D4, 500)) return;    
        
  for(int i=0; i<3; i++) {
    if (beeps(NOTE_G3, 500)) return;
    if (beeps(NOTE_AS3, 250)) return;
    if (beeps(NOTE_C4, 250)) return;
    if (beeps(NOTE_D4, 500)) return;
  }
}
  
  
//******* Happy Birthday ****************
void HappyBirthday() {
  if (beeps(NOTE_G3, 200)) return;
  if (beeps(NOTE_G3, 200)) return;
  if (beeps(NOTE_A3, 500)) return;
  if (beeps(NOTE_G3, 500)) return;
  if (beeps(NOTE_C4, 500)) return;
  if (beeps(NOTE_B3, 1000)) return;
  if (beeps(NOTE_G3, 200)) return;
  if (beeps(NOTE_G3, 200)) return;
  if (beeps(NOTE_A3, 500)) return;
  if (beeps(NOTE_G3, 500)) return;
  if (beeps(NOTE_D4, 500)) return;
  if (beeps(NOTE_C4, 1000)) return;
  if (cutShort) return;
  if (beeps(NOTE_G3, 200)) return;
  if (beeps(NOTE_G3, 200)) return;
  if (beeps(NOTE_G4, 500)) return;
  if (beeps(NOTE_E4, 500)) return;
  if (beeps(NOTE_C4, 500)) return;
  if (beeps(NOTE_B3, 500)) return;
  if (beeps(NOTE_A3, 750)) return;
  if (beeps(NOTE_F4, 200)) return;
  if (beeps(NOTE_F4, 200)) return;
  if (beeps(NOTE_E4, 500)) return;
  if (beeps(NOTE_C4, 500)) return;
  if (beeps(NOTE_D4, 500)) return;
  if (beeps(NOTE_C4, 1000)) return; 
}
  
  
//******* Harry Potter ****************
void HarryPotter() {
  if (beeps(NOTE_B4, 333)) return;  
  if (beeps(NOTE_E5, 500)) return;   
  if (beeps(NOTE_G5, 166)) return;    
  if (beeps(NOTE_FS5, 333)) return;    
  if (beeps(NOTE_E5, 666)) return;
  if (beeps(NOTE_B5, 333)) return;   
  if (beeps(NOTE_A5, 1000)) return;    
  if (beeps(NOTE_FS5, 1000)) return;  
 
  if (beeps(NOTE_E5, 500)) return;   
  if (beeps(NOTE_G5, 166)) return;    
  if (beeps(NOTE_FS5, 333)) return;    
  if (beeps(NOTE_DS5, 666)) return;    
  if (beeps(NOTE_F5, 333)) return;    
  if (beeps(NOTE_B4, 1666)) return;  
   if (cutShort) return;    
  if (beeps(NOTE_B4, 333)) return;    
  if (beeps(NOTE_E5, 500)) return;    
  if (beeps(NOTE_G5, 166)) return;    
  if (beeps(NOTE_FS5, 333)) return;    
  if (beeps(NOTE_E5, 666)) return;    
  if (beeps(NOTE_B5, 333)) return;    
  if (beeps(NOTE_D6, 666)) return;    
  if (beeps(NOTE_CS6, 333)) return;    
  if (beeps(NOTE_C6, 666)) return;    
  if (beeps(NOTE_GS5, 333)) return;    
  if (beeps(NOTE_C6, 500)) return;    
  if (beeps(NOTE_B5, 166)) return;    
  if (beeps(NOTE_AS5, 333)) return; 
  if (beeps(NOTE_AS4, 666)) return;    
  if (beeps(NOTE_G5, 333)) return;    
  if (beeps(NOTE_E5, 1666)) return;

  if (beeps(NOTE_G5, 333)) return;    
  if (beeps(NOTE_B5, 666)) return;    
  if (beeps(NOTE_G5, 333)) return;    
  if (beeps(NOTE_B5, 666)) return;    
  if (beeps(NOTE_G5, 333)) return;    
  if (beeps(NOTE_C6, 666)) return;    
  if (beeps(NOTE_B5, 333)) return;    
  if (beeps(NOTE_AS5, 666)) return;    
  if (beeps(NOTE_FS5, 333)) return;    
  if (beeps(NOTE_G5, 500)) return;    
  if (beeps(NOTE_B5, 166)) return;    
  if (beeps(NOTE_AS5, 333)) return;    
  if (beeps(NOTE_AS4, 666)) return;    
  if (beeps(NOTE_B4, 333)) return;    
  if (beeps(NOTE_B5, 1666)) return;    
  if (beeps(NOTE_G5, 333)) return;    
  if (beeps(NOTE_B5, 666)) return;    
  if (beeps(NOTE_G5, 333)) return;    
  if (beeps(NOTE_B5, 666)) return;    
  if (beeps(NOTE_G5, 333)) return;    
  if (beeps(NOTE_D6, 666)) return;    
  if (beeps(NOTE_CS6, 333)) return;    
  if (beeps(NOTE_C6, 666)) return;    
  if (beeps(NOTE_GS5, 333)) return;    
  if (beeps(NOTE_C6, 500)) return;    
  if (beeps(NOTE_B5, 166)) return;    
  if (beeps(NOTE_AS5, 333)) return;    
  if (beeps(NOTE_AS4, 666)) return;    
  if (beeps(NOTE_G5, 333)) return;    
  if (beeps(NOTE_E5, 1666)) return;    
}
  
  
//******* Pirate des caraïbes ****************
void Pirate() {  
  if (beeps(NOTE_E4, 125)) return;
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  if (beeps(NOTE_A4, 125)) return;
  delay(50); 
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_B4, 125)) return; 
  if (beeps(NOTE_C5, 250)) return; 
  if (beeps(NOTE_C5, 125)) return;
  delay(50);
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_D5, 125)) return;
  if (beeps(NOTE_B4, 250)) return;
  if (beeps(NOTE_B4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 375)) return;
  delay(50);  
  if (beeps(NOTE_E4, 125)) return;
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  if (beeps(NOTE_A4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_B4, 125)) return;
  if (beeps(NOTE_C5, 250)) return;
  if (beeps(NOTE_C5, 125)) return;
  if (cutShort) return;
  delay(50);
  if (beeps(NOTE_C5,125)) return; 
  if (beeps(NOTE_D5, 125)) return;
  if (beeps(NOTE_B4, 250)) return;
  if (beeps(NOTE_B4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return; 
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 375)) return;
  delay(50);    
  if (beeps(NOTE_E4, 125)) return;
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  if (beeps(NOTE_A4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_D5, 250)) return;
  if (beeps(NOTE_D5, 125)) return;
  delay(50);
  if (beeps(NOTE_D5, 125)) return; 
  if (beeps(NOTE_E5, 125)) return;
  if (beeps(NOTE_F5,250)) return;
  if (beeps(NOTE_F5, 125)) return;
  delay(50);
  if (beeps(NOTE_E5, 125)) return; 
  if (beeps(NOTE_D5, 125)) return;
  if (beeps(NOTE_E5, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  delay(50);  
  if (beeps(NOTE_A4,125)) return; 
  if (beeps(NOTE_B4, 125)) return;
  if (beeps(NOTE_C5, 250)) return;
  if (beeps(NOTE_C5, 125)) return;
  delay(50);
  if (beeps(NOTE_D5, 250)) return;
  if (beeps(NOTE_E5, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_B4, 250)) return;
  if (beeps(NOTE_B4, 125)) return;
  delay(50);
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_B4, 375)) return;
  delay(200);
  if (beeps(NOTE_A4, 250)) return; 
  if (beeps(NOTE_A4, 125)) return;     
  if (beeps(NOTE_E4, 125)) return;
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  if (beeps(NOTE_A4, 125)) return;
  delay(50); 
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_B4, 125)) return; 
  if (beeps(NOTE_C5, 250)) return; 
  if (beeps(NOTE_C5, 125)) return;
  delay(50);
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_D5, 125)) return;
  if (beeps(NOTE_B4, 250)) return;
  if (beeps(NOTE_B4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 375)) return;
  delay(50);  
  if (beeps(NOTE_E4, 125)) return;
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  if (beeps(NOTE_A4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_B4, 125)) return;
  if (beeps(NOTE_C5, 250)) return;
  if (beeps(NOTE_C5, 125)) return;
  delay(50);
  if (beeps(NOTE_C5,125)) return; 
  if (beeps(NOTE_D5, 125)) return;
  if (beeps(NOTE_B4, 250)) return;
  if (beeps(NOTE_B4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return; 
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 375)) return;
  delay(50);    
  if (beeps(NOTE_E4, 125)) return;
  if (beeps(NOTE_G4, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  if (beeps(NOTE_A4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_D5, 250)) return;
  if (beeps(NOTE_D5, 125)) return;
  delay(50);
  if (beeps(NOTE_D5, 125)) return; 
  if (beeps(NOTE_E5, 125)) return;
  if (beeps(NOTE_F5, 250)) return;
  if (beeps(NOTE_F5, 125)) return;
  delay(50);
  if (beeps(NOTE_E5, 125)) return; 
  if (beeps(NOTE_D5, 125)) return;
  if (beeps(NOTE_E5, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  delay(50);  
  if (beeps(NOTE_A4,125)) return; 
  if (beeps(NOTE_B4, 125)) return;
  if (beeps(NOTE_C5, 250)) return;
  if (beeps(NOTE_C5, 125)) return;
  delay(50);
  if (beeps(NOTE_D5, 250)) return;
  if (beeps(NOTE_E5, 125)) return;
  if (beeps(NOTE_A4, 250)) return;
  delay(50);
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_B4, 250)) return;
  if (beeps(NOTE_B4, 125)) return;
  delay(50);
  if (beeps(NOTE_C5, 125)) return;
  if (beeps(NOTE_A4, 125)) return;
  if (beeps(NOTE_B4, 375)) return;
  delay(200);
  if (beeps(NOTE_E5, 250)) return;
  delay(400);
  if (beeps(NOTE_F5, 250)) return;
  delay(400); 
  if (beeps(NOTE_E5, 125)) return;
  if (beeps(NOTE_E5, 125)) return;
  delay(50);
  if (beeps(NOTE_G5, 125)) return;
  delay(50);
  if (beeps(NOTE_E5, 125)) return;  
  if (beeps(NOTE_D5, 125)) return;
  delay(400);    
  if (beeps(NOTE_D5, 250)) return;
  delay(400);
  if (beeps(NOTE_C5, 250)) return;
  delay(400);  
  if (beeps(NOTE_B4, 125)) return;  
  if (beeps(NOTE_C5, 125)) return;
  delay(50);
  if (beeps(NOTE_B4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4,  500)) return;    
  if (beeps(NOTE_E5, 250)) return;  
  delay(400);
  if (beeps(NOTE_F5, 250)) return;
  delay(400);    
  if (beeps(NOTE_E5, 125)) return;
  if (beeps(NOTE_E5, 125)) return;
  delay(50);
  if (beeps(NOTE_G5, 125)) return;
  delay(50);
  if (beeps(NOTE_E5, 125)) return;
  if (beeps(NOTE_D5, 125)) return;
  delay(400);  
  if (beeps(NOTE_D5, 250)) return;
  delay(400);
  if (beeps(NOTE_C5, 250)) return;
  delay(400);  
  if (beeps(NOTE_B4, 125)) return;
  if (beeps(NOTE_C5, 125)) return;
  delay(50);
  if (beeps(NOTE_B4, 125)) return;
  delay(50);
  if (beeps(NOTE_A4, 500)) return;
}
  
  
//******* Mario ****************
void mario() {
  if (beeps(NOTE_E7, 120)) return;
  if (beeps(NOTE_E7, 120)) return;
  delay(120);  
  if (beeps(NOTE_E7, 120)) return;
  delay(120);  
  if (beeps(NOTE_C7, 120)) return;
  if (beeps(NOTE_E7, 120)) return;
  delay(120);  
  if (beeps(NOTE_G7, 120)) return;
  delay(240);    
  if (beeps(NOTE_G6, 120)) return;
  delay(360); 
  if (beeps(NOTE_C7, 120)) return;
  delay(240); 
  if (beeps(NOTE_G6, 120)) return;
  delay(240);  
  if (beeps(NOTE_E6, 120)) return;
  delay(240);    
  if (beeps(NOTE_A6, 120)) return;
  delay(120);  
  if (beeps(NOTE_B6, 120)) return;
  delay(120);  
  if (beeps(NOTE_AS6, 120)) return;
  if (beeps(NOTE_A6, 120)) return;
  delay(120); 
  if (beeps(NOTE_G6, 90)) return;
  if (beeps(NOTE_E7, 90)) return;
  if (beeps(NOTE_G7, 90)) return;
  if (beeps(NOTE_A7, 120)) return;
  if (cutShort) return;
  delay(120);  
  if (beeps(NOTE_F7, 120)) return;
  if (beeps(NOTE_G7, 120)) return;
  delay(120);  
  if (beeps(NOTE_E7, 120)) return;
  delay(120);  
  if (beeps(NOTE_C7, 120)) return;
  if (beeps(NOTE_D7, 120)) return;
  if (beeps(NOTE_B6, 120)) return;
  delay(240);  
  if (beeps(NOTE_C7, 120)) return;
  delay(240);  
  if (beeps(NOTE_G6, 120)) return;
  delay(240);    
  if (beeps(NOTE_E6, 120)) return;
  delay(240);     
  if (beeps(NOTE_A6, 120)) return;
  delay(120);  
  if (beeps(NOTE_B6, 120)) return;
  delay(120);  
  if (beeps(NOTE_AS6, 120)) return;
  if (beeps(NOTE_A6, 120)) return;
  delay(120); 
  if (beeps(NOTE_G6, 90)) return;
  if (beeps(NOTE_E7, 90)) return;
  if (beeps(NOTE_G7, 90)) return;
  if (beeps(NOTE_A7, 120)) return;
  delay(120);  
  if (beeps(NOTE_F7, 120)) return;
  if (beeps(NOTE_G7, 120)) return;
  delay(120);  
  if (beeps(NOTE_E7, 120)) return;
  delay(120);  
  if (beeps(NOTE_C7, 120)) return;
  if (beeps(NOTE_D7, 120)) return;
  if (beeps(NOTE_B6, 120)) return;
  delay(240);  
  if (beeps(NOTE_C4, 120)) return;
  if (beeps(NOTE_C5, 120)) return;
  if (beeps(NOTE_A3, 120)) return;
  if (beeps(NOTE_A4, 120)) return;
  if (beeps(NOTE_AS3, 120)) return;
  if (beeps(NOTE_AS4, 120)) return;
  delay(90);    
  if (beeps(NOTE_C4, 120)) return;
  if (beeps(NOTE_C5, 120)) return;
  if (beeps(NOTE_A3, 120)) return;
  if (beeps(NOTE_A4, 120)) return;
  if (beeps(NOTE_AS3, 120)) return;
  if (beeps(NOTE_AS4, 120)) return;
  delay(90);    
  if (beeps(NOTE_F3, 120)) return;
  if (beeps(NOTE_F4, 120)) return;
  if (beeps(NOTE_D3, 120)) return;
  if (beeps(NOTE_D4, 120)) return;
  if (beeps(NOTE_DS3, 120)) return;
  if (beeps(NOTE_DS4, 120)) return;
  delay(90);   
  if (beeps(NOTE_F3, 120)) return;
  if (beeps(NOTE_F4, 120)) return;
  if (beeps(NOTE_D3, 120)) return;
  if (beeps(NOTE_D4, 120)) return;
  if (beeps(NOTE_DS3, 120)) return;
  if (beeps(NOTE_DS4, 120)) return;
  delay(120);    
  if (beeps(NOTE_DS4, 180)) return;
  if (beeps(NOTE_CS4, 180)) return;
  if (beeps(NOTE_D4, 180)) return;
  if (beeps(NOTE_CS4, 60)) return;
  if (beeps(NOTE_DS4, 60)) return;
  if (beeps(NOTE_DS4, 60)) return;
  if (beeps(NOTE_GS3, 60)) return;
  if (beeps(NOTE_G3, 60)) return;
  if (beeps(NOTE_CS4, 60)) return;
  if (beeps(NOTE_C4, 180)) return;
  if (beeps(NOTE_FS4, 180)) return;
  if (beeps(NOTE_F4, 180)) return;
  if (beeps(NOTE_E3, 180)) return;
  if (beeps(NOTE_AS4, 180)) return;
  if (beeps(NOTE_A4, 180)) return;
  if (beeps(NOTE_GS4, 100)) return;
  if (beeps(NOTE_DS4, 100)) return;
  if (beeps(NOTE_B3, 100)) return;
  if (beeps(NOTE_AS3, 100)) return;
  if (beeps(NOTE_A3, 100)) return;
  if (beeps(NOTE_GS3, 100)) return;
  delay(90);  
}
  
//******* McGyver ****************
void McGyver() {
  if (beeps(NOTE_B4, 200)) return;
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 400)) return;
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_B4, 200)) return;
  delay(200);
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_B4, 400)) return;
  delay(200);
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 400)) return;
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_B4, 200)) return;
  if (cutShort) return;
  delay(200);
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_D6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_D6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B4, 200)) return;
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 400)) return;
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_B4, 200)) return;
  delay(200);
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_B4, 400)) return;
  delay(200);
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 400)) return;
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_B4, 200)) return;
  delay(200);
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_D6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_D6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B5, 400)) return;
  delay(200);
  if (beeps(NOTE_B5, 800)) return;
  delay(200);
  if (beeps(NOTE_B5, 400)) return;
  delay(200);
  if (beeps(NOTE_A5, 400)) return;
  if (beeps(NOTE_D6, 600)) return;
  if (beeps(NOTE_B5, 400)) return;
  delay(200);
  if (beeps(NOTE_B5, 800)) return;
  delay(200);
  if (beeps(NOTE_B5, 200)) return;
  delay(200);
  if (beeps(NOTE_A5, 800)) return;
  delay(400);
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_B5, 800)) return;
  delay(100);
  if (beeps(NOTE_FS5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  delay(200);
  if (beeps(NOTE_G5, 800)) return;
  delay(200);
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  delay(200);
  if (beeps(NOTE_B5, 400)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_G5, 200)) return;
  delay(200);
  if (beeps(NOTE_E6, 400)) return;
  if (beeps(NOTE_A5, 800)) return;
  delay(100);
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  delay(200);
  if (beeps(NOTE_B5, 800)) return;
  delay(200);
  if (beeps(NOTE_FS5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  delay(200);
  if (beeps(NOTE_G5, 800)) return;
  delay(200);
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  delay(200);
  if (beeps(NOTE_B5, 400)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_G5, 200)) return;
  delay(200);
  if (beeps(NOTE_E6, 400)) return;
  if (beeps(NOTE_A5, 800)) return;
  if (beeps(NOTE_B5, 800)) return;
  delay(50);
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_C6, 400)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_D6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_D6, 400)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_E6, 400)) return;
  if (beeps(NOTE_D6, 200)) return;
  if (beeps(NOTE_E6, 200)) return;
  if (beeps(NOTE_FS6, 400)) return;
  if (beeps(NOTE_B5, 400)) return;
  if (beeps(NOTE_G6, 400)) return;
  delay(200);
  if (beeps(NOTE_FS6, 400)) return;
  if (beeps(NOTE_F6, 400)) return;
  if (beeps(NOTE_B5, 400)) return;
  if (beeps(NOTE_G6, 200)) return;
  if (beeps(NOTE_E6, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_FS6, 200)) return;
  if (beeps(NOTE_D6, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_E6, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_G5, 200)) return;
  if (beeps(NOTE_D6, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_G5, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_E5, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_D5, 200)) return;
  if (beeps(NOTE_C6, 200)) return;
  if (beeps(NOTE_B5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_G5, 200)) return;
  if (beeps(NOTE_AS5, 400)) return;
  if (beeps(NOTE_A5, 400)) return;
  if (beeps(NOTE_G6, 200)) return;
  if (beeps(NOTE_G5, 200)) return;
  if (beeps(NOTE_D6, 200)) return;
  if (beeps(NOTE_G5, 200)) return;
  if (beeps(NOTE_DS6, 200)) return;
  if (beeps(NOTE_DS5, 200)) return;
  if (beeps(NOTE_AS5, 200)) return;
  if (beeps(NOTE_A5, 200)) return;
  if (beeps(NOTE_G5, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  if (beeps(NOTE_D5, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  if (beeps(NOTE_DS5, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  if (beeps(NOTE_AS4, 200)) return;
  if (beeps(NOTE_A4, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  if (beeps(NOTE_G4, 200)) return;
  delay(200);
}
  
  
//******* IndianaJones ****************
void IndianaJones() {
  if (beeps(NOTE_E5, 240)) return;
  delay(120);
  if (beeps(NOTE_F5, 120)) return;
  if (beeps(NOTE_G5, 120)) return;
  delay(120);
  if (beeps(NOTE_C6, 960)) return;
  delay(180);
  if (beeps(NOTE_D5, 240)) return;
  delay(120);
  if (beeps(NOTE_E5, 120)) return;
  if (beeps(NOTE_F5, 960)) return;
  delay(360);
  if (beeps(NOTE_G5, 240)) return;
  delay(120);
  if (beeps(NOTE_A5, 120)) return;
  if (beeps(NOTE_B5, 120)) return;
  delay(120);
  if (beeps(NOTE_F6, 960)) return;
  delay(240);
  if (beeps(NOTE_A5, 240)) return;
  delay(120);
  if (beeps(NOTE_B5, 120)) return;
  if (beeps(NOTE_C6, 480)) return;
  if (beeps(NOTE_D6, 480)) return;
  if (beeps(NOTE_E6, 480)) return;
  if (beeps(NOTE_E5, 240)) return;
  if (cutShort) return;
  delay(120);
  if (beeps(NOTE_F5, 120)) return;
  if (beeps(NOTE_G5, 120)) return;
  delay(120);
  if (beeps(NOTE_C6, 960)) return;
  delay(240);
  if (beeps(NOTE_D6, 240)) return;
  delay(120);
  if (beeps(NOTE_E6, 120)) return;
  if (beeps(NOTE_F6, 1440)) return;
  if (beeps(NOTE_G5, 240)) return;
  delay(120);
  if (beeps(NOTE_G5, 120)) return;
  if (beeps(NOTE_E6, 360)) return;
  delay(120);
  if (beeps(NOTE_D6, 240)) return;
  delay(120);
  if (beeps(NOTE_G5, 120)) return;
  if (beeps(NOTE_E6, 360)) return;
  delay(120);
  if (beeps(NOTE_D6, 240)) return;
  delay(120);
  if (beeps(NOTE_G5, 120)) return;
  if (beeps(NOTE_F6, 360)) return;
  delay(120);
  if (beeps(NOTE_E6, 240)) return;
  delay(120);
  if (beeps(NOTE_D6, 120)) return;
  if (beeps(NOTE_C6, 480)) return;
}
  
  
//******* StarWars coté force ****************
void StarWars2() {
  if (beeps(NOTE_F5, 166)) return;
  if (beeps(NOTE_F5, 166)) return;
  if (beeps(NOTE_F5, 166)) return;
  if (beeps(NOTE_AS5, 999)) return;
  if (beeps(NOTE_F6, 999)) return;
  if (beeps(NOTE_DS6, 166)) return;
  if (beeps(NOTE_D6, 166)) return;
  if (beeps(NOTE_C6, 166)) return;
  if (beeps(NOTE_AS6, 999)) return;
  if (beeps(NOTE_F6, 499)) return;
  if (beeps(NOTE_DS6, 166)) return;
  if (beeps(NOTE_D6, 166)) return;
  if (beeps(NOTE_C6, 166)) return;
  if (beeps(NOTE_AS6, 999)) return;
  if (beeps(NOTE_F6, 499)) return;
  if (beeps(NOTE_DS6, 166)) return;
  if (beeps(NOTE_D6, 166)) return;
  if (beeps(NOTE_DS6, 166)) return;
  if (beeps(NOTE_C6, 666)) return;
  if (cutShort) return;
  delay(333);
  if (beeps(NOTE_F5, 166)) return;
  if (beeps(NOTE_F5, 166)) return;
  if (beeps(NOTE_F5, 166)) return;
  if (beeps(NOTE_AS5, 999)) return;
  if (beeps(NOTE_F6, 999)) return;
  if (beeps(NOTE_DS6, 166)) return;
  if (beeps(NOTE_D6, 166)) return;
  if (beeps(NOTE_C6, 166)) return;
  if (beeps(NOTE_AS6, 999)) return;
  if (beeps(NOTE_F6, 499)) return;
  if (beeps(NOTE_DS6, 166)) return;
  if (beeps(NOTE_D6, 166)) return;
  if (beeps(NOTE_C6, 166)) return;
  if (beeps(NOTE_AS6, 999)) return;
  if (beeps(NOTE_F6, 499)) return;
  if (beeps(NOTE_DS6, 166)) return;
  if (beeps(NOTE_D6, 166)) return;
  if (beeps(NOTE_DS6, 166)) return;
  if (beeps(NOTE_C6, 666)) return;  
}
  
  
//******* BonBruteTruand ****************
void BonBruteTruand() {
  if (beeps(NOTE_AS5, 133)) return;
  if (beeps(NOTE_DS6, 133)) return;
  if (beeps(NOTE_AS5, 133)) return;
  if (beeps(NOTE_DS6, 133)) return;
  if (beeps(NOTE_AS5, 802)) return;
  if (beeps(NOTE_FS5, 400)) return;
  if (beeps(NOTE_GS5, 400)) return;
  if (beeps(NOTE_DS5, 1071)) return;
  if (beeps(NOTE_AS5, 133)) return;
  if (beeps(NOTE_DS6, 133)) return;
  if (beeps(NOTE_AS5, 133)) return;
  if (beeps(NOTE_DS6, 133)) return;
  if (beeps(NOTE_AS5, 802)) return;
  if (beeps(NOTE_FS5, 400)) return;
  if (beeps(NOTE_GS5, 400)) return;
  if (cutShort) return;
  if (beeps(NOTE_CS6, 1071)) return;
  if (beeps(NOTE_AS5, 133)) return;
  if (beeps(NOTE_DS6, 133)) return;
  if (beeps(NOTE_AS5, 133)) return;
  if (beeps(NOTE_DS6, 133)) return;
  if (beeps(NOTE_AS5, 802)) return;
  if (beeps(NOTE_FS5, 400)) return;
  if (beeps(NOTE_F5, 199)) return;
  if (beeps(NOTE_DS5, 199)) return;
  if (beeps(NOTE_CS5, 1071)) return;
  if (beeps(NOTE_AS5, 133)) return;
  if (beeps(NOTE_DS6, 133)) return;
  if (beeps(NOTE_AS5, 133)) return;
  if (beeps(NOTE_DS6, 133)) return;
  if (beeps(NOTE_AS5, 802)) return;
  if (beeps(NOTE_GS5, 400)) return;
  if (beeps(NOTE_DS5, 1071)) return;  
}
  
  
//******* Entertainement ****************
void Entertainement() {
  if (beeps(NOTE_D5, 214)) return;
  if (beeps(NOTE_DS5, 214)) return;
  if (beeps(NOTE_E5, 214)) return;
  if (beeps(NOTE_C6, 428)) return;
  if (beeps(NOTE_E5, 214)) return;
  if (beeps(NOTE_C6, 428)) return;
  if (beeps(NOTE_E5, 214)) return;
  if (beeps(NOTE_C6, 1284)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_D6, 214)) return;
  if (beeps(NOTE_DS6, 214)) return;
  if (beeps(NOTE_E6, 214)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_D6, 214)) return;
  if (beeps(NOTE_E6, 428)) return;
  if (beeps(NOTE_B5, 214)) return;
  if (beeps(NOTE_D6, 428)) return;
  if (beeps(NOTE_C6, 856)) return;
  if (cutShort) return;
  delay(428);
  if (beeps(NOTE_D5, 214)) return;
  if (beeps(NOTE_DS5, 214)) return;
  if (beeps(NOTE_E5, 214)) return;
  if (beeps(NOTE_C6, 428)) return;
  if (beeps(NOTE_E5, 214)) return;
  if (beeps(NOTE_C6, 428)) return;
  if (beeps(NOTE_E5, 214)) return;
  if (beeps(NOTE_C6, 1284)) return;
  delay(214);
  if (beeps(NOTE_A5, 214)) return;
  if (beeps(NOTE_G5, 214)) return;
  if (beeps(NOTE_FS5, 214)) return;
  if (beeps(NOTE_A5, 214)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_E6, 428)) return;
  if (beeps(NOTE_D6, 214)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_A5, 214)) return;
  if (beeps(NOTE_D6, 856)) return;  
}
  
  
//******* 20thCenFox ****************
void twentyCentFox() {
  if (beeps(NOTE_B5, 107)) return;
  delay(214);
  if (beeps(NOTE_B5, 107)) return;
  if (beeps(NOTE_B5, 107)) return;
  if (beeps(NOTE_B5, 856)) return;
  delay(107);
  if (beeps(NOTE_C6, 107)) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  delay(53);
  if (beeps(NOTE_C6, 107)) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  delay(53);
  if (beeps(NOTE_C6, 107)) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  
  delay(214);
  if (beeps(NOTE_B5, 107)) return;
  if (beeps(NOTE_B5, 107)) return;
  if (beeps(NOTE_B5, 107)) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  if (cutShort) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  delay(53);
  if (beeps(NOTE_GS5, 107)) return;
  delay(53);
  if (beeps(NOTE_A5, 107)) return;
  delay(53);
  if (beeps(NOTE_B5, 107)) return;
  delay(214);
  if (beeps(NOTE_B5, 107)) return;
  if (beeps(NOTE_B5, 107)) return;
  if (beeps(NOTE_B5, 856)) return;
  delay(428);
  if (beeps(NOTE_E5, 214)) return;
  if (beeps(NOTE_GS5, 214)) return;
  if (beeps(NOTE_B5, 214)) return;
  if (beeps(NOTE_CS6, 1712)) return;
  if (beeps(NOTE_FS5, 214)) return;
  if (beeps(NOTE_A5, 214)) return;
  if (beeps(NOTE_CS6, 214)) return;
  if (beeps(NOTE_E6, 1712)) return;
  if (beeps(NOTE_A5, 214)) return;
  if (beeps(NOTE_CS6, 214)) return;
  if (beeps(NOTE_E6, 214)) return;
  if (beeps(NOTE_E6, 1712)) return;
  if (beeps(NOTE_B5, 214)) return;
  if (beeps(NOTE_GS5, 214)) return;
  if (beeps(NOTE_A5, 214)) return;
  if (beeps(NOTE_B5, 856)) return;  
}
  
  
//******* looney ****************
void looney() {
  if (beeps(NOTE_C6, 428)) return;
  if (beeps(NOTE_F6, 214)) return;
  if (beeps(NOTE_E6, 214)) return;
  if (beeps(NOTE_D6, 214)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_A5, 642)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_F6, 214)) return;
  if (beeps(NOTE_E6, 214)) return;
  if (beeps(NOTE_D6, 214)) return;
  if (beeps(NOTE_DS6, 214)) return;
  if (beeps(NOTE_E6, 642)) return;
  if (beeps(NOTE_E6, 214)) return;
  if (beeps(NOTE_E6, 214)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_D6, 214)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_E6, 214)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_D6, 214)) return;
  if (beeps(NOTE_A5, 214)) return;
  if (beeps(NOTE_C6, 214)) return;
  if (beeps(NOTE_G5, 214)) return;
  if (beeps(NOTE_AS5, 214)) return;
  if (beeps(NOTE_A5, 214)) return;
  if (beeps(NOTE_F5, 214)) return;  
}
  
  
//******* BarbieGirl ****************
void BarbieGirl() {
  if (beeps(NOTE_GS5, 240)) return;
  if (beeps(NOTE_E5, 240)) return;
  if (beeps(NOTE_GS5, 240)) return;
  if (beeps(NOTE_CS6, 240)) return;
  if (beeps(NOTE_A5, 480)) return;
  delay(480);
  if (beeps(NOTE_FS5, 240)) return;
  if (beeps(NOTE_DS5, 240)) return;
  if (beeps(NOTE_FS5, 240)) return;
  if (beeps(NOTE_B5, 240)) return;
  if (beeps(NOTE_GS5, 480)) return;
  if (beeps(NOTE_FS5, 240)) return;
  if (beeps(NOTE_E5, 240)) return;
  delay(480);
  if (beeps(NOTE_E5, 240)) return;
  if (beeps(NOTE_CS5, 240)) return;
  if (beeps(NOTE_FS5, 480)) return;
  if (beeps(NOTE_CS5, 480)) return;
  if (cutShort) return;
  delay(480);
  if (beeps(NOTE_FS5, 240)) return;
  if (beeps(NOTE_E5, 240)) return;
  if (beeps(NOTE_GS5, 480)) return;
  if (beeps(NOTE_FS5, 480)) return;
}
  
  
//******* Greensleaves ****************
void Greensleaves() {
  if (beeps(NOTE_G5, 428)) return;
  if (beeps(NOTE_AS5, 856)) return;
  if (beeps(NOTE_C6, 428)) return;
  if (beeps(NOTE_D6, 642)) return;
  if (beeps(NOTE_DS6, 214)) return;
  if (beeps(NOTE_D6, 428)) return;
  if (beeps(NOTE_C6, 856)) return;
  if (beeps(NOTE_A5, 428)) return;
  if (beeps(NOTE_F5, 642)) return;
  if (beeps(NOTE_G5, 214)) return;
  if (beeps(NOTE_A5, 428)) return;
  if (beeps(NOTE_AS5, 856)) return;
  if (cutShort) return;
  if (beeps(NOTE_G5, 428)) return;
  if (beeps(NOTE_G5, 642)) return;
  if (beeps(NOTE_F5, 214)) return;
  if (beeps(NOTE_G5, 428)) return;
  if (beeps(NOTE_A5, 856)) return;
  if (beeps(NOTE_F5, 428)) return;
  if (beeps(NOTE_D5, 856)) return;
  if (beeps(NOTE_G5, 428)) return;
  if (beeps(NOTE_AS5, 856)) return;
  if (beeps(NOTE_C6, 428)) return;
  if (beeps(NOTE_D6, 642)) return;
  if (beeps(NOTE_E6, 214)) return;
  if (beeps(NOTE_D6, 428)) return;
  if (beeps(NOTE_C6, 856)) return;
  if (beeps(NOTE_A5, 428)) return;
  if (beeps(NOTE_F5, 642)) return;
  if (beeps(NOTE_G5, 214)) return;
  if (beeps(NOTE_A5, 428)) return;
  if (beeps(NOTE_AS5, 642)) return;
  if (beeps(NOTE_A5, 214)) return;
  if (beeps(NOTE_G5, 428)) return;
  if (beeps(NOTE_FS5, 642)) return;
  if (beeps(NOTE_E5, 214)) return;
  if (beeps(NOTE_FS5, 428)) return;
  if (beeps(NOTE_G5, 856)) return;
}
  
  
//******* James Bond *******************  
void Bond() {
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_DS6, 93)) return;
  if (beeps(NOTE_DS6, 93)) return;
  if (beeps(NOTE_DS6, 187)) return;
  if (beeps(NOTE_DS6, 375)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_E6, 93)) return;
  if (beeps(NOTE_E6, 93)) return;
  if (beeps(NOTE_E6, 187)) return;
  if (beeps(NOTE_E6, 375)) return;
  if (beeps(NOTE_DS6, 187)) return;
  if (beeps(NOTE_DS6, 187)) return;
  if (beeps(NOTE_DS6, 187)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_DS6, 93)) return;
  if (beeps(NOTE_DS6, 93)) return;
  if (beeps(NOTE_DS6, 187)) return;
  if (beeps(NOTE_DS6, 375)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (cutShort) return;
  if (beeps(NOTE_E6, 93)) return;
  if (beeps(NOTE_E6, 93)) return;
  if (beeps(NOTE_E6, 187)) return;
  if (beeps(NOTE_E6, 375)) return;
  if (beeps(NOTE_DS6, 187)) return;
  if (beeps(NOTE_D6, 187)) return;
  if (beeps(NOTE_CS6, 187)) return;
  if (beeps(NOTE_CS7, 187)) return;
  if (beeps(NOTE_C7, 1125)) return;
  if (beeps(NOTE_GS6, 187)) return;
  if (beeps(NOTE_FS6, 187)) return;
  if (beeps(NOTE_GS6, 1125)) return;
}
