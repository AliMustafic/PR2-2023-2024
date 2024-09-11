#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>
#include <regex>


using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. ATRIBUTI, NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U, "
"OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE "
"POMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. METODE I KLASE KOJE NE IMPLEMENTIRATE NEMOJTE BRISATI (BEZ OBZIRA STO NEMAJU IMPLEMENTACIJU)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"10.SVE NEDOZVOLJENE RADNJE TOKOM ISPITA CE BITI SANKCIONISANE!\n"
"11. ZA POTREBE TESTIRANJA, U MAINU, BUDITE SLOBODNI DODATI TESTNIH PODATAKA (POZIVA FUNKCIJA) KOLIKO GOD SMATRATE DA JE POTREBNO!\n"
"12. ZA IZRADU ISPITNOG RJESENJA KORISTITI VISUAL STUDIO 2022!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum Karakteristike { NARUDZBA, KVALITET, PAKOVANJE, ISPORUKA };
ostream& operator<<(ostream& c, Karakteristike k) {
	const char* NizKarakteristika[]{ "NARUDZBA","KVALITET","PAKOVANJE","ISPORUKA" };
	cout << NizKarakteristika[k];
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

template<class T1, class T2>
class Rjecnik {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;
public:
	Rjecnik(bool omoguciDupliranje = true) {
		_omoguciDupliranje = omoguciDupliranje;
		_trenutno = 0;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	~Rjecnik() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	Rjecnik(const Rjecnik& obj) {
		_omoguciDupliranje = obj._omoguciDupliranje;
		_trenutno = obj._trenutno;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() const { return _trenutno; }
	Rjecnik& operator =(const Rjecnik& obj) {
		if (this == &obj)return *this;
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
		_omoguciDupliranje = obj._omoguciDupliranje;
		_trenutno = obj._trenutno;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		return *this;
	}
	friend ostream& operator<< (ostream& COUT, const Rjecnik& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	void AddElement(T1 el1, T2 el2)
	{
		if (!_omoguciDupliranje)
			for (int i = 0; i < _trenutno; i++)
				if (_elementi1[i] == el1 || _elementi2[i] == el2)
					throw exception("Dupliranje elemenata nije dozvoljeno.");

		T1* tmp1 = _elementi1;
		T2* tmp2 = _elementi2;
		_elementi1 = new T1[_trenutno + 1];
		_elementi2 = new T2[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = tmp1[i];
			_elementi2[i] = tmp2[i];
		}
		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
		delete[] tmp1; tmp1 = nullptr;
		delete[] tmp2; tmp2 = nullptr;
	}

	Rjecnik operator()(int OD, int DO) {
		Rjecnik tmp = new Rjecnik();
		for (int i = OD; i <= DO; i++)
		{
			tmp.AddElement(_elementi1[i], _elementi2[i]);
		}
		return tmp;
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& obj)
	{
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum& operator =(const Datum& obj)
	{
		if (this == &obj)return *this;
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		return*this;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}

};

class ZadovoljstvoKupca {
	int _ocjena; //za svaku kupovinu kupac moze ostaviti jednu ocjenu
	//uz ocjenu, kupci mogu komentarisati svaku od karakteristika proizvoda.
	//onemoguciti dupliranje karakteristika tj. svaka karakteristika se moze komentarisati samo jednom...u suprotnom baciti objekat tipa exception
	Rjecnik<Karakteristike, const char*> _komentariKarakteristika;
public:
	ZadovoljstvoKupca(int ocjena = 0) :_komentariKarakteristika(false) {
		_ocjena = ocjena;
	}
	ZadovoljstvoKupca(const ZadovoljstvoKupca& obj)
	{
		_ocjena = obj._ocjena;
		_komentariKarakteristika = obj._komentariKarakteristika;
	}
	ZadovoljstvoKupca& operator=(const ZadovoljstvoKupca& obj)
	{
		if (this == &obj)return *this;
		_ocjena = obj._ocjena;
		_komentariKarakteristika = obj._komentariKarakteristika;
		return *this;
	}
	friend ostream& operator <<(ostream& c, const ZadovoljstvoKupca& obj) {
		cout << "Ocjena:" << obj._ocjena << endl;
		cout << "Komenatari karateristika:" << endl;
		for (int i = 0; i < obj._komentariKarakteristika.getTrenutno(); i++)
		{
			cout << obj._komentariKarakteristika.getElement1(i) << "-" << obj._komentariKarakteristika.getElement2(i) << endl;
		}
		return cout;
	}
	int GetOcjena() { return _ocjena; }
	Rjecnik<Karakteristike, const char*>& GetKomentareKarakteristika() { return _komentariKarakteristika; }
	void DodajKomentarKarakteristike(Karakteristike karakteristika, const char* text) {
		for (int i = 0; i < _komentariKarakteristika.getTrenutno(); i++)
		{
			if (_komentariKarakteristika.getElement1(i) == karakteristika)
				throw exception("Ne mozete duplirati karakteristiku! ");
		}
		_komentariKarakteristika.AddElement(karakteristika, text);
	}
	bool operator == (ZadovoljstvoKupca& obj)
	{
		if (_ocjena != obj._ocjena || _komentariKarakteristika.getTrenutno() != obj._komentariKarakteristika.getTrenutno())
			return false;

		for (int i = 0; i < _komentariKarakteristika.getTrenutno(); i++)
			if (_komentariKarakteristika.getElement1(i) != obj._komentariKarakteristika.getElement1(i) ||
				strcmp(_komentariKarakteristika.getElement2(i), obj._komentariKarakteristika.getElement2(i)) != 0)
				return false;

		return true;
	}

};

class Osoba {
protected:
	char* _imePrezime;
	Datum _datumRodjenja;
public:
	Osoba(const char* imePrezime = "", Datum datumRodjenja = Datum()) : _datumRodjenja(datumRodjenja) {
		_imePrezime = GetNizKaraktera(imePrezime);
	}
	~Osoba() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	Osoba(const Osoba& obj)
	{
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_datumRodjenja = obj._datumRodjenja;
	}
	Osoba& operator =(const Osoba& obj)
	{
		if (this == &obj) return *this;
		delete[] _imePrezime; _imePrezime = nullptr;

		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_datumRodjenja = obj._datumRodjenja;
		return *this;
	}

	char* GetImePrezime() { return _imePrezime; }
	Datum& GetDatumRodjenja() { return _datumRodjenja; }

	virtual void Info() = 0;
	friend ostream& operator<< (ostream& COUT, const Osoba& obj) {
		COUT << obj._imePrezime << " " << obj._datumRodjenja << endl;
		return COUT;
	}
};
mutex guard;
class Kupac :public Osoba {
	char* _emailAdresa;
	//float se odnosi na iznos racuna za odredjenu kupovinu
	Rjecnik<float, ZadovoljstvoKupca>* _kupovine;
	vector<int> _bodovi; // bodovi sakupljeni tokom kupovine, svakih potrosenih 10KM donosi po 1 bod.
public:
	Kupac(const char* imePrezime = "", Datum datumRodjenja = Datum(), const char* emailAdresa = "") :Osoba(imePrezime, datumRodjenja) {
		_emailAdresa = GetNizKaraktera(emailAdresa);
		_kupovine = new Rjecnik<float, ZadovoljstvoKupca>(false);
	}
	~Kupac()
	{
		delete[] _emailAdresa; _emailAdresa = nullptr;
		delete _kupovine; _kupovine = nullptr;
	}
	char* GetEmail() { return _emailAdresa; }
	Rjecnik<float, ZadovoljstvoKupca>& GetKupovine() { return *_kupovine; }
	vector<int> GetBodovi() { return _bodovi; }
	int GetBodoviUkupno() {
		int ukupno = 0;
		for (size_t i = 0; i < _bodovi.size(); i++) ukupno += _bodovi[i];
		return ukupno;
	}
	void Info() {
		cout << *this;
	}
	friend ostream& operator<< (ostream& COUT, const Kupac& obj) {
		COUT << crt << obj._imePrezime << " " << obj._datumRodjenja << " " << obj._emailAdresa << " " << endl;
		COUT << "KUPOVINE -> " << crt;
		for (size_t i = 0; i < obj._kupovine->getTrenutno(); i++)
			COUT << "Iznos racuna: " << obj._kupovine->getElement1(i) << "KM, zadovoljstvo kupca: " << obj._kupovine->getElement2(i) << crt;
		COUT << "BODOVI -> ";
		for (size_t i = 0; i < obj._bodovi.size(); i++)
			COUT << obj._bodovi[i] << ", ";
		COUT << crt;
		return COUT;
	}
	void SendEmail(char* email,int bodovi, int bodoviUkupno) {
		guard.lock();
		this_thread::sleep_for(3s);
		cout << "\n---------------------------------EMAIL----------------------------------------" << endl;
		cout << "To:" << email << endl;
		cout << "\nPostovani," << endl;
		cout << " Prilikom posljednje kupovine ste ostvarili " << bodovi << " bodova,tako da trenutno vas ukupan broj bodova iznosi " << bodoviUkupno << endl;
		cout << " Zahvaljujemo vam na kupovini.\nPuno pozdrava" << endl;
		guard.unlock();
	}
	void DodajKupovinu(float cijena, ZadovoljstvoKupca zk) {
		_kupovine->AddElement(cijena, zk);
		int bodovi = cijena / 10;
		char* email = _emailAdresa;
		_bodovi.push_back(bodovi);
		int bodoviukupno = GetBodoviUkupno();
		if (bodovi > 5) {
			thread th1(&Kupac::SendEmail, this,email, bodovi, bodoviukupno);
			th1.detach();
		}
	}
	void SacuvajBodove() {
		string emajl = _emailAdresa;
		emajl += ".txt";
		fstream mojFajl(emajl,ios::out);
		for (int i = 0; i < _bodovi.size(); i++)
		{
			mojFajl << _bodovi[i] << " ";
		}
		mojFajl.close();
	}
	void UcitajBodove() {
		string emajl = _emailAdresa;
		emajl += ".txt";
		fstream mojFajl(emajl,ios::in);
		int reading;
		while (!mojFajl.eof()) {
			mojFajl >> reading;
			if (!mojFajl.eof()) {
				_bodovi.push_back(reading);
			}
		}
		mojFajl.close();
	}
	Rjecnik<Karakteristike, const char*>GetKupovineByKomentar(string greska) {
		Rjecnik<Karakteristike, const char*>nova;
		regex pravilo(greska);
		for (int i = 0; i < _kupovine->getTrenutno(); i++)
		{
			Rjecnik<Karakteristike, const char*>& zk = _kupovine->getElement2(i).GetKomentareKarakteristika();
			for (int j = 0; j < zk.getTrenutno(); j++)
			{
				if (regex_search(zk.getElement2(j), pravilo)) {
					nova.AddElement(zk.getElement1(j), zk.getElement2(j));
				}
			}

		}
		return nova;
	}
};

const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Nabrojite i ukratko pojasnite osnovne razlike izmedju list i vector klase?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Pojasnite opcije vezane za preslikavanje (vrijednosti, referenci, dostupnost) parametara prilikom koristenja lambda funkcija?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

void main() {

	/*cout << PORUKA;
	cin.get();

	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();*/

	const int rjecnikTestSize = 9;
	Rjecnik<int, int> rjecnik1(false);
	for (int i = 0; i < rjecnikTestSize - 1; i++)
		rjecnik1.AddElement(i, i);
	try {
		
		rjecnik1.AddElement(3, 3);
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}
	rjecnik1.AddElement(9, 9);

	cout << rjecnik1 << crt;

	Rjecnik<int, int> rjecnik2 = rjecnik1(2, 5);
	
	cout << rjecnik2 << crt;


	const int maxKupaca = 3;
	Osoba* kupci[maxKupaca];
	kupci[0] = new Kupac("Denis Music", Datum(12, 1, 1980), "denis@fit.ba");
	kupci[1] = new Kupac("Jasmin Azemovic", Datum(12, 2, 1980), "jasmin@fit.ba");
	kupci[2] = new Kupac("Adel Handzic", Datum(12, 3, 1980), "adel@edu.fit.ba");

	ZadovoljstvoKupca zadovoljstvoKupca(7);
	zadovoljstvoKupca.DodajKomentarKarakteristike(NARUDZBA, "Nismo mogli odabrati sve potrebne opcije");
	zadovoljstvoKupca.DodajKomentarKarakteristike(KVALITET, "Kvalitet je ocekivan");

	try {
		
		zadovoljstvoKupca.DodajKomentarKarakteristike(KVALITET, "Kvalitet je ocekivan");
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}

	zadovoljstvoKupca.DodajKomentarKarakteristike(PAKOVANJE, "Pakovanje je bio osteceno");
	zadovoljstvoKupca.DodajKomentarKarakteristike(ISPORUKA, "Mada su najavili da ce proizvod biti isporucen u roku od 2 dana, cekali smo 5 dana");


	Kupac* denis = dynamic_cast<Kupac*>(kupci[0]);

	denis->DodajKupovinu(128, zadovoljstvoKupca);
	cout << "Ukupno bodova -> " << denis->GetBodoviUkupno();//12 bodova


	ZadovoljstvoKupca zadovoljstvoKupca2(4);
	zadovoljstvoKupca2.DodajKomentarKarakteristike(KVALITET, "Jako lose, proizvod ostecen");
	denis->DodajKupovinu(81, zadovoljstvoKupca2);

	cout << "\nUkupno bodova -> " << denis->GetBodoviUkupno();//20 bodova

	  denis->Info();

	  denis->SacuvajBodove();

	  denis->UcitajBodove();

	  Rjecnik<Karakteristike, const char*> osteceniProizvodi = denis->GetKupovineByKomentar("ostecen");
	  cout << "Rezultat pretrage -> " << crt << osteceniProizvodi;

	for (size_t i = 0; i < maxKupaca; i++)
		delete kupci[i], kupci[i] = nullptr;

	cin.get();
	system("pause>0");
}