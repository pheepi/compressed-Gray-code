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

#ifndef KOMBIG_CSV_PODPORA_H_
#define KOMBIG_CSV_PODPORA_H_

#include <vector>
#include <string>
#include <fstream>

#include "ZaznamovyFormat.h"



namespace IOPodpora
{
	typedef std::string Polozka;
	typedef std::vector<Polozka> Zaznam;


	class CsvInterface : public ZaznamovyFormat
	{
	public:
		static const char ODDELOVAC = ',';
		static const char UVOZOVKY = '"';

	protected:
		explicit CsvInterface(const char oddelovac = ODDELOVAC,
			const size_t velikost = VELIKOST_BUFFERU,
			const std::fstream::openmode druhPristupu
			= static_cast<std::fstream::openmode>(0));
		~CsvInterface(void);

		const char oddelovac;

	private:
		CsvInterface(const CsvInterface & csvInterface);
		const CsvInterface & operator=(const CsvInterface & csvInterface);
	};


	class CsvVstup : virtual public CsvInterface
	{
	public:
		bool dejZaznam(Zaznam & zaznam);

	protected:
		CsvVstup(void);
		~CsvVstup(void);

	private:
		CsvVstup(const CsvVstup & csvVstup);
		const CsvVstup & operator=(const CsvVstup & csvVstup);

		std::streamsize pocetNactenych;
	};


	class CsvVystup : virtual public CsvInterface
	{
	public:
		void zapisPolozku(const Polozka & polozka);
		void zapisPolozku(const char * polozka, std::streamsize delka);
		void zapisUvozenouPolozku(const Polozka & polozka);
		void zapisUvozenouPolozku(const char * polozka, std::streamsize delka);
		void zapisCislo(size_t cislo);
		void zapis(const Polozka & retezec);
		void zapis(const char * retezec, std::streamsize delka);
		void zapis(char znak);
		void ukonciZaznam(void);
		void synchronizuj(void);

	protected:
		CsvVystup(void);
		~CsvVystup(void);

	private:
		CsvVystup(const CsvVystup & csvVystup);
		const CsvVystup & operator=(const CsvVystup & csvVystup);

		bool jeKonecRadku;
	};


	class CsvTokenizer : public CsvVstup
	{
	public:
		CsvTokenizer(char oddelovac = ODDELOVAC,
			size_t velikost = VELIKOST_BUFFERU);
		~CsvTokenizer(void);

	private:
		CsvTokenizer(const CsvTokenizer & csvTokenizer);
		const CsvTokenizer & operator=(const CsvTokenizer & csvTokenizer);
	};


	class CsvWriter : public CsvVystup
	{
	public:
		CsvWriter(char separator = ODDELOVAC,
			size_t velikostBufferu = VELIKOST_BUFFERU);
		~CsvWriter(void);

	private:
		CsvWriter(const CsvWriter & csvWriter);
		const CsvWriter & operator=(const CsvWriter & csvWriter);
	};


	class CsvUniversal : public CsvVstup, public CsvVystup
	{
	public:
		CsvUniversal(char oddelovac = ODDELOVAC,
			size_t velikostBufferu = VELIKOST_BUFFERU);
		~CsvUniversal(void);

	private:
		CsvUniversal(const CsvUniversal & csvUniversal);
		const CsvUniversal & operator=(const CsvUniversal & csvUniversal);
	};
}



#endif // KOMBIG_CSV_PODPORA_H_
