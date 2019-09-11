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

#ifndef KOMBIG_NASTAVENI_VYPOCTU_H_
#define KOMBIG_NASTAVENI_VYPOCTU_H_

#include <vector>
#include <string>

#include "Bitmapa.h"
#include "Trideni.h"

using namespace Bitmapy;



namespace Bitmapy
{
	class VstupBitmap;
	class VystupBitmap;
}



namespace IOPodpora
{
	enum ChybaVstupu
	{
		CZadneArgumenty,
		CNeniPlatnyPrepinac,
		CPrepinacJeNadbytecny,
		CPrepinacPouzityVicekrat,
		CChybneZadanyVstup,
		CChybneZadanyVystup,
		CChybneZadaneTrideni,
		CNelzeRozpoznatTrideni,
		CNezadanaDelkaKgk,
		CDelkaKgkMusiBytCislo,
		CChybiSloupceStatistiky,
		CNeznamyPrepinac,
		CNeniZadanyVstup,
		CNeniZadanyVystup,
		CNeniZadanaAkce,
		CVstupNelzeOtevrit,
		CVystupNelzeOtevrit,
		CNevytvorenDocasnySoubor,
		CNezjistenaDelkaBitmap,
		CSpatnyFormatZaznamu,
		CNerozpoznanSloupecStatistiky
	};

	enum VlastnostBitmap
	{
		Delka = 0,
		Pocet = 1,
		Velikost = 2
	};

	class CsvVystup;

	typedef std::vector<unsigned short int> SloupceStatistiky;

	class NastaveniVypoctu
	{
	public:
		explicit NastaveniVypoctu(void);
		explicit NastaveniVypoctu(const std::string & vstupniSoubor,
			const std::string & vystupniSoubor, const BitmapoveTrideni::Usporadani
			usporadani, const bool komprimovat);

		static bool zpracuj(const std::vector<std::string> & argumenty,
			NastaveniVypoctu & nastaveniVypoctu, SloupceStatistiky & sloupce);
		bool proved(DelkaIndexu * velikostDat = 0);
		bool provedStatistiku(const SloupceStatistiky & sloupce);

	private:
		static const char ZNAK_PROMENNE = '$';
		static const char * const VLASTNOSTI_BITMAP[];
		static const unsigned short int POCET_VLASTNOSTI_BITMAP;
		static const char * const CHYBOVA_HLASENI[];
		static const unsigned short int POCET_CHYBOVYCH_HLASENI;

		NastaveniVypoctu(const NastaveniVypoctu & nastaveniVypoctu);
		const NastaveniVypoctu & operator=(const NastaveniVypoctu &
			nastaveniVypoctu);

		static bool zkontrolujUsporadani(const std::string & parametr,
			BitmapoveTrideni::Usporadani & usporadani);
		bool setrid(VstupBitmap & vstup, VystupBitmap & vystup);
		bool komprimuj(VstupBitmap & vstup, std::ofstream & vystup) const;
		bool vypocti(VstupBitmap & vstup, DelkaIndexu & velikostDat) const;
		static bool zkontrolujSloupecStatistiky(const std::string & sloupec,
			SloupceStatistiky & sloupce);
		static bool zjistitVlastnostiBitmap(const VlastnostBitmap vlastnost,
			VstupBitmap & vstup, CsvVystup & vystup);
		static void vypsatHlavickuStatistiky(CsvVystup & vystup,
			const SloupceStatistiky & sloupce);

		static void vypisNapovedu();
		static void vypisChybu(const ChybaVstupu chyba);
		static void vypisChybu(const ChybaVstupu chyba,
			const std::string & retezec);
		static void vypisChybu(const ChybaVstupu chyba, const char znak);
		static void vypisChybu(const EBitmapy e);

		std::string vstupniSoubor;
		std::string vystupniSoubor;
		Bitmapy::BitmapoveTrideni trideni;
		bool komprimovat;
	};
}



#endif // KOMBIG_NASTAVENI_VYPOCTU_H_
