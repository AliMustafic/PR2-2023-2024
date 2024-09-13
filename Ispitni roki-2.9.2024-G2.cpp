/*
	

	Ja nisam izlazio na ovaj rok, samo sam prikupio odredene informacije kroz pricu
	drugih studenata i slika koje su nekako uslikane tokom ispita. Moguce je da postoji
	mnogo vise razlicitih stvari nego sto sam naveo ovdje.
	Teoretska pitanja sam obrisao jer o njima nisam cuo nista.
*/

#include <iostream>
#include<vector>
#include<string>
#include<regex>
#include<thread>
#include<mutex>

using namespace std;
const char* crt = "\n-------------------------------------------\n";
enum Drzava { ENGLESKA, SPANIJA, HOLANDIJA, FRANCUSKA, BOSNA_I_HERCEGOVINA };
ostream& operator <<(ostream& cout, Drzava d) {
	const char* drzave[]{ "ENGLESKA","SPANIJA","HOLANDIJA","FRANCUSKA","BOSNA_I_HERCEGOVINA"};
	cout << drzave[d];
	return cout;
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
int brojCifara(int broj) {
	int brojac = 0;
	while (broj > 0) {
		broj /= 10;
		brojac++;
	}
	return brojac;
}
int obrniCifre(int broj) {
	int novi = 0;
	while (broj > 0) {
		int broj1=broj % 10;
		novi = novi * 10 + broj1;
		broj /= 10;
	}
	return novi;
}
char zadnjeSlovoImena(const char* ip) {
	for (int i = 0; i < strlen(ip); i++)
	{
		if (ip[i] == ' ') { return ip[i - 1]; }
	}
}
char prvoSlovoPrezimena(const char* ip) {
	for (int i = 0; i < strlen(ip); i++)
	{
		if (ip[i] == ' ') {
			return ip[i + 1];
		}
	}
	return '\0';
}
char* GenerisiID(const char* ip, int id) {
	string iD = "";
	//S0003M
	iD += toupper(zadnjeSlovoImena(ip));
	if (id < 10) {
		iD += "000";
		iD +=to_string(id);
	}
	else if (id >9&&id<100) {
		iD += "00";
		iD += to_string(obrniCifre(id));
	}
	else if (id >99 && id < 1000) {
		iD += "0";
		iD += to_string(obrniCifre(id));
	}
	else if (id >1000) {
		iD += to_string(obrniCifre(id));
	}
	iD += toupper(prvoSlovoPrezimena(ip));

	return GetNizKaraktera(iD.c_str());
}
bool ValidirajID(const char* id) {
	return regex_match(id, regex("^[A-Z]{1}(000[1-9]{1}|00[1-9]{2}|0[1-9]{3}|[1-9]{4})[A-Z]{1}$"));
}
template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutno;
public:
	Kolekcija()
	{
		_trenutno = 0;
	}
	~Kolekcija()
	{

	}
	int getTrenutno()const {
		return _trenutno;
	}
	T1& getElement1(int lokacija) {
		return _elementi1[lokacija];
	}
	T2& getElement2(int lokacija) {
		return _elementi2[lokacija];
	}
	friend ostream& operator<< (ostream& COUT, Kolekcija& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	void AddElement(T1 el1, T2 el2) {
		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
	}
	Kolekcija(const Kolekcija& obj)
	{
		_trenutno = obj._trenutno;
		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	Kolekcija& operator =(const Kolekcija& obj)
	{
		if (this == &obj)return *this;
		_trenutno = obj._trenutno;
		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		return *this;
	}
	Kolekcija InsertAt(int lokacija, T1 el1, T2 el2) {
		for (int i = _trenutno; i > lokacija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		_elementi1[lokacija] = el1;
		_elementi2[lokacija] = el2;
		_trenutno++;
		return *this;
	}
	void RemoveAt(int indeks) {
		if (indeks < 0 || indeks >= getTrenutno())return;
		_trenutno--;
		for(int i = indeks; i < _trenutno; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
	}
	Kolekcija* RemoveRange(int start,int end) {

		if (start < 0 && end >= _trenutno) {
			throw exception("Izvan validnog opsega!");
		}
		Kolekcija* tmp = new Kolekcija;
		for (int i = start; i <=end; i++)
		{
			tmp->AddElement(getElement1(start), getElement2(start));
			RemoveAt(start);
		}
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
	int sek() const {
		return *_sekunda + *_sat * 3600 + *_minuta * 60;
	}
	bool operator ==(const Vrijeme& obj) {
		return obj.sek() == sek();
	}
};
class Pogodak {
	Vrijeme* _vrijemePogotka;
	char* _napomena;
public:
	Pogodak(Vrijeme vrijeme, const char* napomena) :
		_vrijemePogotka(new Vrijeme(vrijeme))
	{
		_napomena = GetNizKaraktera(napomena);
	}
	~Pogodak()
	{
		delete[] _napomena; _napomena = nullptr;
		delete _vrijemePogotka; _vrijemePogotka = nullptr;
	}
	Pogodak(const Pogodak& obj) :
		_vrijemePogotka(new Vrijeme(*obj._vrijemePogotka))
	{
		_napomena = GetNizKaraktera(obj._napomena);
	}
	Pogodak& operator =(const Pogodak& obj)
	{
		if (this == &obj)return *this;
		delete[] _napomena; _napomena = nullptr;
		delete _vrijemePogotka; _vrijemePogotka = nullptr;
		_vrijemePogotka =new Vrijeme(*obj._vrijemePogotka);
		_napomena = GetNizKaraktera(obj._napomena);
		return *this;
	}
	Vrijeme GetVrijemePogotka() { return *_vrijemePogotka; }
	char* GetNapomena() { return _napomena; }
	friend ostream& operator<< (ostream& COUT, const Pogodak& obj) {
		COUT << *obj._vrijemePogotka << " -> " << obj._napomena;
		return COUT;
	}
	bool operator==(const Pogodak& obj) {
		return (*_vrijemePogotka == *obj._vrijemePogotka) && strcmp(obj._napomena, _napomena) == 0;
	}
};
class Igrac {
	static int _id;
	char* _ID; // za inicijalizaciju _ID-a iskoristiti funkciju GenerisiID i vrijednost statickog clana _id
	char* _imePrezime;
	vector<Pogodak*> _pogoci;
public:
	Igrac(const char* imePrezime)
	{
		++_id;
		_imePrezime = GetNizKaraktera(imePrezime);
		_ID = GenerisiID(_imePrezime, _id);
	}
	~Igrac()
	{
		delete[]_ID; _ID = nullptr;
		delete[]_imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _pogoci.size(); i++) {
			delete _pogoci[i]; _pogoci[i] = nullptr;
		}
	}
	Igrac(const Igrac& obj)
	{
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_ID = GetNizKaraktera(obj._ID);
		for (int i = 0; i < obj._pogoci.size(); i++)
		{
			_pogoci.push_back(new Pogodak(*obj._pogoci[i]));
		}
	}
	Igrac& operator= (const Igrac& obj)
	{
		if (this == &obj)return *this;
		delete[]_ID; _ID = nullptr;
		delete[]_imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _pogoci.size(); i++) {
			delete _pogoci[i]; _pogoci[i] = nullptr;
		}
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_ID = GetNizKaraktera(obj._ID);
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
			cout <<*obj._pogoci[i] << endl;
		return COUT;
	}
	bool isPogodak(Pogodak p) {
		for (int i = 0; i < _pogoci.size(); i++)
		{
			if (p ==*_pogoci[i])return true;
		}
		return false;
	}
	bool operator ==(const Igrac& obj) {
		return strcmp(_ID, obj._ID) == 0;
	}
};
class DrzavaTim {
	Drzava _drzava;
	vector<Igrac> _igraci;
public:
	DrzavaTim(Drzava drzava= BOSNA_I_HERCEGOVINA) {
		_drzava = drzava;
	}
	DrzavaTim(const DrzavaTim& obj)
	{
		_drzava = obj._drzava;
		_igraci = obj._igraci;
	}
	DrzavaTim& operator =(const DrzavaTim& obj)
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
				throw exception("Igrac je vec dodan!");
			}
		}
		_igraci.push_back(ig);
	}
	bool operator==(const DrzavaTim& obj) {
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
	int UkupnoGolova() {
		int rez = 0;
		for (int i = 0; i < _igraci.size(); i++)
		{
			rez += _igraci[i].GetPogoci().size();
		}
		return rez;
	}
};
mutex guard;
class Prventstvo {
	Kolekcija<DrzavaTim, DrzavaTim, 20> _utakmice;
public:
	Prventstvo() = default;
	Kolekcija<DrzavaTim, DrzavaTim, 20>& GetUtakmice() {
		return _utakmice;
	}
	void AddUtakmicu(DrzavaTim d1, DrzavaTim d2) {
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
		{
			if (_utakmice.getElement1(i) == d1 && _utakmice.getElement2(i) == d2||
				_utakmice.getElement1(i) == d2 && _utakmice.getElement2(i) == d1) {
				throw exception("Utakmice su vec odigrane!");
			}
		}
		_utakmice.AddElement(d1, d2);
	}
	bool istiPogodak(Pogodak p) {
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
		{
			for (int j = 0; j <_utakmice.getElement1(i).GetIgraci().size() ; j++)
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
	int brojUtakmice(Drzava d1, Drzava d2) {
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
		{
			if (_utakmice.getElement1(i).GetDrzava() == d1 && _utakmice.getElement2(i).GetDrzava() == d2) { return i; }
		}
		return 0;
	}
	void SendEmail(Igrac ig,Drzava d1,Drzava d2) {
		guard.lock();
		this_thread::sleep_for(2s);
		cout << "To:" << ig.GetID() << "euro2024.com" << endl;
		cout << "From: info@euro2024.com" << endl;
		cout << "Subject: Informacija" << endl;
		cout << "Postovani," << endl;
		cout << "U " << ig.GetPogoci()[ig.GetPogoci().size() - 1]->GetVrijemePogotka() << " sati igrac " << ig.GetImePrezime() << " je zabiljezio svoj " << ig.GetPogoci().size();
		cout << " pogodak na ovoj utakmici." << endl;
		cout << "Trenutni rezultat je:" << endl;
		cout << d1 << " : " << d2 << endl;
		cout << "     (" << _utakmice.getElement1(brojUtakmice(d1, d2)).UkupnoGolova() << ")          (" << _utakmice.getElement2(brojUtakmice(d1, d2)).UkupnoGolova() << ") " << endl;
		cout << "Puno srece u nastavku susreta." << endl;
		cout << "Neka bolji tim pobijedi." << endl;
		guard.unlock();
	}
	bool AddPogodak(Drzava d1,Drzava d2,const char* idip,Pogodak p) {
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
		{
			if (_utakmice.getElement1(i).GetDrzava() == d1 && _utakmice.getElement2(i).GetDrzava() == d2||
				_utakmice.getElement1(i).GetDrzava() == d2 && _utakmice.getElement2(i).GetDrzava() == d1) {
				Igrac* igrac = _utakmice.getElement1(i).getIdIp(idip);
				if (!igrac) {
					igrac = _utakmice.getElement2(i).getIdIp(idip);
				}
				if (igrac && !istiPogodak(p)) {
					igrac->GetPogoci().push_back(new Pogodak(p));
					thread th1(&Prventstvo::SendEmail,this,*igrac,d1,d2);
					th1.join();
					return true;
				}
			}
		}
		return false;
	}
	friend ostream& operator <<(ostream& cout,  Prventstvo& obj) {
		cout << crt;
		for (int i = 0; i <obj._utakmice.getTrenutno(); i++)
		{
			cout << obj._utakmice.getElement1(i).GetDrzava() << " : " << obj._utakmice.getElement2(i).GetDrzava() << endl;
			cout <<"            "<< obj._utakmice.getElement1(i).UkupnoGolova() << "          " << obj._utakmice.getElement2(i).UkupnoGolova() << endl;
			cout << crt;

			for (auto& igrac:obj._utakmice.getElement1(i).GetIgraci())
			{
				for (int j = 0; j < igrac.GetPogoci().size(); j++)
				{
					cout << igrac.GetImePrezime() << endl;
				}
			}
			for (auto& igrac :obj._utakmice.getElement2(i).GetIgraci() )
			{
				for (int j = 0; j < igrac.GetPogoci().size(); j++)
				{
					cout << igrac.GetImePrezime() << endl;
				}
			}
		}
		cout << crt;
		return cout;
	}
	vector<Igrac*>operator()(int brojGolova) {
		vector<Igrac*>novi;
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
		{
			for (auto& igrac:_utakmice.getElement1(i).GetIgraci())
			{
				if (igrac.GetPogoci().size() >= brojGolova) {
					novi.push_back(new Igrac(igrac));
				}
			}
			for (auto& igrac:_utakmice.getElement2(i).GetIgraci())
			{
				if (igrac.GetPogoci().size() >= brojGolova) {
					novi.push_back(new Igrac(igrac));
				}
			}
		}
		return novi;
	}
};
int Igrac::_id = 0;
void main() {
	/*cout << PORUKA;
	cin.get();*/

	cout << GenerisiID("Denis Music", 3) << endl;//treba vratiti S0003M
	cout << GenerisiID("jasmin azemovic", 14) << endl;//treba vratiti N0041A
	cout << GenerisiID("Goran skondric", 156) << endl;//treba vratiti N0651S
	cout << GenerisiID("emina Junuz", 1798) << endl;//treba vratiti A8971J

	if (ValidirajID("S0003M"))
		cout << "ID VALIDAN" << endl;
	if (ValidirajID("N0041A"))
		cout << "ID VALIDAN" << endl;
	if (!ValidirajID("N00651S"))
		cout << "ID NIJE VALIDAN" << endl;
	if (!ValidirajID("a8971j"))
		cout << "ID NIJE VALIDAN" << endl;
	int kolekcijaTestSize = 9;
	Kolekcija<int, int, 10> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);//dodaje vrijednosti u kolekciju
	cout << kolekcija1 << crt;

	Kolekcija<int, int, 10> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
	cout << kolekcija2 << crt;
	
	Kolekcija<int, int, 10>* kolekcija3 = kolekcija1.RemoveRange(1, 3);
	cout << *kolekcija3 << endl;
	cout << kolekcija1 << crt;
	
	kolekcija1 = *kolekcija3;
	cout << kolekcija1;
	Vrijeme
		prviPogodak201633(20, 16, 33),
		drugiPogodak202319(20, 23, 19),
		treciPogodak205108(20, 51, 8),
		cetvrtiPogodak210654(21, 6, 54);
	Igrac denis("Denis Music"), jasmin("Jasmin Azemovic"),
		goran("Goran Skondric"), adil("Adil Joldic");
	if (strcmp(denis.GetID(), "S0001M") == 0 && strcmp(jasmin.GetID(), "N0002A") == 0)
		cout << "ID se uspjesno generise!" << endl;
	Pogodak prviPogodak(prviPogodak201633, "podaci o prvom pogotku"),
		drugiPogodak(drugiPogodak202319, "podaci o drugom pogotku"),
		treciPogodak(treciPogodak205108, "podaci o trecem pogotku"),
		cetvrtiPogodak(cetvrtiPogodak210654, "podaci o cetvrtom pogotku");
	DrzavaTim BIH(BOSNA_I_HERCEGOVINA), ENG(ENGLESKA);
	BIH.AddIgrac(denis);
	BIH.AddIgrac(jasmin);
	ENG.AddIgrac(goran);
	ENG.AddIgrac(adil);
	try
	{
		//onemoguciti dodavanje istih igraca - provjeravati ID, baciti izuzetak
		BIH.AddIgrac(denis);
	}
	catch (exception& obj)
	{
		cout << obj.what() << '\n';
	}
	Prventstvo euro2024;
	euro2024.AddUtakmicu(BIH, ENG);
	try
	{
		//onemoguciti ponovne susrete drzave tokom istog prvenstva, baciti izuzetak
		euro2024.AddUtakmicu(BIH, ENG);
	}
	catch (exception& obj)
	{
		cout << obj.what() << '\n';
	}

	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "S0001M", prviPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	if (!euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Denis Music", prviPogodak))
		cout << "Pogodak NIJE uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "N0002A", drugiPogodak))
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
}