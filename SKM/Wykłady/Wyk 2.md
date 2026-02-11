
### IPv6

#### Budowa adresów IPv6

SLA - Site level aggregation
NLA - next level aggregation (24 bity na nr klienta)
res - reserved - gdzie dokleic?
TLA - top level aggregation (13 bitów operatora)

#### Stateless address autoconf

Skąd znać adres routera? Adres multicastowy routera.



1. Link-Local

2. Zapytanie do routera o prefixy




##### Prefiksy
Numer subnetów dla local
Do prefiks global agregatable

## ICMPv4

W pakiecie jest:

##### Status zapytania
Type - *kategoria* statusu (tak jak pierwsza cyfra HTML status)
Code - *konkretny* status (tak jak kolejne dwie cyfry)

Redirect był wykorzystywany przez hackerów xD

Router co jakieś 20 minut robi:
Type = 9 = *router advetisment*

Komputer przy podłączeniu wysyła
Type = 10 *router solicitation*


Type = 13, 14, 15, 16 - timestamp request, reply

### Protokoły RARP, BOOTP, DHCP
	Jak wygenerować nasz adres IP?

RARP - reverse arp xD

##### RARP :
- kod `2` - zapytanie
- - kod `3` - odpowiedź
Wysyła swój adres MAC, dostaje odpowiedź jakiego adresu IP ma używać

##### BOOTHP
Lepsze RARP - komputer dostaje wszystkie potrzebne mu parametry.

Możliwe przydzielanie dynamiczne 

Domyślnie adresy były przydzielane na zawsze, więc nie było możliwości odzyskiwania adresu.

##### DHCP
Lepsze BOOTHP - protokół przydziela adres na konkretny czas
Jeśli komputer chce korzystać dłużej można przedłużyć dzierżawę.

###### Działanie

Broadcast - jakim adresem mam się posługiwać
Serwer swraca odpowiedni adres

Klient wysyła zapytanie z tym co chciałby uzyskać, dostaje odpowiedź.

Przekaźnik - odbiera broadcast, przesyła unicast do router.




