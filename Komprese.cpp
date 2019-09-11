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

#include "Komprese.h"

#include <fstream>

#include "ZaznamovyFormat.h"
#include "Bitmapa.h"
#include "SerializovatelnyIndex.h"

using namespace Bitmapy;



bool Komprese::komprimuj(VstupBitmap & vstup, std::ofstream & vystup,
						 EBitmapy & e)
{
	Komprese komprese(vstup, vystup);
	return komprese.komprimuj(e);
}



bool Komprese::vypocti(VstupBitmap & vstup,
	DelkaIndexu & velikostDat, EBitmapy & e)
{
	vstup.vycisti();
	vstup.nastavMaximumBitmapu(Bitmapa::VELIKOST_SLOVA - 1u);

	if (!vstup.zjistiDelkuBitmap())
		return false;

	std::vector<bool> jeFillWord(vstup.delkaBitmap(), false);
	std::vector<bool> fillBit(vstup.delkaBitmap(), false);

	velikostDat = 0u;
	while (vstup.jeVstupDostupny())
	{
		if ((e = vstup.nacti()) != EBitmapyOk)
			return false;

		for (DelkaBitmapy i = 0u; i < vstup.delkaBitmap(); ++i)
		{
			const Bitmapa::SlovoBitu slovo = vytvorSlovo(vstup, i);

			if (((slovo != ZERO_FILL) && (slovo != ONE_FILL)) ||
				static_cast<DelkaBitmapy>(vstup.delka()) <
				(Bitmapa::VELIKOST_SLOVA - 1u))
			{
				if (jeFillWord.at(i))
				{
					jeFillWord.at(i) = false;
					++velikostDat;
				}

				++velikostDat;
			}
			else
			{
				if (jeFillWord.at(i))
				{
					if ((slovo != ZERO_FILL) ^ (fillBit.at(i)))
					{	
						fillBit.at(i) = !fillBit.at(i);
						++velikostDat;
					}
				}
				else
				{
					jeFillWord.at(i) = true;
					fillBit.at(i) = (slovo != ZERO_FILL);
				}
			}
		}

		vstup.vycisti();
	}

	for (DelkaBitmapy i = 0u; i < vstup.delkaBitmap(); ++i)
	{
		if (jeFillWord.at(i))
			++velikostDat;
	}

	velikostDat *= Bitmapa::VELIKOST_SLOVA;
	return true;
}



const Bitmapa::SlovoBitu Komprese::ONE_FILL =
(static_cast<Bitmapa::SlovoBitu>(1u) << (Bitmapa::VELIKOST_SLOVA - 1u)) - 1u;
const std::string Komprese::PRAZDNE_SLOVO(Bitmapa::VELIKOST_SLOVA, MEZERA);



Komprese::Komprese(VstupBitmap & vstup, std::ofstream & vystup) :
	vstup(vstup),
	vystup(vystup),
	jeFillWord(false),
	fillBit(false),
	delkaFillWord(0u) {}



Komprese::~Komprese(void) {}



bool Komprese::komprimuj(EBitmapy & e)
{
	this->vstup.vycisti();
	this->vstup.nastavMaximumBitmapu(Bitmapa::VELIKOST_SLOVA - 1u);

	if (!this->vstup.zjistiDelkuBitmap())
		return false;

	this->resetujObsah();
	for (DelkaBitmapy i = 0u; i < this->vstup.delkaBitmap(); ++i)
	{
		while (this->vstup.jeVstupDostupny())
		{
			if ((e = this->vstup.nacti()) != EBitmapyOk)
				return false;

			Bitmapa::SlovoBitu slovo = vytvorSlovo(this->vstup, i);

			this->pridejSlovo(slovo, static_cast<DelkaBitmapy>(
				this->vstup.delka()) < (Bitmapa::VELIKOST_SLOVA - 1u));

			this->vstup.vycisti();
		}

		if (this->jeFillWord)
			this->zapisFillWord();

		this->vystup.put(IOPodpora::ZaznamovyFormat::NAVRAT_VOZIKU);
		this->vystup.put(IOPodpora::ZaznamovyFormat::NOVY_RADEK);
		this->vstup.presunSeNaZacatek();
		this->vstup.zjistiDelkuBitmap();
	}

	return true;
}



Bitmapa::SlovoBitu Komprese::vytvorSlovo(const BitmapovyIndex & index,
	DelkaBitmapy sloupec)
{
	Bitmapa::SlovoBitu slovo = 0u;
	EBitmapy e;
	for (DelkaIndexu j = 0u; j < index.delka(); ++j)
	{
		if (index.bitmapa(j, e)->bit(sloupec, e))
			slovo |= static_cast<Bitmapa::SlovoBitu>(1u) << j;
	}

	return slovo;
}



void Komprese::pridejSlovo(Bitmapa::SlovoBitu slovo, bool jeAktivni)
{
	if ((slovo != ZERO_FILL && slovo != ONE_FILL) || jeAktivni)
	{
		if (this->jeFillWord)
			this->zapisFillWord();

		this->vystup.put(static_cast<char>(Bitmapa::NULA));
		for (DelkaBitmapy i = 0u; i < Bitmapa::VELIKOST_SLOVA - 1u; ++i)
		{
			this->vystup.put(((slovo & (static_cast<Bitmapa::SlovoBitu>(1u)
				<< i)) == 0u) ? static_cast<char>(Bitmapa::NULA)
				: static_cast<char>(Bitmapa::JEDNA));
		}

		this->vystup.put(MEZERA);
	}
	else
	{
		if (this->jeFillWord)
		{
			if ((slovo != ZERO_FILL) ^ (this->fillBit))
			{
				bool zalozniFillBit = !this->fillBit;
				this->zapisFillWord();
				this->jeFillWord = true;
				this->fillBit = zalozniFillBit;
				this->delkaFillWord = 1u;
			}
			else
				++this->delkaFillWord;
		}
		else
		{
			this->jeFillWord = true;
			this->fillBit = (slovo != ZERO_FILL);
			this->delkaFillWord = 1u;
		}
	}
}



void Komprese::zapisFillWord(void)
{
	this->vystup.put(static_cast<char>(Bitmapa::JEDNA));
	this->vystup.put((this->fillBit) ? static_cast<char>(Bitmapa::JEDNA)
		: static_cast<char>(Bitmapa::NULA));

	std::string slovo(Bitmapa::VELIKOST_SLOVA - 2, Bitmapa::NULA);
	Bitmapa::SlovoBitu delkaSlova = this->delkaFillWord;

	for (std::string::reverse_iterator i = slovo.rbegin();
		i < slovo.rend(); ++i)
	{
		if ((delkaSlova & 1u) != 0u)
			*i = Bitmapa::JEDNA;

		delkaSlova >>= 1u;

		if (delkaSlova == 0u)
			break;
	}

	this->vystup.write(slovo.c_str(), slovo.size());
	this->vystup.put(MEZERA);

	for (Bitmapa::SlovoBitu i = 0u; i < this->delkaFillWord - 1u; ++i)
	{
		this->vystup.write(this->PRAZDNE_SLOVO.c_str(),
			this->PRAZDNE_SLOVO.size());
		this->vystup.put(MEZERA);
	}

	this->resetujObsah();
}



void Komprese::resetujObsah()
{
	this->jeFillWord = false;
	this->fillBit = false;
	this->delkaFillWord = 0u;
}
