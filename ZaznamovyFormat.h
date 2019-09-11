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

#ifndef KOMBIG_ZAZNAMOVY_FORMAT_H_
#define KOMBIG_ZAZNAMOVY_FORMAT_H_

#include <fstream>



namespace IOPodpora
{
	enum ModPristupu
	{
		Zadny = 0,
		Cteni = 1,
		Zapis = 2
	};


	class ZaznamovyFormat
	{
	public:
		static const char NOVY_RADEK;
		static const char NAVRAT_VOZIKU;

		bool jeOtevreny(void) const;
		bool otevriSoubor(const char * jmenoSouboru);
		void zavriSoubor(void);
		void presunSeNaZacatek(void);
		ModPristupu dejModPristupu(void) const;

	protected:
		static const size_t VELIKOST_BUFFERU = 4096u;

		explicit ZaznamovyFormat(size_t velikost = VELIKOST_BUFFERU,
			std::fstream::openmode druhPristupu
			= static_cast<std::fstream::openmode>(0));
		~ZaznamovyFormat(void);

		bool nastavCteni(void);
		bool nastavZapis(void);

		std::filebuf proud;
		char * const buffer;
		const std::fstream::openmode druhPristupu;
		ModPristupu modPristupu;

	private:

		explicit ZaznamovyFormat(const ZaznamovyFormat & zaznamovyFormat);
		const ZaznamovyFormat & operator=(const ZaznamovyFormat & zaznamovyFormat);

		const std::streamsize velikostBufferu;
	};
}



#endif // KOMBIG_ZAZNAMOVY_FORMAT_H_
