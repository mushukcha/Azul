#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <time.h>

using namespace std;
const char szinek[] = "ABCDEX";

//3.feladat
struct Game {
    char** korongok;
    int korongok_szama;
    int* kozos_csempek = new int[6];
    int* zsak = new int[5];
    int* eldobott_csempek = new int[5];
};

//7. feladat
struct Player {
    char tabla[5][5];
    int pontszam = 0;
    char** mintasor;
    char* padlovonal;
    int sorszam;
};

char Huzas(Game*);
char** Korongok(int);
void CsempeRajzol(Game);
void Fordulo(Game*);
void PlayerRajzol(Player*);
void Kivalaszt(Player*, Game*);
void PadlovonalhozAdas(Player*, Game*, char, int);
bool KorongEllenorzes(Game*, int);
bool MintasorEllenorzes(Player*, int, char);
int CsempeIndex(char);
int CsempeDb(char*, char);
void Csempezes(Player*, Game*);
bool OszlopEllenorzes(Player*, int, char);
bool OsszesKorongUres(Game*);
int PontSzamolas(Player*, int, int);
void BonuszPontok(Player*[], int);
int TeliOszlop(Player*);
int TeliSorok(Player*);
bool OsszesKorongUres(Game*);
bool OszlopEllenorzes(Player*, int, char);
int TeliSorok(Player*);
int TeliOszlop(Player*);
void BonuszPontok(Player* [], int);

//1. es 5.feladat
char Huzas(Game *jatek) {
    int szum = 0; //hany darab csempe van osszesen

    for (int i = 0; i < 5; i++) {
        szum += jatek->zsak[i];
    }

    if (szum == 0) { // ha elfogyna a zsakbol az osszes csempe, attolti az eldobottakat a zsakba es lenullazza az eldobottakat
        for (int i = 0; i < 5; i++)
        {
            jatek->zsak[i] = jatek->eldobott_csempek[i];
            jatek->eldobott_csempek[i] = 0;
        }
    }

    int random = 1 + rand() % szum;

    //megnezi, hogy ha sorba raknank a csempeket akkor milyen lenne a kisorsolt
    int akt = 0;
    char sorsolt = 'A';
    for (int i = 0; i < 5; i++) {
        akt += jatek->zsak[i];
        if (akt >= random && jatek->zsak[i] > 0) {
            sorsolt = szinek[i];
            jatek->zsak[i]--;
            break;
        }
    }
    return sorsolt;
}

//2. feladat
char** Korongok(int jatekosszam) {
    char** korongok = new char*[jatekosszam*2 + 1];
    for (int i = 0; i < jatekosszam * 2 + 1; i++)
    {
        korongok[i] = new char[4];
    }
    return korongok;
}

//4.feladat
void CsempeRajzol(Game jatek) {
    for (int i = 0; i < jatek.korongok_szama; i++)
    {
        cout << i + 1 << ". ";
        if (jatek.korongok[i] != nullptr) {
            for (int j = 0; j < 4; j++)
            {
                cout << jatek.korongok[i][j];
            }
        }
        cout << "\n";
    }

    cout << "0. ";

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < jatek.kozos_csempek[i]; j++)
        {
            cout << szinek[i];
        }
    }

    cout << "\n";
}

//6. feladat
void Fordulo(Game *jatek) {
    for (int i = 0; i < jatek->korongok_szama; i++)
    {
        jatek->korongok[i] = new char[4];
        for (int j = 0; j < 4; j++)
        {
            jatek->korongok[i][j] = Huzas(jatek);
        }
    }
    jatek->kozos_csempek = new int[6]{0, 0, 0, 0, 0, 1};//kezdo jatekos csempe
}

//7. feladat
Player* Jatekosok(int n) {
    Player* jatekosok = new Player[n];
    for (int i = 0; i < n; i++)
    {
        jatekosok[i].sorszam = i + 1;
        jatekosok[i].mintasor = new char*[5];
        jatekosok[i].padlovonal = new char[7]{'_', '_', '_', '_', '_', '_', '_'};
        for (int j = 0; j < 5; j++)
        {
            jatekosok[i].mintasor[j] = new char[j];
            for (int k = 0; k < 5; k++)
            {
                jatekosok[i].tabla[j][k] = '.';
                if (k <= j) {
                    jatekosok[i].mintasor[j][k] = '-';
                }
            }
        }
    }

    return jatekosok;
}

//8.feladat
void PlayerRajzol(Player* jatekos) {
    cout << "1 2 3 4 5   Pontok: " << jatekos->pontszam << "\n";
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << jatekos->tabla[i][j] << " ";
        }
        cout << " " << i + 1 << " ";
        for (int j = 0; j <= i; j++)
        {
            cout << jatekos->mintasor[i][j];
        }
        
        cout << "\n";
    }

    for (int i = 0; i < 7; i++)
    {
        cout << " " << jatekos->padlovonal[i] << " ";
    }
    cout << "\n-1 -1 -2 -2 -2 -3 -3\n";
}

void Cin() { //cin bufferenek tolese
    cin.clear();
    cin.ignore(10000, '\n');
}

//9. feladat
void Kivalaszt(Player* jatekos, Game* jatek) {
    int korong_sorszam, mintasor_sorszam;
    char csempe;
    bool jo_korong, jo_csempe, jo_mintasor;
    do
    {
        cout << "Kerem a korong sorszamat: ";
        cin >> korong_sorszam;
        if (!(jo_korong = cin.good())) {
            Cin();
        }
    } while (korong_sorszam < 0 || korong_sorszam > jatek->korongok_szama || !jo_korong || KorongEllenorzes(jatek, korong_sorszam));

    do
    {
        cout << "Kerem a csempet: ";
        cin >> csempe;
        Cin();

        //van e a kivalasztott korongon ilyen csempe
        if (korong_sorszam > 0) jo_csempe = CsempeDb(jatek->korongok[korong_sorszam - 1], csempe) > 0; 
        else jo_csempe = jatek->kozos_csempek[CsempeIndex(csempe)] > 0;
    } while (!jo_csempe || cin.fail());

    do
    {
        cout << "Kerem a mintasor sorszamat: ";
        cin >> mintasor_sorszam;
        if (!(jo_mintasor = cin.good())) {
            Cin();
        }
    } while (mintasor_sorszam < 0 || mintasor_sorszam > 5 || !jo_mintasor || (mintasor_sorszam > 0 && !MintasorEllenorzes(jatekos, mintasor_sorszam, csempe)) );

    int csempe_db = 0; //hany darab van a kivalasztott csempebol a kivalasztott korongon

    if (korong_sorszam == 0) { //kozos csempekbol valaszt
        csempe_db = jatek->kozos_csempek[CsempeIndex(csempe)];
        jatek->kozos_csempek[CsempeIndex(csempe)] = 0;
        if (jatek->kozos_csempek[5] == 1) { //ha meg senki nem vett el a kozos csempekbol akkor most a jatekos megkapja
            PadlovonalhozAdas(jatekos, jatek, 'X', 1);
            jatek->kozos_csempek[5] = 0;
        }
    } else { //korongrol valaszt
        //a nem kivalasztott csemepek hozzaadasa a kozos csempekhez
        csempe_db = CsempeDb(jatek->korongok[korong_sorszam - 1], csempe);
        for (int i = 0; i < 4; i++)
        {
            if (jatek->korongok[korong_sorszam - 1][i] != csempe) jatek->kozos_csempek[CsempeIndex(jatek->korongok[korong_sorszam - 1][i])]++;
        }
        delete jatek->korongok[korong_sorszam - 1];
        jatek->korongok[korong_sorszam - 1] = nullptr;
    }

    if (mintasor_sorszam == 0) {//a padlovolanra rakja
        PadlovonalhozAdas(jatekos, jatek, csempe, csempe_db);
    } else { //mintasorra rakja
        int lerakott = 0; //hany csempe lett lerakva a mintasorra
        for (int i = 0; i < mintasor_sorszam; i++)
        {
            if (csempe_db > lerakott && jatekos->mintasor[mintasor_sorszam - 1][i] != csempe) {
                jatekos->mintasor[mintasor_sorszam - 1][i] = csempe;
                lerakott++;
            }
        }
        if (csempe_db - lerakott > 0) {//ami nem fert a mintasorra az a padlovonalra kerul
            PadlovonalhozAdas(jatekos, jatek, csempe, csempe_db - lerakott);
        }
    }
}

void PadlovonalhozAdas(Player* jatekos, Game* jatek, char csempe, int db) {
    int ures = 0; //ures helyek a padlovonalon
    int lerakott = 0;
    for (int i = 0; i < 7; i++)
    {
        if (db > lerakott && jatekos->padlovonal[i] == '_') {
            ures++;
            lerakott++;
            jatekos->padlovonal[i] = csempe;
        }
    }
    if (db > ures) { //eldobja azokat amik nem fertek ra a padlovonalra
        jatek->eldobott_csempek[CsempeIndex(csempe)] += db - ures;
    }
}

bool KorongEllenorzes(Game* jatek, int korong_sorszam) {
    if (korong_sorszam > 0 && jatek->korongok[korong_sorszam - 1] != nullptr) { //ha nem a kozos csempekbol valaszt
        return false;
    }
    if (korong_sorszam == 0) {//ha a kozos csempeket valasztja, akkor van e ott elveheto csempe
        int szum = 0;
        for (int i = 0; i < 5; i++)
        {
            szum += jatek->kozos_csempek[i];
        }
        return szum == 0;
    }
    return true;
}

bool MintasorEllenorzes(Player* jatekos, int mintasor_sorszam, char csempe) {
    char megkezdett = NULL; //milyen csempe van megkezdve a mintasoron
    int megkezdett_darab = 0; //hany darab ilyen csempe van
    for (int i = 0; i < mintasor_sorszam; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (szinek[j] == jatekos->mintasor[mintasor_sorszam-1][i]) {
                megkezdett = szinek[j];
                megkezdett_darab++;
            }
        }
    }

    for (int i = 0; i < 5; i++)
    {
        //van mar ilyen csempe ebben a sorban
        if (jatekos->tabla[mintasor_sorszam - 1][i] == csempe) return false;
    }

    if ((megkezdett == NULL || megkezdett == csempe) && megkezdett_darab < mintasor_sorszam) return true; //ha ilyen vagy semmilyen van megkezdve es lehet meg pakolni ra
    return false;
}

int CsempeIndex(char csempe) { //a csempe karakterenek indexe a szinek kozul
    int index = NULL;
    for (int i = 0; i < 5; i++)
    {
        if (szinek[i] == csempe) index = i;
    }
    return index;
}

int CsempeDb(char* csempek, char csempe) {//hany darab adott csempe talalhato egy sorba
    int darab = 0;
    for (int i = 0; i < 7; i++)
    {
        if (csempek[i] == csempe) darab++;
    }
    return darab;
}

//10. feladat
void Csempezes(Player* jatekos, Game* jatek) {
    int oszlop_sorszam;
    bool jo_oszlop, van_jo_oszlop;
    for (int i = 0; i < 5; i++)
    {
        van_jo_oszlop = false;
        if (jatekos->mintasor[i][0] != '-' && CsempeDb(jatekos->mintasor[i], jatekos->mintasor[i][0]) == i + 1) { //a mintasor tele van

            for (int j = 0; j < 5; j++) //megnezi van e olyan oszlop amibe le lehet rakni
            {
                if (OszlopEllenorzes(jatekos, j, jatekos->mintasor[i][0]))
                    van_jo_oszlop = true;
            }
            if (van_jo_oszlop) { //ha letezik jo oszlop a jatekos valaszthat
                do
                {
                    cout << "(" << i + 1<< ".sor) Kerem az oszlop sorszamat: ";
                    cin >> oszlop_sorszam;
                    if (!(jo_oszlop = cin.good())) {
                        Cin();
                    }
                } while (oszlop_sorszam < 1 || oszlop_sorszam > 5 || !jo_oszlop || jatekos->tabla[i][oszlop_sorszam - 1] != '.' || !OszlopEllenorzes(jatekos, oszlop_sorszam-1, jatekos->mintasor[i][0]));

                jatekos->tabla[i][oszlop_sorszam - 1] = jatekos->mintasor[i][0];
                if(i - 1 > 0) jatek->eldobott_csempek[CsempeIndex(jatekos->mintasor[i][0])] += i - 1; //a maradek csempek eldobasra kerulnek
                jatekos->pontszam += PontSzamolas(jatekos, i, oszlop_sorszam - 1);
                if (jatekos->pontszam < 0) jatekos->pontszam = 0;
            } else {
                PadlovonalhozAdas(jatekos, jatek, jatekos->mintasor[i][0], i);
            }

            jatekos->mintasor[i] = new char[i];
            for (int j = 0; j <= i; j++)
            {
                jatekos->mintasor[i][j] = '-';
            }

        }

        cout << '\n';
        PlayerRajzol(jatekos);
    }

    //buntetopontok levonasas
    for (int i = 0; i < 7; i++)
    {
        if (jatekos->padlovonal[i] != '_') {
            if (i < 2) jatekos->pontszam --;
            if (i >= 2 && i < 5) jatekos->pontszam -= 2;
            if (i >= 5) jatekos->pontszam -= 3;
        }
    }
    if (jatekos->pontszam < 0) jatekos->pontszam = 0;
    delete jatekos->padlovonal;
    jatekos->padlovonal = new char[7]{ '_', '_', '_', '_', '_', '_', '_' };
}

int PontSzamolas(Player* jatekos, int sor, int oszlop) {
    int sor_pontok = 0, oszlop_pontok = 0;
    bool osszefuggo_sor = true, osszefuggo_oszlop = true;
    for (int i = 0; i < 5; i++)
    {
        //elindul egy soron es amig nem talal ures helyet addig osszeadja apontokat
        if (jatekos->tabla[sor][i] != '.' && osszefuggo_sor) {
            sor_pontok++;
        }
        else{
            //ha talal ureset es az az eppen lerakott csempe elott van akkor az nem tartozik a kialakult vonalba mivel az megszakad
            if (i < oszlop)
                sor_pontok = 0;
            else //ha a lerakott csempe utan van akkor nem szamolja tovabb a pontokat
                osszefuggo_sor = false;
        }
        //ugyan ilyen ellenorzes csak oszlopokra
        if (jatekos->tabla[i][oszlop] != '.' && osszefuggo_oszlop) {
            oszlop_pontok++;
        }
        else {
            if (i < sor)
                oszlop_pontok = 0;
            else
                osszefuggo_oszlop = false;
        }
    }
    if (sor_pontok == 1 && oszlop_pontok == 1) return 1; //nem erintkezik mas csempevel
    if (sor_pontok == 1) sor_pontok = 0; //nincs vizszintes sor csak onmaga
    if (oszlop_pontok == 1) oszlop_pontok = 0; //nincs fuggoleges sor csak onmaga
    return sor_pontok + oszlop_pontok;
}

bool OszlopEllenorzes(Player* jatekos, int oszlop_sorszam, char csempe) { //van e mar az oszlopban ilyen csempe
    for (int i = 0; i < 5; i++)
    {
        if (jatekos->tabla[i][oszlop_sorszam] == csempe)return false;
    }
    return true;
}

bool OsszesKorongUres(Game* jatek) { //minden korongrol elfogyott az elveheto csempe
    for (size_t i = 0; i < jatek->korongok_szama; i++)
    {
        if (jatek->korongok[i] != nullptr) return false;
    }
    for (int i = 0; i < 5; i++)
    {
        if (jatek->kozos_csempek[i] > 0) return false;
    }

    return true;
}
//11. feladat
int TeliSorok(Player* jatekos) {
    int tele_sorok = 0;
    bool telesor;
    for (int i = 0; i < 5; i++)
    {
        telesor = true;
        for (int j = 0; j < 5; j++)
        {
            if (jatekos->tabla[i][j] == '.') telesor = false;
        }
        if (telesor)tele_sorok++;
    }

    return tele_sorok;
}


int TeliOszlop(Player* jatekos) {
    int tele_oszlopok = 0;
    bool teleoszlop;
    for (int i = 0; i < 5; i++)
    {
        teleoszlop = true;
        for (int j = 0; j < 5; j++)
        {
            if (jatekos->tabla[j][i] == '.') teleoszlop = false;
        }
        if (teleoszlop)tele_oszlopok++;
    }

    return tele_oszlopok;
}
//12. feladat
void BonuszPontok(Player* jatekosok[], int jatekosok_szama) {
    int lerakott_szinek[5] = { 0, 0, 0, 0, 0 };
    for (int i = 0; i < jatekosok_szama; i++)
    {
        jatekosok[i]->pontszam += 2 * TeliSorok(jatekosok[i]);
        jatekosok[i]->pontszam += 7 * TeliOszlop(jatekosok[i]);
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                if (CsempeIndex(jatekosok[i]->tabla[j][k]) != NULL) {
                    lerakott_szinek[CsempeIndex(jatekosok[i]->tabla[j][k])]++;
                }
            }
        }
        for (int j = 0; j < 5; j++)
        {
            if (lerakott_szinek[j] == 5) {
                jatekosok[i]->pontszam += 10;
            }
        }
    }
}

int main()
{
    srand(time(0));

    int jatekosok_szama;
    do
    {
        cout << "Kerem a jatekosok szamat(2-4): ";
        cin >> jatekosok_szama;
        Cin();
    } while (jatekosok_szama < 2 || jatekosok_szama > 4 || cin.fail());

    Game* jatek = new Game();
    jatek->korongok_szama = 2 * jatekosok_szama + 1;
    jatek->korongok = Korongok(jatekosok_szama);
    jatek->zsak = new int[5]{ 20,20,20,20,20 };
    jatek->eldobott_csempek = new int[5]{ 0, 0, 0, 0, 0 };
    Player* jatekosok = Jatekosok(jatekosok_szama);
    int akt_jatekos = 0;

    bool jatek_vege = false;
    while (!jatek_vege)
    {
        Fordulo(jatek);
        while (!OsszesKorongUres(jatek))
        {
            cout << "\n------- " << akt_jatekos + 1 << ". jatekos -------\n\n";
            CsempeRajzol(*jatek);
            PlayerRajzol(&jatekosok[akt_jatekos]);
            Kivalaszt(&jatekosok[akt_jatekos], jatek);
            cout << '\n';
            PlayerRajzol(&jatekosok[akt_jatekos]);

            if (akt_jatekos == jatekosok_szama - 1)
                akt_jatekos = 0;
            else
                akt_jatekos++;
        }
        for (int i = 0; i < jatekosok_szama; i++)
        {
            cout << "\n------- " << i + 1 << ". jatekos -------\n\n";
            PlayerRajzol(&jatekosok[i]);
            Csempezes(&jatekosok[i], jatek);
            if (TeliSorok(&jatekosok[i]) > 0) jatek_vege = true; //ha valakinek van egy teli sora veget er a jatek
        }
    }

    BonuszPontok(&jatekosok, jatekosok_szama);

    int* helyzes = new int[jatekosok_szama];
    int *index = new int[jatekosok_szama];

    for (int i = 0; i < jatekosok_szama; i++)
    {
        helyzes[i] = jatekosok[i].pontszam;
        index[i] = i + 1;
    }
    int csere;
    for (int i = 0; i < jatekosok_szama; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (helyzes[j] < helyzes[i] || (helyzes[j] == helyzes[i] && TeliSorok(&jatekosok[index[j]]) < TeliSorok(&jatekosok[index[i]]))) {
                csere = helyzes[j];
                helyzes[j] = helyzes[i];
                helyzes[i] = csere;

                csere = index[j];
                index[j] = index[i];
                index[i] = csere;
            }
        }
    }
    cout << "\nA jatek veget ert. Eredmenyek:\n";
    for (int i = 0; i < jatekosok_szama; i++)
    {
        if (helyzes[i] == helyzes[0]) cout << "1";
        else cout << i + 1;
        cout << ". helyezett: " << index[i] << ". jatekos -> " << helyzes[i] << " pont\n";
    }

    //memoria felszabaditasa
    for (int i = jatekosok_szama-1; i >= 0; i--)
    {
        delete[] jatekosok[i].padlovonal;
        delete[] jatekosok[i].mintasor;
    }
    delete[] jatekosok;

    for (int i = 4; i >= 0; i--)
    {
        delete[] jatek->korongok[i];
    }
    delete jatek->korongok;
    delete jatek->zsak;
    delete jatek->kozos_csempek;
    delete jatek;

    cin.get(); //a vegen var egy enter lenyomasara mielott kilep, hogy latni lehesssen az eredmenyeket
}