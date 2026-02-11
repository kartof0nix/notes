**Zadanie zmodyfikowane zgodnie z nowymi założeniami:**

---

**9. Mamy system komputerowy z 16-bitową przestrzenią adresową, natywnym słowem 16-bitowym oraz o 8-bitowym bajcie. Stos rosnący w dół przestrzeni adresowej.**  
Stosując format instrukcji maszynowej: `operacja arg_źródłowy, arg_docelowy` oraz kodowanie wg podanych tabel kodowania, podaj postać heksadecymalną kolejnych bajtów kodu instrukcji.

**Tabelka kodowania adresów:**

- R0: `0000`
- R1: `0001`
- R2: `0010`
- Akumulator (A): `0011`
- RAM: `0100`
- Immediate: `1111`

**Tabelka instrukcji:**

- `mov`: `0x00`
- `add`: `0x01` (dodaj zawartości dwóch rejestrów, wynik w akumulatorze)
- `sub`: `0x02` (odejmij zawartości dwóch rejestrów, wynik w akumulatorze)
- `push`: `0x03`
- `call`: `0x04`
- `jg`: `0x05` (jump greater - jeśli wartość w akumulatorze jest większa od zera)

**Instrukcje do wykonania | Kod operacji | Kodowanie:**

1. `mov R1, [R2]` – Skopiuj zawartość pamięci z RAM spod adresu w R2 do rejestru R1.
2. `add R1, R0` –
3. `sub R1, R2` 
4. `jg 0x2000` 
5. `push R2`
6. `call 0x3000`

Podaj zawartość wymienionych poniżej w tabeli rejestrów oraz zawartość komórek pamięci (jako dane 16-bitowe) po wykonaniu każdej instrukcji, jeśli przed ich wykonaniem było odpowiednio:

- R0 = 0x0000
- R1 = 0x0000
- R2 = 0x7FFF
- Akumulator (A) = 0x0000
- PC (program counter) = 0x1000
- SP (stack pointer) = 0x7FFC
- BP (base pointer) = 0x8003

Przestrzeń adresowa pamięci (każda komórka przechowuje 16-bitowe dane):

- [0x7FFF] = 0x59AB
- [0x2000] = kod funkcji (zakładamy, że funkcja nie modyfikuje rejestrów poza SP).
- [0x3000] = kod funkcji wywołanej przez `call` (również zakładamy brak modyfikacji rejestrów poza SP).
- [0x7FFC] = niezainicjalizowane

---

**Twoje zadanie:**

1. Przeanalizuj kod maszynowy dla wskazanych instrukcji, wykorzystując powyższe kodowania.
2. Oblicz efekty działania kodu na wskazane rejestry i pamięć.

Dla każdej operacji podaj:

- Postać heksadecymalną kodu maszynowego.
- Zawartość rejestrów po wykonaniu każdej instrukcji.
- Zawartość pamięci po zakończeniu całej sekwencji.

logiczniejsza byłaby chybba inna kolejność -