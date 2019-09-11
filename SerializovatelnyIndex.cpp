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

#include "SerializovatelnyIndex.h"

#include <sstream>
#include <cstdlib>
#include <ctime>

#include "ZaznamovyFormat.h"
#include "Bitmapa.h"
#include "BitmapovyIndex.h"
#include "Trideni.h"

using namespace Bitmapy;
using namespace IOPodpora;



SerializovatelnyIndex::SerializovatelnyIndex(DelkaBitmapy n, size_t velikostBufferu,
	const std::fstream::openmode druhPristupu) :
	BitmapovyIndex(n),
	ZaznamovyFormat(velikostBufferu, druhPristupu) {}



SerializovatelnyIndex::~SerializovatelnyIndex(void) {}



DelkaIndexu VstupBitmap::dejMaximumBitmapu(void) const
{
	return this->maximumBitmapu;
}



void VstupBitmap::nastavMaximumBitmapu(DelkaIndexu hodnotaMaximumBitmapu)
{
	this->maximumBitmapu = hodnotaMaximumBitmapu;
}



bool VstupBitmap::setridSoubor(VystupBitmap & vystup,
	const BitmapoveTrideni & trideni, EBitmapy & e)
{
	if (this->n != vystup.n)
	{
		e = EOdlisnaDelkaBitmapy;
		return false;
	}

	if (!this->nastavCteni())
		return false;

	if (!vystup.nastavZapis())
		return false;

	DelkaIndexu pocetSetridenychCasti = 0;
	DelkaIndexu velikostBloku = this->maximumBitmapu;

	ObalDocasnehoSouboru docasnyVstupniPrvni;
	ObalDocasnehoSouboru docasnyVstupniDruhy;
	ObalDocasnehoSouboru docasnyVystupniPrvni;
	ObalDocasnehoSouboru docasnyVystupniDruhy;

	e = this->nacti();
	if (e != EBitmapyOk)
		return false;

	trideni.vytvorPoradiProKgk(*this);
	trideni.setrid(*this);

	if (!this->jeVstupDostupny())
	{
		this->prohod(vystup);
		vystup.zapis();
		return true;
	}
	else
	{
		if (!docasnyVystupniPrvni.vytvorDocasnySoubor(
			new BitmapovyUniversal(velikostBloku, this->n)))
			return false;
		if (!docasnyVystupniDruhy.vytvorDocasnySoubor(
			new BitmapovyUniversal(velikostBloku, this->n)))
			return false;

		BitmapovyUniversal & docasnyIndexPrvni =
			*docasnyVystupniPrvni.dejIndex();
		BitmapovyUniversal & docasnyIndexDruhy =
			*docasnyVystupniDruhy.dejIndex();

		if (trideni.dejUsporadani() == BitmapoveTrideni::Kgk)
		{
			docasnyIndexPrvni.jePoradiNastaveno = true;
			docasnyIndexDruhy.jePoradiNastaveno = true;
		}

		this->prohod(docasnyIndexPrvni);
		docasnyIndexPrvni.zapis();
		++pocetSetridenychCasti;

		while (this->jeVstupDostupny())
		{
			e = this->nacti();
			if (e != EBitmapyOk)
				return false;

			trideni.vytvorPoradiProKgk(*this);
			trideni.setrid(*this);

			BitmapovyUniversal & docasny = ((pocetSetridenychCasti % 2)
				== 0) ? docasnyIndexPrvni : docasnyIndexDruhy;

			this->prohod(docasny);
			docasny.zapis();
			++pocetSetridenychCasti;
		}

		docasnyIndexPrvni.zapisKonec();
		docasnyIndexDruhy.zapisKonec();
		docasnyIndexPrvni.presunSeNaZacatek();
		docasnyIndexDruhy.presunSeNaZacatek();

		if (pocetSetridenychCasti > 2)
		{
			if (!docasnyVstupniPrvni.vytvorDocasnySoubor(
				new BitmapovyUniversal(velikostBloku, this->n)))
				return false;
			if (!docasnyVstupniDruhy.vytvorDocasnySoubor(
				new BitmapovyUniversal(velikostBloku, this->n)))
				return false;

			if (trideni.dejUsporadani() == BitmapoveTrideni::Kgk)
			{
				docasnyVstupniPrvni.dejIndex()->jePoradiNastaveno = true;
				docasnyVstupniDruhy.dejIndex()->jePoradiNastaveno = true;
			}
		}
	}

	while (pocetSetridenychCasti > 2)
	{
		docasnyVystupniPrvni.prohod(docasnyVstupniPrvni);
		docasnyVystupniDruhy.prohod(docasnyVstupniDruhy);

		pocetSetridenychCasti = (pocetSetridenychCasti - 1u) / 2u + 1u;
		for (DelkaIndexu i = 0u; i < pocetSetridenychCasti; ++i)
		{
			if ((i % 2u) == 0u)
				docasnyVystupniPrvni.dejIndex()->slij(
				*docasnyVstupniPrvni.dejIndex(),
				*docasnyVstupniDruhy.dejIndex(), trideni, velikostBloku);
			else
				docasnyVystupniDruhy.dejIndex()->slij(
				*docasnyVstupniPrvni.dejIndex(),
				*docasnyVstupniDruhy.dejIndex(), trideni, velikostBloku);
		}

		docasnyVstupniPrvni.dejIndex()->presunSeNaZacatek();
		docasnyVstupniDruhy.dejIndex()->presunSeNaZacatek();
		docasnyVystupniPrvni.dejIndex()->zapisKonec();
		docasnyVystupniDruhy.dejIndex()->zapisKonec();
		docasnyVystupniPrvni.dejIndex()->presunSeNaZacatek();
		docasnyVystupniDruhy.dejIndex()->presunSeNaZacatek();

		velikostBloku *= 2u;
	}

	vystup.slij(*docasnyVystupniPrvni.dejIndex(),
		*docasnyVystupniDruhy.dejIndex(), trideni, velikostBloku);

	return true;
}



EBitmapy VstupBitmap::nacti(void)
{
	EBitmapy e = EBitmapyOk;
	if (!this->nastavCteni())
		return e;

	for (DelkaIndexu i = this->delka(); i < this->maximumBitmapu; ++i)
	{
		if (!this->nactiBitmapu(e))
		{
			return e;
		}
		else
		{
			if (e != EBitmapyOk)
				return e;
		}
	}

	this->odstranPrazdneZnaky();
	return e;
}



EBitmapy VstupBitmap::nacti(const DelkaIndexu pocet)
{
	if (pocet < this->maximumBitmapu)
	{
		const DelkaIndexu puvodniMaximumBitmapu = this->maximumBitmapu;
		this->maximumBitmapu = pocet;
		const EBitmapy e = this->nacti();
		this->maximumBitmapu = puvodniMaximumBitmapu;
		return e;
	}
	else
		return this->nacti();
}



bool VstupBitmap::zjistiDelkuBitmap(void)
{
	if (!this->nastavCteni())
		return false;

	if (!this->jeVstupDostupny())
		return false;

	char znak = static_cast<char>(this->proud.sgetc());
	std::stringstream zaznam;

	while ((znak != NAVRAT_VOZIKU) && (znak != NOVY_RADEK) &&
		(this->proud.sgetc() != EOF))
	{
		zaznam.put(znak);
		znak = static_cast<char>(this->proud.snextc());
	}

	zaznam >> this->n;
	this->odstranPrazdneZnaky();

	return (!zaznam.fail() && (this->n <= Bitmapa::MAX_DELKA_BITMAPU));
}



bool VstupBitmap::zjistiDelkuIndexu(DelkaIndexu & delka, EBitmapy & e)
{
	if (!this->nastavCteni())
		return false;

	while (this->nactiBitmapu(e))
	{
		if (e != EBitmapyOk)
			return false;

		this->vycisti();
		++delka;
	}

	return true;
}



bool VstupBitmap::jeVstupDostupny(void)
{
	if (!this->nastavCteni())
		return false;

	this->odstranPrazdneZnaky();

	return (this->proud.sgetc() != EOF) &&
		(static_cast<char>(this->proud.sgetc()) != this->UMELY_KONEC_SOUBORU);
}



VstupBitmap::VstupBitmap(const DelkaIndexu maximumBitmapu)
	: maximumBitmapu(maximumBitmapu) {}



VstupBitmap::~VstupBitmap(void) {}



bool VstupBitmap::nactiBitmapu(EBitmapy & e)
{
	if (!this->jeVstupDostupny())
		return false;

	char znak = static_cast<char>(this->proud.sgetc());

	this->index.push_back(Bitmapa(this->n));
	Bitmapa & bitmapa = this->index.back();
	Bitmapa::SeznamSlov::iterator slovo = bitmapa.slova.begin();

	Bitmapa::SlovoBitu bit = static_cast<Bitmapa::SlovoBitu>(1u);
	for (DelkaBitmapy i = 0; i < bitmapa.n; ++i)
	{
		if (this->proud.sgetc() == EOF)
		{
			this->index.pop_back();
			e = EOdlisnaDelkaBitmapy;
			return false;
		}

		switch (znak)
		{
		case Bitmapa::NULA:
			break;
		case Bitmapa::JEDNA:
			(*slovo) |= bit;
			break;
		default:
			this->index.pop_back();
			e = ENeplatnyInicializacniRetezec;
			return false;
		}

		if (bit == Bitmapa::POSLEDNI_BIT)
		{
			++slovo;
			bit = static_cast<Bitmapa::SlovoBitu>(1u);
		}
		else
			bit <<= static_cast<Bitmapa::SlovoBitu>(1u);

		znak = static_cast<char>(this->proud.snextc());
	}

	if (this->jePoradiNastaveno)
		this->nactiPoradi();

	znak = static_cast<char>(this->proud.sgetc());
	if ((znak != NAVRAT_VOZIKU) && (znak != NOVY_RADEK) &&
		(this->proud.sgetc() != EOF))
	{
		this->index.pop_back();
		e = EOdlisnaDelkaBitmapy;
		return false;
	}

	return true;
}



void VstupBitmap::nactiPoradi(void)
{
	char znak = static_cast<char>(this->proud.snextc());
	std::stringstream zaznam;

	while ((znak != NAVRAT_VOZIKU) && (znak != NOVY_RADEK) &&
		(this->proud.sgetc() != EOF))
	{
		zaznam.put(znak);
		znak = static_cast<char>(this->proud.snextc());
	}

	DelkaIndexu delka;
	zaznam >> delka;
	this->poradi.push_back(delka);
}



void VstupBitmap::odstranPrazdneZnaky(void)
{
	char znak = static_cast<char>(this->proud.sgetc());

	while (((znak == NAVRAT_VOZIKU) || (znak == NOVY_RADEK)) &&
		(this->proud.sgetc() != EOF))
	{
		znak = static_cast<char>(this->proud.snextc());
	}
}



void VystupBitmap::zapis(void)
{
	if (!this->nastavZapis())
		return;

	std::string retezec;
	for (DelkaIndexu i = 0; i < this->delka(); ++i)
	{
		this->index.at(i).retezec(retezec);
		this->proud.sputn(retezec.c_str(), retezec.size());
		if (this->jePoradiNastaveno)
			this->zapisPoradi(this->poradi.at(i));

		this->proud.sputc(NOVY_RADEK);
	}

	this->vycisti();
}



bool VystupBitmap::zapisDelkuBitmap(void)
{
	if (!this->nastavZapis())
		return false;

	std::stringstream zaznam;
	zaznam << this->n;
	this->proud.sputn(zaznam.str().c_str(), zaznam.str().size());
	this->proud.sputc(NOVY_RADEK);
	return true;
}



VystupBitmap::VystupBitmap(void) {}



VystupBitmap::~VystupBitmap(void) {}



EBitmapy VystupBitmap::slij(VstupBitmap & prvniVstup, VstupBitmap & druhyVstup,
							const BitmapoveTrideni & trideni,
							const DelkaIndexu velikostBloku)
{
	EBitmapy e = EBitmapyOk;
	DelkaIndexu prvniKZapsani = velikostBloku;
	DelkaIndexu druhyKZapsani = velikostBloku;

	if ((e = prvniVstup.nacti(prvniKZapsani)) != EBitmapyOk)
		return e;

	if ((e = druhyVstup.nacti(druhyKZapsani)) != EBitmapyOk)
		return e;

	SeznamBitmap::const_iterator pozicePrvni = prvniVstup.index.begin();
	SeznamBitmap::const_iterator poziceDruha = druhyVstup.index.begin();

	bool jsouObaVstupyNeprazdne = !prvniVstup.index.empty() &&
		!druhyVstup.index.empty();
	while (jsouObaVstupyNeprazdne)
	{
		DelkaIndexu poradiPrvni = 0;
		DelkaIndexu poradiDruhe = 0;
		if (trideni.dejUsporadani() == BitmapoveTrideni::Kgk)
		{
			poradiPrvni = prvniVstup.poradi.at(pozicePrvni
				- prvniVstup.index.begin());
			poradiDruhe = druhyVstup.poradi.at(poziceDruha
				- druhyVstup.index.begin());
		}

		if ((trideni.dejUsporadani() == BitmapoveTrideni::Kgk)
			? (poradiPrvni < poradiDruhe)
			: trideni.porovnej(*pozicePrvni, *poziceDruha))
		{
			this->zapisBitmapu(*pozicePrvni, poradiPrvni);
			--prvniKZapsani;
			if (prvniKZapsani <= 0)
				jsouObaVstupyNeprazdne = false;
			else
			{
				++pozicePrvni;
				if (pozicePrvni >= prvniVstup.index.end())
				{
					prvniVstup.vycisti();
					if (prvniVstup.jeVstupDostupny())
					{
						if ((e = prvniVstup.nacti(prvniKZapsani))
							!= EBitmapyOk)
							return e;

						pozicePrvni = prvniVstup.index.begin();
					}
					else
					{
						prvniKZapsani = 0;
						jsouObaVstupyNeprazdne = false;
					}
				}
			}
		}
		else
		{
			this->zapisBitmapu(*poziceDruha, poradiDruhe);
			--druhyKZapsani;
			if (druhyKZapsani <= 0)
				jsouObaVstupyNeprazdne = false;
			else
			{
				++poziceDruha;
				if (poziceDruha >= druhyVstup.index.end())
				{
					druhyVstup.vycisti();
					if (druhyVstup.jeVstupDostupny())
					{
						if ((e = druhyVstup.nacti(druhyKZapsani)) !=
							EBitmapyOk)
							return e;

						poziceDruha = druhyVstup.index.begin();
					}
					else
					{
						druhyKZapsani = 0;
						jsouObaVstupyNeprazdne = false;
					}
				}
			}
		}
	}

	VstupBitmap * zbyvajiciVstup;
	SeznamBitmap::const_iterator pozice;
	DelkaIndexu zbytekKZapsani;
	if (prvniKZapsani > 0)
	{
		zbytekKZapsani = prvniKZapsani;
		zbyvajiciVstup = &prvniVstup;
		pozice = pozicePrvni;
	}
	else
	{
		zbytekKZapsani = druhyKZapsani;
		zbyvajiciVstup = &druhyVstup;
		pozice = poziceDruha;
	}

	for (; zbytekKZapsani > 0; --zbytekKZapsani)
	{
		if (pozice >= zbyvajiciVstup->index.end())
		{
			if (zbyvajiciVstup->jeVstupDostupny())
			{
				zbyvajiciVstup->vycisti();
				if ((e = zbyvajiciVstup->nacti(zbytekKZapsani)) != EBitmapyOk)
					return e;

				pozice = zbyvajiciVstup->index.begin();
			}
			else
				break;
		}

		DelkaIndexu zbyvajiciPoradi = 0;
		if (trideni.dejUsporadani() == BitmapoveTrideni::Kgk)
			zbyvajiciPoradi = zbyvajiciVstup->poradi.at(pozice - zbyvajiciVstup->index.begin());		// TODO: tohle je spatne, tady se ma vzit pozice (uz je to spravne:)

		this->zapisBitmapu(*pozice, zbyvajiciPoradi);
		++pozice;
	}

	prvniVstup.vycisti();
	druhyVstup.vycisti();
	return e;
}



void VystupBitmap::zapisBitmapu(const Bitmapa & mapa, const DelkaIndexu hodnotaPoradi)
{
	std::string retezec;
	mapa.retezec(retezec);
	this->proud.sputn(retezec.c_str(), retezec.size());
	if (this->jePoradiNastaveno)
		this->zapisPoradi(hodnotaPoradi);

	this->proud.sputc(NOVY_RADEK);
}



void VystupBitmap::zapisPoradi(const DelkaIndexu indexPoradi)
{
	this->proud.sputc(ODDELOVAC_PORADI);
	std::stringstream retezec;
	retezec << indexPoradi;
	this->proud.sputn(retezec.str().c_str(), retezec.str().size());
}



void VystupBitmap::zapisKonec(void)
{
	this->proud.sputc(this->UMELY_KONEC_SOUBORU);
	this->proud.sputc(NOVY_RADEK);
}



BitmapovyTokenizer::BitmapovyTokenizer(DelkaIndexu maximumBitmapu,
	DelkaBitmapy n, size_t velikostBufferu) :
	SerializovatelnyIndex(n, velikostBufferu, std::fstream::in),
	VstupBitmap(maximumBitmapu)
{
	this->modPristupu = Cteni;
}



BitmapovyTokenizer::~BitmapovyTokenizer(void) {}



BitmapovyWriter::BitmapovyWriter(DelkaBitmapy n, size_t velikostBufferu)
	: SerializovatelnyIndex(n, velikostBufferu,
		std::fstream::out | std::fstream::trunc),
	VystupBitmap()
{
	this->modPristupu = Zapis;
}



BitmapovyWriter::~BitmapovyWriter(void) {}



BitmapovyUniversal::BitmapovyUniversal(DelkaIndexu maximumBitmapu,
	DelkaBitmapy n, size_t velikostBufferu) :
	SerializovatelnyIndex(n, velikostBufferu,
		std::fstream::in | std::fstream::out),
	VstupBitmap(maximumBitmapu),
	VystupBitmap() {}



BitmapovyUniversal::~BitmapovyUniversal(void) {}



ObalDocasnehoSouboru::ObalDocasnehoSouboru(void) :
	index(0) {}



ObalDocasnehoSouboru::ObalDocasnehoSouboru(BitmapovyUniversal * index,
	const std::string & jmenoSouboru) :
	index(index),
	jmenoSouboru(jmenoSouboru) {}



ObalDocasnehoSouboru::~ObalDocasnehoSouboru(void)
{
	this->odstranDocasnySoubor();
}


BitmapovyUniversal * ObalDocasnehoSouboru::dejIndex(void) const
{
	return this->index;
}



bool ObalDocasnehoSouboru::vytvorDocasnySoubor(BitmapovyUniversal * universal)
{
	this->odstranDocasnySoubor();

	if (universal != 0)
	{
		if (vytvorNepouziteJmenoSouboru(jmenoSouboru))
		{
			{
				std::ofstream vytvarenySoubor;
				vytvarenySoubor.open(jmenoSouboru.c_str());
				vytvarenySoubor.close();
			}

			if (universal->otevriSoubor(jmenoSouboru.c_str()))
			{
				this->index = universal;
			}
			else
			{
				delete universal;
				jmenoSouboru.clear();
				return false;
			}
		}
		else
		{
			delete universal;
			return false;
		}
	}
	else
		return false;

	return true;
}



void ObalDocasnehoSouboru::odstranDocasnySoubor(void)
{
	if (this->index != 0)
	{
		this->index->zavriSoubor();
		delete this->index;
		this->index = 0;
	}

	if (!this->jmenoSouboru.empty())
	{
		remove(this->jmenoSouboru.c_str());
		this->jmenoSouboru.clear();
	}
}



void ObalDocasnehoSouboru::prohod(ObalDocasnehoSouboru & soubor)
{
	std::swap(this->index, soubor.index);
	this->jmenoSouboru.swap(soubor.jmenoSouboru);
}



bool ObalDocasnehoSouboru::vytvorNepouziteJmenoSouboru(std::string &
													   jmenoSouboru)
{
	srand(static_cast<unsigned int>(time(NULL)));
	static const char ZNAKY[] = "0123456789abcdef";
	static const short unsigned int POCET_MOZNYCH_ZNAKU =
		sizeof(ZNAKY) / sizeof(ZNAKY[0]) - 1;

	static const char * prefix = "KomBIG_tempfile_";
	static long unsigned int POSTFIX_SEMINKO = time(NULL) %
		(~(static_cast<long unsigned int>(0u)));
	static const short unsigned int DELKA_POSTFIXU =
		sizeof(long unsigned int) * 2;

	static const unsigned short int MAX_POCET_POKUSU = 100;

	unsigned short int cisloPokusu = 0;

	while (cisloPokusu < MAX_POCET_POKUSU)
	{
		long unsigned int seminko = POSTFIX_SEMINKO;
		jmenoSouboru.clear();
		jmenoSouboru.append(prefix);

		for (short unsigned int i = 0; i < DELKA_POSTFIXU; ++i)
		{
			jmenoSouboru.push_back(ZNAKY[seminko % POCET_MOZNYCH_ZNAKU]);
			seminko /= POCET_MOZNYCH_ZNAKU;
		}

		POSTFIX_SEMINKO = static_cast<unsigned long int>(
			POSTFIX_SEMINKO * rand() + 1);
		POSTFIX_SEMINKO = static_cast<unsigned long int>(
			POSTFIX_SEMINKO * POSTFIX_SEMINKO + 1);

		std::ifstream tempSoubor;
		tempSoubor.open(jmenoSouboru.c_str());
		if (tempSoubor.is_open())
		{
			tempSoubor.close();
			++cisloPokusu;
		}
		else
		{
			break;
		}
	}

	if (cisloPokusu >= MAX_POCET_POKUSU)
	{
		jmenoSouboru.clear();
		return false;
	}

	return true;
}
