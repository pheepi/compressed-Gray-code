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

#include "BitmapovyIndex.h"

#include <vector>
#include <algorithm>

#include "Bitmapa.h"

using namespace Bitmapy;



const DelkaIndexu BitmapovyIndex::MAX_DELKA_INDEXU =
static_cast<DelkaIndexu>(~0u);



BitmapovyIndex::BitmapovyIndex(const DelkaBitmapy n) :
	n(n),
	index(),
	poradi(),
	jePoradiNastaveno(false) {}



BitmapovyIndex::BitmapovyIndex(const BitmapovyIndex & bitmapovyIndex) :
	n(bitmapovyIndex.n),
	index(bitmapovyIndex.index),
	poradi(),
	jePoradiNastaveno(false) {}



BitmapovyIndex::~BitmapovyIndex(void) {}



 BitmapovyIndex & BitmapovyIndex::operator=(const BitmapovyIndex &
											bitmapovyIndex)
{
	if (&bitmapovyIndex != this)
	{
		this->n = bitmapovyIndex.n;
		this->index = bitmapovyIndex.index;
		this->poradi = bitmapovyIndex.poradi;
		this->jePoradiNastaveno = bitmapovyIndex.jePoradiNastaveno;
	}

	return *this;
}



EBitmapy BitmapovyIndex::pridej(const Bitmapa & bitmapa)
{
	if (this->n != bitmapa.n) return EOdlisnaDelkaBitmapy;
	if (this->delka() >= this->MAX_DELKA_INDEXU)
		return EPrekrocenaMaxVelikostIndexu;

	this->index.push_back(bitmapa);
	return EBitmapyOk;
}



EBitmapy BitmapovyIndex::pridej(const BitmapovyIndex & bitmapovyIndex)
{
	if (this->n != bitmapovyIndex.n) return EOdlisnaDelkaBitmapy;
	if (this->delka() > (this->MAX_DELKA_INDEXU - bitmapovyIndex.delka()))
		return EPrekrocenaMaxVelikostIndexu;

	this->index.insert(this->index.end(), bitmapovyIndex.index.begin(),
		bitmapovyIndex.index.end());
	return EBitmapyOk;
}



EBitmapy BitmapovyIndex::pridej(const SeznamBitmap & seznam)
{
	if (this->delka() > (this->MAX_DELKA_INDEXU - static_cast<DelkaIndexu>(
		seznam.size())))
		return EPrekrocenaMaxVelikostIndexu;

	for (SeznamBitmap::const_iterator i = seznam.begin();
		i < seznam.end(); ++i)
	{
		if (this->n != i->n) return EOdlisnaDelkaBitmapy;
		this->index.push_back(*i);
	}

	return EBitmapyOk;
}



EBitmapy BitmapovyIndex::odeber(void)
{
	if (this->n == 0u) return EIndexJePrazdny;
	this->index.pop_back();
	return EBitmapyOk;
}



EBitmapy BitmapovyIndex::bitXor(const Bitmapa & bitmapa)
{
	if (this->n != bitmapa.n) return EOdlisnaDelkaBitmapy;

	for (SeznamBitmap::iterator i = this->index.begin();
		i < this->index.end(); ++i)
	{
		i->bitXor(bitmapa);
	}

	return EBitmapyOk;
}



EBitmapy BitmapovyIndex::pripojPrefix(const Bitmapa & bitmapa)
{
	if (this->n > (Bitmapa::MAX_DELKA_BITMAPU - bitmapa.n))
		return EPrekrocenaMaxVelikostBitmapy;

	for (SeznamBitmap::iterator i = this->index.begin();
		i < this->index.end(); ++i)
	{
		i->pripojPrefix(bitmapa);
	}

	this->n += bitmapa.n;
	return EBitmapyOk;
}



EBitmapy BitmapovyIndex::pripojPostfix(const Bitmapa & bitmapa)
{
	if (this->n > (Bitmapa::MAX_DELKA_BITMAPU - bitmapa.n))
		return EPrekrocenaMaxVelikostBitmapy;

	for (SeznamBitmap::iterator i = this->index.begin();
		i < this->index.end(); ++i)
	{
		i->pripojPostfix(bitmapa);
	}

	this->n += bitmapa.n;
	return EBitmapyOk;
}



EBitmapy BitmapovyIndex::pripojIndex(const BitmapovyIndex & bitmapovyIndex)
{
	if (this->delka() != bitmapovyIndex.delka())
		return EIndexyNejsouStejneDlouhe;

	SeznamBitmap::const_iterator j = bitmapovyIndex.index.begin();
	for (SeznamBitmap::iterator i = this->index.begin();
		i < this->index.end(); ++i)
	{
		i->pripojPostfix(*j);
		++j;
	}

	this->n += bitmapovyIndex.n;
	return EBitmapyOk;
}



void BitmapovyIndex::obrat(void)
{
	std::reverse(this->index.begin(),this->index.end());
}



EBitmapy BitmapovyIndex::rotuj(const DelkaIndexu pozice)
{
	if (pozice >= this->delka()) return EBitmapaMimoRozsah;

	SeznamBitmap::iterator i = this->index.begin();
	std::advance(i, pozice);
	std::rotate(this->index.begin(), i, this->index.end());

	return EBitmapyOk;
}



EBitmapy BitmapovyIndex::prohodSloupce(const DelkaBitmapy prvni,
									   const DelkaBitmapy druha)
{
	if (prvni >= this->n || druha >= this->n)
		return EBitMimoRozsah;

	for (SeznamBitmap::iterator i = this->index.begin();
		i < this->index.end(); ++i)
	{
		i->prohodBity(prvni, druha);
	}

	return EBitmapyOk;
}



void BitmapovyIndex::prohod(BitmapovyIndex & bitmapovyIndex)
{
	std::swap(this->n, bitmapovyIndex.n);
	this->index.swap(bitmapovyIndex.index);
	this->poradi.swap(bitmapovyIndex.poradi);
}



void BitmapovyIndex::vycisti(void)
{
	this->index.clear();
	this->poradi.clear();
}



void BitmapovyIndex::reservujKapacitu(const DelkaIndexu kapacita)
{
	this->index.reserve(kapacita);
}



const Bitmapa * BitmapovyIndex::bitmapa(const DelkaIndexu pozice,
										EBitmapy & e) const
{
	if (pozice >= this->delka())
	{
		e = EBitmapaMimoRozsah;
		return 0;
	}

	return &this->index.at(pozice);
}



DelkaBitmapy BitmapovyIndex::delkaBitmap() const
{
	return this->n;
}



DelkaIndexu BitmapovyIndex::delka(void) const
{
	return static_cast<DelkaIndexu>(this->index.size());
}



BitmapovyIndex::BitmapovyIndex(const DelkaBitmapy n, const std::string *
							   const retezce, const DelkaIndexu pocetRezezcu)
							   : n(n), jePoradiNastaveno(false)
{
	this->index.resize(pocetRezezcu);
	for (DelkaIndexu i = 0u; i < pocetRezezcu; ++i)
	{
		this->index.at(i).inicializaceRetezcem(retezce[i].c_str(), n);
	}
}
