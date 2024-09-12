#include <iostream>
#include<vector>
using namespace std;
char* GetNizKaraktera(const char* txt) {
    if (txt == nullptr)return nullptr;
    char* tmp = new char[strlen(txt) + 1];
    strcpy_s(tmp, strlen(txt) + 1, txt);
    return tmp;
}
const char* crt = "\n-------------------------------------------\n";
enum Kriteriji { CISTOCA, USLUGA, LOKACIJA, UDOBNOST };
ostream& operator <<(ostream& c, Kriteriji k) {
    const char* kriteriji[]{ "CISTOCA","USLUGA","LOKACIJA","UDOBNOST" };
    cout << kriteriji[k];
    return cout;
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
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
    }
    Kolekcija& operator=(const Kolekcija& obj)
    {
        if (this == &obj)return *this;
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
        _trenutno = new int(*obj._trenutno);
        _omoguciDupliranje = obj._omoguciDupliranje;
        _elementi1 = new T1[*_trenutno];
        _elementi2 = new T2[*_trenutno];
        for (int i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];
            _elementi2[i] = obj._elementi2[i];
        }
        return *this;
    }
    Kolekcija(const Kolekcija& obj)
    {
        _trenutno = new int(*obj._trenutno);
        _omoguciDupliranje = obj._omoguciDupliranje;
        _elementi1 = new T1[*_trenutno];
        _elementi2 = new T2[*_trenutno];
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
        for (size_t i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    void AddElement(T1 el1, T2 el2) {
        if (!_omoguciDupliranje) {
            for (int i = 0; i < *_trenutno; i++)
            {
                if (_elementi1[i] == el1 && _elementi2[i] == el2) {
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
        T1* tmp1 = _elementi1;
        T2* tmp2 = _elementi1;
        _elementi1 = new T1[*_trenutno + 1];
        _elementi2 = new T2[*_trenutno + 1];
        for (int i = 0; i < lokacija; i++)
        {
            _elementi1[i] = tmp1[i];
            _elementi2[i] = tmp2[i];
        }
        _elementi1[lokacija] = el1;
        _elementi2[lokacija] = el2;
        for (int i = lokacija; i < *_trenutno; i++)
        {
            _elementi1[i+1] = tmp1[i];
            _elementi2[i+1] = tmp2[i];
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
    Datum(const Datum& obj) {
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
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." <<* obj._mjesec << "." << *obj._godina;
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
        _ocjeneKriterija = new Kolekcija<Kriteriji, int>();
    }
    Komentar(const Komentar& obj) {
        _sadrzajKomentara = GetNizKaraktera(obj._sadrzajKomentara);
        _ocjeneKriterija = new Kolekcija<Kriteriji, int>;
        for (int i = 0; i < obj._ocjeneKriterija->getTrenutno(); i++)
        {
            _ocjeneKriterija->AddElement(obj._ocjeneKriterija->getElement1(i), obj._ocjeneKriterija->getElement2(i));
        }
    }
    Komentar operator =(const Komentar& obj) {
        if (this == &obj)return *this;
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
    friend ostream& operator <<(ostream& c, const Komentar& obj) {
        cout << "Sadrzaj komentara:" << obj._sadrzajKomentara << endl;
        for (int i = 0; i < obj._ocjeneKriterija->getTrenutno(); i++)
        {
            cout <<"\t"<<obj._ocjeneKriterija->getElement1(i) << " " << "Ocjena " <<"("<< obj._ocjeneKriterija->getElement2(i)<<")" << endl;
        }
        return cout;
    }
    char* GetSadrzajKomentara() { return _sadrzajKomentara; }
    Kolekcija<Kriteriji, int>* GetOcjeneKriterija() { return _ocjeneKriterija; }
    void AddOcjenuKriterija(Kriteriji k, int ocjena) {
        for (int i = 0; i < _ocjeneKriterija->getTrenutno(); i++)
        {
            if (_ocjeneKriterija->getElement1(i) == k && _ocjeneKriterija->getElement2(i) == ocjena) {
                throw exception("Kriterij se moze ocjeniti samo jednom!");
            }
        }
        _ocjeneKriterija->AddElement(k, ocjena);
    }
    float ProsjecnaOcjena() {
        float prosjek = 0.0f;
        for (int i = 0; i < _ocjeneKriterija->getTrenutno(); i++)
        {
            prosjek += _ocjeneKriterija->getElement2(i);
        }
        return prosjek / _ocjeneKriterija->getTrenutno();
    }
};



class Gost {
    char* _imePrezime;
    string _emailAdresa;
    string _brojPasosa;
public:
    Gost(const char* imePrezime, string emailAdresa, string brojPasosa) {
        _imePrezime = GetNizKaraktera(imePrezime);;
        _emailAdresa = emailAdresa;
        _brojPasosa = brojPasosa;
    }
    Gost(const Gost& obj) {
        _imePrezime = GetNizKaraktera(obj._imePrezime);;
        _emailAdresa = obj._emailAdresa;
        _brojPasosa = obj._brojPasosa;
    }
    Gost& operator =(const Gost& obj) {
        if (this == &obj)return *this;
        delete[] _imePrezime; _imePrezime = nullptr;
        _imePrezime = GetNizKaraktera(obj._imePrezime);;
        _emailAdresa = obj._emailAdresa;
        _brojPasosa = obj._brojPasosa;
        return *this;
    }
    ~Gost()
    {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    string GetEmail() { return _emailAdresa; }
    string GetBrojPasosa() { return _brojPasosa; }
    char* GetImePrezime() { return _imePrezime; }
    friend ostream& operator<< (ostream& COUT, const Gost& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojPasosa << endl;
        return COUT;
    }
    bool operator ==(const Gost& obj) {
        return _emailAdresa == obj._emailAdresa && _brojPasosa == obj._brojPasosa;
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
    Rezervacija(const Rezervacija& obj){
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
        cout << "Prosjecna ocjena je:" << obj._komentar.ProsjecnaOcjena();
        return COUT;
    }
    bool AddGost(Gost g) {
       
        for (int i = 0; i < _gosti.size(); i++)
        {
            if (g == _gosti[i]) { return false; }
        }
        _gosti.push_back(g);
    }
    void SetKomentar(Komentar k) {
        _komentar = k;
    }
};


void main() {


    Datum
        datum19062022(19, 6, 2022),
        datum20062022(20, 6, 2022),
        datum30062022(30, 6, 2022),
        datum05072022(5, 7, 2022);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    try {
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }
    cout << kolekcija1 << crt;


    Kolekcija<int, int> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
    cout << kolekcija2 << crt;

    Gost denis("Denis Music", "denis@fit.ba", "BH235-532");
    Gost jasmin("Jasmin Azemovic", "jasmin@fit.ba", "B123321");
    Gost adel("Adel Handzic", "adel@edu.fit.ba", "B1252 521");
    Gost gostPasosNotValid("Ime Prezime", "korisnik@klix.ba", "H4521");

    Rezervacija rezervacija(datum19062022, datum20062022, denis);
    if (rezervacija.AddGost(jasmin))
        cout << "Gost uspjesno dodan!";

    Komentar komentarRezervacija("Nismo pretjerano zadovoljni uslugom, a ni lokacijom.");
    komentarRezervacija.AddOcjenuKriterija(CISTOCA, 7);
    komentarRezervacija.AddOcjenuKriterija(USLUGA, 4);
    komentarRezervacija.AddOcjenuKriterija(LOKACIJA, 3);
    komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);

    try {
        komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);//kriterij udobnosti je vec ocijenjen!
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }


   
    rezervacija.SetKomentar(komentarRezervacija);

    
    cout << rezervacija << endl;


    Rezervacija rezervacijaSaAdelom = rezervacija;
    if (rezervacijaSaAdelom.AddGost(adel))
        cout << "Gost uspjesno dodan!";
    if (!rezervacijaSaAdelom.AddGost(denis))
        cout << "Gost je vec dodan na rezervaciju!";

    cout << rezervacijaSaAdelom << endl;


    cin.get();
    system("pause>0");
}