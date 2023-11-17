# Semestrálne zadanie - Zahratie farebnej melódie
## Popis projektu

Tento projekt využíva mikrokontrolér STM32F303K8, LEDky a klávesnicu z PC na vytvorenie konzolovej aplikácie. Konzolová aplikácia bude mať dva módy. 
Po stlačení určitých kláves na klávesnici sa spustí zvuk a LEDky sa rozsvietia podľa melódie.
Bude sa využívať komunikácia USART, I2C a SPI.

## Funkcionality programu
1. Pomocou putty sa bude uživateľ prepínať medzi dvo módmy. 
2. Prvý mód na základe stlačených tlačidiel na klávesnici vydáva zvuky.
3. Druhý mód prijíma súbor znakov, ktorý ak je v preddefinovanej forme zahrá konkrétnu melódiu.
4. Počas celého behu programu LED reagujú na zahranú melódiu. 

## Hardware
- Mikrokontrolér STM32F303K8
- 7 segmentové spektrum LED
- Klávesnica z PC
- Zosilňovač
- Reprák

## Autori
- Damián Bombara
- Patrícia Pálková
- Mátyás Sátor-Érsek
- Tomáš Stupeň (C)
