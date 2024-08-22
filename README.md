# Nanogame

Da einige meiner Freunde interesse am Programmieren mit Arduino zeigten, habe ich vor einiger Zeit einen kleinen Workshop organisiert und für diesen Zweck eine eigene Platine als Plattform für diverse Module konstruiert.

Diese Hardware-Plattform bietet Platz für:

1x Arduino Nano
1x DFPlayer Mini (zur Audiowiedergabe von microSD)
1x PCD8544 LC-Display
1x TP4056 Li-Ion Laderegler
1x Step-Up Wandler (5V für Arduino Nano)

Als Grundlage sind auf der Platine 11 Tasten vorhanden, ein microSD Kartenslot sowie die benötigte Außenbeschaltung.
Um IOs zu sparen, werden die Tasten über Schieberegister abgefragt, so werden nur 3 IOs benötigt.

Alle noch freien IOs habe ich auf einen Pin Header auf der Rückseite gelegt.

Um das ganze Testen zu können, habe ich Snake, bzw. ein Snake ähnliches Spiel, implementiert. (Siehe Demo Video und Code unter Beispiele)


Hier die fehlerbehaftete erste Variante der Leiterplatte die ich noch zu Hause hergestellt habe:<br />


Hier nun die fertige Variante vom professionellen Leiterplattenfertiger:<br />
