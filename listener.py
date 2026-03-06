#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
STROM-WEG ADAPTER - DER LAUSCHER (v1.0)
Dieses Skript wartet auf das USB-Signal vom ESP32 und faehrt den Laptop sicher runter.
"""

import serial
import os
import time

# --- EINSTELLUNGEN ---
# Der Port kann mit 'dmesg | grep tty' geprueft werden
SERIAL_PORT = '/dev/ttyUSB0' 
BAUD_RATE = 9600
COMMAND = "SHUTDOWN_NOW"
ERROR_LOG = "/tmp/power_monitor_error.log"

def start_listening():
    print(f"--- stromweg: Listener v1.0 aktiv ---")
    print(f"Suche Signal auf {SERIAL_PORT}...")
    
    try:
        # Verbindung zum ESP32 aufbauen
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        # Kurze Pause, damit die serielle Verbindung stabil steht
        time.sleep(2)
        
        while True:
            # Pruefen, ob Daten in der Leitung sind
            if ser.in_waiting > 0:
                # Nachricht lesen und Leerzeichen/Umbrueche entfernen
                line = ser.readline().decode('utf-8').strip()
                
                # Wenn das geheime Codewort kommt:
                if line == COMMAND:
                    print(f"[ALARM] Befehl '{COMMAND}' empfangen!")
                    print("System wird jetzt sicher heruntergefahren...")
                    # Den Chef-Befehl an das Haus Zorin absetzen
                    os.system("sudo shutdown -h now")
            
            # Die CPU nicht zu stark belasten
            time.sleep(1)
            
    except Exception as e:
        # Fehlermeldung in das Log-Buch schreiben
        error_msg = f"[{time.ctime()}] Fehler beim Zugriff auf Serial: {str(e)}\n"
        with open(ERROR_LOG, "a") as f:
            f.write(error_msg)
        print(f"[FEHLER] {str(e)}")

if __name__ == "__main__":
    start_listening()
