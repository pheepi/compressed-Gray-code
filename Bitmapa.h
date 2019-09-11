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

#ifndef KOMBIG_BITMAPA_H_
#define KOMBIG_BITMAPA_H_

#include <vector>
#include <string>



namespace Bitmapy
{
	typedef unsigned short int DelkaBitmapy;

#if __cplusplus >= 201103L || _MSC_VER >= 1600
	enum EBitmapy : int
#else
	enum EBitmapy
#endif
	{
		EBitmapyOk = 0,
		EPrekrocenaMaxVelikostBitmapy = 1,
		ENeplatnyInicializacniRetezec = 2,
		EBitMimoRozsah = 3,
		EOdlisnaDelkaBitmapy = 4,
		ENeexistujePrechod = 5,
		EPrekrocenaMaxVelikostIndexu = 6,
		EIndexJePrazdny = 7,
		EBitmapaMimoRozsah = 8,
		EIndexyNejsouStejneDlouhe = 9
	};


	class Bitmapa
	{
	public:
		enum ZnakyBitmapy
		{
			NULA = '0',
			JEDNA = '1'
		};


		static const DelkaBitmapy MAX_DELKA_BITMAPU;

		explicit Bitmapa(DelkaBitmapy n = 0u);
		explicit Bitmapa(const std::string & hodnoty);
		explicit Bitmapa(const char * hodnoty, const DelkaBitmapy n);
		explicit Bitmapa(ZnakyBitmapy znak);
		Bitmapa(const Bitmapa & bitmapa);
		~Bitmapa(void);

		Bitmapa & operator=(const Bitmapa & bitmapa);
		bool operator==(const Bitmapa & bitmapa) const;
		bool operator!=(const Bitmapa & bitmapa) const;

		EBitmapy nastavBit(DelkaBitmapy n, bool hodnota);
		EBitmapy invertujBit(DelkaBitmapy n);
		EBitmapy prohodBity(DelkaBitmapy prvni, DelkaBitmapy druhy);
		void bitNot(void);
		EBitmapy bitXor(const Bitmapa & bitmapa);
		DelkaBitmapy dejPrvniPrechod(const Bitmapa & bitmapa,
			EBitmapy & e) const;
		DelkaBitmapy dejPocetJednotek(void) const;
		EBitmapy pripojPrefix(const Bitmapa & bitmapa);
		EBitmapy pripojPostfix(const Bitmapa & bitmapa);
		void prohod(Bitmapa & bitmapa);
		void reservujKapacitu(DelkaBitmapy n);
		static const char * chyboveHlaseni( EBitmapy e);

		bool bit(DelkaBitmapy n, EBitmapy & e) const;
		DelkaBitmapy delka(void) const;
		void retezec(std::string & retezec) const;

		bool jeNulovy(void) const;
		bool jePraveJedenPrechod(const Bitmapa & bitmapa, EBitmapy & e) const;

	private:
		typedef unsigned int SlovoBitu;
		typedef std::vector<SlovoBitu> SeznamSlov;

		static const DelkaBitmapy VELIKOST_SLOVA;
		static const SlovoBitu POSLEDNI_BIT;
		static const unsigned short int POCET_CHYBOVYCH_HLASENI;
		static const char * const CHYBOVA_HLASENI[];

		EBitmapy inicializaceRetezcem(const char * hodnoty, DelkaBitmapy n);
		static DelkaBitmapy vratPocetSlov(DelkaBitmapy n);
		static void pripojPostfix(Bitmapa & bitmapa, const Bitmapa & postfix);
		static bool jePraveJedenPrechod(DelkaBitmapy prvni, DelkaBitmapy druha);

		DelkaBitmapy n;
		SeznamSlov slova;

		friend class BitmapovyIndex;
		friend class Kgk;
		friend class BitmapoveTrideni;
		friend class Komprese;

		friend class VstupBitmap;
	};
}



#endif
