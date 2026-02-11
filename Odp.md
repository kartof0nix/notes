
## Ile stopni swobody ma robot z napędem różnicowym poruszający się na płaszczyznie?
2

### Zad 2 - koło 
[0, 1, 0], pi

### Zad 3 - i, j, jk
1, 0, +i, -k


### Zadanie 7
Drogę przebytą podczas przyśpieszannia/opóźniania wyrażamy:

$$ s_1 = s_3 = (a * t_a^2) / 2 $$

Drogę podczas jazdy z prędkością jednostajną:

$$ s_2 = v * (t_f − 2*t_a)$$

Otrzymujemy układ równań:

$$ s_1 + s_2 + s_3 = \Delta q $$
$$ v = a * t_a \,\,\,\, a = v / t_a $$ 
Czyli

$$ (a * t_a^2) + v *  (t_f − 2*t_a) = 8 $$
Co sprowadza się do równania 
$$ v t_a - 2 v t_a = 8 - v t_f $$
$$ -v t_a = 8 - v t_f $$
$$ t_a = (8 - 4 * 3) / -4 = -4/-4 = 1 $$
Podstawiając otrzymujemy $a = 4$, stąd otrzymujemy równania:


$$y_a = q_0 + v_0 * t + a/2 * t^2 = 0 + 0 t + 2 * t^2$$
$$y_b = q_1 + v*(t-t_a) = 2 + 4 t - 4 = -2 + 2 * t$$
$$ y_c = q_2 + v*t - (a/2) * t^2 = 6 + 4*(t-2) - 2 * (t-2)^2 = -2 + 4 t - 2 t^2 + 8 t - 8 = -10 + 12t - 2 t^2  $$
Stąd
$$a_0 = 0 \, a_1 = 0 \, a_2 = 2$$
$$b_0 = -2 \, b_1 = 2 $$
Oraz
$$ c_0 = -10 \, c_1 = 12 \, c_2 = -2 $$

### Podsumowanie

| Kto       | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  |
| --------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Ja        | x   | x   | x   | x   | x   |     | x   | x   | x   | x   |
| Slajdy    | x   |     |     |     |     |     |     |     |     |     |
| Gemini    | OK  | OK  | OK  |     |     |     |     |     |     |     |
| Discord   | OK  | OK  | OK  | ERR | ERR |     |     | ERR | NON | NON |
| Symulator |     |     |     |     |     |     |     |     |     |     |
