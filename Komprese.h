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

#ifndef KOMBIG_KOMPRESE_H_
#define KOMBIG_KOMPRESE_H_

#include <fstream>

#include "Bitmapa.h"




namespace Bitmapy
{
	class VstupBitmap;
	class BitmapovyIndex;

	typedef std::vector<Bitmapa> SeznamBitmap;
	typedef SeznamBitmap::size_type DelkaIndexu;

	class Komprese
	{
	public:
		static bool komprimuj(VstupBitmap & vstup, std::ofstream & vystup,
			EBitmapy & e);
		static bool vypocti(VstupBitmap & vstup, DelkaIndexu & velikostDat,
			EBitmapy & e);

	private:
		static const Bitmapa::SlovoBitu ZERO_FILL = 0u;
		static const Bitmapa::SlovoBitu ONE_FILL;
		static const char MEZERA = ' ';
		static const std::string PRAZDNE_SLOVO;

		explicit Komprese(VstupBitmap & vstup, std::ofstream & vystup);
		explicit Komprese(const Komprese & komprese);
		~Komprese(void);

		const Komprese & operator=(const Komprese & komprese);

		bool komprimuj(EBitmapy & e);

		static Bitmapa::SlovoBitu vytvorSlovo(const BitmapovyIndex & index,
			const DelkaBitmapy sloupec);
		void pridejSlovo(Bitmapa::SlovoBitu slovo, bool jeAktivni = false);
		void zapisFillWord(void);
		void resetujObsah(void);

		VstupBitmap & vstup;
		std::ofstream & vystup;

		bool jeFillWord;
		bool fillBit;
		Bitmapa::SlovoBitu delkaFillWord;
	};
}



#endif // KOMBIG_KOMPRESE_H_
