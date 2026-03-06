/* * STROM-WEG ADAPTER - DER PROFI-ERZÄHLER (v1.0)
 * Ein intelligenter Wächter, der Zorin OS über USB zum Schlafen schickt.
 */

// --- EINSTELLUNGEN ---
const int triggerPin = 4;           // Unser "Fuehler" zum Tuya-Schalter (CH2)
const long warteZeit = 5000;         // 5 Sekunden Pause, nachdem ein Befehl gesendet wurde
const long gedankenPause = 20000;    // Alle 20 Sekunden erzaehlt der Waechter ein Stück Geschichte

// --- VARIABLE SPEICHER ---
unsigned long letzteAktionZeit = 0;  
unsigned long letzteGedankenZeit = 0; 
int geschichtenZaehler = 0;          
bool befehlGesendet = false;

void setup() {
  // Wir warten 5 Sekunden, damit die Elektronik stabil wird und du den Monitor oeffnen kannst
  delay(5000); 
  Serial.begin(9600);
  
  // Den Übertragungsweg einmal komplett "putzen"
  Serial.flush();
  for(int i=0; i<10; i++) { Serial.println(""); }
  
  // Pin 4 vorbereiten: Er ist standardmaessig "HIGH" (1)
  pinMode(triggerPin, INPUT_PULLUP);
  
  // Begruessung und Versions-Stempel im Log
  Serial.println("******************************************");
  Serial.println("* stromweg - DER WAECHTER (v1.0)         *");
  Serial.println("* System bereit und Leitung geputzt!     *");
  Serial.println("******************************************");
  
  // Wir starten die Zeitmessung fuer die erste Geschichte
  letzteGedankenZeit = millis();
}

void loop() {
  unsigned long aktuelleZeit = millis();
  int schalterZustand = digitalRead(triggerPin);

  // --- DAS GESCHICHTENBUCH (Erzaehlt alle 20 Sek. etwas Wissenswertes) ---
  if (aktuelleZeit - letzteGedankenZeit >= gedankenPause) {
    letzteGedankenZeit = aktuelleZeit; // Zeit sofort fuer den naechsten Takt merken

    if (!befehlGesendet) {
      // Wenn eine neue Runde der Geschichte beginnt, zeigen wir den Trenner
      if (geschichtenZaehler == 0) {
        Serial.println("[LOG] ###");
        delay(5); 
      }

      // Die Geschichte in kleinen, verstaendlichen Haeppchen
      switch (geschichtenZaehler) {
        case 0: Serial.println("[LOG] Ich bewache Pin 4. Er ist mein Draht zur Tuya-Aussenwelt."); break;
        case 1: Serial.println("[LOG] Wusstest du? Die Webcam-Leitung ist jetzt mein Geheimgang."); break;
        case 2: Serial.println("[LOG] Wenn der Schalter kippt, schicke ich Zorin OS ins Bett."); break;
        case 3: Serial.println("[LOG] Ich warte geduldig... Strom ist wie Wasser, man muss vorsichtig sein."); break;
        case 4: Serial.println("[LOG] Sicherheit geht vor: Erst Speichern, dann Strom aus!"); break;
        case 5: Serial.println("[LOG] Ich bin ein kleiner Chip, aber ich habe eine grosse Aufgabe."); break;
      }

      geschichtenZaehler++;
      if (geschichtenZaehler > 5) { 
        geschichtenZaehler = 0; 
      }
      
      // Eine winzige Pause, damit der Text nicht "stottert"
      delay(50); 
    }
  }

  // --- DIE ACTION (Wenn der Tuya-Switch auf AUS schaltet / Kontakt schliesst) ---
  if (schalterZustand == LOW && !befehlGesendet) {
    letzteAktionZeit = aktuelleZeit;
    befehlGesendet = true;
    
    // Visuelle Warnung im Log
    Serial.println(""); 
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    Serial.println("!!! SIGNAL ERKANNT: SHUTDOWN_NOW       !!!");
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    
    // Der eigentliche Befehl, den das Python-Skript am Laptop versteht
    Serial.println("SHUTDOWN_NOW"); 
    
    Serial.println(""); 
    delay(100); // Sicherheits-Pause fuer die Datenleitung
  }

  // --- BEREITSCHAFT WIEDERHERSTELLEN ---
  if (befehlGesendet && (aktuelleZeit - letzteAktionZeit >= warteZeit)) {
    befehlGesendet = false;
    letzteGedankenZeit = aktuelleZeit; // Herzschlag-Timer zuruecksetzen
    Serial.println("[LOG] Ich bin wieder bereit und warte auf dein Signal!");
    Serial.println("------------------------------------------");
  }
}
