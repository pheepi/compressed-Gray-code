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

#ifndef KOMBIG_SERIALIZOVATELNY_INDEX_H_
#define KOMBIG_SERIALIZOVATELNY_INDEX_H_

#include "ZaznamovyFormat.h"
#include "BitmapovyIndex.h"



namespace Bitmapy
{
	class BitmapoveTrideni;
	class VstupBitmap;
	class VystupBitmap;


	class SerializovatelnyIndex : public BitmapovyIndex,
		public IOPodpora::ZaznamovyFormat
	{
	public:
		static const char UMELY_KONEC_SOUBORU = 'X';
		static const char ODDELOVAC_PORADI = '\t';

	protected:
		explicit SerializovatelnyIndex(DelkaBitmapy n = 0u,
			size_t velikostBufferu = VELIKOST_BUFFERU,
			std::fstream::openmode druhPristupu
			= static_cast<std::fstream::openmode>(0));
		~SerializovatelnyIndex(void);

	private:
		explicit SerializovatelnyIndex(const SerializovatelnyIndex & index);
		const SerializovatelnyIndex & operator=(const SerializovatelnyIndex & index);
	};


	class VstupBitmap : virtual public SerializovatelnyIndex
	{
	public:
		static const DelkaIndexu MINIMALNI_MAXIMUM_BITMAP = 1047552u;

		DelkaIndexu dejMaximumBitmapu(void) const;
		void nastavMaximumBitmapu(DelkaIndexu maximumBitmapu);

		bool setridSoubor(VystupBitmap & vystup,
			const BitmapoveTrideni & trideni, EBitmapy & e);
		EBitmapy nacti(void);
		EBitmapy nacti(const DelkaIndexu pocet);
		bool zjistiDelkuBitmap(void);
		bool zjistiDelkuIndexu(DelkaIndexu & delka, EBitmapy & e);
		bool jeVstupDostupny(void);

	protected:
		explicit VstupBitmap(DelkaIndexu maximumBitmapu =
			MINIMALNI_MAXIMUM_BITMAP);
		~VstupBitmap(void);

	private:
		explicit VstupBitmap(const VstupBitmap & vstupBitmap);
		const VstupBitmap & operator=(const VstupBitmap & vstupBitmap);

		bool nactiBitmapu(EBitmapy & e);
		void nactiPoradi(void);
		void odstranPrazdneZnaky(void);

		DelkaIndexu maximumBitmapu;

		friend class VystupBitmap;
	};


	class VystupBitmap : virtual public SerializovatelnyIndex
	{
	public:
		void zapis(void);
		bool zapisDelkuBitmap(void);

	protected:
		explicit VystupBitmap(void);
		~VystupBitmap(void);

	private:
		explicit VystupBitmap(const VystupBitmap & vystupBitmap);
		const VystupBitmap & operator=(const VystupBitmap & vystupBitmap);

		EBitmapy slij(VstupBitmap & prvniVstup, VstupBitmap & druhyVstup,
			const BitmapoveTrideni & trideni, DelkaIndexu velikostBloku);
		void zapisBitmapu(const Bitmapa & mapa, DelkaIndexu poradi);
		void zapisPoradi(DelkaIndexu indexPoradi);
		void zapisKonec(void);

		friend class VstupBitmap;
	};


	class BitmapovyTokenizer : public VstupBitmap
	{
	public:
		explicit BitmapovyTokenizer(DelkaIndexu maximumBitmapu =
			MINIMALNI_MAXIMUM_BITMAP, DelkaBitmapy n = 0u,
			size_t velikostBufferu = VELIKOST_BUFFERU);
		~BitmapovyTokenizer(void);

	private:
		explicit BitmapovyTokenizer(const BitmapovyTokenizer & tokenizer);
		const BitmapovyTokenizer & operator=(const BitmapovyTokenizer & tokenizer);
	};


	class BitmapovyWriter : public VystupBitmap
	{
	public:
		explicit BitmapovyWriter(DelkaBitmapy n = 0u,
			size_t velikostBufferu = VELIKOST_BUFFERU);
		~BitmapovyWriter(void);

	private:
		explicit BitmapovyWriter(const BitmapovyWriter & writer);
		const BitmapovyWriter & operator=(const BitmapovyWriter & writer);
	};


	class BitmapovyUniversal : public VstupBitmap, public VystupBitmap
	{
	public:
		explicit BitmapovyUniversal(DelkaIndexu maximumBitmapu =
			MINIMALNI_MAXIMUM_BITMAP, DelkaBitmapy n = 0u,
			size_t velikostBufferu = VELIKOST_BUFFERU);
		~BitmapovyUniversal(void);

	private:
		explicit BitmapovyUniversal(const BitmapovyUniversal & universal);
		const BitmapovyUniversal & operator=(const BitmapovyUniversal & universal);
	};


	class ObalDocasnehoSouboru
	{
	public:
		explicit ObalDocasnehoSouboru(void);
		explicit ObalDocasnehoSouboru(BitmapovyUniversal * index,
			const std::string & jmenoSouboru);
		~ObalDocasnehoSouboru(void);

		BitmapovyUniversal * dejIndex(void) const;

		bool vytvorDocasnySoubor(BitmapovyUniversal * universal);
		void odstranDocasnySoubor(void);
		void prohod(ObalDocasnehoSouboru & soubor);

	private:
		static bool vytvorNepouziteJmenoSouboru(std::string & jmenoSouboru);

		explicit ObalDocasnehoSouboru(const ObalDocasnehoSouboru & obal);
		const ObalDocasnehoSouboru & operator=(const ObalDocasnehoSouboru & obal);

		BitmapovyUniversal * index;
		std::string jmenoSouboru;
	};
}



#endif // KOMBIG_SERIALIZOVATELNY_INDEX_H_
