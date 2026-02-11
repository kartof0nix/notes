
# Wstęp
W ostatnich latach temat komputerów kwantowych stał się niezwykle popularny, choć wokół niego krąży wiele mitów i nieporozumień. Zacznijmy od podstaw, które – choć powszechnie znane – często są nie w pełni rozumiane.

Jednym z kluczowych elementów komputerów kwantowych są kubity. W przeciwieństwie do klasycznych bitów, które mogą przyjmować jedynie dwa stany (0 lub 1), kubity mogą istnieć w tzw. superpozycji. Oznacza to, że mogą być jednocześnie w stanie 0 i 1. W praktyce oznacza to, że komputer kwantowy jest w stanie przeprowadzić obliczenia dla wszystkich możliwych kombinacji wejściowych jednocześnie. Na przykład, dla czterech kubitów można obliczyć wyniki dla 16 różnych kombinacji (0000, 0001, ..., 1111) w tym samym czasie.

Jednakże jest pewien haczyk – odczyt wyniku z komputera kwantowego powoduje „zawalenie” superpozycji, co oznacza, że możemy uzyskać wynik jedynie dla jednego, losowo wybranego wejścia (np. 1011). To rodzi zasadnicze pytanie: **dlaczego w ogóle budujemy komputery kwantowe, skoro podobny efekt można osiągnąć na klasycznym komputerze, losując dane wejściowe?**

Pomimo tej pozornej sprzeczności, giganty technologiczne, takie jak IBM i Google, intensywnie pracują nad rozwojem komputerów kwantowych, inwestując miliardy dolarów w tę technologię. W skali globalnej na badania nad komputerami kwantowymi przeznaczane są niewyobrażalne sumy pieniędzy.

## Podstawy komputerów kwantowych
## Czym jest kubit?
Żeby zrozumieć istotę komputerów kwantowych, należy wpierw pojąć podstawy na których działają - kubity. Kubit to najmniejsza, niepodzielna jednostka informacji w komputerze kwantowym. Jest to odpowiednik klasycznego bitu, jednakże w przeciwieństwie do niego, może znajdować się w stanie 0 i 1, co jest fundamentalnym zjawiskiem mechaniki kwantowej zwanym superpozycją. 
  

Ale co to właściwie oznacza, że jest jednocześnie w dwóch stanach? 
Kubit działa na zasadzie podobnej do paradoksu kota Schrodinger’a. Nie wiemy w którym stanie - 0 czy 1 - znajduje się kubit i dopóki nie zmierzymy jego stanu, wykonujemy operacje na obu tych stanach naraz. Warto dodać, że w zależności od jego właściwości, kubit może mieć różne  prawdopodobieństwo na bycie w danym stanie.

### Jak wygląda kubit w praktyce?
Najprostszym przykładem kubitu jest foton, a dokładniej informacja o tym czy przechodzi przez polaryzator pionowy. W zależności od kątu polaryzacji fotonu, zmienia się prawdopodobieństwo że taki foton przechodzi przez konkretny polaryzator.
Zgodnie z zasadami mechaniki kwantowej, przed pomiarem stan fotonu jest superpozycją – jednocześnie "przechodzi" i "nie przechodzi" przez polaryzator. Dopiero dokonanie pomiaru ujawnia, w którym z tych stanów rzeczywiście się znajduje. 

Właśnie na tej właściwości opierają się komputery kwantowe - działają one na obu tych stanach naraz, a dopiero po dokonaniu wszystkich obliczeń, mierzą one stan kubitów.

### Bramki logiczne
Komputery kwantowe działąc na kubitach muszą uwzględniać obie 
Pierwszy rodzaj bramki jest dość prosty. Zmienia on prawdopodobieństwo konretnego stanu na inny. W przypadku fotonu będzie to odpowiadało ‘obróceniu’ jego polaryzacji. Nie odczytujemy więc informacji, a co za tym idzie szansa na konkretny odczyt dalej zależna od poprzedniego stanu kubitu.

  

Trochę mniej oczywistym rodzajem bramki jest C-NOT. Powoduje on że jeśli pierwszy kwant jest |0>, to drugi się nie zmienia, ale jeśli pierwszy kwant jest |1> to drugi się neguje. W praktyce jest to skutek interferencji fal, ale nie jest to temat tej prezentacji. Stąd nazwa: Controlled-not.

  

Skutkiem przejścia dwóch kubitów przez taką bramkę jest ich splątanie. Na tym schemacie, odczytując informację o kubicie |V>, zmodyfikujemy prawdopodobieństwo konkretnych wartości kubita |H> - w zależności od tego, jaki rozkład prawdopodobieństwa miał |V> przed C-NOT’em.

## Algorytm detush'a
Algorytm detush’a jest bardzo prostym algorytmem który demonstruje teoretyczną przewagę komputerów kwantowych nad klasycznymi. Algorytm przyjmuje funkcję f, której argument wynosi 0 albo 1, i która przyjmuje wartość 0 albo 1. Ma za zadanie określić, czy funkcja jest równowartościowa (f(0) wynosi tyle samo co f(1) czy różnowartościowa). Na przykład, jeśli f(0) to 0 a f(1) to 0, powinniśmy zwrócić 1, a jeśli f(0) to 1 a f(1) to 0 powinniśmy zwrócić 0. Algorytm klasyczny sprawdziłby wartość f(0), f(1), a następnie porównałby wartości tej funkcji - sumarycznie wywołując funkcję dwa razy. A co z algorytmem kwantowym? Algorytm detush’a działa na prawdopodobieństwach jednego kubitu - jest tak określony, że jest w stanie rozwiązać ten sam problem w jednym wywołaniu funkcji f, co pokazuje przewagę komputerów kwantowych nad klasycznymi. To może być niewielka przewaga - jedno zapytanie w odróżnieniu od dwóch - ale często rozwój zaczyna się od niepozornych osiągnięć, które dopiero zachęcają teoretyków do rozwijania dziedziny. 

Algorytm można podzielić na trzy fazy. W pierwszej, algorytm generuje dwa kubity które mają równe prawdopodobieństwa na bycie |1> i |0>. Następnie, oblicza wartość badanej funkcji w zalżności od jednego z kubitów oraz wykonuje operację CNOT na wyniku funkcji i drugim kubicie. Jeżeli funckja była stałowartościowa, to dwa początkowe kubity nie sprzężą się, a rozkład prawdopodobieństwa będzie niezależny. Jeżeli jednak funkcja była różnowartościowa, kubity się sprzężą a co za tym idzie rozkład prawdopodobieństwa posdzczególnych stanów się zmieni. Ostatnią operacją jest odpowiedni obrót jednego z kubitów tak, aby 
Algorytm opiera się na wykorzystaniu bramki CNOT. w której bitem operacyjnym jest wartość badanej funkcji

### Algorytm Grover'a

Algorytm Grover'a jest jednym z najważniejszych algorytmów kwantowych, ponieważ rozwiązuje praktyczny problem w czasie o wiele szybszym niż komputery klasyczne.

Algorytm ten rozwiązuje problem wyszukiwania. Ma na wejściu funkcję f(x) która może przyjmować wartość 0 lub 1. Algorytm znajduje argument funkcji, dla którego f(x) = 1.
Kiedy komputer klasyczny rozwiązuje ten problem w czasie O(n), algorytm Grover'a rozwiązuje ten problem w czasie O(sqrt(N)). 

Poza zastosowaniami w algorytmice, algorytm ten może np. znajdować hasło szybciej niż próbując każdą możliwość po kolei, co może stanowić zagrożenie dla bezpieczeństwa cyfrowego.

### Algorytm Shor'a
Kolejnym popularnym algorytmem w komputerach kwantowych jest algorytm Shor'a. Rozkłada on liczbę na jej czynniki w czasie N^3 zamiast sqrt(2^N) w którym robią to komputery klasyczne - gdzie N to ilosć bitów liczby. 
Problem z tym algorytmem jest następujący: cała dzisiejsza kryptografia opiera się na wysokiej złożoności obliczeniowej rozkładu liczby na jej czynniki pierwsze. Komputer klasyczny na złamanie szyfru RSA512 potrzebowałby 10^60 lat, kiedy komputer kwantowy wykonujący tyle samo operacji na sekundę potrzebowałby pesymistycznie 10 sekund! Stanowi to oczywiście ogromne zagrożenie dla kryptografii.


# Historia komputerów kwantowych
Komputery kwantowe zrodzdiły się z pomysłu Richard’a Feyman’na: świat jest kwantowy z natury, więc symulowanie zjawisk fizycznych z wysoką precyzją wymagałoby komputera który działa na podobnej zasadzie. Nie proponował on więc stosowania go, co dzisiaj jest głównym obszarem zainteresowania, do rozwiązywania problemów algorytmicznych, jednakże wzbudził powszechne zainteresowanie komputerami kwantowymi.

Niedługo po jego wykładach, pomysłem zainteresowali się matematycy i algorytmicy. Szybko zaczęły powstawać teoretyczne komputery kwantowe, oraz algorytmy rozwiązujące problemy matematyczne w - teoretycznie - szybszym czasie niż klasyczne. 


W 1998 IBM stworzyło pierwszy działający ‘układ kwantowy’. Był to w pewnym sensie odpowiednik klasycznego ‘układu cyfrowego’. Działał na dwóch kubitach - kwantowych odpowiednikach bitów.
  

Na przełomie pierwszego dzisięciolecia 21 wieku trwał jeszcze intensywny rozwój teoretyczny w dziedzinie komputacji kwantowej. W tym okresie powstało wiele modeli różnych rodzajów komputerów kwantowych. Jednakże konstrukcja każdego z nich była nie małym wyzwaniem w związku z bardzo trudnymi do osiągnięcia warunkami, w których taki komputer mógłby operować. 


Mimo że wiele źródeł podaje jako pierwszy komputer kwantowy “D-Wave One” rozpowszechniony w 2011 roku, trzeba zaznaczyć że nie był to komputer uniwersalny, jako że był przystosowany do rozwiązywania problemu optymalizacyjnego za pomocą zjawiska  wyżarzania kwantowego. Jednakże rozwiązywanie tego problemu wymagało zaimplementowania ‘programowalnej’ funkcji, przez co można nazwać ten model pierwszym programowalnym komputerem kwantowym. Nie był jednak znacząco szybszy od klasycznych komputerów.

### Rodzaje komputerów kwantowych
Istnieje wiele rodzajów komputerów kwantowych. Każdy ma swoje wady i zalety. Generalnie większość technologii ma albo bardzo słabą wydajność albo są bardzo trudne warunki, w których komputer może operować.


  

Przyjżyjmy się parametrom komputerów kwantowych, które nas interesują. Pierwszym ważnym parametrem jest długość trwania kubitu. W skrócie, kiedy cokolwiek co reprezentuje nasz kubit wchodzi w interakcję z otoczeniem, traci interesujące nas właściwości - superpozycję. Czas trwania informuje nas o przeciętnym czasie, w którym możemy badać właściwości kubitu, zanim on sam wejdzie w jakąś interakcję.

Kolejnym ważnym problemem jest wiarygodność pomniarów. Jest ona także związana z traceniem informacji przez kubity, ale także z jakością sprzętu, czujników i środowiska. Im mniejsza jest wiarygodność, tym więcej razy musimy powtórzyć eksperyment, aby uzyskać rzetelne wyniki.

Ostatnim interesującym aspektem jest ilość kubitów które możemy ze sobą związać. Ponownie, jest ona ograniczona przez kilka czynników : jak długo może istnieć pojedyńczy kubit, ile trwa przesył kubitu przez bramkę, oraz ile kubitów sumarycznie mieści procesor. Im więcej kubitów możemy splątać, tym bardziej skomplikowany algorytm jesteśmy w stanie zaimplementować.

  
Chyba najbardziej popularnym jest komputer bazujący na pętlach z nadprzewodników. Ma bardzo długi czas trwania oraz wysoką maksymalną ilość splątanych elektronowów. Mimo że musi on operować w zerze absolutnym (bo tylko wtedy nadprzewodniki mają swoje właściwości), cieszy się on niezwykłą popularnośćią, szczególnie że jesteśmy wstanie wykorzystywać częśći z komputerów klasycznych w ich konstruckji - takie jak kondensatory czy cewki. Jest też nadzieja na rozwój w tej branży wraz z badaniem nadprzewodników w temperaturze pokojowej.

  

Kolejną grupą komputerów są komputery operujące na właściwościach pojedyńczych cząsteczek - na przykład jonów, lub kryształów. Generalnie mają tą zaletę że mogą operować w temperaturach pokojowych (aczkolwiek jony utrzymuje się w jednym miejscu za pomocą - własńie nadprzewodników, więc one nie mają tej zalety). Muszą często niestety być w idealnej próżni, a na dodatek wymagają bardzo precyzyjnego sprzętu pomiarowego. 

Ostatnią grupą są komputery kwantowe działające na fotonach. Nie mają taki dużych problemów z warunkami, jednakże nie da się, jak w przypadku innych, przechowywać informacji a co za tym idzie nie da się zbudować ‘procesora’ który mógłby wykonywać dowolny algorytm (tak jak w przypadku innych komputerów). Każdy układ musi być zaprojektowany pod poszczegółny algorytm, a co za tym idzie, nie da się w jasny sposób skalować.


