# Solutions of project 2

## SPI

Die SPI-Schnittstelle (Serial Peripheral Interface) ist ein synchrones seriellen Kommunikationsprotokoll, das hauptsächlich zur Verbindung von Mikrocontrollern mit verschiedenen Peripheriegeräten wie Sensoren, Speicherchips und weiteren integrierten Schaltungen verwendet wird.

### Wesentliche Elemente der SPI-Schnittstelle:

**1. Master und Slave(s):** SPI arbeitet in einer Master-Slave-Konfiguration, wobei der Master das MCU ist, das die Kommunikation steuert und initiiert, während die Slaves die Geräte sind, die mit dem Master kommunizieren.

**2.Vier Leitungen:** 

- MISO (Master In Slave Out): Über diese Leitung senden die Slave-Geräte Daten an den Master.

- MOSI (Master Out Slave In): Über diese Leitung sendet der Master Daten an die Slaves.

- SCK (Serial Clock): Diese Leitung wird **vom Master** gesteuert und liefert den Takt, der die Datenübertragung synchronisiert.

- CS (Chip Select): Eine separate Leitung für jeden Slave, gesteuert vom Master, um einen spezifischen Slave für die Kommunikation zu aktivieren.

### Funktionsweise der Datenübertragung:

1. Der Master aktiviert den Slave, mit dem kommuniziert werden soll, indem er dessen CS-Line auf einen niedrigen Pegel setzt.

2. Der Master generiert ein Takt-Signal auf der SCK-Line. Die Flanken dieses Taktsignals bestimmen die Zeitpunkte, zu denen die Daten übertragen werden.

3. Daten werden gleichzeitig in beide Richtungen übertragen (Vollduplex):

- Vom Master zum Slave über die MOSI-Line.
- Vom Slave zum Master über die MISO-Line.
- Die Datenübertragung erfolgt bitweise und ist durch das Takt-Signal synchronisiert.

4. Nach der Übertragung setzt der Master die CS-Line des Slaves wieder auf einen hohen Pegel, um das Ende der Kommunikation zu signalisieren.

## I2C

Die I²C-Schnittstelle ist ein Kommunikationsprotokoll, das zur Verbindung von MCU mit verschiedenen Peripheriegeräten über kurze Distanzen eingesetzt wird.

### Wesentliche Elemente der I²C-Schnittstelle:

**1. Master und Slave:** I²C ermöglicht die Kommunikation zwischen einem oder mehreren Master-Geräten und einem oder mehreren Slave-Geräten.

**2. Zwei Leitungen:**

- SDA (Serial Data Line): Über diese Leitung werden Daten zwischen den Geräten übertragen.
- SCL (Serial Clock Line): Über diese Leitung wird das Takt-Signal übertragen, das die Datenübertragung steuert.

**3.Adressierung:** Jedes Slave-Gerät auf dem Bus hat eine einzigartige Adresse, die es dem Master ermöglicht, spezifische Geräte für die Kommunikation anzusprechen.

**4. Open-Drain Konfiguration der Leitungen:** Sowohl SDA als auch SCL sind Open-Drain-Leitungen. Das bedeutet, dass mehrere Geräte die Leitung gemeinsam nutzen und auf einen niedrigen Pegel ziehen können, aber keines der Geräte die Leitung aktiv auf einen hohen Pegel setzt. Stattdessen wird ein **Pull-up-Widerstand** verwendet, um die Leitung auf einen hohen Pegel zu ziehen, wenn kein Gerät sie auf einen niedrigen Pegel zieht.

### Funktionsweise der Datenübertragung:

1. Start-Kondition: Jede Kommunikation beginnt damit, dass der Master eine Start-Kondition erzeugt, indem er SDA von hoch auf niedrig zieht, während SCL hoch bleibt.
2. Adressierung: Nach der Start-Kondition sendet der Master die Adresse des Slave-Geräts, gefolgt von einem W/R Bit.
3. Datenübertragung: Daten werden Byte für Byte über die SDA-Leitung übertragen, wobei jedes Byte von einem ACK-Bit gefolgt wird, das vom Empfänger gesendet wird.
4. Stop-Kondition: Die Kommunikation wird beendet, indem der Master eine Stop-Kondition erzeugt, bei der SDA von niedrig auf hoch gezogen wird, während SCL hoch bleibt.

### Warum Open-drain

Bei einer Open-Drain-Konfiguration kann jeder an die Leitung angeschlossene Treiber die Leitung auf einen niedrigen Pegel ziehen, aber keiner kann sie aktiv auf einen hohen Pegel setzen. Stattdessen wird ein Pull-up-Widerstand verwendet, um die Leitung auf einen hohen Pegel zu ziehen, wenn kein Gerät sie aktiv auf niedrig zieht. Dies ermöglicht es mehreren Geräten, die Leitung gemeinsam zu nutzen und verhindert **Kurzschlüsse**, falls zwei Geräte gleichzeitig versuchen, die Leitung auf unterschiedliche Pegel zu setzen.

## One-Wire

Die One-Wire-Schnittstelle ist ein Kommunikationsbus-System, das vor allem für die Kommunikation mit kleineren Geräten wie Sensoren oder Identifikationschips verwendet wird. Der Hauptvorteil von One-Wire liegt in seiner Einfachheit, da nur ein einzelner Draht für die Datenübertragung benötigt wird.

### Wesentliche Elemente der One-Wire-Schnittstelle:

1. Einzelner Datenleitungsdraht: One-Wire benötigt nur eine Leitung für die Datenübertragung, zusätzlich zu einer Erdung (Ground).
2. Stromversorgung über Datenleitung: keine separate Stromversorgung benötigt wird.
3. Master-Slave-Konfiguration: Das System ist in der Regel so konfiguriert, dass ein Master (z.B. ein MCU) mit mehreren Slaves (z.B. Sensoren) kommuniziert.
4. Unique ID: Jedes One-Wire-Gerät hat eine eindeutige Identifikationsnummer (ID), die es dem Master ermöglicht, einzelne Geräte auf dem Bus anzusprechen.

### Funktionsweise der Datenübertragung:

1. Initialisierung: Jede Kommunikation beginnt mit einem Reset vom Master, gefolgt von einer Präsenz-Antwort von jedem Slave-Gerät auf dem Bus.
2. Adressierung: Der Master sendet einen Befehl mit der ID des spezifischen Slaves, mit dem kommuniziert werden soll. Bei Bussen mit nur einem Slave kann dieser Schritt entfallen.
3. Datenübertragung: Die eigentliche Datenübertragung erfolgt bitweise. Der Master oder der Slave kann das Datenbit durch Ziehen der Leitung auf niedriges Potenzial für eine bestimmte Dauer steuern. **Die Zeitdauer des Signals** bestimmt, ob das Bit als '0' oder '1' interpretiert wird.
4. Zeitsynchronisation: Da die Kommunikation von den genauen Zeitintervallen abhängt, ist eine präzise Zeitsynchronisation zwischen Master und Slave wichtig.

(All of this information can be found in wikipedia or corresponding datasheet)