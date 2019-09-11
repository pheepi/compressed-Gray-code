# Compressed Gray Code

Construction of compressed Gray code and its usage in compression.

# Summary

An n-bit cyclic Gray code is a cyclic list of all n-bit strings where consecutive strings differ in exactly one bit. We may see Gray code also as a graph where strings are vertices of a hypercube and transitions are edges between vertices in a dimension which is defined by the position of the transition. The index of the transition can be encoded as a binary number and it gives again an n-bit string, now with length of [log2n]. If we create another graph from these strings as vertices, we can have a sub-graph of dimension [log2n]. Compressed n-bit Gray code is such list of strings whose graph of transitions is isomorphic to an induced sub-graph of hypercube with dimension [log2n]. the compressed Gray Code may be represented by the first string, the first transition and list of transitions in induced graph.

# Documentation

Documentation of the source code in Czech language.

## �vod

Program na kompresi bitmapov�ch index� pomoc� Grayov�ch k�d� (d�le jen KomBIG) je aplikace na kompresi dat v podob� index� bitmap. Bitmapov� index je seznam n-bitov�ch posloupnost�, tedy takov�ch, �e jejich jednotliv� hodnoty m��ou b�t pouze pravda �i nepravda. Tento index se velmi �asto pou��v� nap��klad v datab�z�ch, kde hodnoty atribut� maj� velmi malou dom�nu. Tento druh z�znamu m� tu v�hodu, �e se na n�j snadno kladou datab�zov� dotazy, hlavn� v podob� bitov�ch operac�, kter� jsou velmi rychl� na dne�n�ch po��ta��ch. Je v�ak n�ro�n� na diskovou pam� a je tedy v�hodn� ho komprimovat.

### Motivace

Pokud bychom cht�li pou��t standardn� komprimovac� techniky (nap�. Huffmanovo k�dov�n�), ztrat�me v�hodu rychl�ho dotazov�n�. Je tedy t�eba pou��t takovou techniku, kter� data sice zhust�, ale st�le na nich bude mo�no kl�st dotazy. Nejpou��van�j�� algoritmus pro takovou kompresi je dnes WAH. Ten v principu funguj� tak, �e v datech nalezne dlouh� posloupnosti stejn�ch hodnot (nul nebo jedni�ek) a ulo�� je jako jednu hodnotu a po�et. Je tedy jasn�, �e komprese bude t�m v�t��, ��m del�� posloupnosti v datech budou. Jednotliv� bitmapy m��eme poskl�dat tak, �e v nich bude co nejm�n� posloupnost�. Set��dit takto optim�ln� bitmapy je v3ak NP-t�k� probl�m. P�esto existuj� heuristiky na p�erozd�len�. Jednou z nich jsou i Grayovy k�dy.

Gray�v k�d (d�le jen GK) je posloupnost v�ech n-prvkov�ch bitmap takov�ch, �e dv� sousedn� bitmapy se li�� pr�v� v jedn� hodnot�. Tato posloupnost nen� jedna, ale je jich mnoho. Pokud bychom m�li soubor, kter� obsahuje v�echny n-prvkov� bitmapy, a set��dili ho podle GK, potom jsou data se�azena nejlep��m mo�n�m zp�sobem. Ve skute�n�ch souborech dat se v�ak zdaleka v�echny n-prvkov� bitmapy nevyskytuj�. Je tedy ot�zkou, zda se v takov�chto datech p�erozd�len� pomoc� GK vyplat�.

Z experiment�ln�ch v�sledk� vypl�v�, �e pokud pou�ijeme zrcadlov� typ GK (ZGK), je v�sledn� p�erozd�len� stejn� v�hodn� jako prost� lexikografick� rozd�len�. Zrcadlov� typ GK je definov�n rekurzivn�. M�jme GK S1 = (0,1). Dal�� n-bitov� GK vytvo��me z rovnosti Sn+1 = 0Sn, 1SRn, kde SR posloupnost S se�azen pozp�tku. Otev�enou ot�zkou z�st�v�, zda p�i pou�it� jin�ho typu GK nebude v�sledn� uspo��d�n� v�hodn�j��. Program KomBIG implementuje speci�ln� typ GK pr�v� pro ��ely zkoum�n� jeho v�hodnosti.

### Komprimovan� Gray�v k�d

Jednou z vlastnost� GK je jejich samotn� uspo��d�n�. V tomto uspo��d�n� si nemus�me ukl�dat cel� bitmapy, ale pouze jedna bitmapa a seznam ��sel pozic, ve kter�ch se li�� dva sousedn� bitmapy (tzv. p�echody). Takto dok�eme zmen�it k�d z p�vodn�ch n bit� na [log2n] bit� na jedna bitmapa. Existuje je�t� dal�� zp�sob reprezentace k�du, kter� dok�e zhustit pr�m�rn� jedna bitmapa na velikost [log2log2n] V�echny GK ale t�mto zp�sobem zmen�it nelze, pro nalezen� takov�ho k�du existuje speci�ln� technika, kterou implementuje KomBIG.

�ekn�me, �e cyklick� Gray�v k�d je takov�, ve kter�m se li�� prvn� prvek od posledn�ho pr�v� v jednom bitu. Cyklick� GK si m��eme p�edstavit jako Hamiltonovskou kru�nici v n-dimenzion�ln�-hyperkrychli Qn. N-dimenzion�ln� hyperkrychle je graf na 2n vrcholech takov�, �e pokud vrcholy ozna��me sekvenc� n nul a jedni�ek, potom mezi vrcholy vede hrana pr�v�, kdy� se vrcholy li�� pr�v� v jedn� hodnot� sv�ch sekvenc�. Hamiltonovsk� kru�nice je kru�nice grafu takov�, �e proch�z� v�echny vrcholy grafu. Ka�d� hrana v hyperkrychli m� sm�r. Sm�r hrany ur�uje pozice li��c�ho se bitu mezi dv�mi sekvencemi koncov�ch vrchol� hrany. Hamiltonovsk� kru�nice se tedy d� reprezentovat jako jeden vrchol Hyperkrychle a posloupnost sm�r�. 

Nyn� definujme indukovan� graf Hamiltonovsk� kru�nice jako graf, jeho� vrcholy jsou sm�ry hyperkrychle a mezi t�mito vrcholy vede hrana pr�v�, kdy� z alespo� jednoho vrcholu Hamiltonovsk� kru�nice vede hrana do sm�r� udan�ch t�mito vrcholy indukovan�ho grafu. Indukovan� graf m��e m�t rozli�n� �tvary� [5], n�s zaj�maj� jen ty Hamiltonovsk� kru�nice, kter� indukuj� hyperkrychli. GK, kter� tato kru�nice p�edstavuje, nazveme komprimovan� Gray�v k�d (d�le jen KGK) [1]. Zde je p��klad KGK s bitmapy d�lky 4 a po�tem bitmap 24 = 16.

```
0011000011001111
0001111110000110
0111100111100000
0000001111111100
```

## ��sti programu

Program je v podstat� logicky rozd�len� na t�i ��sti:

1. Prvn� ��st programu um� vstupn� bitmapov� index set��dit, a to hlavn� podle KGK. Generuje se tedy rekurzivn� metodou KGK s velikost� bitmap n, kde n je velikost bitmap v neset��d�n�m souboru. V�stup je nov� soubor se bitmapy ze vstupn�ho souboru set��d�n�mi pomoc� KGK. Pokud program ��dn� vstupn� soubor nedostane, je mo�n� vygenerovat pro zadan� n do v�stupn�ho souboru samotn� KGK.
1. Na bitmapov� index set��d�n� podle KGK (ale i na jak�koliv jin� bitmapov� index) se pou��v� komprima�n� algoritmus WAH.
1. Posledn� ��st vytvo�� pro seznam jmen soubor� statistiku, kter� pro ka�d� soubor zn�zor�uje velikost soubor� p�i pou�it� dan�ho p�erozd�len� a komprima�n�ho algoritmu WAH. Ve statistice se mohou objevit tyto �ty�i p�erozd�len�: ��dn�, lexikografick�, pomoc� zrcadlov�ho typu GK a pomoc� KGK. Aby se nemusel ka�d� soubor komprimovat zvl᚝, podporuje KomBIG typ souboru se seznamem jmen soubor�.

## Vstup

KomBIG pracuje podle vstupu u�ivatele, kter� p�ed� programu informace pomoc� argument� p��kazov� ��dky nebo pomoc� vstupn�ch soubor�. Nyn� si uk�eme druhy soubor�, kter� program p�ij�m�. Pokud chceme pracovat s n�jak�mi daty z extern�ch zdroj�, mus�me je p�ev�st do form�tu podporovan�m KomBIGem pomoc� jin�ch program�. To samoz�ejm� m��e zm�nit velkost p�vodn�ho souboru, ale my vyu��v�me p�edev��m informaci z bitmap, tedy na velikost komprimovan�ho souboru to nic nem�n�.

Soubor s indexem m� jednoduch� tvar. Na ka�d�m ��dku souboru se nal�z� bitov� vektor. Tento vektor je p�edstavov�n �et�zcem znak� 0 a 1. Ka�d� znak p�edstavuje bitovou informaci v indexu tak, �e prvn� znak je i prvn� bitov� hodnota v indexu. Na jenom ��dku mohou b�t pouze tyto dva znaky. Pokud jsou v souboru pr�zdn� ��dky, program je p�esko��. Aby program v�d�l, jak� je ���ka indexu, jako prvn� ��dek je nutn� zadat ��slo, kter� ud�v� ���ku indexu. V�echny bitov� �et�zce mus� b�t stejn� dlouh�. P�i chybn�m vstupu se vyp�e p��slu�n� chybov� hl�en�.

Seznam jmen soubor� m� tak� jednoduch� form�t. Jedn� se o list jmen, ka�d� na jednom ��dku. Jeliko� ve jm�nu souboru mohou b�t r�zn� divn� znaky, je vhodn� d�vat jm�na do uvozovek. V uvozen�m jm�nu se jednoduch� uvozovky zap�ou jejich zdvojen�m.

## V�stup

Jako v�stup programu m��eme br�t jednak v�pis zpr�v na standardn� v�stup, jednak v�stupn� soubory. P�i pouh�m t��d�n� indexu je v�stupn� soubor znovu index s form�tem popsan�m v��e. P�i kompresi se vytvo�� komprimovan� soubor. Ten je v podobn�m form�tu jako index, akor�t sloupce jsou zobrazeny jako ��dky. Implementace v tomto programu ve skute�nosti soubor nezmen��. Jeliko� n�m jde o n�zornost, v m�stech, ve kter�ch algoritmus data redukoval, jsou mezery. Je potom pouhym okem vid�t, jak byl algoritmus �sp�n�. Komprimovan� soubor lze pot� z�skat jednodu�e odstran�n�m mezer.

Soubor se statistikou se ukl�d� do form�tu CSV. Tento form�t je jednoduch� a pro ulo�en� statistiky naprosto posta�uj�c�, nav�c je podporov�n mnoha aplikacemi pro spr�vu dat. Data, kter� se p�i vytv��en� statistiky vytvo��, jsou v CSV souboru usp��d�na takto:

* Prvn� polo�ka prvn�ho ��dku je pr�zdn� �et�zec
* Dal�� polo�ky na prvn�m ��dku jsou jm�na sloupc�, kter� mohou p�edstavovat pou�it� p�erozd�lovac� techniky a komprima�n�ho algoritmu nebo n�jak� vlastnosti soubor�.
* Prvn� polo�ky ostatn�ch ��dk� jsou jm�na soubor� (i s cestou)
* Ostatn� polo�ky jsou ��sla ozna�uj�c� velikost komprimovan�ch soubor� v bytech takov�, �e ��dek ur�uje, jak� soubor byl komprimov�n, a sloupec ur�uje, jakou technikou byl sloupec komprimov�n. M��ou to tak� b�t hodnoty ud�vaj�c� po�et bitmap, velikost bitmap nebo n�sobek t�chto dvou hodnot.

## Ovl�d�n�

Program se ovl�d� z p��kazov� ��dky. Zde je �ablona p��kazu:

```
kombig [-i <soubor>] [-o <soubor>] [-c] [-r <trideni>] [-s (trideni | vlastnost) ...] [-g n] [-h]
```

Po�ad� parametr� m��e b�t libovoln� p�eh�zen� (a� na v�jimky). Form�tov�n� ��dku je pouze v rukou opera�n�ho syst�mu, na kter�m program b��, tedy nap��klad b�l� znaky a uvozen� nem��e program rozeznat. Program rozli�uje mal� a velk� p�smena.

Vysv�tl�me si v�znam p�ep�na��:

* -i - P�ij�m� jako parametr jm�no vstupn�ho souboru. Jm�no m��e b�t jak s �plnou cestou, tak i s relativn� cestou vzhledem k um�st�n� programu.
* -o - P�ij�m� jako parametr jm�no souboru, do kter�ho bude zapsan� v�sledek operace. Pokud soubor existuje, program vyp�e chybu a skon��.
* -c - Prov�d� kompresi pomoc� algoritmu WAH. P�ep�na�e �i a �o mus� b�t uveden�.
* -r - Set��d� vstupn� soubor podle zadan�ho druhu t��d�n� a vyp�e do v�stupn�ho souboru. P�ep�na�e �i a �o mus� b�t uveden�. P�i kombinaci s p�ep�na�em �c se nejd��ve provede set��d�n� a pot� komprese.
* -s - Vytvo�� statistiku. Jako parametr je seznam hodnot ve tvaru t��d�n� nebo vlastnost, kde t��d�n� je typ t��d�n� (viz n�e) a vlastnost n�jak� dal�� vlastnost (viz n�e). Jako vstupn� soubor bere seznam jmen soubor�, na kter�ch m� b�t provedena statistika. V�stupn� soubor je ve form�tu CSV. Jeliko� p�ep�na� bere seznam parametr�, mus� b�t uveden jako posledn�. P�ep�na�e �i a �o mus� b�t uveden�.
* -g - Vygeneruje KGK s bitmapy d�lky n a vyp�e je do v�stupn�ho souboru. P�ep�na� �o mus� b�t uveden.
* -h - vyp�e n�pov�du

Druhy t��d�n� mohou b�t Zadne � ��dn�, Lexikograficke � lexikografick�, Zgk � podle zrcadlov�ho typu GK a Kgk � podle komprimovan�ho GK. Dal�� vlastnosti, kter� mohou b�t uvedeny do statistiky, jsou Delka � d�lka bitmap, Pocet � po�et bitmap a Velikost � sou�in p�edchoz�ch dvou vlastnost�. Pro ilustraci je uvedeno n�kolik p��klad�:

```
kombig �i vstup.txt �o komprimovany.txt �r Kgk -c
```

V prvn�m p��kladu se vstupn� soubor set��d� podle KGK a pot� se zkomprimuje pomoc� WAH. Programu nez�le�� na po�ad� p�ep�na�� (krom� -s), toto je tak� platn� z�pis:

```
kombig �o komprimovany.txt -c �r Lexikograficke �i vstup.txt  
```

V tomto p��pad� se soubor set��d� lexikograficky. Nyn� si uvedeme p��klad vytvo�en� statistiky:

```
kombig �i seznam.txt �o stats.csv �s Velikost Zgk Kgk
```

Program zde vytvo�� statistiku se t�emi sloupci. V prvn�m budou sou�iny d�lek bitmap a po�t� bitmap, v dal��ch dvou budou velikosti soubor� ze seznamu set��d�n� postupn� pomoc� ZGK a KGK a komprimovan� pomoc� WAH.

## Popis k�du

Zde pop�eme, jak jsou jednotliv� algoritmy implementov�ny. Program je napsan� v jazyce C++. Budeme postupovat po jednotliv�ch souborech, p�i�em� implementaci a hlavi�kov� soubory jsou pr�ny jako jeden soubor:

### Bitmapa

Zde je ulo�ena reprezentace bitov�ho vektoru. Aby byla pr�ce rychl�, implementuje se jako vektor ��sel typu int. S t�mto typem toti� pracuje procesor nejrychleji. V�hodou t�to reprezentace je tak� to, �e n�kter� bitov� operace na bitov�m vektoru nen� nutn� prov�d�t po jednom bitu, ale lze je prov�st najednou na typu int. Metody bitmapy v�t�inou tuto bitmapu upravuj� (nap��klad metody nastavBit,  invertujBit, prohodBity). Bitmapy lze tak� zv�t�ovat (metody pripojPrefix, pripojPostfix).

### BitmapovyIndex

Bitmapov� index je reprezentov�n jako vektor ��dkov�ch bitmap. T��da BitmapovyIndex je v podstat� pouze kontejner na vektorem bitmap. Jsou zde v�ak implementov�ny n�jak� dodate�n� u�ite�n� metody (bitXor, pripojPrefix, pripojPostfix, prohodSloupce). Pro pot�eby t��d�n� dle KGK obsahuje dodate�n� vektor, kter� ud�v� po�ad� ka�d�ho bitmapu v KGK. Tento vektor se v�ak pou��v� pouze v p��pad� t��d�n�.

### ZaznamovyFormat

T��da ZaznamovyFormat se jednoduch�m obalem nad standardn�mi knihovnami, kter� �tou nebo zapisuj� do soubor�. Je u�ite�n� hlavn� t�m, �e uchov�v� informaci o tom, zda se pr�v� ze souboru �te nebo se do n�j zapisuje. D�le se pou��v� jako interface pro t��dy CsvInterface a SerializovatelnyIndex.

### CsvPodpora

Tento soubor obsahuje jednoduchou implementaci parseru a form�tova�e CSV. T��da CsvTokenizer um� na��tat z�znamy z form�tu CSV, m� pouze jednu u�ite�nou metodu dejZ�znam, kter� na�te z�znam. T��da CsvWriter um� zapisovat soubor CSV. M� r�zn� metody pro z�pis r�zn�ch druh� dat. T��da CsvUniversal um� oboj�.

### SerializovatelnyIndex

T��da SerializovatelnyIndex je potomek t��dy BitmapovyIndex a ZaznamovyFormat. U��v� se k z�pisu indexu do souboru. V�hoda t�to implementace je v tom, �e bitmapov� index se chov� jako stream, sta�� tedy zavolat jednu metodu a index se ze souboru na�te nebo do souboru zap�e korektn�. Tato t��da m� dva potomky.

T��da VstupBitmap se u��v� k na��tan� bitmap, k �emu� slou�� metoda nacti. U�ite�nou metodou je tak� zjistiDelkuBitmap, kter� se pod�v� na prvn� ��dek souboru s indexem a p�e�te ��slo, kter� ud�v� d�lku indexu. Nejd�le�it�j�� je v�ak funkce setridSoubor. Ta jako parametry bere typ t��d�n� a odkaz na t��du VystupBitmap (viz n�e). Funkce t��d� bitmapov� index podle zadan�ho t��d�n�. Pokud je index p��li� velk�, set��d� bloky ur�it� velikosti a ty pot� sl�v�.

T��da VstupBitmap se u��v� k z�pisu bitmap do souboru. Za zm�nku stoj� metoda slij, kter� sl�v� bloky indexu a pou��v� se v ji� zm�n�n� metod� setridSoubor.

### Trideni

V tomto souboru je ulo�ena t��da BitmapoveTrideni. Metoda setrid podle toho, jak� m� nastaven� uspo��d�n�. Jsou implementov�ny t�� t��d�c� algoritmy: Dle lexikografick�ho uspo��d�n�, podle ZGK a podle KGK. Jeliko� u KGK nen� zn�m efektivn� t��d�c� algoritmus, t��d�n� funguje tak, �e se nejd��ve podle existuj�c�ho KGK ka�d�mu bitmapu p�i�ad� jeho po�ad� (to d�la metoda vytvorPoradiProKgk) a dle tohoto po�ad� se t��d�. Je v�ak nutno upozornit, �e tato procedura m� exponenci�ln� slo�itost podle ���ky indexu.

### Kgk

T��da Kgk je nejd�le�it�j�� ��st� programu. M� jednu hlavn� ve�ejnou statickou metodu vytvorKgk, kter� vytvo�� KGK. Konstrukce KGK funguje rekurzivn�. Z k�du d�lky 4 je pomoc� funkce zdvojnasobKgk mo�n� vytvo�it postupn� v�echny KGK mocniny dv�. Funkce funguje zhruba tak, �e vytv��� automorfn� k�dy KGK, kter� pot� spojuje do KGK s dvojn�sobnou d�lkou �et�zc�. Princip v�roby t�chto automorfism� je nad r�mec t�to dokumentace. Pro zv�t�en� na obecnou ���ku indexu se pou��v� podobn� technika spojov�n�, jako u ZGK. To prov�d� metoda rozsirKgkNaObecneN.

### Komprese

T��da Komprese obsahuje dv� ve�ejn� metody. Metoda komprimuj dostane jako parametr bitmapov� index. Ten pomoc� kompresn�ho algoritmu WAH komprimuje do v�stupn�ho souboru. Kompresn� algoritmus hled� dlouh� �et�zce stejn�ch bit� a ty pot� komprimuje do jednoho slova, v�pis tohoto slova d�la metoda zapisFillWord. Metoda vypocti d�l� t�m�� to sam� jako komprimuj, jen neukl�d� v�sledek do souboru, ale vypo��t�v� velikost (hypotetick�ho) komprimovan�ho souboru, co� je rychlej��.

### NastaveniVypoctu

T��da zpracov�v� vstup z p��kazov� ��dky a vykon�v� jednotliv� operace. Nejd��ve pomoc� metody zpracuj zkontroluje v�echny parametry p��kazov� ��dky, zda d�vaj� smysl a pot� vr�t� objekt t��dy NastaveniVypoctu, kde je uchov�na konfigurace, co se m� prov�st. N�sledn� cel� po�adovan� procedura lze vyvolat metodou proved. Metoda provedStatistiku je speci�ln� ur�ena pro vytv��en� statistiky, proto�e pro ka�dou statistiku prov�d� velkou ��st stejn�ch operac� jako metoda proved.

## License

This project is licensed under the terms of the BSD 3-clause "New" (or "Revised") license. See the [LICENSE](LICENSE) file for more details.
