/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
Funktion: NANO Weihnachtsbaum mit Jingle Bells Melodie

**************************************************************************************************
Version: 21.06.2022
**************************************************************************************************
Board: NANO
**************************************************************************************************
Libraries:
https://github.com/espressif/arduino-esp32/tree/master/libraries

**************************************************************************************************
C++ Arduino IDE V1.8.19

**************************************************************************************************
Einstellungen:
https://dl.espressif.com/dl/package_esp32_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
http://arduino.esp8266.com/stable/package_esp8266com_index.json

Bauteile:
9 LEDs 5 mm oder 3 mm, (Farbe je nach eigener Wahl)
9 Widerstände a 220 Ohm,
1 Piezo Buzzer,
12 Pins einer Stiftleiste mit einem Reihenabstand von 2,54mm
Für den Betrieb des PCBs wird benötigt:
12x Breadboardkabel, 20cm, männlich – weiblich,
1x Arduino Nano
**************************************************************************************************/
//LEDs an den digitalen Pins 2 bis 10 angeschlossen
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6
#define LED6 7
#define LED7 8
#define LED8 9
#define LED9 10
//Array mit den LEDs sowie dem Status der LED
int leds[9][2] = {{LED1,LOW}, {LED2,LOW}, {LED3,LOW}, {LED4,LOW}, {LED5,LOW}, {LED6,LOW}, {LED7,LOW}, {LED8,LOW}, {LED9,LOW}};
//Intervall zum ändern eines Status der LED
//Wenn die LEDs öffters / schneller blinken sollen muss diese Variable kleiner werden ansonsten größer.
const int LED_ACTION_INTERVALL = 750;
//Variable zum speichern der letzten Ausführung.
long lastLedAction =0;

//Piezo Buzzer am digitalen PWM Pin 11 angeschlossen
#define BUZZER 0

//Definieren der Töne
#define  C     2100
#define  D     1870 
#define  E     1670
#define  f     1580
#define  G     1400 
#define  R     0 //reset

//Melodie von Jingle Bells
const int jingleBellsMelody[] = {E, E, E,R, E, E, E,R, E, G, C, D, E, R, f, f, f,f, f, E, E,E, E, D ,D,E, D, R, G ,R, E, E, E,R, E, E, E,R, E, G, C, D, E, R, f, f, f,f, f, E, E, E,  G,G, f, D, C,R };
//Berechnen der Anzahl der Töne in der Melodie
const int MAX_COUNT = sizeof(jingleBellsMelody) / 2;
const int beat = 50;
const long tempo = 10000;
int rest_count = 100;
int pause = 1000;

void setup(){
  Serial.begin(115200);
  //Initialisieren der LEDs als Ausgänge und mit dem Status LOW (AUS)
//  for(int i=0;i<9;i++){
//   digitalWrite(leds[i][0], leds[i][1]); 
// }

  pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
      pinMode(LED3, OUTPUT);
        pinMode(LED4, OUTPUT);
          pinMode(LED5, OUTPUT);
            pinMode(LED6, OUTPUT);
              pinMode(LED7, OUTPUT);
                pinMode(LED8, OUTPUT);
                  pinMode(LED9, OUTPUT);

  //Setzen des Buzzer Pins als Ausgang
  pinMode(BUZZER, OUTPUT);  
}

/**
 * Spielt die Melodie ab.
 */
void playTone(int tone_, long duration) {
   long elapsed_time = 0;
   //Wenn der Ton kein Reset ist dann...
   if (tone_ > 0) {
     //Prüfen ob die Zeit vom letzten ausführen eines LED blinkens kleiner als 
     //die aktuelle Zeit + dem Intervall ist.
     if(lastLedAction < (millis()+LED_ACTION_INTERVALL)){
        //speichern der aktuellen Zeit
        lastLedAction = millis();
        //erzeugen einer Zufallszahl von 0 bis 8
        int randomValue = random(9);
        //lesen des Status aus dem Array mit den LEDs 
        int status = leds[randomValue][1];
        //Wenn der Status LOW / AUS ist dann soll er auf HIGH gewechselt werden.
        if(status == LOW){
          leds[randomValue][1] = (HIGH);
        } else {
          //Wenn der Status HIGH / AN ist dann soll auf LOW gewehcselt werden.
          leds[randomValue][1] = LOW;
        }
        //neues Auslesen des Status aus dem Array
        status = leds[randomValue][1];
         Serial.print("Zufallszahl: ");
         Serial.println(randomValue);

        //Setzen der LED mit dem neuen Status
        digitalWrite(leds[randomValue][0], status); 
 //       analogWrite (leds[randomValue][0], status);
 
     }

     //Abspielen der Melodie
     while (elapsed_time < duration) {
       //Buzzer an
       digitalWrite(BUZZER,HIGH);
       //Pause von der länge des Tones durch 2
       delayMicroseconds(tone_ / 2);
       // DOWN
       //Buzzer aus
       digitalWrite(BUZZER, LOW);
       //Pause von der länge des Tones durch 2
       delayMicroseconds(tone_ / 2);
       //Speichern wielange der Ton dauerte
       elapsed_time += (tone_);
     }
   }
   else { // Reset 
     for (int j = 0; j < rest_count; j++) {
       delayMicroseconds(duration); 
     } 
   }                                
 }

void loop(){
 
  long duration = beat * tempo; // Set up timing
  //Für jede Note aus der Melodie....
  for (int i=0; i<MAX_COUNT; i++) {
    //lesen der Note i aus dem Array der Melodie    
    playTone(jingleBellsMelody[i], duration);
    //Wenn die Melodie schneller abgespielt werden soll dann muss 
    //die Variable für den beat sowie die pause angepasst werden.
    delayMicroseconds(pause);  // eine kleine Pause zwischen den Noten....
  }
    
  delay(500);
}
