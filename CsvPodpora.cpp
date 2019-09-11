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

#include "CsvPodpora.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "ZaznamovyFormat.h"

using namespace IOPodpora;



CsvInterface::CsvInterface(char oddelovac, size_t velikost,
	const std::fstream::openmode druhPristupu) :
	ZaznamovyFormat(velikost, druhPristupu),
	oddelovac(oddelovac) {}



CsvInterface::~CsvInterface(void) {}



bool CsvVstup::dejZaznam(Zaznam & zaznam)
{
	if (this->modPristupu != Cteni)
	{
		if (this->modPristupu == Zadny)
			this->modPristupu = Cteni;
		else
			return false;
	}

	if (this->proud.in_avail() == 0)
	{
		this->proud.sgetc();
		this->pocetNactenych = this->proud.in_avail();
	}

	if (this->proud.sgetc() == EOF)
		return false;

	char znak = static_cast<char>(this->proud.sgetc());
	std::streamsize poziceVBufferu = this->pocetNactenych -
		this->proud.in_avail();

	while ((znak == NAVRAT_VOZIKU) || (znak == NOVY_RADEK))
	{
		this->proud.sbumpc();
		if (this->proud.in_avail() == 0)
		{
			this->proud.sgetc();
			this->pocetNactenych = this->proud.in_avail();
			poziceVBufferu = 0;
		}
		else
			++poziceVBufferu;

		if (this->proud.sgetc() == EOF)
			return false;
		else
			znak = static_cast<char>(this->proud.sgetc());
	}

	Polozka polozka;
	std::streamsize zacatekPridavanych = poziceVBufferu;
	bool jeZaznamUvozeny = false;
	bool jeMoznyKonecUvozeni = false;
	zaznam.clear();

	while (this->proud.sgetc() != EOF)
	{
		if (jeZaznamUvozeny)
		{
			if (znak == this->UVOZOVKY)
			{
				polozka.append(this->buffer + zacatekPridavanych,
					static_cast<size_t>(poziceVBufferu - zacatekPridavanych));
				zacatekPridavanych = poziceVBufferu + 1;
				jeZaznamUvozeny = false;
				jeMoznyKonecUvozeni = true;
			}
		}
		else
		{
			if (znak == this->UVOZOVKY)
			{
				if (jeMoznyKonecUvozeni)
				{
					jeZaznamUvozeny = true;
				}
				else
				{
					if (polozka.empty())
					{
						jeZaznamUvozeny = true;
						++zacatekPridavanych;
					}
				}
			}
			else
			{
				if (znak == NAVRAT_VOZIKU || znak == NOVY_RADEK ||
					znak == this->oddelovac)
				{
					polozka.append(this->buffer + zacatekPridavanych,
						static_cast<size_t>(poziceVBufferu
						- zacatekPridavanych));
					zaznam.push_back(polozka);

					if (znak == NAVRAT_VOZIKU || znak == NOVY_RADEK)
					{
						this->proud.sbumpc();
						return true;
					}

					zacatekPridavanych = poziceVBufferu + 1;
					polozka.clear();
				}
			}

			if (jeMoznyKonecUvozeni)
				jeMoznyKonecUvozeni = false;
		}

		this->proud.sbumpc();
		if (this->proud.in_avail() == 0)
		{	
			polozka.append(this->buffer + zacatekPridavanych,
				static_cast<size_t>(this->pocetNactenych - zacatekPridavanych));
			zacatekPridavanych = 0;
			this->proud.sgetc();
			this->pocetNactenych = this->proud.in_avail();
			poziceVBufferu = 0;
		}
		else
			++poziceVBufferu;

		znak = static_cast<char>(this->proud.sgetc());
	}

	zaznam.push_back(polozka);
	return true;
}



CsvVstup::CsvVstup(void) : pocetNactenych(0) {}



CsvVstup::~CsvVstup(void) {}


		
void CsvVystup::zapisPolozku(const Polozka & polozka)
{
	this->zapisPolozku(polozka.data(),
		static_cast<std::streamsize>(polozka.size()));
}



void CsvVystup::zapisPolozku(const char * polozka, std::streamsize delka)
{
	if (this->modPristupu != Zapis)
	{
		if (this->modPristupu == Zadny)
			this->modPristupu = Zapis;
		else
			return;
	}

	if (!this->jeKonecRadku)
		this->proud.sputc(this->oddelovac);
	else
		this->jeKonecRadku = false;

	this->proud.sputn(polozka, delka);
}



void CsvVystup::zapisUvozenouPolozku(const Polozka & polozka)
{
	this->zapisUvozenouPolozku(polozka.data(),
		static_cast<std::streamsize>(polozka.size()));
}



void CsvVystup::zapisUvozenouPolozku(const char * polozka, std::streamsize delka)
{
	if (this->modPristupu != Zapis)
	{
		if (this->modPristupu == Zadny)
			this->modPristupu = Zapis;
		else
			return;
	}

	if (!this->jeKonecRadku)
		this->proud.sputc(this->oddelovac);
	else
		this->jeKonecRadku = false;

	std::streamsize zacatek = 0;
	std::streamsize konec;

	this->proud.sputc(this->UVOZOVKY);
	for (konec = 0; konec < delka; ++konec)
	{
		if (polozka[konec] == this->UVOZOVKY)
		{
			this->proud.sputn(polozka + zacatek, konec - zacatek + 1);
			zacatek = konec;
		}
	}

	this->proud.sputn(polozka + zacatek, konec - zacatek);
	this->proud.sputc(this->UVOZOVKY);
}



void CsvVystup::zapisCislo(size_t cislo)
{
	std::stringstream polozka;
	polozka << cislo;
	this->zapisPolozku(polozka.str());
}



void CsvVystup::zapis(const Polozka & retezec)
{
	if (this->modPristupu != Zapis)
	{
		if (this->modPristupu == Zadny)
			this->modPristupu = Zapis;
		else
			return;
	}

	this->proud.sputn(retezec.data(),
		static_cast<std::streamsize>(retezec.size()));
}



void CsvVystup::zapis(const char * retezec, std::streamsize delka)
{
	if (this->modPristupu != Zapis)
	{
		if (this->modPristupu == Zadny)
			this->modPristupu = Zapis;
		else
			return;
	}

	this->proud.sputn(retezec, delka);
}



void CsvVystup::zapis(const char znak)
{
	if (this->modPristupu != Zapis)
	{
		if (this->modPristupu == Zadny)
			this->modPristupu = Zapis;
		else
			return;
	}

	this->proud.sputc(znak);
}



void CsvVystup::ukonciZaznam(void)
{
	if (this->modPristupu != Zapis)
	{
		if (this->modPristupu == Zadny)
			this->modPristupu = Zapis;
		else
			return;
	}

	this->proud.sputc(NOVY_RADEK);
	this->jeKonecRadku = true;
}



void CsvVystup::synchronizuj(void)
{
	this->proud.pubsync();
}



CsvVystup::CsvVystup(void) : jeKonecRadku(true) {}



CsvVystup::~CsvVystup(void) {}



CsvTokenizer::CsvTokenizer(char oddelovac, size_t velikost) :
	CsvInterface(oddelovac, velikost, std::fstream::in),
	CsvVstup()
{
	this->modPristupu = Cteni;
}



CsvTokenizer::~CsvTokenizer(void) {}



CsvWriter::CsvWriter(char separator, size_t velikostBufferu) :
	CsvInterface(separator, velikostBufferu,
		std::fstream::out | std::fstream::trunc),
	CsvVystup()
{
	this->modPristupu = Zapis;
}



CsvWriter::~CsvWriter(void) {}



CsvUniversal::CsvUniversal(char oddelovac, size_t velikostBufferu) :
	CsvInterface(oddelovac, velikostBufferu,
		std::fstream::in | std::fstream::out),
	CsvVstup(),
	CsvVystup() {}



CsvUniversal::~CsvUniversal(void) {}
