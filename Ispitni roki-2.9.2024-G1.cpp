#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <mutex>
#include <thread>

using namespace std;
const char* crt = "\n-------------------------------------------\n";
enum Drzava {
	ENGLESKA, SPANIJA, HOLANDIJA, FRANCUSKA,
	BOSNA_I_HERCEGOVINA
};
ostream& operator<<(ostream& c, Drzava d) {
	const char* drzave[]{ "ENGLESKA","SPANIJA","HOLANDIJA","FRANCUSKA","BOSNA_I_HERCEGOVINA" };
	cout << drzave[d];
	return c;
}
int brojCifara(int broj) {
	int brojac = 0;
	while (broj > 0) {
		broj /= 10;
		brojac++;
	}
	return brojac;
}
char PrvoSlovoImena(const char* ip) {
	return toupper(ip[0]);
}
int obrniCifru(int broj) {
	int novibroj = 0;
	while (broj > 0) {
		int broj1 = broj % 10;
		novibroj = novibroj * 10 + broj1;
		broj /= 10;
	}
	return novibroj;
}
char PrvoSlovoPrezimean(const char* ip) {
	for (int i = 0; i < strlen(ip); i++)
	{
		if (ip[i] == ' ') {
			return ip[i + 1];
		}
	}
	return '\0';
}
char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}
char* GenerisiID(const char* imeprezime, int id) {

	string Id = "";
	string id2 = to_string(obrniCifru(id));
	if (brojCifara(id) > 4)return nullptr;
	if (id < 10) {
		Id += PrvoSlovoImena(imeprezime);
		Id += "000";
		Id += toupper(PrvoSlovoPrezimean(imeprezime));
		Id += to_string(id);
	}
	else if (id> 9&&id<100)
	{
		Id += PrvoSlovoImena(imeprezime);
		Id += "00";
		Id += toupper(PrvoSlovoPrezimean(imeprezime));
		Id += id2;
	}
	else if (id >99&&id<1000)
	{
		Id += PrvoSlovoImena(imeprezime);
		Id += "0";
		Id += toupper(PrvoSlovoPrezimean(imeprezime));
		Id += id2;
	}
	else if (id>1000) 
	{
		Id += PrvoSlovoImena(imeprezime);
		Id += toupper(PrvoSlovoPrezimean(imeprezime));
		Id += id2;
	}
	return GetNizKaraktera(Id.c_str());
}
bool ValidirajID(const char* ID) {
	return regex_match(ID, regex("^[A-Z]{1}(000[A-Z]{1}[0-9]{1}|00[A-Z]{1}[0-9]{2}|0[A-Z]{1}[0-9]{3}|[A-Z]{1}[0-9]{4})"));
}
template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int* _trenutno;
public:
	Kolekcija()
	{
		_trenutno = new int(0);
	}
	Kolekcija(const Kolekcija& obj)
	{
		_trenutno = new int(*obj._trenutno);
		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	Kolekcija& operator =(const Kolekcija& obj)
	{
		if (this == &obj)return *this;
		delete _trenutno; _trenutno = nullptr;
		_trenutno = new int(*obj._trenutno);
		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		return *this;
	}
	~Kolekcija() {
		delete _trenutno; _trenutno = nullptr;
	}
	T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() const { return *_trenutno; }
	friend ostream& operator<< (ostream& COUT, Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) <<
			endl;
		return COUT;
	}
	void AddElement(T1 el1, T2 el2) {
		if (*_trenutno ==max)return;
		_elementi1[*_trenutno] = el1;
		_elementi2[*_trenutno] = el2;
		(*_trenutno)++;
	};
	Kolekcija InsertAt( T1 el1, T2 el2,int lokacija) {
		
		for (int i = *_trenutno; i > lokacija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		_elementi1[lokacija] =(el1);
		_elementi2[lokacija] = (el2);
		(*_trenutno)++;
		return *this;
	}
	Kolekcija RemoveRange(int start, int end) {
		Kolekcija<T1, T2, max>tmp;
		for (int i = start; i <=end; i++)
		{
			tmp.AddElement(_elementi1[i], _elementi2[i]);
		}
		for (int i = end +1; i < *_trenutno; i++)
		{
			_elementi1[i - (end - start + 1)] = _elementi1[i];
			_elementi2[i - (end - start + 1)] = _elementi2[i];
		}
		*_trenutno -= (end - start + 1);
		return tmp;
	}

};
class Vrijeme {
		int* _sat, * _minuta, * _sekunda;
	public:
		Vrijeme(int sat = 10, int minuta = 0, int sekunda = 0) {
			_sat = new int(sat);
			_minuta = new int(minuta);
			_sekunda = new int(sekunda);
		}
		Vrijeme(const Vrijeme& obj) {
			_sat = new int(*obj._sat);
			_minuta = new int(*obj._minuta);
			_sekunda = new int(*obj._sekunda);
		}
		Vrijeme& operator =(const Vrijeme& obj) {
			if (this == &obj)return *this;
			delete _sat; _sat = nullptr;
			delete _minuta; _minuta = nullptr;
			delete _sekunda; _sekunda = nullptr;
			_sat = new int(*obj._sat);
			_minuta = new int(*obj._minuta);
			_sekunda = new int(*obj._sekunda);
			return *this;
		}

		~Vrijeme() {
			delete _sat; _sat = nullptr;
			delete _minuta; _minuta = nullptr;
			delete _sekunda; _sekunda = nullptr;
		}
		friend ostream& operator<< (ostream& COUT, const Vrijeme& obj) {
			COUT << *obj._sat << ":" << *obj._minuta << ":" << *obj._sekunda;
			return COUT;
		}
		int sekunde()const {
			return  *_sekunda + *_minuta * 60 + *_sat * 3600;
		}
		bool operator==(const Vrijeme& obj) {
			return obj.sekunde() == sekunde();
		}
	};
class Pogodak {
		Vrijeme* _vrijemePogotka;
		char* _napomena;
	public:
		Pogodak(Vrijeme vrijeme, const char* napomena) {
			_napomena = GetNizKaraktera(napomena);
			_vrijemePogotka = new Vrijeme(vrijeme);
		}
		~Pogodak() {
			delete[] _napomena; _napomena = nullptr;
			delete _vrijemePogotka; _vrijemePogotka = nullptr;
		}
		Pogodak(const Pogodak& obj)
		{
			_vrijemePogotka = new Vrijeme(*obj._vrijemePogotka);
			_napomena = GetNizKaraktera(obj._napomena);
		}
		Pogodak& operator =(const Pogodak& obj)
		{
			if (this == &obj)return *this;
			delete[] _napomena; _napomena = nullptr;
			delete _vrijemePogotka; _vrijemePogotka = nullptr;
			_vrijemePogotka = new Vrijeme(*obj._vrijemePogotka);
			_napomena = GetNizKaraktera(obj._napomena);
			return *this;
		}
		Vrijeme GetVrijemePogotka() { return *_vrijemePogotka; }
		char* GetNapomena() { return _napomena; }
		friend ostream& operator<< (ostream& COUT, const Pogodak& obj) {
			COUT << *obj._vrijemePogotka << " -> " << obj._napomena;
			return COUT;
		}
		bool operator ==(const Pogodak& obj) {
			return (strcmp(_napomena, obj._napomena) == 0) && (*_vrijemePogotka == *obj._vrijemePogotka);
		}

	};
class Igrac {
		static int _id;
		char* _ID; 
		char* _imePrezime;
		vector<Pogodak*> _pogoci;
	public:
		Igrac(const char* imePrezime) {
			++_id;
			_imePrezime = GetNizKaraktera(imePrezime);
			_ID = GenerisiID(_imePrezime,_id);
		}
		~Igrac() {
			delete[]_ID; _ID = nullptr;
			delete[]_imePrezime; _imePrezime = nullptr;
			for (size_t i = 0; i < _pogoci.size(); i++) {
				delete _pogoci[i];
				_pogoci[i] = nullptr;
			}
		}
		Igrac(const Igrac& obj)
		{
			_ID = GetNizKaraktera(obj._ID);
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			for (int i = 0; i < obj._pogoci.size(); i++)
			{
				_pogoci.push_back(new Pogodak(*obj._pogoci[i]));
			}
		}
		Igrac& operator =(const Igrac& obj)
		{
			if (this == &obj)return *this;
			delete[]_ID; _ID = nullptr;
			delete[]_imePrezime; _imePrezime = nullptr;
			for (size_t i = 0; i < _pogoci.size(); i++) {
				delete _pogoci[i];
				_pogoci[i] = nullptr;
			}
			_pogoci.clear();
			_ID = GetNizKaraktera(obj._ID);
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			for (int i = 0; i < obj._pogoci.size(); i++)
			{
				_pogoci.push_back(new Pogodak(*obj._pogoci[i]));
			}
			return *this;
		}
		char* GetImePrezime() { return _imePrezime; }
		char* GetID() { return _ID; }
		vector<Pogodak*>& GetPogoci() { return _pogoci; }
		friend ostream& operator<< (ostream& COUT, Igrac& obj) {
			COUT << *obj._ID << " -> " << obj._imePrezime;
			for (size_t i = 0; i < obj._pogoci.size(); i++)
				cout << *obj._pogoci[i] << endl;
			return COUT;
		}
		bool operator ==(const Igrac& obj) {
			return strcmp(_ID, obj._ID) == 0;}
		bool isPogodak(Pogodak& pogodak) {
			if (!_pogoci.empty()) {
				for (int i = 0; i < _pogoci.size(); i++)
				{
					if (*_pogoci[i] == pogodak) {
						return true;
					}
				}
			}
			return false;
		}
		int brojPogodaka() {
			return _pogoci.size();
		}
	

	};
class Reprezentacija {
		Drzava _drzava;
		vector<Igrac> _igraci;
	public:
		Reprezentacija(Drzava drzava = BOSNA_I_HERCEGOVINA) {
			_drzava = drzava;
		}
		Reprezentacija(const Reprezentacija& obj)
		{
			_drzava = obj._drzava;
			_igraci = obj._igraci;
		}
		Reprezentacija& operator =(const Reprezentacija& obj)
		{
			if (this == &obj)return *this;
			_drzava = obj._drzava;
			_igraci = obj._igraci;
			return *this;
		}
		Drzava GetDrzava() { return _drzava; }
		vector<Igrac>& GetIgraci() { return _igraci; }
		void AddIgrac(Igrac ig) {
			for (int i = 0; i < _igraci.size(); i++)
			{
				if (_igraci[i] == ig) {
					throw exception("Igrac vec postoji!");
				}
			}
			_igraci.push_back(ig);
		}
		bool operator ==(const Reprezentacija& obj) {
			return _drzava == obj._drzava;
		}
		Igrac* getIdIp(const char* idip) {
			for (int i = 0; i < _igraci.size(); i++)
			{
				if (strcmp(_igraci[i].GetID(), idip) == 0 || strcmp(_igraci[i].GetImePrezime(), idip) == 0) {
					return &_igraci[i];

				}
			}
			return nullptr;

		}
		int UkupnoGolova()  {
			size_t rez = 0;

			for (int i = 0; i < _igraci.size(); i++)
				rez += _igraci[i].GetPogoci().size();

			return int(rez);
		}
		
	};
mutex guard;
class Prventstvo {
		Kolekcija<Reprezentacija, Reprezentacija, 20> _utakmice;
	public:
		Prventstvo() = default;
		Kolekcija<Reprezentacija, Reprezentacija, 20>& GetUtakmice() {
			return _utakmice;
		}
		void AddUtakmicu(Reprezentacija r1, Reprezentacija r2) {
			for (int i = 0; i < _utakmice.getTrenutno(); i++)
			{
				if (_utakmice.getElement1(i) == r1 && _utakmice.getElement2(i) == r2 ||
					_utakmice.getElement1(i) == r2 && _utakmice.getElement2(i) == r1) {
					throw exception("Reprezentacije su se vec susrele");
				}
			}
			_utakmice.AddElement(r1, r2);
		}
		bool istiPogodak(Pogodak p) {
			for (int i = 0; i < _utakmice.getTrenutno(); i++)
			{
				for (int j = 0; j < _utakmice.getElement1(i).GetIgraci().size(); j++)
				{
					if (_utakmice.getElement1(i).GetIgraci()[j].isPogodak(p)) {
						return true;
					}
				}
				for (int j = 0; j < _utakmice.getElement2(i).GetIgraci().size(); j++)
				{
					if (_utakmice.getElement2(i).GetIgraci()[j].isPogodak(p)) {
						return true;
					}
				}
			}
			return false;
		}
		int BrojUtakmice(Drzava d1, Drzava d2) {
			for (int i = 0; i < _utakmice.getTrenutno(); i++)
			{
				if (_utakmice.getElement1(i).GetDrzava() == d1 && _utakmice.getElement2(i).GetDrzava() == d2) {
					return i;
				}
			}
			return 0;
		}
		void SendEmail(Igrac ig,Drzava d1,Drzava d2) {

			guard.lock();
			this_thread::sleep_for(2s);
			cout << "------------------------EMAIL---------------------------------" << endl;
			cout << "TO: " << ig.GetID() << "@euro2024.com" << endl;
			cout << "From: info@euro2024.com" << endl;
			cout << "Subject: Informacija " << endl;
			cout << "Postovani," << endl;
			cout << "U " << ig.GetPogoci()[ig.GetPogoci().size() - 1]->GetVrijemePogotka() << " sati igrac " << ig.GetImePrezime() << " je zabiljezio svoj " << ig.GetPogoci().size() << endl;
			cout << "Trenutni rezultat je:" << endl;
			cout << d1 << " " << _utakmice.getElement1(BrojUtakmice(d1, d2)).UkupnoGolova() << " : " << d2<<" "<< _utakmice.getElement2(BrojUtakmice(d1, d2)).UkupnoGolova() << endl;
			cout << "Puno srece u nastavku susreta." << endl;
			cout << "Neka bolji tim pobijedi." << endl;
			guard.unlock();
		}
		bool AddPogodak(Drzava d1, Drzava d2, const char* iPiD, Pogodak p) {
			//omoguciti dodavanje pogotka po ID-u ili imenu i prezimenu
				//onemoguciti dodavanje istih pogodaka
			for (int i = 0; i < _utakmice.getTrenutno(); i++)
			{
				//provjera da li su se ekipe sastale na toj utakmici da bih se mogao dodati pogodak
				if (_utakmice.getElement1(i).GetDrzava() == d1 && _utakmice.getElement2(i).GetDrzava() == d2 ||
					_utakmice.getElement1(i).GetDrzava() == d2 && _utakmice.getElement2(i).GetDrzava() == d1) {
					//provjera da li postoji igrac sa datim ip ili id
					Igrac* igrac = _utakmice.getElement1(i).getIdIp(iPiD);
					if (!igrac) {
						 igrac = _utakmice.getElement2(i).getIdIp(iPiD);
					}
					if (igrac && !istiPogodak(p)) {
						igrac->GetPogoci().push_back(new Pogodak(p));
						thread th1(&Prventstvo::SendEmail, this, *igrac, d1, d2);
						th1.join();
						return true;
					}


				}
			}
			return false;
		}
		friend ostream& operator <<(ostream& c,  Prventstvo& obj) {
			c << crt;
			for (int i = 0; i <obj._utakmice.getTrenutno() ; i++)
			{
				c <<"("<< obj._utakmice.getElement1(i).UkupnoGolova() << ") " << obj._utakmice.getElement1(i).GetDrzava() << " : " << obj._utakmice.getElement2(i).GetDrzava() << " (" << obj._utakmice.getElement2(i).UkupnoGolova()<<")" << endl;
				for (auto& igrac: obj._utakmice.getElement1(i).GetIgraci())
				{
					for (int k = 0; k < igrac.GetPogoci().size(); k++)
					{
						cout << igrac.GetImePrezime() << endl;
					}
				}
				for (auto& igrac:obj._utakmice.getElement2(i).GetIgraci())
				{
					for (int i = 0; i < igrac.GetPogoci().size(); i++)
					{
						cout << igrac.GetImePrezime() << endl;
					}
				}
			
			}
			c << crt;
			return c;
		}
		vector<Igrac*> operator()(int brojPogodaka){
			vector<Igrac*>novi;
			for (int i = 0; i < _utakmice.getTrenutno(); i++)
			{
				for (auto& igrac:_utakmice.getElement1(i).GetIgraci())
				{
					if (igrac.GetPogoci().size() >= brojPogodaka) {
						novi.push_back(new Igrac(igrac));
					}
				}
				for (auto& igrac : _utakmice.getElement2(i).GetIgraci())
				{
					if (igrac.GetPogoci().size() >= brojPogodaka) {
						novi.push_back(new Igrac(igrac));
					}
				}
			}
			return novi;
		}
};
const char* GetOdgovorNaPrvoPitanje() {
		cout << "Pitanje -> Pojasnite osnovne preduslove koji moraju biti ispunjeni da bi se realizovao polimorfizam(navesti kratkiprimjer) ? \n";
		return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
	}
const char* GetOdgovorNaDrugoPitanje() {
		cout << "Pitanje -> Pojasnite razloge koristenja kljucnih rijeciabstract i ciste virtualne metode, te razlike izmedju njih ? \n";
		return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
	}
int Igrac::_id = 0;
void main() {
		/*cout << PORUKA;
		cin.get();
		cout << GetOdgovorNaPrvoPitanje() << endl;
		cin.get();
		cout << GetOdgovorNaDrugoPitanje() << endl;
		cin.get();*/

		cout << GenerisiID("Denis Music", 3) << endl;//treba vratiti D000M3
			cout << GenerisiID("Jasmin Azemovic", 14) << endl;//treba vratiti J00A41
			cout << GenerisiID("Goran Skondric", 156) << endl;//treba vratiti G0S651
			cout << GenerisiID("emina junuz", 1798) << endl;//treba vratiti EJ8971

		
		if (ValidirajID("D000M3"))
				cout << "ID VALIDAN" << endl;
		if (ValidirajID("J00A41"))
			cout << "ID VALIDAN" << endl;
		if (!ValidirajID("G00S651"))
			cout << "ID NIJE VALIDAN" << endl;
		if (!ValidirajID("Ej8971"))
			cout << "ID NIJE VALIDAN" << endl;
		int kolekcijaTestSize = 9;
		Kolekcija<int, int, 10> kolekcija1;
		for (int i = 0; i < kolekcijaTestSize; i++)
			kolekcija1.AddElement(i, i);//dodaje vrijednosti u kolekciju 
		cout << kolekcija1 << crt;
		Kolekcija<int, int, 10> kolekcija2 = kolekcija1.InsertAt(10, 10,0);
		cout << kolekcija2 << crt;
	
		Kolekcija<int, int, 10> kolekcija3 = kolekcija1.RemoveRange(1, 3);
		cout << kolekcija3 << endl;
		cout << kolekcija1 << crt;
	
		kolekcija1 = kolekcija3;
		cout << kolekcija1;
		Vrijeme
			prviPogodak201633(20, 16, 33),
			drugiPogodak202319(20, 23, 19),
			treciPogodak205108(20, 51, 8),
			cetvrtiPogodak210654(21, 6, 54);
		Igrac denis("Denis Music"), jasmin("Jasmin Azemovic"),
			goran("Goran Skondric"), adil("Adil Joldic");

		if (strcmp(denis.GetID(), "D000M1") == 0 &&
			strcmp(jasmin.GetID(), "J000A2") == 0)
			cout << "ID se uspjesno generise!" << endl;
		Pogodak prviPogodak(prviPogodak201633, "podaci o prvom pogotku"),
			drugiPogodak(drugiPogodak202319, "podaci o drugom pogotku"),
			treciPogodak(treciPogodak205108, "podaci o trecem pogotku"),
			cetvrtiPogodak(cetvrtiPogodak210654, "podaci o cetvrtom pogotku");
				Reprezentacija BIH(BOSNA_I_HERCEGOVINA), ENG(ENGLESKA);
		BIH.AddIgrac(denis);
		BIH.AddIgrac(jasmin);
		ENG.AddIgrac(goran);
		ENG.AddIgrac(adil);
		try
		{
			BIH.AddIgrac(denis);
			
		}
		catch (exception& obj)
		{
			cout << obj.what();
		}
		Prventstvo euro2024;
		euro2024.AddUtakmicu(BIH, ENG);
		try
		{
			euro2024.AddUtakmicu(BIH, ENG);
			
		}
		catch (exception& obj)
		{
			cout << obj.what();
		}
		cout << crt;
		if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "D000M1",prviPogodak))
			cout << "Pogodak uspjesno dodat" << endl;
		if (!euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Denis Music", prviPogodak))
			cout << "Pogodak NIJE uspjesno dodat" << endl;
		if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "J000A2",drugiPogodak))
			cout << "Pogodak uspjesno dodat" << endl;
		if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Jasmin Azemovic", treciPogodak))
			cout << "Pogodak uspjesno dodat" << endl;
		if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Goran Skondric", cetvrtiPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
		
		cout << euro2024;
		
		
			vector<Igrac*> igraci = euro2024(2);
		for (size_t i = 0; i < igraci.size(); i++)
			cout << igraci[i]->GetImePrezime() << endl;
		cin.get();
		system("pause>0");
};