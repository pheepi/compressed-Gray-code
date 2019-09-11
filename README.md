# Compressed Gray Code

Construction of compressed Gray code and its usage in compression.

# Summary

An n-bit cyclic Gray code is a cyclic list of all n-bit strings where consecutive strings differ in exactly one bit. We may see Gray code also as a graph where strings are vertices of a hypercube and transitions are edges between vertices in a dimension which is defined by the position of the transition. The index of the transition can be encoded as a binary number and it gives again an n-bit string, now with length of [log2n]. If we create another graph from these strings as vertices, we can have a sub-graph of dimension [log2n]. Compressed n-bit Gray code is such list of strings whose graph of transitions is isomorphic to an induced sub-graph of hypercube with dimension [log2n]. the compressed Gray Code may be represented by the first string, the first transition and list of transitions in induced graph.

# Documentation

Documentation of the source code in Czech language.

## Úvod

Program na kompresi bitmapovıch indexù pomocí Grayovıch kódù (dále jen KomBIG) je aplikace na kompresi dat v podobì indexù bitmap. Bitmapovı index je seznam n-bitovıch posloupností, tedy takovıch, e jejich jednotlivé hodnoty mùou bıt pouze pravda èi nepravda. Tento index se velmi èasto pouívá napøíklad v databázích, kde hodnoty atributù mají velmi malou doménu. Tento druh záznamu má tu vıhodu, e se na nìj snadno kladou databázové dotazy, hlavnì v podobì bitovıch operací, které jsou velmi rychlé na dnešních poèítaèích. Je však nároènı na diskovou pamì a je tedy vıhodné ho komprimovat.

### Motivace

Pokud bychom chtìli pouít standardní komprimovací techniky (napø. Huffmanovo kódování), ztratíme vıhodu rychlého dotazování. Je tedy tøeba pouít takovou techniku, která data sice zhustí, ale stále na nich bude mono klást dotazy. Nejpouívanìjší algoritmus pro takovou kompresi je dnes WAH. Ten v principu fungují tak, e v datech nalezne dlouhé posloupnosti stejnıch hodnot (nul nebo jednièek) a uloí je jako jednu hodnotu a poèet. Je tedy jasné, e komprese bude tím vìtší, èím delší posloupnosti v datech budou. Jednotlivé bitmapy mùeme poskládat tak, e v nich bude co nejménì posloupností. Setøídit takto optimálnì bitmapy je v3ak NP-tìkı problém. Pøesto existují heuristiky na pøerozdìlení. Jednou z nich jsou i Grayovy kódy.

Grayùv kód (dále jen GK) je posloupnost všech n-prvkovıch bitmap takovıch, e dvì sousední bitmapy se liší právì v jedné hodnotì. Tato posloupnost není jedna, ale je jich mnoho. Pokud bychom mìli soubor, kterı obsahuje všechny n-prvkové bitmapy, a setøídili ho podle GK, potom jsou data seøazena nejlepším monım zpùsobem. Ve skuteènıch souborech dat se však zdaleka všechny n-prvkové bitmapy nevyskytují. Je tedy otázkou, zda se v takovıchto datech pøerozdìlení pomocí GK vyplatí.

Z experimentálních vısledkù vyplıvá, e pokud pouijeme zrcadlovı typ GK (ZGK), je vısledné pøerozdìlení stejnì vıhodné jako prosté lexikografické rozdìlení. Zrcadlovı typ GK je definován rekurzivnì. Mìjme GK S1 = (0,1). Další n-bitové GK vytvoøíme z rovnosti Sn+1 = 0Sn, 1SRn, kde SR posloupnost S seøazen pozpátku. Otevøenou otázkou zùstává, zda pøi pouití jiného typu GK nebude vısledné uspoøádání vıhodnìjší. Program KomBIG implementuje speciální typ GK právì pro úèely zkoumání jeho vıhodnosti.

### Komprimovanı Grayùv kód

Jednou z vlastností GK je jejich samotné uspoøádání. V tomto uspoøádání si nemusíme ukládat celé bitmapy, ale pouze jedna bitmapa a seznam èísel pozic, ve kterıch se liší dva sousední bitmapy (tzv. pøechody). Takto dokáeme zmenšit kód z pùvodních n bitù na [log2n] bitù na jedna bitmapa. Existuje ještì další zpùsob reprezentace kódu, kterı dokáe zhustit prùmìrnì jedna bitmapa na velikost [log2log2n] Všechny GK ale tímto zpùsobem zmenšit nelze, pro nalezení takového kódu existuje speciální technika, kterou implementuje KomBIG.

Øeknìme, e cyklickı Grayùv kód je takovı, ve kterém se liší první prvek od posledního právì v jednom bitu. Cyklickı GK si mùeme pøedstavit jako Hamiltonovskou krunici v n-dimenzionální-hyperkrychli Qn. N-dimenzionální hyperkrychle je graf na 2n vrcholech takovı, e pokud vrcholy oznaèíme sekvencí n nul a jednièek, potom mezi vrcholy vede hrana právì, kdy se vrcholy liší právì v jedné hodnotì svıch sekvencí. Hamiltonovská krunice je krunice grafu taková, e prochází všechny vrcholy grafu. Kadá hrana v hyperkrychli má smìr. Smìr hrany urèuje pozice lišícího se bitu mezi dvìmi sekvencemi koncovıch vrcholù hrany. Hamiltonovská krunice se tedy dá reprezentovat jako jeden vrchol Hyperkrychle a posloupnost smìrù. 

Nyní definujme indukovanı graf Hamiltonovské krunice jako graf, jeho vrcholy jsou smìry hyperkrychle a mezi tìmito vrcholy vede hrana právì, kdy z alespoò jednoho vrcholu Hamiltonovské krunice vede hrana do smìrù udanıch tìmito vrcholy indukovaného grafu. Indukovanı graf mùe mít rozlièné „tvary“ [5], nás zajímají jen ty Hamiltonovské krunice, které indukují hyperkrychli. GK, kterı tato krunice pøedstavuje, nazveme komprimovanı Grayùv kód (dále jen KGK) [1]. Zde je pøíklad KGK s bitmapy délky 4 a poètem bitmap 24 = 16.

```
0011000011001111
0001111110000110
0111100111100000
0000001111111100
```

## Èásti programu

Program je v podstatì logicky rozdìlenı na tøi èásti:

1. První èást programu umí vstupní bitmapovı index setøídit, a to hlavnì podle KGK. Generuje se tedy rekurzivní metodou KGK s velikostí bitmap n, kde n je velikost bitmap v nesetøídìném souboru. Vıstup je novı soubor se bitmapy ze vstupního souboru setøídìnımi pomocí KGK. Pokud program ádnı vstupní soubor nedostane, je moné vygenerovat pro zadané n do vıstupního souboru samotnı KGK.
1. Na bitmapovı index setøídìnı podle KGK (ale i na jakıkoliv jinı bitmapovı index) se pouívá komprimaèní algoritmus WAH.
1. Poslední èást vytvoøí pro seznam jmen souborù statistiku, která pro kadı soubor znázoròuje velikost souborù pøi pouití daného pøerozdìlení a komprimaèního algoritmu WAH. Ve statistice se mohou objevit tyto ètyøi pøerozdìlení: ádné, lexikografické, pomocí zrcadlového typu GK a pomocí KGK. Aby se nemusel kadı soubor komprimovat zvláš, podporuje KomBIG typ souboru se seznamem jmen souborù.

## Vstup

KomBIG pracuje podle vstupu uivatele, kterı pøedá programu informace pomocí argumentù pøíkazové øádky nebo pomocí vstupních souborù. Nyní si ukáeme druhy souborù, které program pøijímá. Pokud chceme pracovat s nìjakımi daty z externích zdrojù, musíme je pøevést do formátu podporovaném KomBIGem pomocí jinıch programù. To samozøejmì mùe zmìnit velkost pùvodního souboru, ale my vyuíváme pøedevším informaci z bitmap, tedy na velikost komprimovaného souboru to nic nemìní.

Soubor s indexem má jednoduchı tvar. Na kadém øádku souboru se nalézá bitovı vektor. Tento vektor je pøedstavován øetìzcem znakù 0 a 1. Kadı znak pøedstavuje bitovou informaci v indexu tak, e první znak je i první bitová hodnota v indexu. Na jenom øádku mohou bıt pouze tyto dva znaky. Pokud jsou v souboru prázdné øádky, program je pøeskoèí. Aby program vìdìl, jaká je šíøka indexu, jako první øádek je nutné zadat èíslo, které udává šíøku indexu. Všechny bitové øetìzce musí bıt stejnì dlouhé. Pøi chybném vstupu se vypíše pøíslušné chybové hlášení.

Seznam jmen souborù má také jednoduchı formát. Jedná se o list jmen, kadı na jednom øádku. Jeliko ve jménu souboru mohou bıt rùzné divné znaky, je vhodné dávat jména do uvozovek. V uvozeném jménu se jednoduché uvozovky zapíšou jejich zdvojením.

## Vıstup

Jako vıstup programu mùeme brát jednak vıpis zpráv na standardní vıstup, jednak vıstupní soubory. Pøi pouhém tøídìní indexu je vıstupní soubor znovu index s formátem popsanım vıše. Pøi kompresi se vytvoøí komprimovanı soubor. Ten je v podobném formátu jako index, akorát sloupce jsou zobrazeny jako øádky. Implementace v tomto programu ve skuteènosti soubor nezmenší. Jeliko nám jde o názornost, v místech, ve kterıch algoritmus data redukoval, jsou mezery. Je potom pouhym okem vidìt, jak byl algoritmus úspìšnı. Komprimovanı soubor lze poté získat jednoduše odstranìním mezer.

Soubor se statistikou se ukládá do formátu CSV. Tento formát je jednoduchı a pro uloení statistiky naprosto postaèující, navíc je podporován mnoha aplikacemi pro správu dat. Data, která se pøi vytváøení statistiky vytvoøí, jsou v CSV souboru uspøádána takto:

* První poloka prvního øádku je prázdnı øetìzec
* Další poloky na prvním øádku jsou jména sloupcù, které mohou pøedstavovat pouité pøerozdìlovací techniky a komprimaèního algoritmu nebo nìjaké vlastnosti souborù.
* První poloky ostatních øádkù jsou jména souborù (i s cestou)
* Ostatní poloky jsou èísla oznaèující velikost komprimovanıch souborù v bytech taková, e øádek urèuje, jakı soubor byl komprimován, a sloupec urèuje, jakou technikou byl sloupec komprimován. Mùou to také bıt hodnoty udávající poèet bitmap, velikost bitmap nebo násobek tìchto dvou hodnot.

## Ovládání

Program se ovládá z pøíkazové øádky. Zde je šablona pøíkazu:

```
kombig [-i <soubor>] [-o <soubor>] [-c] [-r <trideni>] [-s (trideni | vlastnost) ...] [-g n] [-h]
```

Poøadí parametrù mùe bıt libovolnì pøeházené (a na vıjimky). Formátování øádku je pouze v rukou operaèního systému, na kterém program bìí, tedy napøíklad bílé znaky a uvození nemùe program rozeznat. Program rozlišuje malá a velká písmena.

Vysvìtlíme si vıznam pøepínaèù:

* -i - Pøijímá jako parametr jméno vstupního souboru. Jméno mùe bıt jak s úplnou cestou, tak i s relativní cestou vzhledem k umístìní programu.
* -o - Pøijímá jako parametr jméno souboru, do kterého bude zapsanı vısledek operace. Pokud soubor existuje, program vypíše chybu a skonèí.
* -c - Provádí kompresi pomocí algoritmu WAH. Pøepínaèe –i a –o musí bıt uvedené.
* -r - Setøídí vstupní soubor podle zadaného druhu tøídìní a vypíše do vıstupního souboru. Pøepínaèe –i a –o musí bıt uvedené. Pøi kombinaci s pøepínaèem –c se nejdøíve provede setøídìní a poté komprese.
* -s - Vytvoøí statistiku. Jako parametr je seznam hodnot ve tvaru tøídìní nebo vlastnost, kde tøídìní je typ tøídìní (viz níe) a vlastnost nìjaká další vlastnost (viz níe). Jako vstupní soubor bere seznam jmen souborù, na kterıch má bıt provedena statistika. Vıstupní soubor je ve formátu CSV. Jeliko pøepínaè bere seznam parametrù, musí bıt uveden jako poslední. Pøepínaèe –i a –o musí bıt uvedené.
* -g - Vygeneruje KGK s bitmapy délky n a vypíše je do vıstupního souboru. Pøepínaè –o musí bıt uveden.
* -h - vypíše nápovìdu

Druhy tøídìní mohou bıt Zadne – ádné, Lexikograficke – lexikografické, Zgk – podle zrcadlového typu GK a Kgk – podle komprimovaného GK. Další vlastnosti, které mohou bıt uvedeny do statistiky, jsou Delka – délka bitmap, Pocet – poèet bitmap a Velikost – souèin pøedchozích dvou vlastností. Pro ilustraci je uvedeno nìkolik pøíkladù:

```
kombig –i vstup.txt –o komprimovany.txt –r Kgk -c
```

V prvním pøíkladu se vstupní soubor setøídí podle KGK a poté se zkomprimuje pomocí WAH. Programu nezáleí na poøadí pøepínaèù (kromì -s), toto je také platnı zápis:

```
kombig –o komprimovany.txt -c –r Lexikograficke –i vstup.txt  
```

V tomto pøípadì se soubor setøídí lexikograficky. Nyní si uvedeme pøíklad vytvoøení statistiky:

```
kombig –i seznam.txt –o stats.csv –s Velikost Zgk Kgk
```

Program zde vytvoøí statistiku se tøemi sloupci. V prvním budou souèiny délek bitmap a poètù bitmap, v dalších dvou budou velikosti souborù ze seznamu setøídìné postupnì pomocí ZGK a KGK a komprimované pomocí WAH.

## Popis kódu

Zde popíšeme, jak jsou jednotlivé algoritmy implementovány. Program je napsanı v jazyce C++. Budeme postupovat po jednotlivıch souborech, pøièem implementaci a hlavièkové soubory jsou prány jako jeden soubor:

### Bitmapa

Zde je uloena reprezentace bitového vektoru. Aby byla práce rychlá, implementuje se jako vektor èísel typu int. S tímto typem toti pracuje procesor nejrychleji. Vıhodou této reprezentace je také to, e nìkteré bitové operace na bitovém vektoru není nutné provádìt po jednom bitu, ale lze je provést najednou na typu int. Metody bitmapy vìtšinou tuto bitmapu upravují (napøíklad metody nastavBit,  invertujBit, prohodBity). Bitmapy lze také zvìtšovat (metody pripojPrefix, pripojPostfix).

### BitmapovyIndex

Bitmapovı index je reprezentován jako vektor øádkovıch bitmap. Tøída BitmapovyIndex je v podstatì pouze kontejner na vektorem bitmap. Jsou zde však implementovány nìjaké dodateèné uiteèné metody (bitXor, pripojPrefix, pripojPostfix, prohodSloupce). Pro potøeby tøídìní dle KGK obsahuje dodateènı vektor, kterı udává poøadí kadého bitmapu v KGK. Tento vektor se však pouívá pouze v pøípadì tøídìní.

### ZaznamovyFormat

Tøída ZaznamovyFormat se jednoduchım obalem nad standardními knihovnami, které ètou nebo zapisují do souborù. Je uiteènı hlavnì tím, e uchovává informaci o tom, zda se právì ze souboru ète nebo se do nìj zapisuje. Dále se pouívá jako interface pro tøídy CsvInterface a SerializovatelnyIndex.

### CsvPodpora

Tento soubor obsahuje jednoduchou implementaci parseru a formátovaèe CSV. Tøída CsvTokenizer umí naèítat záznamy z formátu CSV, má pouze jednu uiteènou metodu dejZáznam, která naète záznam. Tøída CsvWriter umí zapisovat soubor CSV. Má rùzné metody pro zápis rùznıch druhù dat. Tøída CsvUniversal umí obojí.

### SerializovatelnyIndex

Tøída SerializovatelnyIndex je potomek tøídy BitmapovyIndex a ZaznamovyFormat. Uívá se k zápisu indexu do souboru. Vıhoda této implementace je v tom, e bitmapovı index se chová jako stream, staèí tedy zavolat jednu metodu a index se ze souboru naète nebo do souboru zapíše korektnì. Tato tøída má dva potomky.

Tøída VstupBitmap se uívá k naèítaní bitmap, k èemu slouí metoda nacti. Uiteènou metodou je také zjistiDelkuBitmap, která se podívá na první øádek souboru s indexem a pøeète èíslo, které udává délku indexu. Nejdùleitìjší je však funkce setridSoubor. Ta jako parametry bere typ tøídìní a odkaz na tøídu VystupBitmap (viz níe). Funkce tøídí bitmapovı index podle zadaného tøídìní. Pokud je index pøíliš velkı, setøídí bloky urèité velikosti a ty poté slévá.

Tøída VstupBitmap se uívá k zápisu bitmap do souboru. Za zmínku stojí metoda slij, která slévá bloky indexu a pouívá se v ji zmínìné metodì setridSoubor.

### Trideni

V tomto souboru je uloena tøída BitmapoveTrideni. Metoda setrid podle toho, jaké má nastavené uspoøádání. Jsou implementovány tøí tøídící algoritmy: Dle lexikografického uspoøádání, podle ZGK a podle KGK. Jeliko u KGK není znám efektivní tøídící algoritmus, tøídìní funguje tak, e se nejdøíve podle existujícího KGK kadému bitmapu pøiøadí jeho poøadí (to dìla metoda vytvorPoradiProKgk) a dle tohoto poøadí se tøídí. Je však nutno upozornit, e tato procedura má exponenciální sloitost podle šíøky indexu.

### Kgk

Tøída Kgk je nejdùleitìjší èástí programu. Má jednu hlavní veøejnou statickou metodu vytvorKgk, která vytvoøí KGK. Konstrukce KGK funguje rekurzivnì. Z kódu délky 4 je pomocí funkce zdvojnasobKgk moné vytvoøit postupnì všechny KGK mocniny dvì. Funkce funguje zhruba tak, e vytváøí automorfní kódy KGK, které poté spojuje do KGK s dvojnásobnou délkou øetìzcù. Princip vıroby tìchto automorfismù je nad rámec této dokumentace. Pro zvìtšení na obecnou šíøku indexu se pouívá podobná technika spojování, jako u ZGK. To provádí metoda rozsirKgkNaObecneN.

### Komprese

Tøída Komprese obsahuje dvì veøejné metody. Metoda komprimuj dostane jako parametr bitmapovı index. Ten pomocí kompresního algoritmu WAH komprimuje do vıstupního souboru. Kompresní algoritmus hledá dlouhé øetìzce stejnıch bitù a ty poté komprimuje do jednoho slova, vıpis tohoto slova dìla metoda zapisFillWord. Metoda vypocti dìlá témìø to samé jako komprimuj, jen neukládá vısledek do souboru, ale vypoèítává velikost (hypotetického) komprimovaného souboru, co je rychlejší.

### NastaveniVypoctu

Tøída zpracovává vstup z pøíkazové øádky a vykonává jednotlivé operace. Nejdøíve pomocí metody zpracuj zkontroluje všechny parametry pøíkazové øádky, zda dávají smysl a poté vrátí objekt tøídy NastaveniVypoctu, kde je uchována konfigurace, co se má provést. Následnì celá poadovaná procedura lze vyvolat metodou proved. Metoda provedStatistiku je speciálnì urèena pro vytváøení statistiky, protoe pro kadou statistiku provádí velkou èást stejnıch operací jako metoda proved.

## License

This project is licensed under the terms of the BSD 3-clause "New" (or "Revised") license. See the [LICENSE](LICENSE) file for more details.
