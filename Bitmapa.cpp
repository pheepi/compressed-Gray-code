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

#include "Bitmapa.h"

#include <vector>
#include <string>
#include <climits>

using namespace Bitmapy;



const DelkaBitmapy Bitmapa::MAX_DELKA_BITMAPU =
	static_cast<DelkaBitmapy>(~static_cast<DelkaBitmapy>(0u));



Bitmapa::Bitmapa(DelkaBitmapy n) :
	n(n),
	slova(vratPocetSlov(n), 0u) {}



Bitmapa::Bitmapa(const std::string & hodnoty)
{
	this->inicializaceRetezcem(hodnoty.data(),
		(hodnoty.length() >= this->MAX_DELKA_BITMAPU) ?
		this->MAX_DELKA_BITMAPU : static_cast<DelkaBitmapy>(
		hodnoty.length()));
}



Bitmapa::Bitmapa(const char * hodnoty, DelkaBitmapy n)
{
	this->inicializaceRetezcem(hodnoty, n);
}



Bitmapa::Bitmapa(const ZnakyBitmapy znak) :
	n(1u),
	slova(1u, (znak == JEDNA) ? 1u : 0u) {}



Bitmapa::Bitmapa(const Bitmapa & bitmapa) :
	n(bitmapa.n),
	slova(bitmapa.slova) {}



Bitmapa::~Bitmapa(void) {}



Bitmapa & Bitmapa::operator=(const Bitmapa & bitmapa)
{
	if (this != &bitmapa)
	{
		this->n = bitmapa.n;
		this->slova = bitmapa.slova;
	}

	return *this;
}



bool Bitmapa::operator==(const Bitmapa & bitmap) const
{
	if (this->n != bitmap.n) return false;

	SeznamSlov::const_iterator j = bitmap.slova.begin();
	for (SeznamSlov::const_iterator i = this->slova.begin();
		i < this->slova.end(); ++i)
	{
		if (*i != *j) return false;
		++j;
	}

	return true;
}



bool Bitmapa::operator!=(const Bitmapa & bitmapa) const
{
	return !((*this) == bitmapa);
}



EBitmapy Bitmapa::nastavBit(DelkaBitmapy d, bool hodnota)
{
	if (d >= this->n) return EBitMimoRozsah;

	if (hodnota)
		this->slova.at(d / this->VELIKOST_SLOVA) |=
		static_cast<SlovoBitu>(1u) << (d % this->VELIKOST_SLOVA);
	else
		this->slova.at(d / this->VELIKOST_SLOVA) &=
		static_cast<SlovoBitu>(~(static_cast<SlovoBitu>(1u) <<
		(d % this->VELIKOST_SLOVA)));

	return EBitmapyOk;
}



EBitmapy Bitmapa::invertujBit(DelkaBitmapy d)
{
	if (d >= this->n) return EBitMimoRozsah;

	this->slova.at(d / this->VELIKOST_SLOVA) ^=
		static_cast<SlovoBitu>(1u) << (d % this->VELIKOST_SLOVA);

	return EBitmapyOk;
}



EBitmapy Bitmapa::prohodBity(DelkaBitmapy prvni, DelkaBitmapy druhy)
{
	if ((prvni >= this->n) || (druhy >= this->n)) return EBitMimoRozsah;

	if (((this->slova.at(prvni / this->VELIKOST_SLOVA) >> (prvni %
		this->VELIKOST_SLOVA) ^ this->slova.at(druhy /
		this->VELIKOST_SLOVA) >> (druhy % this->VELIKOST_SLOVA)) &
		static_cast<SlovoBitu>(1u)) != 0u)
	{
		this->slova.at(prvni / this->VELIKOST_SLOVA) ^=
			static_cast<SlovoBitu>(1u) << (prvni % this->VELIKOST_SLOVA);
		this->slova.at(druhy / this->VELIKOST_SLOVA) ^=
			static_cast<SlovoBitu>(1u) << (druhy % this->VELIKOST_SLOVA);
	}

	return EBitmapyOk;
}



void Bitmapa::bitNot(void)
{
	if (this->n == 0u) return;

	for (SeznamSlov::iterator i = this->slova.begin();
		i < this->slova.end(); ++i)
	{
		*i = ~(*i);
	}

	if ((this->n % this->VELIKOST_SLOVA) != 0u)
	{
		this->slova.back() &= (static_cast<SlovoBitu>(1u)
			<< (this->n % this->VELIKOST_SLOVA)) - 1u;
	}
}



EBitmapy Bitmapa::bitXor(const Bitmapa & bitmapa)
{
	if (this->n != bitmapa.n) return EOdlisnaDelkaBitmapy;

	SeznamSlov::const_iterator j = bitmapa.slova.begin();
	for (SeznamSlov::iterator i = this->slova.begin(); i < this->slova.end(); ++i)
	{
		(*i) ^= (*j);
		++j;
	}

	return EBitmapyOk;
}



DelkaBitmapy Bitmapa::dejPrvniPrechod(const Bitmapa & bitmapa,
	EBitmapy & e) const
{
	if (this->n != bitmapa.n)
	{
		e = EOdlisnaDelkaBitmapy;
		return this->n;
	}

	SeznamSlov::const_iterator j = bitmapa.slova.begin();
	for (SeznamSlov::const_iterator i = this->slova.begin();
		i < this->slova.end(); ++i)
	{
		if ((*i) != (*j))
		{
			SlovoBitu jenPrechody = *i;
			jenPrechody ^= *j;
			jenPrechody ^= jenPrechody - 1u;
			DelkaBitmapy posun = this->VELIKOST_SLOVA;
			posun >>= 1u;
			DelkaBitmapy pozice = 0u;

			for (; posun > 0u; posun >>= 1u)
			{
				if (jenPrechody >= static_cast<SlovoBitu>(1u) << posun)
				{
					jenPrechody >>= posun;
					pozice += posun;
				}
			}

			return (static_cast<DelkaBitmapy>(i - this->slova.begin()) *
				this->VELIKOST_SLOVA) + pozice;
		}

		++j;
	}

	e = ENeexistujePrechod;
	return this->n;
}



DelkaBitmapy Bitmapa::dejPocetJednotek(void) const
{
	DelkaBitmapy pocetJednotek = 0u;

	for (SeznamSlov::const_iterator i = this->slova.begin();
		i < this->slova.end(); ++i)
	{
		SlovoBitu slovo = *i;

		for (; slovo != 0u; ++pocetJednotek)
		{
			slovo &= slovo - 1u;
		}
	}

	return pocetJednotek;
}



EBitmapy Bitmapa::pripojPrefix(const Bitmapa & bitmapa)
{
	if (this->n > (this->MAX_DELKA_BITMAPU - bitmapa.n))
		return EPrekrocenaMaxVelikostBitmapy;

	Bitmapa novaBitmapa;
	const DelkaBitmapy kapacita = this->n + bitmapa.n;
	novaBitmapa.reservujKapacitu(kapacita);
	this->pripojPostfix(novaBitmapa, bitmapa);
	this->pripojPostfix(novaBitmapa, *this);
	novaBitmapa.prohod(*this);

	return EBitmapyOk;
}



EBitmapy Bitmapa::pripojPostfix(const Bitmapa & bitmapa)
{
	if (this->n > (this->MAX_DELKA_BITMAPU - bitmapa.n))
		return EPrekrocenaMaxVelikostBitmapy;

	this->pripojPostfix(*this, bitmapa);

	return EBitmapyOk;
}



void Bitmapa::prohod(Bitmapa & bitmapa)
{
	this->slova.swap(bitmapa.slova);
	std::swap(this->n, bitmapa.n);
}



void Bitmapa::reservujKapacitu(DelkaBitmapy d)
{
	this->slova.reserve(this->vratPocetSlov(d));
}



const char * Bitmapa::chyboveHlaseni(EBitmapy e)
{
	return CHYBOVA_HLASENI[static_cast<size_t>(e)];
}



DelkaBitmapy Bitmapa::delka(void) const
{
	return this->n;
}



bool Bitmapa::bit(DelkaBitmapy d, EBitmapy & e) const
{
	if (d >= this->n)
	{
		e = EBitMimoRozsah;
		return false;
	}

	return ((this->slova.at(d / this->VELIKOST_SLOVA) &
		(static_cast<SlovoBitu>(1u) <<
		(d % this->VELIKOST_SLOVA))) == 0u) ? false : true;
}



void Bitmapa::retezec(std::string & retezec) const
{
	retezec.resize(this->n, static_cast<char>(this->NULA));
	DelkaBitmapy pozice = 0u;

	for (SeznamSlov::const_iterator i = this->slova.begin();
		i < this->slova.end(); ++i)
	{
		DelkaBitmapy delkaSlova = ((this->n - pozice) <
			this->VELIKOST_SLOVA) ? (this->n - pozice) : this->VELIKOST_SLOVA;

		for (DelkaBitmapy posun = 0u; posun < delkaSlova; ++posun)
		{
			if (((*i) & (static_cast<SlovoBitu>(1u) << posun)) == 0u)
				retezec.at(pozice) = static_cast<char>(this->NULA);
			else
				retezec.at(pozice) = static_cast<char>(this->JEDNA);

			++pozice;
		}
	}
}



bool Bitmapa::jeNulovy(void) const
{
	for (SeznamSlov::const_iterator i = this->slova.begin();
		i < this->slova.end(); ++i)
	{
		if (*i != 0u) return false;
	}
	return true;
}



bool Bitmapa::jePraveJedenPrechod(const Bitmapa & bitmapa, EBitmapy & e) const
{
	if (this->n != bitmapa.n)
	{
		e = EOdlisnaDelkaBitmapy;
		return false;
	}

	
	SeznamSlov::const_iterator j = bitmapa.slova.begin();
	bool nalezenPrechod = false;
	for (SeznamSlov::const_iterator i = this->slova.begin();
		i < this->slova.end(); ++i)
	{
		SlovoBitu jenPrechody = *i;
		jenPrechody ^= *j;
		if (jenPrechody != 0u)
		{
			if ((jenPrechody & (jenPrechody - 1u)) == 0u)
			{
				if (nalezenPrechod)
					return false;
				else
					nalezenPrechod = true;
			}
			else
				return false;
		}

		++j;
	}

	return nalezenPrechod;
}



const DelkaBitmapy Bitmapa::VELIKOST_SLOVA = static_cast<DelkaBitmapy>(
	sizeof(SlovoBitu) * static_cast<DelkaBitmapy>(CHAR_BIT));
const Bitmapa::SlovoBitu Bitmapa::POSLEDNI_BIT =
static_cast<Bitmapa::SlovoBitu>(1u) << (VELIKOST_SLOVA - 1u);
const unsigned short int Bitmapa::POCET_CHYBOVYCH_HLASENI = 10;
const char * const Bitmapa::CHYBOVA_HLASENI[] =
{
	"Bitmapy jsou v poradku",
	"Prekrocena maximalni velikost bitmapy",
	"Neplatny inicializacni retezec pro vytvoreni bitmapy",
	"Pozice bitu bitmapu je mimo rozsah",
	"Delky bitmap jsou odlisne",
	"Neexistuje prechod mezi bitmapami",
	"Prekrocena maximalni velikost indexu",
	"Index je prazdny",
	"Pozice bitmapy indexu je mimo rozsah",
	"Indexy nejsou stejne dlouhe"
};



EBitmapy Bitmapa::inicializaceRetezcem(const char * hodnoty, DelkaBitmapy hodnotaN)
{
	this->n = hodnotaN;
	this->slova = SeznamSlov(this->vratPocetSlov(this->n), 0u);

	SeznamSlov::iterator slovo = this->slova.begin();
	SlovoBitu bit = static_cast<SlovoBitu>(1u);

	for (DelkaBitmapy i = 0u; i < this->n; ++i)
	{
		switch (hodnoty[i])
		{
		case NULA:
			break;
		case JEDNA:
			(*slovo) |= bit;
			break;
		default:
			return ENeplatnyInicializacniRetezec;
		}

		if (bit == POSLEDNI_BIT)
		{
			++slovo;
			bit = static_cast<SlovoBitu>(1u);
		}
		else
			bit <<= static_cast<SlovoBitu>(1u);
	}

	return EBitmapyOk;
}



DelkaBitmapy Bitmapa::vratPocetSlov(DelkaBitmapy n)
{
	return (((n % VELIKOST_SLOVA) == 0u) ? 0u : 1u) +
		(n / VELIKOST_SLOVA);
}



void Bitmapa::pripojPostfix(Bitmapa & bitmapa, const Bitmapa & postfix)
{
	const DelkaBitmapy zbyleBity = bitmapa.n % VELIKOST_SLOVA;

	if (zbyleBity == 0u)
	{
		bitmapa.slova.insert(bitmapa.slova.end(), postfix.slova.begin(),
			postfix.slova.end());
	}
	else
	{
		bitmapa.slova.reserve(bitmapa.slova.size() + postfix.slova.size());

		for (SeznamSlov::const_iterator i = postfix.slova.begin();
			i < postfix.slova.end(); ++i)
		{
			bitmapa.slova.back() |= static_cast<SlovoBitu>((*i) << zbyleBity);
			bitmapa.slova.push_back(0u);
			bitmapa.slova.back() |= static_cast<SlovoBitu>((*i) >>
				(VELIKOST_SLOVA - zbyleBity));
		}

		if (((postfix.n % VELIKOST_SLOVA) + zbyleBity) <= VELIKOST_SLOVA)
			bitmapa.slova.pop_back();
	}

	bitmapa.n += postfix.n;
}



bool Bitmapa::jePraveJedenPrechod(DelkaBitmapy prvni, DelkaBitmapy druha)
{
	DelkaBitmapy jenPrechody = prvni;
	jenPrechody ^= druha;
	return (((jenPrechody & (jenPrechody - 1u)) == 0u)
		&& (jenPrechody != 0u));
}
