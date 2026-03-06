# stromweg - Smarter Hardware-Shutdown Adapter (v1.0)

Dieses Projekt bietet eine vollständige Lösung, um einen Laptop (optimiert für Zorin OS) sicher per Hardware-Signal herunterzufahren, bevor die Stromzufuhr durch ein externes Relais (z. B. Tuya Zigbee) getrennt wird. Dies verhindert Datenverlust und schont das Dateisystem.

## 1. Das Konzept: Die Haus-Zorin-Geschichte 🏠
*Für neugierige Kinder (und Erwachsene)*

Stell dir vor, dein Laptop ist ein riesiges Haus namens **Zorin**. Darin arbeiten viele kleine Helfer (Programme). Wenn man einfach den Strom abstellt, erschrecken sie und lassen alles fallen.

* **Der Wächter (ESP32):** Er sitzt draussen am Zaun und ruft per Rohrpost (USB-Kabel) "SHUTDOWN_NOW!", wenn Feierabend ist.
* **Der Lauscher (Python-Skript):** Er sitzt im Haus, hört das Signal und läutet die goldene Glocke (`sudo shutdown`), damit alle Helfer Zeit zum Aufräumen haben.
* **Der magische Ausweis (sudo):** Er erlaubt dem Lauscher, dem ganzen Haus zu befehlen: "Ab ins Bett!"

---

## 2. Hardware-Aufbau
Wir nutzen eine **Zwei-Kabel-Lösung**, um Signalstörungen zwischen der 4A-Last und der Datenleitung zu vermeiden.

### Verdrahtungsschema
![IMAGE 1: Vollständiger Schaltplan v1.0](images/schaltplan.jpg)

### Kabel-Belegung
| Kabel / Paar | Funktion | Verbindung |
| :--- | :--- | :--- |
| **Kabel A (Last)** | Stromversorgung (4A) | Laptop DC-Buchse <-> Tuya Kanal 1 |
| **Kabel B (RJ45)** | Signal & USB-Brücke | Interne Webcam-Leitung <-> ESP32 |

![IMAGE 2: Detailaufnahme Kabel-Modding](images/kabel_mod.jpg)
![IMAGE 3: ESP32 Gehäuse & Anschluss](images/esp32_case.jpg)

---

## 3. Installation (Zorin OS) 💻

### Vorbereitung
`mkdir -p ~/.local/bin`  
`sudo apt update && sudo apt install python3-serial -y`

### Dateien kopieren
1. Kopiere `listener.py` nach `~/.local/bin/`.
2. Kopiere `listener.service` nach `/etc/systemd/system/`.

### Dienst aktivieren
`sudo systemctl daemon-reload`  
`sudo systemctl enable listener.service`  
`sudo systemctl start listener.service`

![IMAGE 4: Screenshot Terminal Installation](images/terminal_install.jpg)

---

## 4. Deinstallation 🗑️
*(Falls du das System sauber entfernen möchtest)*

1. **Dienst stoppen:** `sudo systemctl stop listener.service && sudo systemctl disable listener.service`
2. **Dateien löschen:** `sudo rm /etc/systemd/system/listener.service`  
   `rm ~/.local/bin/listener.py`
3. **Konfiguration bereinigen:** `sudo systemctl daemon-reload`

---

## 5. Tuya Zigbee Konfiguration 🔌
Stelle in deiner App folgende Szenen ein, um den korrekten Ablauf zu sichern:

**Szene: Laptop Ausschalten**
1. Aktion: Schalte **Kanal 2** (Signal an ESP32) auf AUS.
2. Verzögerung: **45 Sekunden** warten (Zeit für den Shutdown).
3. Aktion: Schalte **Kanal 1** (Hauptstrom 4A) auf AUS.

**Szene: Laptop Einschalten**
1. Aktion: Schalte **Kanal 1** (Strom) auf AN.
2. Aktion: Schalte **Kanal 2** (Signal) auf AN.

![IMAGE 5: Screenshot Tuya App Szenen](images/tuya_app.jpg)

---

## 6. Technische Details (ESP32) 🛠️
Der ESP32 läuft mit dem **"Profi-Erzähler" Sketch (v1.0)**. Er überwacht Pin 4 und sendet Informationen im 20-Sekunden-Takt an das Log.

* **Baudrate:** 9600
* **Trigger:** GPIO 4 (INPUT_PULLUP)
* **Log-Merkmal:** Kapitel-Trenner `[LOG] ###` nach jeder Geschichtenrunde.

![IMAGE 6: Foto des fertigen Adapters im Einsatz](images/fertiges_projekt.jpg)

---
**Autor:** Albertus Zerk | **Version:** 1.0 (Final) | **Rechtschreibung:** Konsequent "ss" statt "ß".
