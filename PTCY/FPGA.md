
#### Standardowe układy scalone
(Small/Middle/Large/Very Large) Scale Integration
SSI/MSI - układy asycnchroniczne / proste bloki funkcjonalne

LSI/VLSI - zawierają większe pamięci, zestawy bloków funkcjonalnych, itd.

#### Układy wyspecjalizowane
PLD, GA, SC, FC

### Układy programowalne
#### SPLD - Simple Programmable Logic Device

#### CPLD - Complex Programmable Logic Device

#### Układy FPGA


### PLE - Programmable Logic Element
aka. Look Up Table
Wpisujemy konkretne wartości i adresujemy odpowiednie miejsca i wyczytujemy.
Mają dwie matryce - jedna matryca AND i matrycę OR...

Układy typu PLE, charakteryzują się tym że:
* Matryca AND stała
* Matryca OR programowalna
Co to oznacza? Dla n wejść będziemy mieli 2^n iloczynów.
### PAL

Układy typu PAL, charakteryzują się tym że:
* Matryca AND programowalna
* Matryca OR stała
Np. dla każdej sumy możemy podpiąc dwa iloczyny, które możemy zaprogramować

### PLA - Programmable Logic Array
Układy typu PAL, charakteryzują się tym że:
* Matryca AND programowalna
* Matryca OR programowalna
Wydawałoby się że jest to best of dwóch poprzednich, cn?

### CPLD
Składają się z środkowej 'krosownicy' - czyli elementu połączeniowego, czyli matryca połączeń.
Poszczególne elementy składają się z bloków logicznych i układów wejścia wyjścia.
#### FPGA
bloki leżą wewnątrz obszaru programowalnego. Bloczek może się łączyć z poziomą lub pionową szyną danych