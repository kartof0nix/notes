### Pliki kropkowe
`rm -r **`
Gdyby była dopasowana do każdego folderu, to by wyczyściło cały system plików bo by poszło w Górę.

### Zmienne powłoki

w $PATH nie ma obecnego katalogu.

$IFS - wewnętrzny separaor pól - defaultowo jest to " " - może też być jakiś \n czy coś takiego


Żeby zminimalizować szanse kolizji konwencja "./" żeby lokalnie.
Można dodać do PATH ":."

$PS1 - pierwszy znak powłoki

$PS2 - drugi znak powłoki

$TERM  - rodzaj terminala
Definiuje rodzaj sekwencji sterujących - przesuwanie kurssora, itd. - xterm / ansi / vt100

Serwer nie ma jak dowiedzieć się jak wygląda terminal u programisty - QWERTY /   ЙЦУКЕ , rozmiat, ird.

### Wywołania systemowe

fork - sklonowanie procesu : dostaje 0 jeśli jestem sklonowany albo process id jeśli ojcem

exec(path)
FORKUJE proces ojsca i następnie
Zawartość pamięci jest zastępowana pamięcią programu.

Jek robimy strumieniowanie:
Proces odgórny przygotuje strumienie itd i dwtedy exec zastępuje 

Czyli 

nazwa_polecenia arg1 arg2
Powoduje:
1. Ustawienie środowiska 
2. Fork 
3. Exec żeby podmienić

Jak zrobimy:
ZMIENNA="wartość"
ls arg 1 arg 2

### Co shell sam sobie ustawia
Katalog bieżący:
#### Polecenia wbudowane:
* cd
* Export
* Set
(gdyby cd było normalnym poleceniem, tylko podproces zmieniłby katalog bieżący)

##### Polecenie set
set - 

#### Parametry powłoki

Zmienne 0-9 ($0-$9) kolejne argumenty
Dziesiąty argument???
Argumenty mogą być przesunięte - "shift" - 1<=2 2<=3
$0 - nazwa skryptu tak jak był wywołany


$* - wszystkie argumenty
"$\*" = "$1 $2 ..."
"$@"  - argumenty jako osobne ciągi znaków
"$@" = "$1" "$2"
`$#` = liczba argumentów

`$?` - stań końcowy
`$$` - numer pid 
`$!` - numer ostatnio wykonanego w tle


#### Proces zombie
init:
while (True):
	pobież kod wyjscie



### Metaznaki


#### Cytowanie
'...' -  blokowanie jakichkolwiek podstawien
"..." - blokowanie podstawień poza $ `.``.`   i `\` 


### Interpretacja komend

1. Podzielenie ciągu na komendy i agrumenty za pomocą $IFS
2. Substytucja 1 zmiennych ${zmienna} na zmienna
3. Substytucja 2 rozszerzanie metaznaków: \* \? \[ \]
4. Substytucja 3: Interpretacja łańcucha ujętego w znaki akcentu lub $()

eval - wymuszenie dodatkowej substytucji
Możnaby robić dynamicznie nazwy zmiennych za pomocą eval:

a0 -> a20
eval

$a`eval $i`

##### Grupowanie

* Nawiasy okrągłe () grupują proces w jednym podprocesie
* Nawuasy klamrowe {} grupują proces w obecnym procesie


#### Posix

Do każdego procesu należą trzy pliki:
* stdin
* stdout
* stderr

##### Przekierowania


./x > plik ; stdout > plik
./x >> plik;  dopisz stdout  do plik

./x < plik; stdin z pliku
./x << plik; stdin z pliku aż do EOT

różnica < oraz <<

W zależności od path


./x 2 >  plik  ; STDERR do pliku
./x 2 >> plik; 

n >& m  - przekieruj wyjście strumienia n do wejścia strumienia m
n <& m  - przekieruj wejście n do wejście m


Schowanie stderr:
./x 2> /dev/null


