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

#ifndef KOMBIG_BITMAPOVY_INDEX_H_
#define KOMBIG_BITMAPOVY_INDEX_H_

#include <vector>

#include "Bitmapa.h"



namespace Bitmapy
{
	typedef std::vector<Bitmapa> SeznamBitmap;
	typedef SeznamBitmap::size_type DelkaIndexu;
	typedef std::vector<DelkaIndexu> PoradiBitmap;

	class BitmapovyIndex
	{
	public:
		static const DelkaIndexu MAX_DELKA_INDEXU;

		explicit BitmapovyIndex(DelkaBitmapy n = 0u);
		explicit BitmapovyIndex(const BitmapovyIndex & bitmapovyIndex);
		~BitmapovyIndex(void);

		BitmapovyIndex & operator=(const BitmapovyIndex & bitmapovyIndex);

		EBitmapy pridej(const Bitmapa & bitmapa);
		EBitmapy pridej(const BitmapovyIndex & bitmapovyIndex);
		EBitmapy pridej(const SeznamBitmap & seznam);
		EBitmapy odeber(void);
		EBitmapy bitXor(const Bitmapa & bitmapa);
		EBitmapy pripojPrefix(const Bitmapa & bitmapa);
		EBitmapy pripojPostfix(const Bitmapa & bitmapa);
		EBitmapy pripojIndex(const BitmapovyIndex & bitmapovyIndex);
		void obrat(void);
		EBitmapy rotuj(DelkaIndexu pozice);
		EBitmapy prohodSloupce(DelkaBitmapy prvni, DelkaBitmapy druha);
		void prohod(BitmapovyIndex & bitmapovyIndex);
		void vycisti(void);
		void reservujKapacitu(DelkaIndexu kapacita);

		const Bitmapa * bitmapa(DelkaIndexu pozice, EBitmapy & e) const;
		DelkaBitmapy delkaBitmap(void) const;
		DelkaIndexu delka(void) const;

	protected:
		DelkaBitmapy n;
		SeznamBitmap index;
		PoradiBitmap poradi;
		bool jePoradiNastaveno;

	private:
		explicit BitmapovyIndex(DelkaBitmapy n, const std::string * retezce,
			DelkaIndexu pocetRezezcu);

		friend class BitmapoveTrideni;
		friend class Kgk;
	};
}



#endif // KOMBIG_BITMAPOVY_INDEX_H_
