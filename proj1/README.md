## IPK - Klient-server pro získání informace o uživatelích
### Autor: Jan Vávra
#### Popis varianty:
Vašim úkolem je:
1. Seznamte se s kostrami kódů pro programování klientských a serverových síťových aplikací (přednáška třetí) za použití BSD socketů. Navrhněte vlastní aplikační protokol realizující přenos informací o uživatelích na straně serveru a relevantní informace k projektu uveďte v dokumentaci. (8 bodů)
2. Naprogramujte jak klientskou, tak serverovou aplikaci v C/C++ realizující zprostředkování informace o uživatelích na serveru. (12 bodů)

##### KONVENCE ODEVZDÁVANÉHO ZIP ARCHIVU xlogin00.zip

- dokumentace.pdf - výstupy zadání 1.
- readme - základní informace, případná omezení projektu
- Makefile
- *.c, *.cpp, *.cc, *.h - zdrojové a hlavičkové soubory výstupů zadání 2.

##### Ad 1.
Navrhěte vlastní aplikační protokol, kterými poté spolu budou komunikovat klient a server z bodu 2. tohoto zadání.
Tento protokol bude sloužit pro přenos informací o uživatelích na serveru. 
Informace o uživatelích bude server získávat ze souboru /etc/passwd. 

V dobré dokumentaci se očekává následující: titulní strana, obsah, logické strukturování textu, 
výcuc relevantních informací z nastudované literatury, popis aplikačního protokolu vhodnou formou, 
popis zajímavějších pasáží implementace, demonstrace činnosti implementovaných aplikací, normovaná bibliografie.

##### Ad 2.
Spuštění klienta předpokládá provedení pouze jedné operace. Komunikace mezi serverem 
a klientem bude spolehlivá. Server by měl umět obsloužit více požadavků.

Konvence jména klientské aplikace a jejích povinných vstupních parametrů:

```
./ipk-client -h host -p port [-n|-f|-l] login
```

- host (IP adresa nebo fully-qualified DNS name) identifikace serveru jakožto koncového bodu komunikace klienta;
- port (číslo) cílové číslo portu;
- -n značí, že bude vráceno plné jméno uživatele včetně případných dalších informací pro uvedený login (User ID Info);
- -f značí, že bude vrácena informace o domácím adresáři uživatele pro uvedený login (Home directory);
- -l značí, že bude vrácen seznam všech uživatelů, tento bude vypsán tak, že každé uživatelské jméno bude na zvláštním řádku; v tomto případě je login nepovinný. Je-li však uveden bude použit jako prefix pro výběr uživatelů.
- login určuje přihlašovací jméno uživatele pro výše uvedené operace.
