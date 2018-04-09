## IPK - Bandwidth Measurement
### Jan Vávra (xvavra20)

#### Popis
Cílem aplikace je pomocí vhodné komunikace změřit dostupnou přenosovou rychlost mezi dvěma stanicemi v Internetu.
 
#### Základní informace
##### Spuštění měřáku
```
./ipk-mtrip meter -p port -h vzdaleny_host -s velikost_sondy -t cas
```
kde:
+ port - číslo portu, na kterém je reflektor spuštěn.
+ vzdaleny_host - doménové jméno nebo IP adresa stanice, na které je reflektor spuštěn.
+ velikost_sondy - velikost dat použitých v sondě (v bajtech)
+ cas - čas měření (v sekundách)

##### Spuštění reflektoru
```
./ipk-server reflect -p port  
```
kde:
+ port - číslo portu, na kterém bude reflektor spuštěn

#### Omezení
+ Klient-server aplikace pracuje pouze s IPv4 adresami.
+ UDP je omezeno svou maximální velikostí (65507 B) a minimální (9 + počet číslic v argumenu "cas")
