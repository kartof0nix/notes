

Wywłaszczanie - w dowolnym momencie obsługa przerwania procesu i odebrania mu zasobów (bez kontuzji żeby go wznowić)

#### Algorytm z wywłaszczeniem

Gdy potrzebna jest interakcja czy multimedia potrzebne są interakcje:
	Częstość planisty musi być mocno dopasowana do potrzeb

Klasyczny unix:
	18x na sekundę

W dzisiejszym świecie 
10k razy na sekundę

Istitnie większy procent każdej sekundy jest zajmomwany przez działania czysto administracyjne (logistyka planisty i innej biurokracji)

(w naszych komputerach chodzi o *rezponsywność* i *synchronizacjię* )
#### Algorytm bez wywłaszczenia
Może tylko odebrać sterowanie w ramach funckji wewnętrznej systemowej.

Gdy proces nie wywoła takiego wywołania się w nieskończoność zapętlić mogę.

ALE są bardziej wydajne.

Linux jest gorszą bazą do multimediów niż windows i macos też z tą myślą.

### Metody konstrukcji serwerów
- Serwery wielowątkowe.
Wewnątrz wątku jesteśmy sekwencyjni co jest bardzo łatwe.

- Procesy jednowątkowe

- Automaty skończone
Wywołania asynchroniczne I/O:
Rozpocznij zapisywanie / odczyt :
Kończymy instrukcję nawet jeśli nie skończyliśmy wczytywania i dostajemy info jak się uda: **NIEBLUKUJĄCE WYWOŁANIA SYSTEMOWE**

#### Metody wielowątkowości

##### Wątki planowane przez proces
Proces ma planistę który sam przełącza wątki. OS nie widzi wątków tylko 

Plusy:
+ Proces może lepiej szeregować
+ Nie spowalniamy jądra naszymi wątkami

Minusy:
- Gdy dowolny wątek robi I/O PROCES - WSZYSTKIE WĄTKI są wstrzymane

##### Wątki planowane przez jądro
Proces zgłasza wątki do osobnego planisty jądra. 



##### Rozwiązanie hybrydowe
Programista robi dużą ilość wątków poziomu użytkownika a biblioteka grupuje je w mniejszą ilość  'grup' wątków.

##### Przykład : Solaris

Proces 
Wątki poziomu użytkownika

LWP - punkty odwzorywania 

n:m
### Procesy obciążające procesor i we/wy


Zorientowane na we/wy - większoszć życia czekają LUB są zawieszone na potokach.

Procesy zajmujące procesor - niewielka działka 


## Cechy dobrego szeregowania

### ogólne
- sprawiedliwość = równą część czasu CPU
- Zgodność z polityką
- wyrównywanie - zbliżona zajętość wszystkich części systemu

### Systemy wsadowe
- Przepustowość - (zadania w czasie)
- Minimializacja czasu procesu w systemie
- Wykorzystanie procesora = max

(najlepiej bez wywłaszczania)
### Systemy interaktywne
Czas odpowiedzi

### Systemy czasu rzeczywistego
Miękki - nie ma równego przedziału czasu
Silne - każde przerwanie co równy przedział czasu

qnx - unix systemu czasu rzeczywistego

