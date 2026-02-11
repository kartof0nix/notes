# Sprawozdanie SKM Lab 1
Karol Bonat

## Ćwiczenie 1 - Protokół ARP

### Kroki

1. Za pomocą komend `arp -n` i `arp - d` tablica arp została wyczyszczona.
2. Uruchomić program Wireshark


### Wyniki

Zanim można przechwycić odpowiedni pakiet istotne jest zapisanie adresu MAC komputera:

```
➜  ~ ip add
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host noprefixroute 
       valid_lft forever preferred_lft forever
2: enr: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN group default qlen 1000
    link/ether 9c:5c:8e:bc:47:4b brd ff:ff:ff:ff:ff:ff
    altname enp5s0
3: eni: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether 9c:5c:8e:bb:83:fb brd ff:ff:ff:ff:ff:ff
    altname enp0s31f6
    inet 192.168.96.211/24 brd 192.168.96.255 scope global eni
[...]
```

Stąd widzimy nasz adres MAX `9c:5c:8e:bc:47:4b` oraz IP `192.168.96.211`

Następnie przy wysyłaniu zapytania ping `ping 192.168.96.222` szukamy zapytania arp z/do naszego adresu MAC.

### Analiza Pakietu zapytania

Znaleziony pakiet zapytania:

```
0000   ff ff ff ff ff ff 9c 5c 8e bb 83 fb 08 06 00 01
0010   08 00 06 04 00 01 9c 5c 8e bb 83 fb c0 a8 60 d3
0020   00 00 00 00 00 00 c0 a8 60 d2
```


W paczece widzimy:

#### Warstwa łącza

1. Cel paczki : Adres MAC broadcast : `ff ff ff ff ff ff`
2. Źródło paczki : Adres MAC nadawcy : `9c 5c 8e bb 83 fb`
3. Nagłówek protokołu - `08 06` - oznaczenie paczki ARP

### Warstwa ARP

4. Hardware Type - `00 01` - Ethernet
5. Protocool Type - `08 00` - IP
6. HLen - `06` = 6 (długość adresu MAC)
7. PLen - `04` = 04 (długość adresu IP)
8. opcode - `00 01` - zapytanie
9. Adres MAC nadawcy : `9c 5c 8e bb 83 fb`
10. Adres IP Nadawcy : `c0 a8 60 d3` = 192.168.96.211
11. Adres MAC Odbiorcy : `00 00 00 00 00 00`
12. Adres IP odbiorcy: `c0 a8 60 d2` = 192.168.96.210


Jedynie punkty 4-12 należą do protokołu ARP, punkty 1-3 należą do warstwy łącza.


### Analiza pakietu odpowiedzi

Znaleziony pakiet odpowiedzi:

```
0000   9c 5c 8e bb 83 fb 9c 5c 8e bb 84 12 08 06 00 01
0010   08 00 06 04 00 02 9c 5c 8e bb 84 12 c0 a8 60 d2
0020   9c 5c 8e bb 83 fb c0 a8 60 d3 00 00 00 00 00 00
0030   00 00 00 00 00 00 00 00 00 00 00 00
```

W paczece widzimy:


#### Warstwa łącza

1. Cel paczki: Adres MAC Nadawcy zapytania  :  `9c 5c 8e bb 83 fb`
2. Źródło paczki :  Adres MAC odbiorcy zapytania : `9c 5c 8e bb 84 12`
3. Nagłówek protokołu - `08 06` - oznaczenie paczki ARP

#### Warstwa ARP

4. Hardware Type - `00 01` - Ethernet
5. Protocool Type - `08 00` - IP
6. HLen - `06` = 6 (długość adresu MAC)
7. PLen - `04` = 04 (długość adresu IP)
8. opcode - `00 02` - odpowiedź
9. Adres MAC nadawcy : `9c 5c 8e bb 84 fb`
10. Adres IP Nadawcy : `c0 a8 60 d2` = 192.168.96.210
11. Adres MAC Odbiorcy : `9c 5c 8e bb 83 fb`
12. Adres IP odbiorcy: `c0 a8 60 d3` = 192.168.96.211

Widzimy jeszcze dodatkową ramkę warstwy łącza po naszym pakiecie.


Tak samo, punkty 4-12 należą do protokołu ARP, punkty 1-3 należą do warstwy łącza.

### Wnioski

Nasz host znajduje adres MAC odbiorcy komendy ping za pomocą zapytania arp do wszystkich komputerów w sieci.

Oznaczenie
* A = Komputer wysyłający zapytanie arp
* B = komputer wysyłający odpowiedź arp 

W nagłówku warstwy łącza, w zapytaniu w polu nadawcy jest adres MAC komputera A, a wpolu odbiorcy adres MAC broadcast.

W odpowiedzi, w warstwie łącza nadawcą jewst komputer B, a odpbiorcą komputer A.

W zapytaniu arp, adres MAC i IP nadawcy należą do komputera A, a adres IP odbiorcy to adres komputera B.

W odpowiedzi arp, adres MAC i IP nadawcy należą do komputera B, a adres MAC i IP odbiorcy należą do komputera A (zgodnie z polami nadawca / odbiorca warstwy łącza).

## 5. Powtarzanie punktów 2-4

Powtarzając punkty 2-4 nie widać pakietu ARP pochodzącego z naszego komputera.
Stąd wniosek, że komputer zapamiętał adres MAC odbiorcy i nie musi ponownie go szukać.

## 6. Zapytanie do komputera spoza sieci

Po wysłaniu `ping 8.8.8.8 ` można znaleźć zapytania ARP:

### Analiza zapytania:

```
0000   ff ff ff ff ff ff 9c 5c 8e bb 83 fb 08 06 00 01
0010   08 00 06 04 00 01 9c 5c 8e bb 83 fb c0 a8 60 d3
0020   00 00 00 00 00 00 c0 a8 60 fe
```

Jak poprzednio, pierwsze 14 bajtów należy do warstwy łącza.
Widzimy że wysyłamy zapytanie do adresu broadcast.
Jednakże, ptamy się o adres IP `c0 a8 60 fe` - czyli `192.168.96.254`
Widzimy więc że wysyłamy zapytanie do routera.
Inne pola są takie same jak poprzednio.

### Analiza odpowiedzi

```
0000   9c 5c 8e bb 83 fb 00 22 64 9f 20 ac 08 06 00 01
0010   08 00 06 04 00 02 00 22 64 9f 20 ac c0 a8 60 fe
0020   9c 5c 8e bb 83 fb c0 a8 60 d3 00 00 00 00 00 00
0030   00 00 00 00 00 00 00 00 91 ef 27 7a
```

#### Warstwa łącza:
* Odbiorca - MAC naszego komputera
* Nadawca - MAC routera `00 22 64 9f 20 ac`
* Protokół - ARP `08 06`

#### Warstwa ARP
Jak w poprzednim ćwiczeniu, pola MAC i IP odbiorcy to adresy naszego komputera.
Tym razem jednak, w polach nadawcy widzimy adres MAC i IP routera.


Nasz komputer znajduje adres MAC routera - w zapytaniu ptamy się o adres ip `192.168.96.254`


### 7. Zapytanie do nieistniejącego komputera

Po wpisaniu komendy `ping 192.168.96.45`, nie otrzymujemy odpowiedzi:

```
➜  ~ ping 192.168.96.45 
PING 192.168.96.45 (192.168.96.45) 56(84) bytes of data.
^C
--- 192.168.96.45 ping statistics ---
2 packets transmitted, 0 received, 100% packet loss, time 1004ms

```

Nasz komputer wysyła wiele zapytań ARP nie otrzymując żadnej odpowiedzi:

```7	8.692194008	ASUSTekC_bb:83:fb	Broadcast	ARP	42	Who has 192.168.96.45? Tell 192.168.96.211```

```8	9.715389277	ASUSTekC_bb:83:fb	Broadcast	ARP	42	Who has 192.168.96.45? Tell 192.168.96.211```

```10	10.739376519	ASUSTekC_bb:83:fb	Broadcast	ARP	42	Who has 192.168.96.45? Tell 192.168.96.211```


Gdy dla niestniejącego adresu IP spoza sieci:
```➜  ~ ping 8.8.123.45
PING 8.8.123.45 (8.8.123.45) 56(84) bytes of data.
^C
--- 8.8.123.45 ping statistics ---
3 packets transmitted, 0 received, 100% packet loss, time 2053ms
```

Nie wykonujemy żadnych zapytań ARP (poza routerem). Wysyłamy natomiast od razu zapytania ICMP:

```3	2.655538879	192.168.96.211	8.8.123.45	ICMP	98	Echo (ping) request  id=0x73a7, seq=1/256, ttl=64 (no response found!)```

Stąd wniosek, że dla komputera z lokalnej sieci próbujemy znaleźć jego adres MAC zanim wyślemy pakiety ICMP, lecz dla adresu IP spoza sieci nie szukamy jego adresu MAC, zostawiając to któremuś routerowi.

## Ćwiczenie 2 - protokoły IP i ICMP

Analizując prezechwycone w poprzednim zadaniu paczki ICMP:

### Zapytanie

```
       0           4           8           c
0000   9c 5c 8e bb 84 12 9c 5c 8e bb 83 fb 08 00 45 00
0010   00 54 cf 69 40 00 40 01 28 49 c0 a8 60 d3 c0 a8
0020   60 d2 08 00 f2 47 38 a3 00 01 37 64 f6 68 00 00
0030   00 00 df 73 01 00 00 00 00 00 10 11 12 13 14 15
0040   16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25
0050   26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33 34 35
0060   36 37
```


#### Warstwa łącza - bajty 0 - 000d

1. Cel paczki: Adres MAC Nadawcy zapytania  :  `9c 5c 8e bb 84 12`
2. Źródło paczki :  Adres MAC odbiorcy zapytania : `9c 5c 8e bb 83 fb`
3. Nagłówek protokołu - `08 00` - oznaczenie protokołu IP

#### Warstwa IP bajty 000e - 0022

1. 0.5B Wersja protokołu `4` z `45` - wersja 4
2. 0.5B Długość nagłówka `5` z `45` - 20 bajtów
3. 1B Serwis - `00` 
4. 2B Całkowita długość - `00 54`
5. 4B [...] Cztery bajty
6. 1B TTL = `40` = `64`
7. 1B Protocool - `01` = ICMP
8. 2B Header Checksum - `28 49`
9. 4B Adres Nadawcy: `c0 a8 60 d3` = `192.168.96.211`
10. 4B Adres odbiorcy: `c0 a8 60 d2` = `192.168.0.210`

Całkowita długość, zgodnie z informacją w nagłówku, wynosi 20B.
Widzimy adres nadawcy i odbiorcy naszych komputerów.

#### Warstwa ICMP 0023 - .
1. Typ: `08` - ping echo
2. Kod operacji - `00`
3. Suma kontrolna - `96 60`
4. Numer ID sesji PING: `f0 9c`
5. Numer zapytania: `00 01`
6. Timestamp
7. Dane










### Odpowiedź

```
0000   9c 5c 8e bb 83 fb 9c 5c 8e bb 84 12 08 00 45 00
0010   00 54 7f e3 00 00 40 01 b7 cf c0 a8 60 d2 c0 a8
0020   60 d3 00 00 fa 47 38 a3 00 01 37 64 f6 68 00 00
0030   00 00 df 73 01 00 00 00 00 00 10 11 12 13 14 15
0040   16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25
0050   26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33 34 35
0060   36 37
```


#### Warstwa łącza

1. Cel paczki :  Adres MAC odbiorcy zapytania : `9c 5c 8e bb 83 fb`
2. Źródło paczki: Adres MAC Nadawcy zapytania  :  `9c 5c 8e bb 84 12`
3. Nagłówek protokołu - `08 00` - oznaczenie protokołu IP

#### Warstwa IP bajty 000e - 0022

1. 0.5B Wersja protokołu `4` z `45` - wersja 4
2. 0.5B Długość nagłówka `5` z `45` - 20 bajtów
3. 1B Serwis - `00` 
4. 2B Całkowita długość - `00 54`
5. 4B - Identyfikator i flagi - m.inn flaga Don't Fragment
6. 1B TTL = `40` = `64`
7. 1B Protocool - `01` = ICMP
8. 2B Header Checksum - `81 9d`
9. 4B Adres Nadawcy: `c0 a8 60 d2` = `192.168.96.210`
10. 4B Adres odbiorcy: `c0 a8 60 d3` = `192.168.0.211`

Całkowita długość, zgodnie z informacją w nagłówku, wynosi 20B.
Widzimy adres nadawcy i odbiorcy naszych komputerów.

#### Warstwa ICMP 0023 - .
1. Typ: `00` - ping echo
2. Kod operacji - `00`
3. Suma kontrolna - `9e 60`
4. Numer ID sesji PING: `f0 9c`
5. Numer zapytania: `00 01`
6. Timestamp (ten sam co zapytania)
7. Dane (te same co zapytania)


### Wysyłanie pakietu długości 3000 oktetów

Komenda:

```
ping -c 1 -s 3000 192.168.96.210
PING 192.168.96.210 (192.168.96.210) 3000(3028) bytes of data.
3008 bytes from 192.168.96.210: icmp_seq=1 ttl=64 time=0.331 ms

--- 192.168.96.210 ping statistics ---
1 packets transmitted, 1 received, 0% packet loss, time 0ms
```

Wysyłając zapytania widzimy trzy pakiety zapytania oraz trzy pakiety odpowiedzi:

```
1   0.000000000 192.168.96.211  192.168.96.210  IPv4    1514
2   0.000010951 192.168.96.211  192.168.96.210  IPv4    1514 
3   0.000014140 192.168.96.211  192.168.96.210  ICMP    82
4   0.000298363 192.168.96.210  192.168.96.211  IPv4    1514
5   0.000298691 192.168.96.210  192.168.96.211  IPv4    1514
6   0.000298842 192.168.96.210  192.168.96.211  ICMP    82
```

W fragmentowych pakietach:
1. Pola warstwy łącza są te same co dla jednego zapytania
2. Dla warstwy IP:
    1. Wszytkie pakiety mają ten sam Identification
    2. Bajty 6-7 odpowiadające za freagmentację się zmieniają, ustawiając offset oraz czy oczekiwać kolejnych fragmentów.
    3. Cel i źródło się nie zmieniają.

W szczególności, pierwsze dwa pakiety mają w 6 bajcie 3-ci bit ustawiony (More fragments) gdy trzeci nie.

Sytuacja wygląda identycznie dla pakietów odpowiedzi.
Dwa fragmenty mają po 1514 długości, gdy ostatni ma 82, stąd wniosek że limit długości tego pakietu wynosi 1514



## Ćwiczenie 3 - protokół UDP

Używając komendy `nc` wysyłamy pakiet UDP:

```
➜  ~ nc 192.168.96.210 daytime
Mon Oct 20 18:41:49 2025
```

### Analiza zapytania

``` 
       0           4           8           c
0000   9c 5c 8e bb 84 12 9c 5c 8e bb 83 fb 08 00 45 00
0010   00 1d 99 0a 40 00 40 11 5e cf c0 a8 60 d3 c0 a8
0020   60 d2 99 ee 00 0d 00 09 43 11 0a
```

Bajty 0000 - 000d warstwy łącza wyglądają identycznie jak w poprzednich zapytaniach.

#### Warstwa IP bajty 000e - 0022

1. 0.5B Wersja protokołu `4` z `45` - wersja 4
2. 0.5B Długość nagłówka `5` z `45` - 20 bajtów
3. 1B Serwis - `00` 
4. 2B Całkowita długość - `00 1d` = 29
5. 4B - Identyfikator i flagi - m.inn flaga Don't Fragment
6. 1B TTL = `40` = `64`
7. 1B Protocool - `11` = UDP(17)
[...]

#### Warstwa UDP

1. Port źr. : `99 ee` = `39406`
2. Port cel : `00 0d` = `13`
3. Długość : `00 09` = 9
4. Checksum


### Analiza odpowiedzi

```
       0           4           8           c
0000   9c 5c 8e bb 83 fb 9c 5c 8e bb 84 12 08 00 45 00
0010   00 36 3c b5 40 00 40 11 bb 0b c0 a8 60 d2 c0 a8
0020   60 d3 00 0d 99 ee 00 22 08 10 4d 6f 6e 20 4f 63
0030   74 20 32 30 20 31 39 3a 34 35 3a 34 30 20 32 30
0040   32 35 0d 0a
```

Bajty 0000 - 000d warstwy łącza wyglądają identycznie jak w poprzednich zapytaniach.

#### Warstwa IP bajty 000e - 0022

1. 0.5B Wersja protokołu `4` z `45` - wersja 4
2. 0.5B Długość nagłówka `5` z `45` - 20 bajtów
3. 1B Serwis - `00` 
4. 2B Całkowita długość - `00 1d` = 29
5. 4B - Identyfikator i flagi - m.inn flaga Don't Fragment
6. 1B TTL = `40` = `64`
7. 1B Protocool - `11` = UDP(17)
[...]

#### Warstwa UDP

1. Port źr : `00 0d` = `13`
2. Port cel. : `99 ee` = `39406`
3. Długość : `00 22` = 34
4. Checksum

Oraz dane będące tekstową reprezentacją czasu.

Całe połączenie składa się więc tylko z dwóch pakietów, a serwer nie ma gwarancji że klient otrzymał dane.
Jedyne co UDP dodaje do protokołu IP to wsparcie portów - aplikacji.


## Ćwiczenie 4 - protokół TCP

Oznaczenie:
* ISN - Initial Sequence Number - pierwszy numer sekwencji
* cli / srv - klient / serwer

Przechwycone paczki to:

```
1	192.168.96.211	192.168.96.210	TCP	74	42248 → 13 [SYN] Seq=0 Win=64240 Len=0 MSS=1460 
16	192.168.96.210	192.168.96.211	TCP	74	13 → 42248 [SYN, ACK] Seq=0 Ack=1 Win=65160 Len=0 MSS=1460 
2	192.168.96.211	192.168.96.210	TCP	66	42248 → 13 [ACK] Seq=1 Ack=1 Win=64256 
3	192.168.96.210	192.168.96.211	DAYTIME	92	DAYTIME Response
4	192.168.96.210	192.168.96.211	TCP	66	13 → 42248 [FIN, ACK] Seq=27 Ack=1 Win=65280 
5	192.168.96.211	192.168.96.210	TCP	66	42248 → 13 [ACK] Seq=1 Ack=27 Win=64256 
6	192.168.96.211	192.168.96.210	TCP	66	42248 → 13 [FIN, ACK] Seq=1 Ack=28 Win=64256 
7	192.168.96.210	192.168.96.211	TCP	66	13 → 42248 [ACK] Seq=28 Ack=2 Win=65280 
```

Przechwycone pakiety:

### Pakiet 1 Handshake krok 1 A->B
```
       0           4           8           c
0000   9c 5c 8e bb 84 12 9c 5c 8e bb 83 fb 08 00 45 00
0010   00 3c 1a 77 40 00 40 06 dd 4e c0 a8 60 d3 c0 a8
0020   60 d2 a5 08 00 0d ca fd 32 8d 00 00 00 00 a0 02
0030   fa f0 43 25 00 00 02 04 05 b4 04 02 08 0a e7 9b
0040   b1 b7 00 00 00 00 01 03 03 07
```

#### Warstwa łącza bajty 00 - 0d

1. Adres MAC odbiorcy : `9c 5c 8e bb 84 12`
2. Adres MAC nadawcy :  `9c 5c 8e bb 83 fb`
3. Nagłówek protokołu - `08 00` - oznaczenie protokołu IP

#### Warstwa IP bajty 0e - 21

1. Wersja protokołu `4` z `45` - wersja 4
2. Długość nagłówka `5` z `45` - 20 bajtów
3. Serwis - `00` 
4. Całkowita długość - `00 3c` = 60
5. Identyfikator `1a 77`
6. Fragmentacja `40 00` - Don't fragment
6. TTL = `40` = `64`
7. Protocool - `06` = TCP(6)
8. Checksum - `dd 4e`
9. Adres nadawcy `c0 a8 60 d3`
10. Adres odbiorcy `c0 a8 60 d2`

#### Warstwa TCP 22 - .

1. Port źrófłowy `a5 08` = 42248
2. Port docelowy `00 0d` = 13
3. Sequence number `ca fd 32 8d` - cli ISN 
4. Ack `00 00 00 00`
5. Długość nagłówka `a0 02` - `1010` - 40
6. **Flagi** `a0 02` - `0000 0000 0010` - SYN
7. Okno `fa f0` - (możliwe że przeskalowane w opcjach)
8. Checksum `43 25`
9. Wskażnik priorytetu `00 00`
10. Opcje : [...], NOP 

### Pakiet 2 Handshake krok 2 B->A

```
       0           4           8           c
0000   9c 5c 8e bb 83 fb 9c 5c 8e bb 84 12 08 00 45 00
0010   00 3c 00 00 40 00 40 06 f7 c5 c0 a8 60 d2 c0 a8
0020   60 d3 00 0d a5 08 d6 3c 37 91 ca fd 32 8e a0 12
0030   fe 88 53 d4 00 00 02 04 05 b4 04 02 08 0a 91 7d
0040   d7 5b e7 9b b1 b7 01 03 03 07
```

#### Warstwa łącza bajty 00 - 0d

2. Adres MAC odbiorcy :  `9c 5c 8e bb 83 fb`
1. Adres MAC nadawcy : `9c 5c 8e bb 84 12`
3. Nagłówek protokołu - `08 00` - oznaczenie protokołu IP

#### Warstwa IP bajty 0e - 21

1. Wersja protokołu `4` z `45` - wersja 4
2. Długość nagłówka `5` z `45` - 20 bajtów
3. Serwis - `00` 
4. Całkowita długość - `00 3c` = 60
5. **Identyfikator** `00 00`
6. Fragmentacja `40 00` - Don't fragment
6. TTL = `40` = `64`
7. Protocool - `06` = TCP(6)
8. Checksum - `f7 c5`
9. Adres nadawcy `c0 a8 60 d2`
10. Adres odbiorcy `c0 a8 60 d3`

#### Warstwa TCP 22 - .

1. Port źródłowy `00 0d` - 13 (ten sam co doc poprzedniej)
2. Port docelowy `a5 08` - 42248 (ten sam co źr poprzedniej)
3. Sequence `d6 3c 37 91` - srv ISN - niezależny od paczki A->B
4. Ack `ca fd 32 8e` = cli ISN + 1
5. Header length `a0 12` - `1010` - 40
6. **Flagi** `a0 12` `0000 0001 0010` - ACK, SYN
7. Okno `fe 88` - (możliwe że przeskalowane w opcjach)
8. Checksum `53 d4`
9. Priorytet `00 00`
10. Opcje [...] NOP


### Pakiet 3 - Handshake krok 3 - A->B

```
       0           4           8           c
0000   9c 5c 8e bb 84 12 9c 5c 8e bb 83 fb 08 00 45 00
0010   00 34 1a 78 40 00 40 06 dd 55 c0 a8 60 d3 c0 a8
0020   60 d2 a5 08 00 0d ca fd 32 8e d6 3c 37 92 80 10
0030   01 f6 43 1d 00 00 01 01 08 0a e7 9b b1 b7 91 7d
0040   d7 5b
```
#### Warstwa łącza bajty 00 - 0d

1. Adres MAC odbiorcy : `9c 5c 8e bb 84 12`
2. Adres MAC nadawcy :  `9c 5c 8e bb 83 fb`
3. Nagłówek protokołu - `08 00` - oznaczenie protokołu IP

#### Warstwa IP bajty 0e - 21

1. Wersja protokołu `4` z `45` - wersja 4
2. Długość nagłówka `5` z `45` - 20 bajtów
3. Serwis - `00` 
4. Całkowita długość - `00 34` = 52
5. **Identyfikator** `1a 78` - poprzedni + 1
6. Fragmentacja `40 00` - Don't fragment
6. TTL = `40` = `64`
7. Protocool - `06` = TCP(6)
8. Checksum - `dd 55`
9. Adres nadawcy `c0 a8 60 d3`
10. Adres odbiorcy `c0 a8 60 d2`

#### Warstwa TCP 22 - .

1. Port źrófłowy `a5 08` = 42248
2. Port docelowy `00 0d` = 13
3. Sequence number `ca fd 32 8e` - cli ISN + 1
4. Ack `d6 3c 37 92` = srv ISN + 1
5. Długość nagłówka `80 10` - `1000` - 32
6. **Flagi** `80 10` - `0000 0001 0000` - ACK
7. Okno `01 f6` - 502B
8. Checksum `43 1d`
9. Wskażnik priorytetu `00 00`
10. Opcje : [...], NOP 


### Pakiet 4 - Dane B->A
```
       0           4           8           c
0000   9c 5c 8e bb 83 fb 9c 5c 8e bb 84 12 08 00 45 00
0010   00 4e 6c 7b 40 00 40 06 8b 38 c0 a8 60 d2 c0 a8
0020   60 d3 00 0d a5 08 d6 3c 37 92 ca fd 32 8e 80 18
0030   01 fe 60 5f 00 00 01 01 08 0a 91 7d d7 5b e7 9b
0040   b1 b7 4d 6f 6e 20 4f 63 74 20 32 30 20 31 38 3a
0050   34 37 3a 34 35 20 32 30 32 35 0d 0a
```

#### Warstwa łącza bajty 00 - 0d

2. Adres MAC odbiorcy :  `9c 5c 8e bb 83 fb`
1. Adres MAC nadawcy : `9c 5c 8e bb 84 12`
3. Nagłówek protokołu - `08 00` - oznaczenie protokołu IP

#### Warstwa IP bajty 0e - 21

1. Wersja protokołu `4` z `45` - wersja 4
2. Długość nagłówka `5` z `45` - 20 bajtów
3. Serwis - `00` 
4. Całkowita długość - `00 4e` = 78
5. **Identyfikator** `6c 7b` - niezależny
6. Fragmentacja `40 00` - Don't fragment
6. TTL = `40` = `64`
7. Protocool - `06` = TCP(6)
8. Checksum - `8b 38`
9. Adres nadawcy `c0 a8 60 d2`
10. Adres odbiorcy `c0 a8 60 d3`

#### Warstwa TCP 22 - 41

1. Port źródłowy `00 0d` - 13 (ten sam co doc poprzedniej)
2. Port docelowy `a5 08` - 42248 (ten sam co źr poprzedniej)
3. Sequence `d6 3c 37 92` - srv ISN + 1
4. Ack `ca fd 32 8e` = cli ISN + 1 
5. Header length `80 18` - `1000` - 32B
6. **Flagi** `80 18` `0000 0001 1000` - ACK, PUSH
7. Okno `01 fe` - 510B
8. Checksum `60 5f`
9. Priorytet `00 00`
10. Opcje NOP, NOP, TIMESTAMP

#### Tekst 42 - .
Bajty przedstawiają "Mon  Oct   20   18:47:45 2025" zakodowane w ASCII

### Pakiet 5 - Zakończenie - B -> A
```
       0           4           8           c
0000   9c 5c 8e bb 83 fb 9c 5c 8e bb 84 12 08 00 45 00
0010   00 34 6c 7c 40 00 40 06 8b 51 c0 a8 60 d2 c0 a8
0020   60 d3 00 0d a5 08 d6 3c 37 ac ca fd 32 8e 80 11
0030   01 fe 7f 10 00 00 01 01 08 0a 91 7d d7 5b e7 9b
0040   b1 b7
```

#### Warstwa łącza bajty 00 - 0d

2. Adres MAC odbiorcy :  `9c 5c 8e bb 83 fb`
1. Adres MAC nadawcy : `9c 5c 8e bb 84 12`
3. Nagłówek protokołu - `08 00` - oznaczenie protokołu IP

#### Warstwa IP bajty 0e - 21

1. Wersja protokołu `4` z `45` - wersja 4
2. Długość nagłówka `5` z `45` - 20 bajtów
3. Serwis - `00` 
4. Całkowita długość - `00 34` = 52
5. **Identyfikator** `6c 7c` - poprzedni + 1
6. Fragmentacja `40 00` - Don't fragment
6. TTL = `40` = `64`
7. Protocool - `06` = TCP(6)
8. Checksum - `8b 51`
9. Adres nadawcy `c0 a8 60 d2`
10. Adres odbiorcy `c0 a8 60 d3`

#### Warstwa TCP 22 - 41

1. Port źródłowy `00 0d` - 13 (ten sam co doc poprzedniej)
2. Port docelowy `a5 08` - 42248 (ten sam co źr poprzedniej)
3. Sequence `d6 3c 37 ac` - srv ISN + 26
4. Ack `ca fd 32 8e` = cli ISN + 1 
5. Header length `80 11` - `1000` - 32B
6. **Flagi** `80 11` `0000 0001 0001` - ACK, FIN
7. Okno `01 fe` - 510B
8. Checksum `7f 10`
9. Priorytet `00 00`
10. Opcje NOP, NOP, TIMESTAMP

### Pakiety 5-7

Pakiet 5-7 wyglądają bardzo podobnie do zapytania 4, ponieważ następuje czterostronne zakończenie połączenie. Łącznie z zapytaniem 4, następuje po kolei:

1. Serwer wysyła flagę FIN do klienta
2. Klient wysyła flagę ACK w odpowiedzi na pakiet FIN serwera (identyfikacja przez pole ACK)
3. Klient wysyła pakiet FIN do serwera
4. Serwer wysyła pakiet ACK do klienta w odpowiedzi na pakiet FIN klienta  (identyfikacja przez pole ACK)

### Podsumowanie

Widzimy więc w protokole TCP mechanizmy:
* trójstronnego handshake
* jednostronnego przesyłu danych (z możliwością potwierdzenia odebioru danych)
* czterostronnego zakończenia połączenia


Podczas potrójnego uzgodnienia kluczowe są Flagi SYN oraz ACK, gdy podczas zakończenia kluczowe jest flaga FIN.

Co ciekawe, klient potwierdza odbiór przesłanych danych ORAZ odbiór sygnału zakończenia jednym pakietem - wysyła ACK = 27, czyli potwierdza odebranie zawartości do 27 Bajtu.
