# Das Gelöt von Pentabug

In dieser Episode erfährst Du, junger Padawan, was Dein Pentabug so kann, wie
Du ihn zusammenbaust und dann zum Leben erweckst.

## Was kann son Pentabug eigentlich?

Der Pentabug ist ein kleiner Roboter, den Du selbst zusammebauen und
programieren kannst.  Er ist recht einfach gehalten und gerade für Einsteiger
im Bereich der Elektronik besonders gut geeignet.

Der Pentabug kann leuchten, piepen, sich bewegen, auf Geräusche reagieren,
Infrarot senden und empfangen. Das kann man zum einen dafür nutzen, dass sich
verschiedene Pentabugs unterhalten, zum anderen aber auch um Geräte
fernzusteuern. Zudem ist er erweiterbar, d.h. man kann recht einfach weitere
kleine Schaltungen bauen, die seine Funktionen erweitern.

## Voraussetzungen

Wenn Du einen Pentabug bauen möchtest benötigst Du neben einem Pentabug-Bausatz folgende weitere Dinge:

1. 1 Lötkolben
2. 1 Schwämmchen, feucht. Ein feuchtes Tuch tut es auch.
3. Lötzinn.
4. 1 hitzebeständige Arbeitsunterlage (z.B. kleines Holzbrett oder eine Pappe).
5. 1 Heißklebepistole mit Kleber
6. 1 Seitenschneider
7. 1 Spitzzange (nicht unbedingt zwingend)

Für den Fall, dass Du mal fehler machst, benötigst Du weitere Dinge:

8. 1 Multimeter
9. 1 Lötlutsche und / oder Entlötlitze

### Auswahl des Lötkolbens und des Lötzinns

Der Lötkolben ist ein Werkzeug mit dem man die verschiedenen elektornischen
Bauelemente mit einander verbindet. Er wird mit Strom betrieben und wird sehr
heiß.  Diese Hitze nutzt man um Lötzinn zum schmelzen zu bringen, welches dann
sozusagen die elektrischen Bauelemente elektrisch leitend mit einander
verklebt.

Wenn Du noch keinen Lötkolben zur Verfügung hast, also jetzt loslaufen möchtest
um Dir einen zu besorgen, achte darauf, dass er mindestens 25 Watt hat. Wenn er
mehr Watt hat, macht das Löten allerdings mehr Spass, da man pro Lötstelle
nicht so lange auf das Heißwerden warten muss.

Die Spitze sollte nicht zu dick sein; einen Feinlötkolben benötigst Du für
dieses Projekt allerdings auch nicht.

Überlege, ob Du gleich eine kleine Lötstation kaufst. Die hat meistens gleich
einen Halter für den Kolben, ein kleines Schwämmchen dabei und man kann die
Leistung des Lötkolbens regeln.

Ein Beispiel für einen ausreichendes Lötgeröt findet Ihr
[hier](http://www.amazon.de/dp/B000UNM0QY/). [Disclaimer] Da ist auch schon gleich eine
Lötlutsche dabei.

Wenn Ihr nicht gleich einen Lötkolben kaufen wollt, könnt Ihr ihn bestimmt in
einem [Hackerspace](http://hackerspaces.org) in Eurer Nähe ausleihen, um den
PBG zu bauen.

TODO: kann wer was zum Zinn schreiben?

TODO: Da man bei den ganzen Sachen hier immer wieder was besorgen muss sollten
wir auch Tips zum besorgen der Werkzeuge und Elemente geben. 

### Auswahl des Multimeters

Das Multimeter ist ein Werkzeug mit dem man verschiedene (deswegen "multi")
elektrische Messgrößen erfassen (d.h. messen, deswegen
["meter"](http://de.wikipedia.org/wiki/Meter)) kann.

Für den Pentabug reicht ein ganz billiges aus dem Baumarkt oder halt auch von
[Amazon](http://www.amazon.de/dp/B003IP80SS/).

## Die Bauteile erklärt

Damit Ihr wisst, was Ihr verbaut, folgt hier noch eine kurze Erklärung der einzelnen Bauteile.
Die Erklärung folgt ist nicht unbedingt die Reihenfolge in der Ihr die Bauteile auch später verbaut.

### Die Platine

TODO: Insert hier Bild von PLatine

Die Platine ist das Bauteil, auf der alle anderen Bauteile befestigt werden. Sie hat für jedes benötigte Bauteil vorgefertigte Löcher / Bohrungen, sowie Beschriftungen. Zusätzlich liegen auf ihr unter dem Lack Kupferleitungen zwischen einigen Löchern. Diese Kupferleitungen sind wie Kabel und dienen dazu, später den Strom zu transportieren.

### Die Batterie

TODO: Insert hier Bild von Batterie

Eine Batterie kennt Ihr. 

Sie liefert die benötigte Energie um den Pentabug später zu betreiben. Neben der richtigen Form und Größe gibt es noch weitere wichtige Eigenschaften einer Batterie, die ein Jedi kennen muss: 

Eine Batterie hat immer zwei Pole ("+" und "-"). Im inneren dieser Batterie befinden sich an einem Polende viele Elektronen, die gerne an das andere Polende kommen möchten. Da es im inneren der Batterie aber keinen Durchgang gibt, können die Elektronen nur außen herum, wenn man ihnen eine Möglichkeit dazu gibt. Den Weg, den der Pentabug den Elektronen der Batterie bietet, ist der eigentliche Schaltkreis. Die Elektronen werden also von dem einen Pol der Batterie durch die Schaltung des Pentabug, durch alle Bauteile hindurch, zum anderen Pol der Batterie geleitet. Ein bestimmtes, weiter unten besprochenes, Bauteil verhindert, dass die Elektronen immer durch alle Bauteile fließen, sondern nur dann, wenn der Padawan dies wünscht.

Den Wunsch, wie dolle die Elektronen von einem Pol zum anderen wollen, nennt man *Spannung*. Diese Spannung wird in der Einheit *Volt* (V) angegeben. Auf der Batterie ist aufgedruckt, welche Spannung sie liefert. Die Bauteile stellen eine gewisse Hürde auf um überquert zu werden. Diese Hürde nennt man *Widerstand*. Er wird in der Einheit *Ohm* (Ω) angegeben. Dann wäre da noch der Begriff des *Strom*es. Dieser beschreibt (zumindest ungefähr) wie viele Elektronen an einer gewissen Stelle durchfließen. 

TODO: URI jetzt noch im Zusammenhang erklären und paar Beispiele geben. Vielleicht auch noch eine weniger abstrakte Beschreibung liefern?



## Auf gehts

Jetzt geht es schon ans Zusammenbauen des Pentabugs. Wenn Du kein Bock auf
lesen hast, haben wir dafür auch ein
[Video](http://c3d2.de/news/video-pentabug-bauen.html) vorbereitet, was gut
erklärt, wie der Pentabug gelötet wird.

Während Du jetzt also das Lichtschwert eines jeden Junghackers, also den
Lötkolben an der Steckdose heiß werden lässt, packe die einzelnen Bauteile des
Pentabug vor Dir aus und schaue sie genau an. Am besten sortierst Du sie, damit
Du sie schnell griffbereit hast.

In der PBG-Kiste ist auch ein Zettel, der die nötigsten Informationen
zusammenfasst. Bei einigen Bauteilen muss man aufpassen, in welcher Richtung
man sie einbaut.

Die Bauteile werden in der Reihenfolge verbaut, in der sie auf dem Zettel
stehen. Das macht man deswegen, weil man später an manche Bauteile nicht mehr
so gut rankommt, wenn rundherum schon viele andere Teile verbaut sind.

Das Basis-Bauelement, auf dem alle anderen Teile verbaut werden, ist die
Platine.

### Wie man richtig lötet



[Disclaimer]: Auch andere Läden haben Lötkolben oder andere Werkzeuge / Bauelementeund die von uns angegebenen Links sind keinesfalls die besten oder billigsten Teile. Die Links sollen Euch lediglich in die ungefähre Richtung schicken.
