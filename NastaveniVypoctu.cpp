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

#include "NastaveniVypoctu.h"

#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <iostream>

#include "CsvPodpora.h"
#include "SerializovatelnyIndex.h"
#include "Kgk.h"
#include "Komprese.h"

using namespace IOPodpora;
using namespace Bitmapy;



NastaveniVypoctu::NastaveniVypoctu(void) :
	trideni(BitmapoveTrideni::Zadne),
	komprimovat(false) {}



NastaveniVypoctu::NastaveniVypoctu(const std::string & vstupniSoubor,
	const std::string & vystupniSoubor,
	const BitmapoveTrideni::Usporadani usporadani, const bool komprimovat) :
	vstupniSoubor(vstupniSoubor),
	vystupniSoubor(vystupniSoubor),
	trideni(usporadani),
	komprimovat(komprimovat) {}



bool NastaveniVypoctu::zpracuj(const std::vector<std::string> & argumenty,
	NastaveniVypoctu & nastaveniVypoctu, SloupceStatistiky & sloupce)
{
	if (argumenty.empty())
	{
		vypisChybu(CZadneArgumenty);
		return false;
	}

	if (argumenty.size() == 1)
	{
		vypisNapovedu();
		return false;
	}

	std::set<char> prectenePrepinace;
	std::set<char> nadbytecnePrepinace;
	bool generovatKgk = false;
	DelkaBitmapy n = 0u;

	for (std::vector<std::string>::const_iterator i = argumenty.begin() + 1u;
		i < argumenty.end(); ++i)
	{
		if (i->size() != 2u || i->at(0) != '-')
		{
			vypisChybu(CNeniPlatnyPrepinac, *i);
			return false;
		}

		if(nadbytecnePrepinace.find(i->at(1)) != nadbytecnePrepinace.end())
		{
			vypisChybu(CPrepinacJeNadbytecny, *i);
			return false;
		}

		if (prectenePrepinace.find(i->at(1)) == prectenePrepinace.end())
			prectenePrepinace.insert(i->at(1));
		else
		{
			vypisChybu(CPrepinacPouzityVicekrat, *i);
			return false;
		}

		switch (i->at(1u))
		{
		case 'i': 
			if ((i + 1u) >= argumenty.end())
			{
				vypisChybu(CChybneZadanyVstup);
				return false;
			}
			nastaveniVypoctu.vstupniSoubor = *(i + 1u);
			nadbytecnePrepinace.insert('g');
			++i;
			break;
		case 'o':
			if ((i + 1u) >= argumenty.end())
			{
				vypisChybu(CChybneZadanyVystup);
				return false;
			}
			nastaveniVypoctu.vystupniSoubor = *(i + 1u);
			++i;
			break;
		case 'r':
			if ((i + 1u) >= argumenty.end())
			{
				vypisChybu(CChybneZadaneTrideni);
				return false;
			}

			BitmapoveTrideni::Usporadani usporadani;
			if (zkontrolujUsporadani(*(i + 1u), usporadani))
			{
				nastaveniVypoctu.trideni.nastavUsporadani(usporadani);
			}
			else
			{
				vypisChybu(CNelzeRozpoznatTrideni, *(i + 1u));
				return false;
			}

			nadbytecnePrepinace.insert('s');
			nadbytecnePrepinace.insert('g');
			++i;
			break;
		case 'c':
			nastaveniVypoctu.komprimovat = true;
			nadbytecnePrepinace.insert('s');
			nadbytecnePrepinace.insert('g');
			break;
		case 'g':
			if ((i + 1u) >= argumenty.end())
			{
				vypisChybu(CNezadanaDelkaKgk);
				return false;
			}

			{
				std::stringstream cislo(*(i + 1u));
				cislo >> n;
				if (cislo.fail())
				{
					vypisChybu(CDelkaKgkMusiBytCislo);
					return false;
				}
			}

			generovatKgk = true;
			nadbytecnePrepinace.insert('i');
			nadbytecnePrepinace.insert('c');
			nadbytecnePrepinace.insert('r');
			nadbytecnePrepinace.insert('s');
			++i;
			break;
		case 's':
			if ((i + 1u) >= argumenty.end())
			{
				vypisChybu(CChybiSloupceStatistiky);
				return false;
			}

			++i;
			for (; i < argumenty.end(); ++i)
			{
				if (!zkontrolujSloupecStatistiky(*i, sloupce))
					return false;
			}
			nastaveniVypoctu.komprimovat = true;
			--i;
			break;
		case 'h':
			if (argumenty.size() == 2u)
				vypisNapovedu();
			else
			{
				vypisChybu(CPrepinacJeNadbytecny);
				return false;
			}
			return false;
			break;
		default:
			vypisChybu(CNeznamyPrepinac, i->at(1));
			return false;
			break;
		}
	}

	if (prectenePrepinace.find('o') == prectenePrepinace.end())
	{
		vypisChybu(CNeniZadanyVystup);
		return false;
	}

	if ((prectenePrepinace.find('i') == prectenePrepinace.end()) &&
		(prectenePrepinace.find('g') == prectenePrepinace.end()))
	{
		vypisChybu(CNeniZadanyVstup);
		return false;
	}
	else
	{
		if (prectenePrepinace.find('i') != prectenePrepinace.end())
		{
			if ((prectenePrepinace.find('c') == prectenePrepinace.end()) &&
				(prectenePrepinace.find('r') == prectenePrepinace.end()) &&
				(prectenePrepinace.find('s') == prectenePrepinace.end()))
			{
				vypisChybu(CNeniZadanaAkce);
				return false;
			}
		}
		else
		{
			BitmapovyWriter vystup(n);
			if (vystup.otevriSoubor(nastaveniVypoctu.vystupniSoubor.c_str()))
			{
				Kgk::vytvorKgk(vystup, n);
				vystup.zapis();
				vystup.zavriSoubor();
				return false;
			}
			else
			{
				vypisChybu(CVystupNelzeOtevrit);
				return false;
			}
		}
	}

	return true;
}



bool NastaveniVypoctu::proved(DelkaIndexu * velikostDat)
{
	if ((this->trideni.dejUsporadani() == BitmapoveTrideni::Zadne)
		&& !this->komprimovat)
	{
		vypisChybu(CNeniZadanaAkce);
		return false;
	}

	BitmapovyTokenizer vstup;
	if (!vstup.otevriSoubor(this->vstupniSoubor.c_str()))
	{
		vypisChybu(CVstupNelzeOtevrit);
		return false;
	}

	bool jeVypocetUspesny = false;
	if (this->komprimovat)
	{
		if (velikostDat == NULL)
		{
			std::ofstream komprimovanySoubor;
			komprimovanySoubor.open(this->vystupniSoubor.c_str());
			if (komprimovanySoubor.good())
			{
				if (this->trideni.dejUsporadani() == BitmapoveTrideni::Zadne)
					jeVypocetUspesny = this->komprimuj(vstup, komprimovanySoubor);
				else
				{
					if (vstup.zjistiDelkuBitmap())
					{
						ObalDocasnehoSouboru docasnySoubor;
						if (docasnySoubor.vytvorDocasnySoubor(new
							BitmapovyUniversal(VstupBitmap::
							MINIMALNI_MAXIMUM_BITMAP, vstup.delkaBitmap())))
						{
							jeVypocetUspesny = this->setrid(vstup,
								*docasnySoubor.dejIndex());
							if (jeVypocetUspesny)
							{
								docasnySoubor.dejIndex()->presunSeNaZacatek();
								jeVypocetUspesny = this->komprimuj(
									*docasnySoubor.dejIndex(),
									komprimovanySoubor);
							}
						}
						else
							vypisChybu(CNevytvorenDocasnySoubor);
					}
					else
						vypisChybu(CNezjistenaDelkaBitmap);
				}
				komprimovanySoubor.close();
			}
			else
				vypisChybu(CVystupNelzeOtevrit);
		}
		else
		{
			if (this->trideni.dejUsporadani() == BitmapoveTrideni::Zadne)
				jeVypocetUspesny = this->vypocti(vstup, *velikostDat);
			else
			{
				if (vstup.zjistiDelkuBitmap())
				{
					ObalDocasnehoSouboru docasnySoubor;
					if (docasnySoubor.vytvorDocasnySoubor(new
						BitmapovyUniversal(VstupBitmap::
						MINIMALNI_MAXIMUM_BITMAP, vstup.delkaBitmap())))
					{
						jeVypocetUspesny = this->setrid(vstup,
							*docasnySoubor.dejIndex());
						if (jeVypocetUspesny)
						{
							docasnySoubor.dejIndex()->presunSeNaZacatek();
							jeVypocetUspesny = this->vypocti(
								*docasnySoubor.dejIndex(), *velikostDat);
						}
					}
					else
						vypisChybu(CNevytvorenDocasnySoubor);
				}
				else
					vypisChybu(CNezjistenaDelkaBitmap);
			}
		}
	}
	else
	{
		if (vstup.zjistiDelkuBitmap())
		{
			BitmapovyWriter vystup(vstup.delkaBitmap());
			if (vystup.otevriSoubor(this->vystupniSoubor.c_str()))
			{
				jeVypocetUspesny = this->setrid(vstup, vystup);
				vystup.zavriSoubor();
			}
			else
				vypisChybu(CVystupNelzeOtevrit);
		}
		else
			vypisChybu(CNezjistenaDelkaBitmap);
	}

	vstup.zavriSoubor();
	return jeVypocetUspesny;
}



bool NastaveniVypoctu::provedStatistiku(const SloupceStatistiky & sloupce)
{
	CsvTokenizer csvTokenizer;
	if (!csvTokenizer.otevriSoubor(this->vstupniSoubor.c_str()))
	{
		vypisChybu(CVstupNelzeOtevrit);
		return false;
	}

	CsvWriter csvWriter;
	if (!csvWriter.otevriSoubor(this->vystupniSoubor.c_str()))
	{
		vypisChybu(CVystupNelzeOtevrit);
		csvTokenizer.zavriSoubor();
		return false;
	}
	vypsatHlavickuStatistiky(csvWriter, sloupce);

	NastaveniVypoctu nastaveniStatistiky = *this;

	Zaznam zaznam;
	while (csvTokenizer.dejZaznam(zaznam))
	{
		if (zaznam.size() != 1u)
		{
			vypisChybu(CSpatnyFormatZaznamu);
			continue;
		}

		this->vstupniSoubor = zaznam.at(0);
		csvWriter.zapisUvozenouPolozku(this->vstupniSoubor);

		for (SloupceStatistiky::const_iterator i = sloupce.begin();
			i < sloupce.end(); ++i)
		{
			if (*i < BitmapoveTrideni::POCET_DRUHU_USPORADANI)
			{
				this->trideni.nastavUsporadani(
					static_cast<BitmapoveTrideni::Usporadani>(*i));

				DelkaIndexu velikostDat = 0u;
				if (this->proved(&velikostDat))
				{
					csvWriter.zapisCislo(velikostDat);
				}
			}
			else
			{
				if (*i < (BitmapoveTrideni::POCET_DRUHU_USPORADANI
					+ POCET_VLASTNOSTI_BITMAP))
				{
					BitmapovyTokenizer vstup;
					if (vstup.otevriSoubor(this->vstupniSoubor.c_str()))
						zjistitVlastnostiBitmap(static_cast<VlastnostBitmap>(
						*i - BitmapoveTrideni::POCET_DRUHU_USPORADANI),
						vstup, csvWriter);
					else
						vypisChybu(CVstupNelzeOtevrit);

					vstup.zavriSoubor();
				}
				else
					vypisChybu(CNerozpoznanSloupecStatistiky, "");
			}
			csvWriter.synchronizuj();
		}

		csvWriter.ukonciZaznam();
	}

	csvWriter.zavriSoubor();
	csvTokenizer.zavriSoubor();
	*this = nastaveniStatistiky;
	return true;
}



const char * const NastaveniVypoctu::VLASTNOSTI_BITMAP[] =
	{"Delka", "Pocet", "Velikost"};
const unsigned short int NastaveniVypoctu::POCET_VLASTNOSTI_BITMAP
	= sizeof(VLASTNOSTI_BITMAP) / sizeof(*(VLASTNOSTI_BITMAP));
const char * const NastaveniVypoctu::CHYBOVA_HLASENI[] =
{
	"Nejsou poskytnuty argumenty prikazove radky",
	"Parametr \"$\" neni platny prepinac",
	"Prepinac \"$\" je nadbytecny",
	"Prepinac \"$\" je pouzity vicekrat",
	"Po prepinaci -i je treba zadat vstupni soubor",
	"Po prepinaci -o je treba zadat vystupni soubor",
	"Po prepinaci -r je treba zadat typ trideni",
	"Nelze rozpoznat typ trideni \"$\"",
	"Pro vytvoreni SGK je treba zadat delku bitmapu",
	"Parametr u -g musi byt cislo",
	"Po prepinaci -s je treba zadat seznam parametru",
	"Neznamy prepinac \"$\"",
	"Neni zadany zadny vstup",
	"Neni zadany zadny vystup",
	"Neni zadana zadna akce",
	"Vstupni soubor nelze otevrit pro cteni",
	"Vystupni soubor nelze otevrit pro zapis",
	"Nelze vytvorit docasny soubor pro mezivypocty",
	"V souboru neni zapsana platna delka bitmap",
	"Zaznam v souboru statistiky je ve spatnem formatu",
	"Jedna z pozadovanych statistik nebyla rozpoznana: \"$\""
};
const unsigned short int NastaveniVypoctu::POCET_CHYBOVYCH_HLASENI
	= sizeof(CHYBOVA_HLASENI) / sizeof(*(CHYBOVA_HLASENI));



NastaveniVypoctu::NastaveniVypoctu(const NastaveniVypoctu & nastaveniVypoctu) :
	vstupniSoubor(nastaveniVypoctu.vstupniSoubor),
	vystupniSoubor(nastaveniVypoctu.vystupniSoubor),
	trideni(nastaveniVypoctu.trideni.dejUsporadani()),
	komprimovat(nastaveniVypoctu.komprimovat) {}



const NastaveniVypoctu & NastaveniVypoctu::operator=(
	const NastaveniVypoctu & nastaveniVypoctu)
{
	if (&nastaveniVypoctu != this)
	{
		this->vstupniSoubor = nastaveniVypoctu.vstupniSoubor;
		this->vystupniSoubor = nastaveniVypoctu.vystupniSoubor;
		this->trideni.nastavUsporadani(nastaveniVypoctu.trideni.dejUsporadani());
		this->komprimovat = nastaveniVypoctu.komprimovat;
	}

	return *this;
}



bool NastaveniVypoctu::zkontrolujUsporadani(
	const std::string & parametr, BitmapoveTrideni::Usporadani & usporadani)
{
	for (int j = 0; j < BitmapoveTrideni::POCET_DRUHU_USPORADANI; ++j)
	{
		if (BitmapoveTrideni::DRUHY_USPORADANI[j] == parametr)
		{
			usporadani = static_cast<BitmapoveTrideni::Usporadani>(j);
			return true;
		}
	}

	return false;
}



bool NastaveniVypoctu::setrid(VstupBitmap & vstup,
	VystupBitmap & vystup)
{
	BitmapovyIndex slovnik;
	if (trideni.dejUsporadani() == BitmapoveTrideni::Kgk)
	{
		Kgk::vytvorKgk(slovnik, vstup.delkaBitmap());
		this->trideni.nastavSlovnik(&slovnik);
	}
	else
		trideni.nastavSlovnik(0);

	vystup.zapisDelkuBitmap();
	EBitmapy e = EBitmapyOk;
	if (!vstup.setridSoubor(vystup, trideni, e))
	{
		if (e == EBitmapyOk)
			vypisChybu(CNevytvorenDocasnySoubor);
		else
			vypisChybu(e);

		trideni.nastavSlovnik(0);
		return false;
	}

	trideni.nastavSlovnik(0);
	return true;
}



bool NastaveniVypoctu::komprimuj(VstupBitmap & vstup,
	std::ofstream & vystup) const
{
	EBitmapy e = EBitmapyOk;
	if (!Komprese::komprimuj(vstup, vystup, e))
	{
		if (e == EBitmapyOk)
			vypisChybu(CNezjistenaDelkaBitmap);
		else
			vypisChybu(e);

		return false;
	}

	return true;
}





bool NastaveniVypoctu::vypocti(VstupBitmap & vstup,
	DelkaIndexu & velikostDat) const
{
	EBitmapy e = EBitmapyOk;
	if (!Komprese::vypocti(vstup, velikostDat, e))
	{
		if (e == EBitmapyOk)
			vypisChybu(CNezjistenaDelkaBitmap);
		else
			vypisChybu(e);

		return false;
	}

	return true;
}





bool NastaveniVypoctu::zkontrolujSloupecStatistiky(
	const std::string & sloupec, SloupceStatistiky & sloupce)
{
	BitmapoveTrideni::Usporadani usporadani;
	if (zkontrolujUsporadani(sloupec, usporadani))
	{
		sloupce.push_back(static_cast<unsigned short int>(usporadani));
		return true;
	}
	else
	{
		for (unsigned short int i = 0u; i < POCET_VLASTNOSTI_BITMAP; ++i)
		{
			if (VLASTNOSTI_BITMAP[i] == sloupec)
			{
				sloupce.push_back(
					BitmapoveTrideni::POCET_DRUHU_USPORADANI + i);
				return true;
			}
		}
	}

	vypisChybu(CNerozpoznanSloupecStatistiky, sloupec);
	return false;
}



bool NastaveniVypoctu::zjistitVlastnostiBitmap(
	const VlastnostBitmap vlastnost, VstupBitmap & vstup, CsvVystup & vystup)
{
	if (vstup.zjistiDelkuBitmap())
	{
		DelkaIndexu delkaIndexu = 0u;
		EBitmapy e = EBitmapyOk;
		switch (vlastnost)
		{
		case Delka:
			vystup.zapisCislo(vstup.delkaBitmap());
			break;
		case Pocet:
			vstup.zjistiDelkuIndexu(delkaIndexu, e);
			if (e == EBitmapyOk)
				vystup.zapisCislo(delkaIndexu);
			else
			{
				vypisChybu(e);
				return false;
			}
			break;
		case Velikost:
			vstup.zjistiDelkuIndexu(delkaIndexu, e);
			if (e == EBitmapyOk)
				vystup.zapisCislo(vstup.delkaBitmap() * delkaIndexu);
			else
			{
				vypisChybu(e);
				return false;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		vypisChybu(CNezjistenaDelkaBitmap);
		return false;
	}

	return true;
}



void NastaveniVypoctu::vypsatHlavickuStatistiky(CsvVystup & vystup,
	const SloupceStatistiky & sloupce)
{
	vystup.zapisPolozku(Polozka());
	for (SloupceStatistiky::const_iterator i = sloupce.begin();
		i < sloupce.end(); ++i)
	{
		if (*i < BitmapoveTrideni::POCET_DRUHU_USPORADANI)
		{
			vystup.zapisPolozku(BitmapoveTrideni::DRUHY_USPORADANI[*i]);
		}
		else
		{
			if (*i < BitmapoveTrideni::POCET_DRUHU_USPORADANI
				+ POCET_VLASTNOSTI_BITMAP)
			{
				vystup.zapisPolozku(VLASTNOSTI_BITMAP[*i
					- BitmapoveTrideni::POCET_DRUHU_USPORADANI]);
			}
		}
	}

	vystup.ukonciZaznam();
}



void NastaveniVypoctu::vypisNapovedu(void)
{
	std::cout << "kombig [-i infile] -o outfile [-c] [-r order] " <<
		"[-g N] [-s (order | property) ...] [-h]" << std::endl <<
		"  -i\tVstupni soubor" << std::endl <<
		"  -o\tVystupni soubor" << std::endl <<
		"  -c\tKomprese pomoci WAH" << std::endl <<
		"  -r\tTyp trideni" << std::endl <<
		"  -s\tVytvoreni statistiky" << std::endl <<
		"  -g\tVygenerovani KGK s bitmapami delky N" << std::endl <<
		"  -h\tNapoveda" << std::endl;
}



void NastaveniVypoctu::vypisChybu(ChybaVstupu chyba)
{
	std::cerr << CHYBOVA_HLASENI[static_cast<size_t>(chyba)] << std::endl;
}



void NastaveniVypoctu::vypisChybu(const ChybaVstupu chyba,
								  const std::string & retezec)
{
	std::string chybovaHlaska(CHYBOVA_HLASENI[static_cast<size_t>(chyba)]);
	const size_t pozicePromenne = chybovaHlaska.find(ZNAK_PROMENNE);
	if (pozicePromenne != std::string::npos)
	{
		chybovaHlaska.replace(pozicePromenne, 1, retezec);
		std::cerr << chybovaHlaska << std::endl;
	}
	else
		vypisChybu(chyba);
}



void NastaveniVypoctu::vypisChybu(const ChybaVstupu chyba, const char znak)
{
	std::string chybovaHlaska(CHYBOVA_HLASENI[static_cast<size_t>(chyba)]);
	const size_t pozicePromenne = chybovaHlaska.find(ZNAK_PROMENNE);
	if (pozicePromenne != std::string::npos)
	{
		chybovaHlaska.at(pozicePromenne) = znak;
		std::cerr << chybovaHlaska << std::endl;
	}
	else
		vypisChybu(chyba);
}



void NastaveniVypoctu::vypisChybu(const EBitmapy e)
{
	std::cerr << Bitmapa::chyboveHlaseni(e) << std::endl;
}
