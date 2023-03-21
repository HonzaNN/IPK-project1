# IPK-project1

## Autor: Jan Novák <xnovak3i@stud.fit.vutbr.cz>

## Popis projektu

Tento projekt je implementací klienta pro vzdálený kalkulační server. Klient se připojí na server a posílá mu požadavky na výpočet. Server vypočítá požadovaný výsledek a odešle jej zpět klientovi. Klient výsledek vypíše na standardní výstup. V případě chyby se vypíše chybová hláška na standardní chybový výstup.

## Popis implementace

Program je implementován v jazyce C a využívá knihovny pro síťovou komunikaci. Očekavá na vstupu tyto parametry v daném pořadí:
- `-h <server adress>` - IP adresa serveru nebo doménové jméno
- `-p <port>` - číslo portu
- `-m <mode>` - režim komunikace (tcp nebo udp)
Po spustení se ověří, že všechny paratemry byly zadány ve správném formátu. V případě, že né se program ukončí.
Následně se program pokusí připojit na server. Pokud se to nepodaří, program se ukončí s hláškou o chybě. Potom program naleze IP adresu cílového serveru a inicializuje strukturu  `server_address`.
V tomto bodě se zjistí zda je požadována TCP nebo UDP komunikace.

### TCP komunikace

V případě TCP komunikace se program pokusí vytvořit socket a připojit se na server. Pokud se to nepodaří, program se ukončí s hláškou o chybě.
Následně se program připraví na příjem odpovědí od serveru. V případě, že se na vstupu objeví příkaz `HELLO`, program odešle serveru příkaz `HELLO` a čeká na odpověď. Pokud se na vstupu objeví příkaz `BYE`, program odešle serveru příkaz `BYE` a ukončí se. Pokud se na vstupu objeví příkaz `SOLVE`, program odešle serveru příkaz `SOLVE` a čeká na odpověď.

### UDP komunikace

V případě UDP komunikace se program pokusí vytvořit socket a připojit se na server. Pokud se to nepodaří, program se ukončí s hláškou o chybě.
Následně uživatel zadá na `stdin` požadovaný výpočet. Program odešle serveru požadavek na výpočet a čeká na odpověď. Pokud se na vstupu objeví příkaz `(<operace> <číslo> <číslo>)`, vrátí se výsledek výpočtu. Pokud se na vstupu objeví jiný příkaz, program vypíše na `stderr` chybovou hlášku.