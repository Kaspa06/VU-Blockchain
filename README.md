# Hash generatorius

### Reikalavimai

-   Maišos funkcijos įėjimas (angl. input) gali būti bet kokio dydžio simbolių eilutė (angl. string).
-   Maišos funkcijos išėjimas (angl. output) visuomet yra to paties, fiksuoto, dydžio rezultatas (pageidautina 256 bit'ų ilgio, t.y., 64 simbolių hex'as).
-   Maišos funkcija yra deterministinė, t. y., tam pačiam įvedimui (input'ui) išvedimas (output'as) visuomet yra tas pats.
-   Maišos funkcijos reikšmė/kodas (hash‘as) bet kokiai input'o reikšmei yra apskaičiuojamas greitai - efektyviai.
-   Iš hash funkcijos rezultato (output'o) praktiškai neįmanoma atgaminti pradinio įvedimo (input'o).
-   Maišos funkcija yra atspari "kolizijai"
-   Bent minimaliai pakeitus įvedimą, pvz., vietoj "Lietuva" pateikus "lietuva", maišos funkcijos rezultatas-maišos kodas turi skirtis iš esmės, t.y., turi būti tenkinamas taip vadinamas lavinos efektas (angl. Avalanche effect).

## Pseudo-kodas

    Algorithm inputToHash(input)
    // 1. Modifikuoti įvestį
    Create modifiedInput = input + input.length
    for each character c in modifiedInput
        if c is an uppercase letter OR c == '!'
            c = XOR with (position mod 10)
            c = Shift ASCII value up by 5
        else
            c = XOR with (position + 3)
            c = Shift ASCII value down by 2
    Reverse modifiedInput

    // 2. Konvertuoti į bitus
    while modifiedInput length < 32
        Append 'k' to modifiedInput

    Create binaryResult = ""
    for each character c in modifiedInput
        Convert c to 8-bit binary
        Append to binaryResult

    // 3. Skaičiuoti žodžių sumą
    Create wordSum = 0
    for each character c in modifiedInput
        if c is a letter
            Add alphabetical position of c (a = 1, b = 2, ...) to wordSum

    // 4. Modifikuoti bitus pagal žodžių sumą
    for each bit b in binaryResult
        if b == '1'
            if wordSum is even
                b = '0'
            else
                b = '1'
        else
            if wordSum is even
                b = '1'
            else
                b = '0'
        wordSum = wordSum / 2
        if wordSum == 0
            Break the loop

    // 5. Konvertuoti į šešioliktainę reikšmę
    Create hexResult = ""
    Create rollingValue = 1
    for each 4-bit chunk in binaryResult
        Convert chunk to an integer value
        rollingValue = rollingValue + (influence from corresponding character in modifiedInput)
        value = value * rollingValue * (multiplier based on input length) * non-linearity (sin function)
        Convert value to hexadecimal and append to hexResult

    // 6. Modifikuoti šešioliktainę reikšmę
    for each character c in hexResult
        Increment hex character c using helper function

    // 7. Užtikrinti, kad maiša būtų 64 simbolių ilgio
    if hexResult length > 64
        Truncate hexResult to 64 characters
    else
        Append '0' to hexResult until length is 64

    // 8. Grąžinti maišos reikšmę
    Return hexResult

### Testavimas
#### Maišos funkcija yra deterministinė
- Sumaišius tą patį simbolį, identiškus įvedimus rezultatas yra toks pats. Todėl galime teigti, kad sukurta maišos funkcija yra deterministinė.
-- Visi testai buvo atlikti minimum 5 kartus tam, kad įsitikinti, kad rezultatas nesikeičia.
##### Pavyzdžiai:
###### Simbolio a hash reikšmė:
    0c2520f53a712f93548afd251f0b293e6979d3c847b96c7262cfa5b7f2eaf242
###### Stringo Lietuva hash reikšmė: 
    4378771bce67a635bb4be69ad26ddbf8865412942d112e6f686603eff36e6879
###### Stringo lietuva hash reikšmė:
    2d116ece8bd4998c2eb05499a026ac5175eb3190e722b8102506b2e08c424e19
###### Stringo lietuva! hash reikšmė:
    b6287a2c58212052b37925fe8237ab0102c607551baced149b2342f642304976
###### Stringo Lietuva! hash reikšmė:
    4f10293ba2463f9dee1cd598921eb752623d1d782613d0136122d1be7d7821b9

#### Maišos funkcijos efektyvumas
![image](https://github.com/user-attachments/assets/b93c2400-592a-4923-b995-d79cc49a40fb)


#### Kolizijos patikra:
- Šiam testui atlikti buvo sugeneruota 100 000 eilučių porų (25 000 porų, kurių ilgis 10 simbolių, kitas 25 000 porų, kurių ilgis - 100, dar kitas 25 000 poras - 500, ir galiausiai likusias 25 000 poras, kurių ilgis - 1000 simbolių.) ir patikrint, ar iš viso atvejais gautos poros maišos nesutampa.

| Stringo ilgis           | 10  | 100 | 500 | 1000 |
| ----------------------- | --- | --- | --- | ---- |
| Kolizijų skaičius       | 0   | 0   | 0   | 0    |

#### Hash'ų procentinis "skirtingumas"
- Šiam testui atlikti buvo sugeneruota 100 000 eilučių porų (25 000 porų, kurių ilgis 10 simbolių, kitas 25 000 porų, kurių ilgis - 100, dar kitas 25 000 poras - 500, ir galiausiai likusias 25 000 poras, kurių ilgis - 1000 simbolių.) ir patikrint ir įvertinti, kaip skiriasi vieno simbolio skirtumo porų hash'ai:
- V0.2 Iš esmės pakeista hash'o procentinio skirtingumo funkcija pasitelkiant ChatGPT pagalbą, kadangi nuojauta kuždė, kad rezultatai nebuvo skaičiuojami teisingai

| Metric               | Value          |
|----------------------|----------------|
| Number of pairs       | 50,000        |
| Min Hex Difference    | 1.95312%      |
| Max Hex Difference    | 62.1094%      |
| Avg Hex Difference    | 31.5068%      |
| Min Bit Difference    | 4.6875%       |
| Max Bit Difference    | 100%          |
| Avg Bit Difference    | 59.6374%      |

#### Išvados
V0.2 versija mano galva atitinka visus hash funkcijos keliamus reikalavimus.
- Maišos funkcijos įėjimas (angl. input) gali būti bet kokio dydžio simbolių eilutė (angl. string).
- Maišos funkcijos išėjimas (angl. output) visuomet yra to paties, fiksuoto, dydžio rezultatas (pageidautina 256 bit'ų ilgio, t.y., 64 simbolių hex'as).
- Maišos funkcija yra deterministinė, t. y., tam pačiam įvedimui (input'ui) išvedimas (output'as) visuomet yra tas pats.
- Maišos funkcijos reikšmė/kodas (hash‘as) bet kokiai input'o reikšmei yra apskaičiuojamas greitai - efektyviai.
- Iš hash funkcijos rezultato (output'o) praktiškai neįmanoma atgaminti pradinio įvedimo (input'o).
- Maišos funkcija yra atspari "kolizijai" (angl. collision resistance).
- Bent minimaliai pakeitus įvedimą, pvz., vietoj "Lietuva" pateikus "lietuva", maišos funkcijos rezultatas-maišos kodas turi skirtis iš esmės, t.y., turi būti tenkinamas taip vadinamas lavinos efektas (angl. Avalanche effect).

Mano Hash funkcijos didžiausias trūkumas - našumas. Lyginant su kitais populiariais generatoriais, mano Hash generatorius yra pernelyg lėtas, ir paprastas užduotis užtrunka atlikti gerokai ilgiau negu jis tai turėtų daryti. 

# Papildomos užduotys

### Sukurtos Hash funkcijos palyginimas su SHA256

- panaudotas SHA256 kodas: https://github.com/System-Glitch/SHA256

#### Mano hash funckija nuskaitant konstitucija.txt
| Eilučių sk | 1       | 2       | 4       | 8       | 16      | 32      | 64      | 128     | 256     | 512     |
|------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
| Laikas (s) | 0.000448| 0.000899| 0.001   | 0.001997| 0.002885| 0.004985| 0.007979| 0.018613| 0.039893| 0.089325|

![image](https://github.com/user-attachments/assets/804f1a62-65c8-426b-819a-92ebb71253cb)


#### SHA256 nuskaitant konstitucija.txt
| Eilučių sk | 1       | 2       | 4       | 8       | 16      | 32      | 64      | 128     | 256     | 512     |
|------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
| Laikas (s) | 0       | 0       | 0.000595| 0.0007988| 0.0007766| 0.0010096| 0.001638| 0.0027448| 0.0043952| 0.0091228|

![image](https://github.com/user-attachments/assets/4d80cdae-c1a6-4646-bebe-1de8cf4447e9)

#### Kolizijos patikra:
- Mano Hash funkcija:

| Stringo ilgis           | 10  | 100 | 500 | 1000 |
| ----------------------- | --- | --- | --- | ---- |
| Kolizijų skaičius       | 0   | 0   | 0   | 0    |

- SHA256 funkcija:

| Stringo ilgis           | 10  | 100 | 500 | 1000 |
| ----------------------- | --- | --- | --- | ---- |
| Kolizijų skaičius       | 0   | 0   | 0   | 0    |

#### Hash'ų procentinis "skirtingumas"
- Mano Hash funkcija:

| Metric               | Value          |
|----------------------|----------------|
| Number of pairs       | 50,000        |
| Min Hex Difference    | 1.95312%      |
| Max Hex Difference    | 62.1094%      |
| Avg Hex Difference    | 31.5068%      |
| Min Bit Difference    | 4.6875%       |
| Max Bit Difference    | 100%          |
| Avg Bit Difference    | 59.6374%      |

- SHA256 funkcija:

| Metric               | Value          |
|----------------------|----------------|
| Number of pairs       | 50,000        |
| Min Hex Difference    | 36.7188%      |
| Max Hex Difference    | 62.8906%      |
| Avg Hex Difference    | 50.0027%      |
| Min Bit Difference    | 78.125%       |
| Max Bit Difference    | 100%          |
| Avg Bit Difference    | 93.7484%      |
| Time taken for hashing | 2.76s        |

### Išvados
##### Hashavimo laiko skirtumas
- SHA256 algoritmas yra optimizuotas našumui, todėl teste su konstitucija.txt SHA256 funkcija buvo 9,79 karto greitesnė negu mano Hash funkcija.
##### Kolizijos
- Tiek SHA256, tiek mano Hash funkcija neturėjo nei vienos kolizijos atliekant kolizijų testą.
##### Skirtingumo procentai
- Mano ir SHA256 hash funkcijų skirtumai pagal hex ir bitų "skirtingumo" procentus vis dėl to taip pat gerokai skiriasi. SHA256 funkcija generuoja hash'us, kurių hex skirtumų vidurkis yra 50.0027%, o mano hash funkcijoje vidutinis hex skirtumas siekia 31.5068%. Taip pat, SHA256 bitų skirtingumo vidurkis yra 93.7484%, kai mano hash funkcijoje šis rodiklis yra žemesnis – 59.6374%.
##### Hashavimo laikas (50,000 porų)
- SHA256 Hash funckija buvo 24,54 kartus greitesnė negu mano Hash funkcija. Mano hash funkcija užtruko 67.74 sekundes, tuo tarpu SHA256 užtruko 2.76 sekundes.
#### Bendros išvados
Pagrindinis skirtumas tarp funkcijų yra greitis ir hex bei bitų skirtingumo procentai. SHA256 yra žymiai greitesnė, ypač kai didėja apdorojamų duomenų kiekis. Taip pat SHA256 yra kriptografiškai saugesnė hash funkcija negu manoji ir ji užtikrina mažesnę kolizijų tikimybę, nors jų testuojant ir nebuvo.
