#include <iostream>
#include <vector>
#include <regex>
#include <mutex>
#include<thread>
#include<fstream>
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
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"10.SVE NEDOZVOLJENE RADNJE TOKOM ISPITA CE BITI SANKCIONISANE!\n"
"11. ZA POTREBE TESTIRANJA, U MAINU, BUDITE SLOBODNI DODATI TESTNIH PODATAKA (POZIVA FUNKCIJA) KOLIKO GOD SMATRATE DA JE POTREBNO!\n"
"-------------------------------------------------------------------------------\n";
char* GetNizKaraktera(const char* text) {
    if (text == nullptr)return nullptr;
    char* tmp = new char[strlen(text) + 1];
    strcpy_s(tmp, strlen(text) + 1, text);
    return tmp;
}
unique_ptr<char[]> GetNizKarakteraUnique(const char* txt) {
    if (txt == nullptr)return nullptr;
    unique_ptr<char[]> tmp(new char[strlen(txt) + 1]);
    strcpy_s(tmp.get(), strlen(txt) + 1, txt);
    return tmp;
}
bool ValidirajBrojPasosa(string brojPasosa) {
    return regex_match(brojPasosa, regex("^[A-Z]{1,2}[0-9]{3,4}[- ]?[0-9]{2,4}$"));
}
const char* crt = "\n-------------------------------------------\n";
enum Kriteriji { CISTOCA, USLUGA, LOKACIJA, UDOBNOST };
ostream& operator<<(ostream& c, Kriteriji obj) {
    const char* kriterij[]{ "CISTOCA","USLUGA","LOKACIJA","UDOBNOST" };
    cout << kriterij[obj];
    return c;
}
template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int* _trenutno;
    bool _omoguciDupliranje;
public:
    Kolekcija(bool omoguciDupliranje = true) {
        _elementi1 = new T1;
        _elementi2 = new T2;
        _trenutno = new int(0);
        _omoguciDupliranje = omoguciDupliranje;
    }
    ~Kolekcija() {
        delete[] _elementi1; _elementi1 = nullptr;
        delete[] _elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
    }
    Kolekcija(const Kolekcija& obj)
    {
        _trenutno = new int(*obj._trenutno);
        _omoguciDupliranje = obj._omoguciDupliranje;
        _elementi1 = new T1[*obj._trenutno];
        _elementi2 = new T2[*obj._trenutno];
        for (int i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];
            _elementi2[i] = obj._elementi2[i];
        }
    }
    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() { return *_trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (int i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    Kolekcija& operator =(const Kolekcija& obj) {
        if (this == &obj)return *this;
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
        _omoguciDupliranje = obj._omoguciDupliranje;
        _trenutno = new int(*obj._trenutno);
        _elementi1 = new T1[*_trenutno];
        _elementi2 = new T2[*_trenutno];
        for (int i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];
            _elementi2[i] = obj._elementi2[i];
        }
        return *this;
    }
    void AddElement(T1 el1, T2 el2) {
        if (_omoguciDupliranje == false) {
            for (int i = 0; i < *_trenutno; i++)
            {
                if (el1 == _elementi1[i] && el2 == _elementi2[i]) {
                    throw exception("Zabranjeno dupliranje elemenata!");
                }
            }
        }
        T1* tmp1 = _elementi1;
        T2* tmp2 = _elementi2;
        _elementi1 = new T1[*_trenutno + 1];
        _elementi2 = new T2[*_trenutno + 1];
        for (int i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = tmp1[i];
            _elementi2[i] = tmp2[i];
        }
        _elementi1[*_trenutno] = el1;
        _elementi2[*_trenutno] = el2;
        delete[] tmp1; tmp1 = nullptr;
        delete[] tmp2; tmp2 = nullptr;
        (*_trenutno)++;
    }
    Kolekcija& InsertAt(int lokacija, T1 el1, T2 el2) {
        T1* temp1 = _elementi1;
        T2* temp2 = _elementi2;

        _elementi1 = new T1[*_trenutno + 1];
        _elementi2 = new T2[*_trenutno + 1];

        for (size_t i = 0; i < lokacija; i++)
        {
            _elementi1[i] = temp1[i];
            _elementi2[i] = temp2[i];
        }

        _elementi1[lokacija] = el1;
        _elementi2[lokacija] = el2;


        for (size_t i = lokacija; i < *_trenutno; i++)
        {
            _elementi1[i + 1] = temp1[i];
            _elementi2[i + 1] = temp2[i];
        }
        (*_trenutno)++;
        return *this;
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
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    Datum(const Datum& obj)
    {
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }
    Datum& operator =(const Datum& obj) {
        if (this == &obj)return *this;
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
};
class Komentar {
    char* _sadrzajKomentara;
    //uz komentar, gosti mogu ocijeniti svaki od kriterija zasebno, a int se odnosi na ocjenu u rasponu od 1 - 10.
    //onemoguciti dupliranje kriterija tj. svaki kriterij se moze ocijeniti samo jednom...u suprotnom baciti objekat tipa exception
    Kolekcija<Kriteriji, int>* _ocjeneKriterija;
public:
    Komentar(const char* sadrzajKomentara = "") {
        //kreirajte funkciju GetNizKaraktera ili na drugi adekvatan nacin izvrsite inicijalizaciju pokazivaca
        _sadrzajKomentara = GetNizKaraktera(sadrzajKomentara);
        _ocjeneKriterija = new Kolekcija<Kriteriji, int>;
    }
    Komentar(const Komentar& obj)
    {
        _sadrzajKomentara = GetNizKaraktera(obj._sadrzajKomentara);
        _ocjeneKriterija = new Kolekcija<Kriteriji, int>;
        for (int i = 0; i < obj._ocjeneKriterija->getTrenutno(); i++)
        {
            _ocjeneKriterija->AddElement(obj._ocjeneKriterija->getElement1(i), obj._ocjeneKriterija->getElement2(i));
        }

    }
    Komentar& operator =(const Komentar& obj) {
        if (this == &obj) return *this;
        delete[] _sadrzajKomentara; _sadrzajKomentara = nullptr;
        delete _ocjeneKriterija; _ocjeneKriterija = nullptr;

        _sadrzajKomentara = GetNizKaraktera(obj._sadrzajKomentara);
        _ocjeneKriterija = new Kolekcija<Kriteriji, int>;
        for (int i = 0; i < obj._ocjeneKriterija->getTrenutno(); i++)
        {
            _ocjeneKriterija->AddElement(obj._ocjeneKriterija->getElement1(i), obj._ocjeneKriterija->getElement2(i));
        }
        return *this;
    }
    ~Komentar() {
        delete[] _sadrzajKomentara; _sadrzajKomentara = nullptr;
        delete _ocjeneKriterija; _ocjeneKriterija = nullptr;
    }
    char* GetSadrzajKomentara() { return _sadrzajKomentara; }
    Kolekcija<Kriteriji, int>* GetOcjeneKriterija() { return _ocjeneKriterija; }
    friend ostream& operator<<(ostream& c, Komentar& obj) {
        cout << "Sadrzaj komentara:" << obj._sadrzajKomentara << endl;
        cout << "Ocjene kriterija:" << endl;
        for (int i = 0; i < obj._ocjeneKriterija->getTrenutno(); i++)
        {
            cout <<'\t'<<obj._ocjeneKriterija->getElement1(i) << "(" << obj._ocjeneKriterija->getElement2(i) << ")" << endl;
        }
        return cout;

    }
    void AddOcjenuKriterija(Kriteriji k, int ocjena) {
        for (int i = 0; i < _ocjeneKriterija->getTrenutno(); i++)
        {
            if (_ocjeneKriterija->getElement1(i) == k) {
                throw exception("Vec postoji kriterij!");
            }
        }
        _ocjeneKriterija->AddElement(k, ocjena);
    }
    float GetProsjecnaOcjena() {
        float suma = 0.0f;
        for (int i = 0; i < _ocjeneKriterija->getTrenutno(); i++)
        {
            suma += _ocjeneKriterija->getElement2(i);
        }
        return suma /= _ocjeneKriterija->getTrenutno();
    }
};
mutex guard;
class Gost {
    unique_ptr<char[]> _imePrezime;
    string _emailAdresa;
    string _brojPasosa;
public:
    Gost(const char* imePrezime, string emailAdresa, string brojPasosa) {
        _imePrezime = GetNizKarakteraUnique(imePrezime);
        _emailAdresa = emailAdresa;
        _brojPasosa = ValidirajBrojPasosa(brojPasosa) ? brojPasosa : "NOT VALID";
    }
    Gost(const Gost& obj)
    {
        _imePrezime = GetNizKarakteraUnique(obj._imePrezime.get());
        _emailAdresa = obj._emailAdresa;
        _brojPasosa = obj._brojPasosa;
    }
    Gost operator =(const Gost& obj) {
        if (this == &obj)return *this;
        _imePrezime = GetNizKarakteraUnique(obj._imePrezime.get());
        _emailAdresa = obj._emailAdresa;
        _brojPasosa = obj._brojPasosa;
    }
    string GetEmail() { return _emailAdresa; }
    string GetBrojPasosa() { return _brojPasosa; }
    char* GetImePrezime() { return _imePrezime.get(); }
    bool operator ==(const Gost& obj) {
        return _emailAdresa == obj._emailAdresa && _brojPasosa == obj._brojPasosa;
    }
    friend ostream& operator<< (ostream& COUT, const Gost& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " <<obj._brojPasosa << endl;
        return COUT;
    }
};
class Rezervacija {
    Datum _OD;
    Datum _DO;
    //onemoguciti dodavanje gostiju sa istom email adresom ili brojem pasosa u okviru jedne rezervacije
    vector<Gost> _gosti;
    //za svaku rezervaciju se moze ostaviti samo jedan komentar, bez obzira na broj gostiju
    Komentar _komentar;
public:
    Rezervacija(Datum& OD, Datum& DO, Gost& gost) :_OD(OD), _DO(DO) {
        _gosti.push_back(gost);
 
    }
    Rezervacija(const Rezervacija& obj) {
        _OD = obj._OD;
        _DO = obj._DO;
        _gosti = obj._gosti;
        _komentar = obj._komentar;
    }
    Rezervacija& operator=(const Rezervacija& obj) {
        if (this == &obj)return *this;
        _OD = obj._OD;
        _DO = obj._DO;
        _gosti = obj._gosti;
        _komentar = obj._komentar;
        return *this;
    }
    vector<Gost>& GetGosti() { return _gosti; }
    Komentar GetKomentar() { return _komentar; } 
    friend ostream& operator<< (ostream& COUT, Rezervacija& obj) {
        COUT << crt << "Rezervacija " << obj._OD << " - " << obj._DO << " za goste: " << endl;
        for (size_t i = 0; i < obj._gosti.size(); i++)
            COUT << "\t" << i + 1 << "." << obj._gosti[i];
        cout << crt << "Komentar rezervacije: " << endl << obj._komentar << crt;
        return COUT;
    }
    bool AddGost(Gost g) {
        for (int i = 0; i < _gosti.size(); i++)
        {
            if (_gosti[i] == g) {
                return false;
            }
        }
        _gosti.push_back(g);
    }
    void SendMail() {

        guard.lock();
        this_thread::sleep_for(1s);
        cout << "\nTo:" << endl;
        for (int i = 0; i < _gosti.size(); i++)
        {
            cout << _gosti[i].GetEmail() << endl;
        }
        cout << "Subject: Informacija" << endl;
        cout << "Postovani,\n" << "Zaprimili smo vase ocjene,a njihova prosjecna vrijednost je " << _komentar.GetProsjecnaOcjena() << endl;
        cout << " Zao nam je zbog toga, te ce Vas u najkracem periodu kontaktirati nasa Sluzba za odnose sa gostima.\nUgodan boravak Vam zelimo \nPuno pozdrava" << endl;
        guard.unlock();
        cout << crt;

    }
    void SetKomentar(Komentar& k) {
        _komentar = k;
        int ocjenaManjaOd5 = 0;
        for (int i = 0; i < _komentar.GetOcjeneKriterija()->getTrenutno(); i++)
        {
            if (_komentar.GetOcjeneKriterija()->getElement2(i) < 5) {
                ocjenaManjaOd5++;
            }
        }
        if (ocjenaManjaOd5 >= 2) {
            thread th1(&Rezervacija::SendMail, this);
            th1.detach();
        }
    }
    pair<int, int> GetBrojZnakova(const char* nazivFajla,const char* znakovi) {
        ofstream naziv(nazivFajla);
        naziv << "denis+_music*alimustafic";
        naziv.close();
        ifstream naziv2;
        naziv2.open(nazivFajla);
        char karakter;
        int brojkaraktera = 0, pronadjeniznaokovi = 0;
        while (naziv2.get(karakter)) {
            brojkaraktera++;
            for (int i = 0; i < strlen(znakovi); i++)
            {
                if (znakovi[i] == karakter)
                    pronadjeniznaokovi++;
            }

        }

        return make_pair(brojkaraktera - 1, pronadjeniznaokovi);
    }
};
const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Pojasnite osnovne preduslove koji moraju biti ispunjeni da bi se realizovao polimorfizam?\n";
    return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Pojasnite razloge koristenja kljucnih rijeci abstract i ciste virtualne metode, te razlike izmedju njih?\n";
    return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

void main() {

    Datum
        datum19062022(19, 6, 2022),
        datum20062022(20, 6, 2022),
        datum30062022(30, 6, 2022),
        datum05072022(5, 7, 2022);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);//dodaje vrijednosti u kolekciju

    try {

        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }
    cout << kolekcija1 << crt;


    Kolekcija<int, int> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
    cout << kolekcija2 << crt;





    if (ValidirajBrojPasosa("BH235-532"))
        cout << "Broj pasosa validan" << endl;
    if (ValidirajBrojPasosa("B123321"))
        cout << "Broj pasosa validan" << endl;
    if (ValidirajBrojPasosa("B1252 521"))
        cout << "Broj pasosa validan" << endl;
    if (!ValidirajBrojPasosa("H4521"))
        cout << "Broj pasosa NIJE validan" << endl;
    if (!ValidirajBrojPasosa("b1252 521"))
        cout << "Broj pasosa NIJE validan" << endl;


    Gost denis("Denis Music", "denis@fit.ba", "BH235-532");
    Gost jasmin("Jasmin Azemovic", "jasmin@fit.ba", "B123321");
    Gost adel("Adel Handzic", "adel@edu.fit.ba", "B1252 521");
    Gost gostPasosNotValid("Ime Prezime", "korisnik@klix.ba", "H4521");//_brojPasosa = NOT VALID


    Rezervacija rezervacija(datum19062022, datum20062022, denis);
    if (rezervacija.AddGost(jasmin))
        cout << "Gost uspjesno dodan!";
    cout << crt;
    Komentar komentarRezervacija("Nismo pretjerano zadovoljni uslugom, a ni lokacijom.");
    komentarRezervacija.AddOcjenuKriterija(CISTOCA, 7);
    komentarRezervacija.AddOcjenuKriterija(USLUGA, 4);
    komentarRezervacija.AddOcjenuKriterija(LOKACIJA, 3);
    komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);

    try {
        komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);//kriterij udobnosti je vec ocijenjen!
    }
    catch (exception& err) {
        cout << endl << err.what();
    }
 /*   u okviru rezervacije postavlja vrijednost atributa _komentar */
    rezervacija.SetKomentar(komentarRezervacija);


   ////ispisuje sve podatke o rezervaciji, gostima, komentar, ocjene pojedinih kriterija i prosjecnu ocjenu.
    cout << rezervacija << endl;


    Rezervacija rezervacijaSaAdelom = rezervacija;
    if (rezervacijaSaAdelom.AddGost(adel))
        cout << "Gost uspjesno dodan!";
    if (!rezervacijaSaAdelom.AddGost(denis))
        cout << "\nGost je vec dodan na rezervaciju!";

    cout << rezervacijaSaAdelom << endl;

   // /*metoda GetBrojZnakova treba prebrojati koliko se unutar fajla, cije ime je proslijedjeno kao prvi parametar (relativna putanja), nalazi ukupno znakova, te koliko
   // se unutar pomenutog fajla nalazi trazenih znakova koji su proslijedjeni kao drugi parametar. Vrijednosti brojaca se vracaju kao par.*/
    pair<int, int> brojac = rezervacijaSaAdelom.GetBrojZnakova("rezervacije.txt", "*_+");
    cout << "Ukupno znakova u fajlu: " << brojac.first << endl;
    cout << "Ukupno trazenih znakova: " << brojac.second << endl;
    /*
   // ako se u fajlu rezervacije.txt nalazi sadrzaj "denis+music*_" onda povratna vrijednost metode GetBrojZnakova treba biti par 13 i 3. za potrebe testiranja kreirajte
   // fajl sa zeljenim sadrzajem na istoj lokaciji kao i vas .exe fajl. vodite racuna o tome da se zeljeni naziv fajla definise vrijednoscu parametra.
   // */

    cin.get();
    system("pause>0");
}