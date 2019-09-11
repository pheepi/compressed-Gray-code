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

#ifndef KOMBIG_BITMAPOVE_TRIDENI_H_
#define KOMBIG_BITMAPOVE_TRIDENI_H_

#include <vector>



namespace Bitmapy
{
	class Bitmapa;
	class BitmapovyIndex;

	typedef std::vector<Bitmapa> SeznamBitmap;
	typedef SeznamBitmap::size_type DelkaIndexu;
	typedef std::vector<DelkaIndexu> PoradiBitmap;


	class BitmapoveTrideni
	{
	public:
		typedef bool (*Porovnani)(const Bitmapa &, const Bitmapa &);
		typedef void (*Trideni)(BitmapovyIndex &);

		enum Usporadani
		{
			Zadne = 0,
			Lexikograficke = 1,
			Zgk = 2,
			Kgk = 3
		};

		static const char * const DRUHY_USPORADANI[];
		static const unsigned short int POCET_DRUHU_USPORADANI;

		explicit BitmapoveTrideni(Usporadani usporadani = Zadne);

		Usporadani dejUsporadani(void) const;
		void nastavUsporadani(Usporadani noveUsporadani = Zadne);
		void nastavSlovnik(const BitmapovyIndex * slovnik);

		bool porovnej(const Bitmapa & prvni, const Bitmapa & druhy) const;
		void setrid(BitmapovyIndex & bitmapovyIndex) const;

		void vytvorPoradiProKgk(BitmapovyIndex & bitmapovyIndex) const;

	private:
		class TrideniKgk
		{
		public:
			explicit TrideniKgk(const PoradiBitmap & poradi);
			TrideniKgk(const TrideniKgk & trideni);
			~TrideniKgk(void);

			bool operator()(const DelkaIndexu prvni,
				const DelkaIndexu druha) const;

		private:
			const TrideniKgk & operator=(const TrideniKgk & trideni);

			const PoradiBitmap & poradi;
		};

		BitmapoveTrideni(const BitmapoveTrideni & trideni);
		const BitmapoveTrideni & operator=(const BitmapoveTrideni & trideni);

		static bool porovnejLexikograficky(const Bitmapa & prvni,
			const Bitmapa & druhy);
		static bool porovnejZgk(const Bitmapa & prvni, const Bitmapa & druhy);
		static bool porovnejKgk(const Bitmapa & prvni, const Bitmapa & druhy);
		static void setridLexikograficky(BitmapovyIndex & bitmapovyIndex);
		static void setridZgk(BitmapovyIndex & bitmapovyIndex);
		static void setridKgk(BitmapovyIndex & bitmapovyIndex);

		Usporadani usporadani;
		Porovnani porovnani;
		Trideni trideni;
		const BitmapovyIndex * slovnik;
	};
}



#endif // KOMBIG_BITMAPOVE_TRIDENI_H_
