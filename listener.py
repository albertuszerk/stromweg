#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial
import os
import time

# --- EINSTELLUNGEN ---
SERIAL_PORT = '/dev/ttyUSB0' 
BAUD_RATE = 9600
COMMAND = "SHUTDOWN_NOW"
USER_NAME = "sysadmin"
USER_ID = "1000"

def send_single_alert():
    title = "🛑 STROM-WEG ALARM"
    msg = "FEIERABEND! Der Computer wird in 60 Sekunden ausgeschaltet. SPEICHERE SOFORT!"
    
    # Der Befehl für die grafische Benachrichtigung
    cmd = (
        f"sudo -u {USER_NAME} "
        f"DISPLAY=:0 DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/{USER_ID}/bus "
        f"notify-send '{title}' '{msg}' --urgency=critical --icon=dialog-warning"
    )
    os.system(cmd)
    # Text-Nachricht für die Konsole
    os.system(f"echo '{msg}' | wall")

def start_listening():
    print(f"--- stromweg: Listener v1.1.9 aktiv ---")
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        time.sleep(2)
        ser.reset_input_buffer()
        
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                
                if line == COMMAND:
                    print(f"[ALARM] Signal erkannt! 60s Countdown läuft...")
                    
                    # Einmalige Warnung absetzen
                    send_single_alert()
                    
                    # Shutdown in 1 Minute einplanen
                    os.system("sudo shutdown -h +1")
                    
                    # 120s Sperre, um Rescheduling-Schleifen zu verhindern
                    print("Sperre aktiv: System fährt herunter...")
                    time.sleep(120) 
            
            time.sleep(0.5)
    except Exception as e:
        print(f"[FEHLER] {str(e)}")

if __name__ == "__main__":
    start_listening()
