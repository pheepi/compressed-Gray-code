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

#include "Trideni.h"

#include <vector>
#include <algorithm>

#include "Bitmapa.h"
#include "BitmapovyIndex.h"

using namespace Bitmapy;



const char * const BitmapoveTrideni::DRUHY_USPORADANI[]
= {"Zadne", "Lexikograficke", "Zgk", "Kgk"};
const unsigned short int BitmapoveTrideni::POCET_DRUHU_USPORADANI
	= sizeof(DRUHY_USPORADANI) / sizeof(*(DRUHY_USPORADANI));



BitmapoveTrideni::BitmapoveTrideni(Usporadani usporadani) :
	usporadani(usporadani),
	porovnani(NULL),
	trideni(NULL),
	slovnik(NULL)
{
	this->nastavUsporadani(this->usporadani);
}



BitmapoveTrideni::Usporadani BitmapoveTrideni::dejUsporadani(void) const
{
	return this->usporadani;
}



void BitmapoveTrideni::nastavUsporadani(Usporadani noveUsporadani)
{
	this->usporadani = noveUsporadani;

	switch (this->usporadani)
	{
	case Zadne:
		this->porovnani = NULL;
		this->trideni = NULL;
		break;
	case Lexikograficke:
		this->porovnani = this->porovnejLexikograficky;
		this->trideni = this->setridLexikograficky;
		break;
	case Zgk:
		this->porovnani = this->porovnejZgk;
		this->trideni = this->setridZgk;
		break;
	case Kgk:
		this->porovnani = this->porovnejKgk;
		this->trideni = this->setridKgk;
		break;
	default:
		break;
	}
}



void BitmapoveTrideni::nastavSlovnik(const BitmapovyIndex * hodnotaSlovniku)
{
	this->slovnik = hodnotaSlovniku;
}



bool BitmapoveTrideni::porovnej(const Bitmapa & prvni,
	const Bitmapa & druhy) const
{
	return (*this->porovnani)(prvni, druhy);
}



void BitmapoveTrideni::setrid(BitmapovyIndex & bitmapovyIndex) const
{
	(*this->trideni)(bitmapovyIndex);
}



BitmapoveTrideni::TrideniKgk::TrideniKgk(const PoradiBitmap & poradi) :
	poradi(poradi) {}



BitmapoveTrideni::TrideniKgk::TrideniKgk(const TrideniKgk & trideni) :
	poradi(trideni.poradi) {}



BitmapoveTrideni::TrideniKgk::~TrideniKgk(void) {}



bool BitmapoveTrideni::TrideniKgk::operator()(DelkaIndexu prvni,
	DelkaIndexu druha) const
{
	return (poradi.at(prvni) < poradi.at(druha));
}



void BitmapoveTrideni::vytvorPoradiProKgk(
	BitmapovyIndex & bitmapovyIndex) const
{
	if (this->usporadani == Kgk)
	{
		bitmapovyIndex.poradi.clear();
		if (this->slovnik != 0)
		{
			for (SeznamBitmap::const_iterator i
				= bitmapovyIndex.index.begin();
				i < bitmapovyIndex.index.end(); ++i)
			{
				SeznamBitmap::const_iterator pozice = std::find(
					this->slovnik->index.begin(),
					this->slovnik->index.end(), *i);

				bitmapovyIndex.poradi.push_back(pozice
					- this->slovnik->index.begin());
			}
		}
	}
}



bool BitmapoveTrideni::porovnejLexikograficky(const Bitmapa & prvni,
	const Bitmapa & druhy)
{
	if (prvni.n != druhy.n) return (prvni.n < druhy.n);

	Bitmapa::SeznamSlov::const_iterator j = druhy.slova.begin();
	for (Bitmapa::SeznamSlov::const_iterator i = prvni.slova.begin();
		i < prvni.slova.end(); ++i)
	{
		if (*i != *j)
		{
			Bitmapa::SlovoBitu rozdily = *i;
			rozdily ^= *j;
			return ((((rozdily - 1) ^ rozdily) & rozdily) & (*i)) == 0;
		}
		++j;
	}

	return false;
}



bool BitmapoveTrideni::porovnejZgk(const Bitmapa & prvni,
	const Bitmapa & druhy)
{
	if (prvni.n != druhy.n) return (prvni.n < druhy.n);

	Bitmapa::SeznamSlov::const_iterator j = druhy.slova.begin();
	Bitmapa::SlovoBitu paritniSlovo = 0u;

	for (Bitmapa::SeznamSlov::const_iterator i = prvni.slova.begin(); i < prvni.slova.end(); ++i)
	{
		if (*i == *j)
			paritniSlovo ^= *i;
		else
		{
			Bitmapa::SlovoBitu rozdily = *i;
			rozdily ^= *j;
			paritniSlovo ^= (((rozdily - 1) ^ rozdily) & (*i));

			for (DelkaBitmapy posun = Bitmapa::VELIKOST_SLOVA; posun > 1u; )
			{
				posun -= (posun >> 1u);
				paritniSlovo ^= (paritniSlovo >> posun);
			}

			return (((paritniSlovo & 1u) == 0u) ? true : false);
		}
		++j;
	}

	return false;
}



bool BitmapoveTrideni::porovnejKgk(const Bitmapa & prvni,
	const Bitmapa & druhy)
{
	(void)prvni;
	(void)druhy;

	return false;
}



void BitmapoveTrideni::setridLexikograficky(BitmapovyIndex & bitmapovyIndex)
{
	std::sort(bitmapovyIndex.index.begin(), bitmapovyIndex.index.end(),
		porovnejLexikograficky);
}



void BitmapoveTrideni::setridZgk(BitmapovyIndex & bitmapovyIndex)
{
	std::sort(bitmapovyIndex.index.begin(), bitmapovyIndex.index.end(),
		porovnejZgk);
}



void BitmapoveTrideni::setridKgk(BitmapovyIndex & bitmapovyIndex)
{
	std::vector<DelkaIndexu> poradi;
	poradi.reserve(bitmapovyIndex.poradi.size());
	for (DelkaIndexu i = 0u; i < bitmapovyIndex.poradi.size(); ++i)
	{
		poradi.push_back(i);
	}

	const TrideniKgk noveTrideni(bitmapovyIndex.poradi);
	std::sort(poradi.begin(), poradi.end(), noveTrideni);

	for (DelkaIndexu i = 0u; i < poradi.size(); ++i)
	{
		if (poradi.at(i) != i)
		{
			DelkaIndexu p = i;
			for (; poradi.at(p) != poradi.at(poradi.at(p));
				std::swap(p, poradi.at(p)))
			{
				std::swap(bitmapovyIndex.index.at(p),
					bitmapovyIndex.index.at(poradi.at(p)));
				std::swap(bitmapovyIndex.poradi.at(p),
					bitmapovyIndex.poradi.at(poradi.at(p)));
			}

			poradi.at(p) = p;
		}
	}
}
