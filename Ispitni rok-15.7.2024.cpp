#include <iostream> 
#include <vector> 
#include <regex> 
#include <string> 
#include <thread> 
#include <mutex> 
using namespace std;
const char* crt = "\n-------------------------------------------\n";
enum Drzava { BOSNA_I_HERCEGOVINA, ENGLESKA, SPANIJA, HOLANDIJA, FRANCUSKA };
ostream& operator<<(ostream& c, Drzava d) {
	const char* drzava[]{ "BOSNA_I_HERCEGOVINA", "ENGLESKA", "SPANIJA", "HOLANDIJA", "FRANCUSKA" };
	cout << drzava[d];
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
int brojacCif(int broj) {
	int brojac = 0;
	while (broj>0)
	{
		broj /= 10;
		brojac++;
	}
	return brojac;
}
char* GenerisiID(int id) {
	if (brojacCif(id) > 4)return nullptr;
	string ide = "";
	ide += "ID#";
	if (brojacCif(id) == 1) {
		ide += "000-";
		ide += to_string(id);
	}
	else if (brojacCif(id) == 2) {
		ide += "00-";
		ide += to_string(id);

	}
	else if (brojacCif(id) == 3) {
		ide += "0-";
		ide += to_string(id);

	}
	else if (brojacCif(id) == 4) {
		ide += to_string(id);

	}
	return GetNizKaraktera(ide.c_str());
}
bool ValidirajID(const char* id) {
	return regex_match(id, regex("^ID#(000-[1-9]{1}|00-[1-9]{2}|0-[1-9]{3}|[1-9]{4})"));
}
template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max];
	T2* _elementi2[max];
	int* _trenutno;
public:
	Kolekcija() {
		_trenutno = new int(0);
		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	Kolekcija(const Kolekcija& obj)
	{
		_trenutno = new int(*obj._trenutno);

		for (int i = 0; i <*_trenutno ; i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
	}
	Kolekcija& operator =(const Kolekcija& obj)
	{
		if (this == &obj)return *this;
		for (int i = 0; i < *_trenutno; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
		_trenutno = new int(*obj._trenutno);

		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
		return *this;
	}
	~Kolekcija() {
		for (size_t i = 0; i < *_trenutno; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
	}
	T1& getElement1(int lokacija)const {
		return *_elementi1[lokacija];
	}
	T2& getElement2(int lokacija)const {
		return *_elementi2[lokacija];
	}
	int getTrenutno() const { return *_trenutno; }
	friend ostream& operator<< (ostream& COUT,const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) <<
			endl;
		return COUT;
	}
	void AddElement(T1 el1, T2 el2) {
		if (*_trenutno == max)return;
		_elementi1[*_trenutno] =new T1(el1);
		_elementi2[*_trenutno] = new T2(el2);
	     (*_trenutno)++;
	}
	Kolekcija InsertAt(int lokacija, T1 el1, T2 el2) {
		for (int i = *_trenutno; i >lokacija ; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		_elementi1[lokacija] = new T1(el1);
		_elementi2[lokacija] = new T2(el2);
		(*_trenutno)++;
		return *this;
	}
	void RemoveAt(int indeks) {
		if (indeks<0 || indeks>=*_trenutno)return;
		(*_trenutno)--;
		for (int i = indeks; i < *_trenutno; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
	}
	Kolekcija* RemoveRange(int start, int end) {
		if (start<0||end>*_trenutno)throw exception("Opseg nije validan!");
		Kolekcija* nova = new Kolekcija;
		for (int i = start; i <=end; i++)
		{
			nova->AddElement(getElement1(start),getElement2(start));
			RemoveAt(start);
		}
		return nova;
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
	~Vrijeme() {
		delete _sat; _sat = nullptr;
		delete _minuta; _minuta = nullptr;
		delete _sekunda; _sekunda = nullptr;
	}
	Vrijeme(const Vrijeme& obj)
	{
		_sat = new int(*obj._sat);
		_minuta = new int(*obj._minuta);
		_sekunda = new int(*obj._sekunda);
	}
	int skunde() const {
		return *_sat * 3600 + *_minuta * 60 + *_sekunda;
	}
	bool operator ==(const Vrijeme& obj) {
		return skunde() == obj.skunde();
	}
	bool operator <(const Vrijeme& obj) {
		return skunde() <obj.skunde();
	}
	bool operator >(const Vrijeme& obj) {
		return skunde() > obj.skunde();
	}
Vrijeme& operator =(const Vrijeme& obj)
{
	if (this == &obj)return *this;
	delete _sat; _sat = nullptr;
	delete _minuta; _minuta = nullptr;
	delete _sekunda; _sekunda = nullptr;
	_sat = new int(*obj._sat);
	_minuta = new int(*obj._minuta);
	_sekunda = new int(*obj._sekunda);
	return *this;
}

	friend ostream& operator<< (ostream& COUT, const Vrijeme& obj) {
		COUT << *obj._sat << ":" << *obj._minuta << ":" <<
			*obj._sekunda;
		return COUT;
	}
};

class Pogodak {
	Vrijeme _vrijemePogotka;
	char* _napomena;
public:
	Pogodak(Vrijeme vrijeme, const char* napomena) :_vrijemePogotka(vrijeme) {
		_napomena = GetNizKaraktera(napomena);
	}
	~Pogodak() {
		delete[] _napomena; _napomena = nullptr;
	}
	Pogodak(const Pogodak& obj)
	{
		_vrijemePogotka = obj._vrijemePogotka;
		_napomena = GetNizKaraktera(obj._napomena);
	}
	Pogodak& operator =(const Pogodak& obj)
	{
		if (this == &obj)return *this;
		delete[] _napomena; _napomena = nullptr;
		_vrijemePogotka = obj._vrijemePogotka;
		_napomena = GetNizKaraktera(obj._napomena);
		return *this;
	}
	Vrijeme GetVrijemePogotka() { return _vrijemePogotka; }
	char* GetNapomena() { return _napomena; }
	friend ostream& operator<< (ostream& COUT, const Pogodak& obj) {
		COUT << obj._vrijemePogotka << " -> " << obj._napomena;
		return COUT;
	}
	bool operator ==(const Pogodak& obj) {
		return _vrijemePogotka == obj._vrijemePogotka && strcmp(obj._napomena, _napomena) == 0;
	}
};

class Igrac {
	static int _id;
	char* _ID; // za inicijalizaciju _ID-a iskoristiti funkciju GenerisiID i vrijednost statickog clana _id
	char* _imePrezime;
	vector <Pogodak> _pogoci;
public:
	Igrac(const char* imePrezime)
	{
		_id++;
		_imePrezime = GetNizKaraktera(imePrezime);
		_ID = GenerisiID(_id);
	}
	~Igrac()
	{
		delete[]_ID; _ID = nullptr;
		delete[]_imePrezime; _imePrezime = nullptr;
	}
	Igrac(const Igrac& obj)
	{
		_ID = GetNizKaraktera(obj._ID);
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_pogoci = obj._pogoci;
	}
	Igrac& operator =(const Igrac& obj)
	{
		if (this == &obj)return *this;
		delete[]_ID; _ID = nullptr;
		delete[]_imePrezime; _imePrezime = nullptr;
		_ID = GetNizKaraktera(obj._ID);
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_pogoci = obj._pogoci;
		return *this;
	}
	char* GetImePrezime() { return _imePrezime; }
	char* GetID() { return _ID; }
	vector<Pogodak>& GetPogoci() { return _pogoci; }
	friend ostream& operator<< (ostream& COUT, Igrac& obj) {
		COUT << *obj._ID << " -> " << obj._imePrezime;
		for (size_t i = 0; i < obj._pogoci.size(); i++)
			cout << obj._pogoci[i] << endl;
		return COUT;
	}
	bool operator==(const Igrac& obj) {
		return strcmp(_ID, obj._ID) == 0;
	}
	bool isPogodak(Pogodak p) {
		for (int i = 0; i < _pogoci.size(); i++)
		{
			if (_pogoci[i] == p) {
				return true;
			}
		}
		return false;
	}
	
};

class Reprezentacija {
	Drzava _drzava;
	vector<Igrac> _igraci;
public:
	Reprezentacija(Drzava drzava) {
		_drzava = drzava;
	}
	Reprezentacija(const Reprezentacija& obj) {
		_drzava = obj._drzava;
		_igraci = obj._igraci;
	}
	Drzava  GetDrzava() { return _drzava; }
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
	Igrac* getIDip(const char* idip) {
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
	Kolekcija<Reprezentacija, Reprezentacija, 20> _utakmice;
public:
	Prventstvo() = default;
	Kolekcija<Reprezentacija, Reprezentacija, 20>& GetUtakmice() { return _utakmice; }
	void AddUtakmicu(Reprezentacija r1, Reprezentacija r2) {
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
		{
			if (_utakmice.getElement1(i).GetDrzava() == r1.GetDrzava() && _utakmice.getElement2(i).GetDrzava() == r2.GetDrzava() ||
				_utakmice.getElement2(i).GetDrzava() == r1.GetDrzava() && _utakmice.getElement1(i).GetDrzava() == r2.GetDrzava()) {
				throw exception("Utakmica je vec dodana!");
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
	int brojUtakmice(Drzava d1, Drzava d2) {
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
		{
			if (_utakmice.getElement1(i).GetDrzava() == d1 && _utakmice.getElement2(i).GetDrzava() == d2) {
				return i;
			}
		}
		return 0;
	}
	void SendSMS(Igrac ig,Drzava d1,Drzava d2) {
		guard.lock();
		this_thread::sleep_for(2s);
		cout << "-----------------------------EMAIL------------------------------" << endl;
		cout << "To:" << ig.GetID() << "@euro2024.com" << endl;
		cout << "From: info@euro2024.com" << endl;
		cout << "Subject: Informacija" << endl;
		cout << "\nPostovani," << endl;
		cout << "U " << ig.GetPogoci()[ig.GetPogoci().size() - 1].GetVrijemePogotka() << " sati igrac " << ig.GetImePrezime() << " je zabiljezio svoj " << ig.GetPogoci().size() << "\npogodak na ovoj utakmici" << endl;
		cout << "Trenutni rezultat je:\n\n" << d1 << " " << _utakmice.getElement1(brojUtakmice(d1, d2)).UkupnoGolova() << " : " << _utakmice.getElement2(brojUtakmice(d1, d2)).UkupnoGolova() << " " << d2 << endl;
		cout << " Puno srece u nastavku susreta." << endl;
		cout << "Neka bolji tim pobijedi." << endl;
		guard.unlock();
	}
	bool AddPogodak(Drzava d1, Drzava d2, const char* idip, Pogodak p) {
		//omoguciti dodavanje pogotka po ID-u ili imenu i prezimenu 
	    //onemoguciti dodavanje istih pogodaka 
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
		{
			if(_utakmice.getElement1(i).GetDrzava() == d1 && _utakmice.getElement2(i).GetDrzava() == d2 ||
				_utakmice.getElement2(i).GetDrzava() == d1 && _utakmice.getElement1(i).GetDrzava() == d2) {
				Igrac* igrac = _utakmice.getElement1(i).getIDip(idip);
				if (!igrac) {
					igrac = _utakmice.getElement2(i).getIDip(idip);
				}
				if (!istiPogodak(p) && igrac) {
					igrac->GetPogoci().push_back(p);
					thread th1(&Prventstvo::SendSMS, this, *igrac, d1, d2);
					th1.join();
					return true;
				}
			}
		}
		return false;
	}
	friend ostream& operator << (ostream& c,const Prventstvo & obj) {
		cout << crt;
		for (int i = 0; i < obj._utakmice.getTrenutno(); i++)
		{
			cout << obj._utakmice.getElement1(i).GetDrzava() << " " << obj._utakmice.getElement1(i).UkupnoGolova() << " : ";
			cout << obj._utakmice.getElement2(i).UkupnoGolova() << " " << obj._utakmice.getElement2(i).GetDrzava() << endl;
			cout << crt;
			for (auto& igrac:obj._utakmice.getElement1(i).GetIgraci())
			{
				for (int j = 0; j < igrac.GetPogoci().size(); j++)
				{
					cout << igrac.GetImePrezime() << endl;
				}
			}
			for (auto& igrac : obj._utakmice.getElement2(i).GetIgraci())
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
	vector<Igrac*>operator()(Vrijeme start,Vrijeme end) {
		vector<Igrac*>tmp;
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
		{
			for (auto& igrac:_utakmice.getElement1(i).GetIgraci())
			{
				for (auto& pogodak:igrac.GetPogoci())
				{
					if (pogodak.GetVrijemePogotka() > start && pogodak.GetVrijemePogotka() < end) {
						tmp.push_back(new Igrac(igrac));
					}
				}
			}
			for (auto& igrac : _utakmice.getElement2(i).GetIgraci())
			{
				for (auto& pogodak : igrac.GetPogoci())
				{
					if (pogodak.GetVrijemePogotka() > start && pogodak.GetVrijemePogotka() < end) {
						tmp.push_back(new Igrac(igrac));
					}
				}
			}
		}
		return tmp;
	}
};
int Igrac::_id = 0;
void main() {

	
	cout << GenerisiID(3) << endl;//treba vratiti ID#000-3 
	cout << GenerisiID(14) << endl;//treba vratiti ID#00-14 
	cout << GenerisiID(156) << endl;//treba vratiti ID#0-156 
	cout << GenerisiID(1798) << endl;//treba vratiti ID#1798 

	if (ValidirajID("ID#000-3"))
		cout << "ID VALIDAN" << endl;
	if (ValidirajID("ID#0-156"))
		cout << "ID VALIDAN" << endl;
	if (!ValidirajID("ID#120-3"))
		cout << "ID NIJE VALIDAN" << endl;
	if (!ValidirajID("ID#00-02"))
		cout << "ID NIJE VALIDAN" << endl;

	int kolekcijaTestSize = 9;
	Kolekcija<int, int, 10> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i); //dodaje vrijednosti u kolekciju

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

	Igrac denis("Denis Music"), jasmin("Jasmin Azemovic"), goran("Goran Skondric"), adil("Adil Joldic");

	if (strcmp(denis.GetID(), "ID#000-1") == 0 && strcmp(jasmin.GetID(), "ID#000-2") == 0)
		cout << "ID se uspjesno generise!" << endl;

	Pogodak prviPogodak(prviPogodak201633, "podaci o prvom pogotku"),
		drugiPogodak(drugiPogodak202319, "podaci o drugom pogotku"),
		treciPogodak(treciPogodak205108, "podaci o trecem pogotku"),
		cetvrtiPogodak(cetvrtiPogodak210654, "podaci o pogotku");

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
		cout << obj.what() << endl;
	}

	Prventstvo euro2024;
	euro2024.AddUtakmicu(BIH, ENG);
	try
	{
		euro2024.AddUtakmicu(BIH, ENG);
	}
	catch (exception& obj)
	{
		cout << obj.what() << endl;
	}

	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-1", prviPogodak))
		cout << "Pogodak uspjesno dodat" << endl;

	if (!euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Denis Music", prviPogodak))
		cout << "Pogodak NIJE uspjesno dodat" << endl;

	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-2", drugiPogodak))
		cout << "Pogodak uspjesno dodat" << endl;

	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Jasmin Azemovic", treciPogodak))
		cout << "Pogodak uspjesno dodat" << endl;

	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Goran Skondric", cetvrtiPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	
	cout << euro2024;

	vector<Igrac*> igraci = euro2024(Vrijeme(20, 15, 15), Vrijeme(20, 24, 15));

	for (size_t i = 0; i < igraci.size(); i++)
		cout << igraci[i]->GetImePrezime() << endl;

	//cin.get();
	system("pause>0");
}