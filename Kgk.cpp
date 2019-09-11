/**
Copyright (c) 2009 - 2019, David Skorvaga
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Kgk.h"

#include "BitmapovyIndex.h"

using namespace Bitmapy;



void Kgk::vytvorKgk(BitmapovyIndex & bitmapovyIndex, const DelkaBitmapy n)
{
	Kgk kgk(bitmapovyIndex, n);
	kgk.vytvorKgk();
}



bool Kgk::jeGk(const BitmapovyIndex & bitmapovyIndex)
{
	if (bitmapovyIndex.delka() <= 1) return true;

	EBitmapy e = EBitmapyOk;
	for (SeznamBitmap::const_iterator i = bitmapovyIndex.index.begin();
		i < bitmapovyIndex.index.end() - 1; ++i)
	{
		if (!i->jePraveJedenPrechod(*(i + 1), e))
			return false;
	}

	return (bitmapovyIndex.index.front().jePraveJedenPrechod(
		bitmapovyIndex.index.back(), e));
}



bool Kgk::jeKgk(const BitmapovyIndex & bitmapovyIndex)
{
	if (bitmapovyIndex.delka() <= 1) return true;

	Prechody prechody = vytvorPosloupnostPrechodu(bitmapovyIndex);
	for (Prechody::const_iterator i = prechody.begin();
		i < prechody.end() - 1; ++i)
	{
		if (!Bitmapa::jePraveJedenPrechod(*i, *(i + 1)))
			return false;
	}

	return (Bitmapa::jePraveJedenPrechod(prechody.front(), prechody.back()));
}



const std::string Kgk::RETEZCE_KGK_S_1_BITEM[2] = {"0", "1"};
const std::string Kgk::RETEZCE_KGK_S_2_BITY[4] = {"00", "10", "11", "01"};
const std::string Kgk::RETEZCE_KGK_S_3_BITY[8] = {"000", "010", "011", "001",
"101", "111", "110", "100"};
const std::string Kgk::RETEZCE_KGK_S_4_BITY[16] = {"0000", "0010", "0011",
"0001", "1001", "1101", "0101", "0111", "1111", "1011", "1010", "1110",
"0110", "0100", "1100", "1000"};

const BitmapovyIndex Kgk::KGK_S_1_BITEM(1u, RETEZCE_KGK_S_1_BITEM, 1u << 1u);
const BitmapovyIndex Kgk::KGK_S_2_BITY(2u, RETEZCE_KGK_S_2_BITY, 1u << 2u);
const BitmapovyIndex Kgk::KGK_S_3_BITY(3u, RETEZCE_KGK_S_3_BITY, 1u << 3u);
const BitmapovyIndex Kgk::KGK_S_4_BITY(4u, RETEZCE_KGK_S_4_BITY, 1u << 4u);

const DelkaBitmapy Kgk::opakujiciVKgk4 = 0u;
const DelkaBitmapy Kgk::prostredniVKgk4 = 2u;
const DelkaBitmapy Kgk::krajovyVKgk4 = 1u;
const DelkaIndexu Kgk::poziceHornihoCenteru = 6u;
const DelkaIndexu Kgk::poziceDolnihoCenteru = 7u;



Kgk::Kgk(BitmapovyIndex & bitmapovyIndex, DelkaBitmapy n) :
	bitmapovyIndex(bitmapovyIndex),
	n(n) {}



Kgk::~Kgk(void) {}



void Kgk::vytvorKgk(void)
{
	this->vytvorTrivialniKgk();
	for (DelkaBitmapy i = this->DELKA_PRVNIHO_KGK << 1; i <= n; i <<= 1u)
	{
		this->zdvojnasobKgk();
	}

	this->rozsirKgkNaObecneN();
}



void Kgk::vytvorTrivialniKgk(void)
{
	switch (this->n)
	{
	case 0u:
		this->bitmapovyIndex = BitmapovyIndex();
		break;
	case 1u:
		this->bitmapovyIndex = this->KGK_S_1_BITEM;
		break;
	case 2u:
		this->bitmapovyIndex = this->KGK_S_2_BITY;
		break;
	case 3u:
		this->bitmapovyIndex = this->KGK_S_3_BITY;
		break;
	case DELKA_PRVNIHO_KGK:
	default:
		this->bitmapovyIndex = this->KGK_S_4_BITY;
		break;
	}
}



void Kgk::zdvojnasobKgk(void)
{
	const BitmapovyIndex staryKgk(this->bitmapovyIndex);
	const Bitmapa nulovyBitmap(staryKgk.delkaBitmap());

	const Prechody prechody = vytvorPosloupnostPrechodu(staryKgk);
	const Prechody pomocnaPosloupnost = vytvorPomocnouPosloupnost(prechody,
		staryKgk.delkaBitmap());
	this->bitmapovyIndex.pripojPrefix(nulovyBitmap);

	for (DelkaIndexu i = staryKgk.delka(); i > 0; --i)
	{
		BitmapovyIndex pracovniKgk(staryKgk);
		nahradSegment(pracovniKgk, i-1, nulovyBitmap, prechody,
			pomocnaPosloupnost);
		pracovniKgk.obrat();
		pracovniKgk.odeber();
		EBitmapy e;
		pracovniKgk.pripojPostfix(*staryKgk.bitmapa(i-1, e));
		this->bitmapovyIndex.pridej(pracovniKgk);
	}
}



void Kgk::rozsirKgkNaObecneN(void)
{
	const DelkaBitmapy pocetNovychBitu = this->n
		- this->bitmapovyIndex.delkaBitmap();
	const Bitmapa nula(Bitmapa::NULA);
	const Bitmapa jedna(Bitmapa::JEDNA);

	for (DelkaBitmapy i = 0u; i < pocetNovychBitu; ++i)
	{
		DelkaIndexu poziceBitmapu = 0;
		const Prechody prechody = vytvorPosloupnostPrechodu(this->bitmapovyIndex);

		for (DelkaBitmapy j = 0; j < this->bitmapovyIndex.delka() - 2; ++j)
		{
			if ((prechody.at(j) == i) && (prechody.at(j+2) == i))
			{
				poziceBitmapu = j + 2;
				break;
			}
		}

		this->bitmapovyIndex.rotuj(poziceBitmapu);
		BitmapovyIndex reverzniIndex(this->bitmapovyIndex);
		reverzniIndex.obrat();

		this->bitmapovyIndex.pripojPostfix(nula);
		reverzniIndex.pripojPostfix(jedna);

		this->bitmapovyIndex.pridej(reverzniIndex);
	}

	DelkaIndexu poziceNulovehoBitmapu = 0;
	for (DelkaIndexu i = 0u; i < this->bitmapovyIndex.delka(); ++i)
	{
		EBitmapy e;
		if (this->bitmapovyIndex.bitmapa(i, e)->jeNulovy())
		{
			poziceNulovehoBitmapu = i;
			break;
		}
	}

	this->bitmapovyIndex.rotuj(poziceNulovehoBitmapu);
}



Kgk::Prechody Kgk::vytvorPosloupnostPrechodu(const BitmapovyIndex & gk)
{
	Prechody prechody;
	prechody.reserve(gk.delka());
	DelkaIndexu dalsiBitmap = 1u;

	for (DelkaIndexu i = 0u; i < gk.delka(); ++i)
	{
		if (dalsiBitmap == gk.delka())
			dalsiBitmap = 0;

		EBitmapy e;
		prechody.push_back(gk.bitmapa(i, e)->dejPrvniPrechod(
			*gk.bitmapa(dalsiBitmap, e), e));
		++dalsiBitmap;
	}

	return prechody;
}



Kgk::Prechody Kgk::vytvorPomocnouPosloupnost(const Prechody & prechody,
											 DelkaBitmapy n)
{
	Prechody pomocnaPosloupnost;
	pomocnaPosloupnost.push_back(*(prechody.begin() + 1u));

	for (Prechody::const_iterator i = prechody.begin() + 1;
		i < prechody.end(); ++i)
	{
		if ((*i) == pomocnaPosloupnost.back())
		{
			if (((*(i-1)) ^ (*i)) == 1)
				pomocnaPosloupnost.push_back(static_cast<DelkaBitmapy>(
				((n >> 1) ^ (*i))));
			else
				pomocnaPosloupnost.push_back(static_cast<DelkaBitmapy>(
				(((*(i-1)) ^ (*i)) >> 1) ^ (*i)));
		}
		else
		{
			pomocnaPosloupnost.push_back(*(i-1));
		}
	}

	return pomocnaPosloupnost;
}



void Kgk::nahradSegment(BitmapovyIndex & kgk, DelkaIndexu poradi,
						const Bitmapa & prvniBitmapa, const Prechody &
						prechody, const Prechody & pomocnaPosloupnost)
{
	if ((poradi != 0) && (poradi != (kgk.delka() - 1)))
	{
		if (prechody.at(poradi) == pomocnaPosloupnost.at(poradi-1))
			nahradSegment(kgk, prechody.at(poradi), pomocnaPosloupnost.at(
			poradi), prechody.at(poradi-1), prvniBitmapa, true);
		else
			nahradSegment(kgk, prechody.at(poradi-1), pomocnaPosloupnost.at(
			poradi-1), prechody.at(poradi), prvniBitmapa, false);
	}
	else
	{
		DelkaBitmapy krajovy;

		if (poradi == 0)
		{
			if (((pomocnaPosloupnost.at(0)) ^ (prechody.at(0))) == 1)
				krajovy = static_cast<DelkaBitmapy>(((kgk.delkaBitmap() >>
				1) ^ (prechody.at(0))));
			else
				krajovy = static_cast<DelkaBitmapy>((((pomocnaPosloupnost.at(
				0)) ^ (prechody.at(0))) >> 1) ^ (prechody.at(0)));

			nahradSegment(kgk, prechody.at(0), pomocnaPosloupnost.at(0),
				krajovy, prvniBitmapa, true);
		}
		else
		{
			if (((pomocnaPosloupnost.at(poradi-1)) ^
				(prechody.at(poradi-1))) == 1)
				krajovy = static_cast<DelkaBitmapy>(((kgk.delkaBitmap() >>
				1) ^ (prechody.at(poradi-1))));
			else
				krajovy = static_cast<DelkaBitmapy>((((pomocnaPosloupnost.at(
				poradi-1)) ^ (prechody.at(poradi-1))) >> 1) ^
				(prechody.at(poradi-1)));

			nahradSegment(kgk,  prechody.at(poradi-1), pomocnaPosloupnost.at(
				poradi-1), krajovy, prvniBitmapa, false);
		}
	}
}
void Kgk::nahradSegment(BitmapovyIndex & kgk, DelkaBitmapy opakujici,
						DelkaBitmapy prostredni, DelkaBitmapy
						krajovy, const Bitmapa & prvniBitmapa,
						bool opakujiciJeNalevo)
{
	Prechody permutacePrechodu = ziskejPermutaciNovychPrechodu(
		kgk.delkaBitmap(), opakujici, prostredni, krajovy);

	for (DelkaBitmapy i = 0; i < permutacePrechodu.size(); ++i)
	{
		while (i != permutacePrechodu.at(i))
		{
			DelkaBitmapy j = permutacePrechodu.at(i);
			kgk.prohodSloupce(i, j);
			DelkaBitmapy temp = permutacePrechodu.at(i);
			permutacePrechodu.at(i) = permutacePrechodu.at(j);
			permutacePrechodu.at(j) = temp;
		}
	}

	const DelkaIndexu poziceCenteru = (opakujiciJeNalevo)
		? poziceDolnihoCenteru : poziceHornihoCenteru;
	EBitmapy e;
	Bitmapa maska(*kgk.bitmapa(poziceCenteru, e));
	maska.bitXor(prvniBitmapa);

	BitmapovyIndex pomocnyKgk(kgk.delkaBitmap());

	for (DelkaIndexu i = poziceCenteru; i < kgk.delka(); ++i)
	{
		Bitmapa novaBitmapa(*kgk.bitmapa(i, e));
		novaBitmapa.bitXor(maska);
		pomocnyKgk.pridej(novaBitmapa);
	}

	for (DelkaIndexu i = 0; i < poziceCenteru; ++i)
	{
		Bitmapa novaBitmapa(*kgk.bitmapa(i, e));
		novaBitmapa.bitXor(maska);
		pomocnyKgk.pridej(novaBitmapa);
	}

	kgk.vycisti();
	kgk.prohod(pomocnyKgk);
}



Kgk::Prechody Kgk::ziskejPermutaciNovychPrechodu(
	DelkaBitmapy delkaBitmapy, DelkaBitmapy opakujici,
	DelkaBitmapy prostredni, DelkaBitmapy krajovy)
{
	const DelkaBitmapy rozsirujiciDelka = delkaBitmapy -
		Kgk::DELKA_PRVNIHO_KGK;
	const DelkaBitmapy pocetPPrechodu = dvojkovyLog(delkaBitmapy);

	DelkaBitmapy prvniStaryPPrechod = dvojkovyLog((opakujiciVKgk4 +
		rozsirujiciDelka) ^ (prostredniVKgk4 + rozsirujiciDelka));
	DelkaBitmapy druhyStaryPPrechod = dvojkovyLog((opakujiciVKgk4 +
		rozsirujiciDelka) ^ (krajovyVKgk4 + rozsirujiciDelka));
	DelkaBitmapy prvniNovyPPrechod = dvojkovyLog(
		opakujici ^ prostredni);
	DelkaBitmapy druhyNovyPPrechod = dvojkovyLog(opakujici ^ krajovy);

	Prechody permutacePrechodu(delkaBitmapy, 0);
	std::vector<bool> prechodZjisteny(delkaBitmapy, false);
	Prechody zasobnikPrechodu;

	permutacePrechodu[opakujiciVKgk4 + rozsirujiciDelka] = opakujici;
	prechodZjisteny[opakujiciVKgk4 + rozsirujiciDelka] = true;
	zasobnikPrechodu.push_back(opakujiciVKgk4 + rozsirujiciDelka);
	permutacePrechodu[prostredniVKgk4 + rozsirujiciDelka] = prostredni;
	prechodZjisteny[prostredniVKgk4 + rozsirujiciDelka] = true;
	zasobnikPrechodu.push_back(prostredniVKgk4 + rozsirujiciDelka);
	permutacePrechodu[krajovyVKgk4 + rozsirujiciDelka] = krajovy;
	prechodZjisteny[krajovyVKgk4 + rozsirujiciDelka] = true;
	zasobnikPrechodu.push_back(krajovyVKgk4 + rozsirujiciDelka);

	while (!zasobnikPrechodu.empty())
	{
		Prechody zasobnikCyklus = zasobnikPrechodu;
		zasobnikPrechodu.clear();
		for (Prechody::iterator pr = zasobnikCyklus.begin();
			pr < zasobnikCyklus.end(); ++pr)
		{
			for (DelkaBitmapy i = 0u; i < pocetPPrechodu; ++i)
			{
				const DelkaBitmapy soused = (1u << i) ^ (*pr);
				if (!prechodZjisteny.at(soused))
				{
					prechodZjisteny.at(soused) = true;
					DelkaBitmapy novySoused = permutacePrechodu.at(*pr);

					if (i == prvniStaryPPrechod)
						novySoused ^= (1u << prvniNovyPPrechod);
					else if (i == druhyStaryPPrechod)
						novySoused ^= (1u << druhyNovyPPrechod);
					else if (i == prvniNovyPPrechod)
						if (prvniStaryPPrechod == druhyNovyPPrechod)
							novySoused ^= (1u << druhyStaryPPrechod);
						else
							novySoused ^= (1u << prvniStaryPPrechod);
					else if (i == druhyNovyPPrechod)
						if (druhyStaryPPrechod == prvniNovyPPrechod)
							novySoused ^= (1u << prvniStaryPPrechod);
						else
							novySoused ^= (1u << druhyStaryPPrechod);
					else
						novySoused ^= (1u << i);

					permutacePrechodu.at(soused) = novySoused;
					zasobnikPrechodu.push_back(soused);
				}
			}
		}
	}

	return permutacePrechodu;
}



DelkaBitmapy Kgk::dvojkovyLog(DelkaBitmapy cislo)
{
	DelkaBitmapy pozice = 0u;
	for (DelkaBitmapy i = Bitmapa::VELIKOST_SLOVA >> 1u; i != 0u; i >>= 1u)
	{
		if (cislo >= (static_cast<Bitmapa::SlovoBitu>(1u) << i))
		{
			cislo >>= i;
			pozice += i;
		}
	}

	return ((cislo == 0u) ? static_cast<DelkaBitmapy>(-1) : pozice);
}
