# Installation

Diese Anleitung geht davon aus, dass du ein Ubuntu, Debian oder ein ähnliches
Linux vor dir hast. Es wird grundlegendes Wissen im Umgang mit der Kommandozeile
vorausgesetzt.

Als erstes müssen die benötigten Programme installiert werden:

	sudo apt-get install avr-libc avrdude gcc-avr build-essential

Danach kannst du dir den Quellcode für die Firmware des Bugs herunterladen.
Bewege dich dafür in das Verzeichnis in das du den Quellcode kopieren möchtest
und führe folgenden Befehl aus:

	git clone https://github.com/c3d2/pentabug.git

Jetzt hast du einen neuen Ordner namens `pentabug` in dem sich die Firmware
befindet:

	cd pentabug/firmware/

Nun kannst du den menschenlesbaren Quellcode in maschinenlesbare Befehle
überführen. Diesen Vorgang nennt man Kompillieren. Dabei unterstützt dich das
sogenannte `Makefile`, dass automatisch überprüft welche Dateien nicht vorhanden
sind oder geändert wurden 

	make

Wenn der Befehl oben erfolgreich durchgelaufen ist kannst du die Firmware mit
folgendem Befehl auf deinen Pentabug laden:

	make flsmart

Nun kannst du anfangen den Quellcode anpassen und deine eigenen Programme
entwerfen. Die einzelnen Programme befinden sich im Ordner

	pentabug/firmware/app/

Jedes mal wenn du eine Änderung ausprobieren willst musst die wieder das
Makefile ausführen mit folgenden Befehlen

	make
	make flsmart

Wenn du ein neues Programm anlegst oder ein existierendes Programm aus der
Firmware für deinen Pentabug entfernen musst du die erste Zeile folgender Datei
editieren:

	pentabug/firmware/Makefile

Die einzelnen Einträge hier entsprechen den Dateinamen in

	pentabug/firmware/app/

