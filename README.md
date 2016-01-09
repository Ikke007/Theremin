## Tangible Theremin

Projekt zum Thema "*tangible sound control*"

---

Bestandteile:

1. Ein **Arduino Sketch** zum Auslesen zweier [FSR]-Sensoren an einem Arduino Due: [fsr_reading](fsr_reading).
Die gemessenen Werte werden über eine Serielle Verbindung (USB) an einen Computer gesendet.

2. Eine **Qt Anwendung** zur Verarbeitung der vom Arduino gesendeten Daten und Generierung von [MIDI] Signalen.

---

Audio-Video-Programmierung  
Hochschule für Angewandte Wissenschaften Hamburg  
Wintersemester 2015/16

[FSR]: https://de.wikipedia.org/wiki/Force_Sensing_Resistor
[MIDI]: https://de.wikipedia.org/wiki/Musical_Instrument_Digital_Interface
