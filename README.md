# Nanogame

Da einige meiner Freunde interesse am Programmieren mit Arduino zeigten, habe ich vor einiger Zeit einen kleinen Workshop organisiert und für diesen Zweck eine eigene Platine als Plattform für diverse Module konstruiert.<br />

Diese Hardware-Plattform bietet Platz für:<br />

1x Arduino Nano<br />
1x DFPlayer Mini (zur Audiowiedergabe von microSD)<br />
1x PCD8544 LC-Display<br />
1x TP4056 Li-Ion Laderegler<br />
1x Step-Up Wandler (5V für Arduino Nano)<br />

Als Grundlage sind auf der Platine 11 Tasten vorhanden, ein microSD Kartenslot sowie die benötigte Außenbeschaltung.<br />
Um IOs zu sparen, werden die Tasten über Schieberegister abgefragt, so werden nur 3 IOs benötigt.<br />

Alle noch freien IOs habe ich auf einen Pin Header auf der Rückseite gelegt.<br />

Um das ganze Testen zu können, habe ich Snake, bzw. ein Snake ähnliches Spiel, implementiert. (Siehe Demo Video und Code unter Beispiele)<br />


Hier die fehlerbehaftete erste Variante der Leiterplatte die ich noch zu Hause hergestellt habe:<br />

![Prototyp](https://github.com/kyouma-mad-scientisto/Nanogame/blob/master/protoFront.jpg)
![Prototyp](https://github.com/kyouma-mad-scientisto/Nanogame/blob/master/protoBack.jpg)

Hier nun die fertige Variante vom professionellen Leiterplattenfertiger:<br />

![Prototyp](https://github.com/kyouma-mad-scientisto/Nanogame/blob/master/finalFront.jpg)
![Prototyp](https://github.com/kyouma-mad-scientisto/Nanogame/blob/master/finalBack.jpg)
