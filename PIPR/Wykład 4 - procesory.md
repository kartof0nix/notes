


## Budowa procesora

To jest tylko model procesora a nie jakiś właściwy procesor
Nie mamy na nie problem przeznaczenia prawo nam się jeszcze jeden rejestr noszony jako peter cooper czego tobie z tęsknoty od programator taki rejestr tak naprawdę oblicza na obrazie instrukcji powodowane przez aspekty procesora jak byk się dziesięć sześć rejestracja robert naszego nazwano.
Dobra kawa się to szalenie trzy.
Zespół pracy tak naprawdę pamięcią zewnętrzne pojawia się memory adres register trafi adres albo do Zapisu czegoś tak tym adresy albo do czytania czegoś z tego adresau. 

CIR register - obecna instrukcja do wykonania. Może mieć np. inną długość.

Zauważ, że wszystko jest liczbą - instrukcje, dane, adresy...


Jak procesor startuje ma na sztywno adres PC ustalony na 0.
Procesor zaczyna wykonywać cykl wykonywania instrukcji.

### Cykl wykonywania instrukcji

1. Instruction fetch
PC - > MAR 
Instrukcja do pamięci żeby ściągnąć adres 0.

Jednostka steruj≥ąca sczytuje i zapamiętuje do obecnej instrukcji.

2. Decode
PC+=1
Wykonaj odpowiednią instrukcję

3. Execte




### Ilość danych / instruckji jednocześnie
SISD - Single Instruction Single Data
The most basic one.

SIMD - Single Instruction Multiple Data
Rejestry są 'wektorami' - mają kilka komórek we każdym rejestrze...
Można założyc że mamy kilka ALU które każdą komórkę robią jednocześnie...

## Przetwarzanie potokowe i superskalarne

#### Potokowe 
Mamy kilka jednoczesnych 'potoków' wykonywania instrukcji 



