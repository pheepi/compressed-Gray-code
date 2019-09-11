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

#ifndef KOMBIG_KGK_H_
#define KOMBIG_KGK_H_

#include <vector>
#include <string>



namespace Bitmapy
{
	typedef unsigned short int DelkaBitmapy;
	class Bitmapa;
	typedef std::vector<Bitmapa> SeznamBitmap;
	typedef SeznamBitmap::size_type DelkaIndexu;
	class BitmapovyIndex;

	class Kgk
	{
	public:
		typedef std::vector<DelkaBitmapy> Prechody;

		static const DelkaBitmapy DELKA_PRVNIHO_KGK = 4u;

		static void vytvorKgk(BitmapovyIndex & bitmapovyIndex,
			DelkaBitmapy n = 0u);

		static bool jeGk(const BitmapovyIndex & bitmapovyIndex);
		static bool jeKgk(const BitmapovyIndex & bitmapovyIndex);

	private:
		static const std::string RETEZCE_KGK_S_1_BITEM[2];
		static const std::string RETEZCE_KGK_S_2_BITY[4];
		static const std::string RETEZCE_KGK_S_3_BITY[8];
		static const std::string RETEZCE_KGK_S_4_BITY[16];

		static const BitmapovyIndex KGK_S_1_BITEM;
		static const BitmapovyIndex KGK_S_2_BITY;
		static const BitmapovyIndex KGK_S_3_BITY;
		static const BitmapovyIndex KGK_S_4_BITY;

		static const DelkaBitmapy opakujiciVKgk4;
		static const DelkaBitmapy prostredniVKgk4;
		static const DelkaBitmapy krajovyVKgk4;
		static const DelkaIndexu poziceHornihoCenteru;
		static const DelkaIndexu poziceDolnihoCenteru;

		explicit Kgk(BitmapovyIndex & bitmapovyIndex, DelkaBitmapy n = 0u);
		explicit Kgk(const Kgk & kgk);
		~Kgk(void);

		const Kgk & operator=(const Kgk & kgk);

		void vytvorKgk(void);
		void vytvorTrivialniKgk(void);
		void zdvojnasobKgk(void);
		void rozsirKgkNaObecneN(void);

		static Prechody vytvorPosloupnostPrechodu(const BitmapovyIndex & gk);
		static Prechody vytvorPomocnouPosloupnost(const Prechody & prechody,
			DelkaBitmapy n);

		static void nahradSegment(BitmapovyIndex & kgk, DelkaIndexu poradi,
			const Bitmapa & prvniBitmap, const Prechody & prechody,
			const Prechody & pomocnaPosloupnost);
		static void nahradSegment(BitmapovyIndex & Kgk, DelkaBitmapy
			opakujici, DelkaBitmapy prostredni, DelkaBitmapy
			krajovy, const Bitmapa & prvniBitmap, bool opakujiciJeNalevo);
		static Prechody ziskejPermutaciNovychPrechodu(DelkaBitmapy
			delkaBitmapy, DelkaBitmapy opakujici, DelkaBitmapy prostredni,
			DelkaBitmapy krajovy);

		static DelkaBitmapy dvojkovyLog(DelkaBitmapy cislo);

		BitmapovyIndex & bitmapovyIndex;
		const DelkaBitmapy n;
	};
}



#endif // KOMBIG_KGK_H_
