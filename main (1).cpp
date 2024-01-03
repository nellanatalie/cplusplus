/*
* Ohjelmointi 2 , Kirjasto- projekti
* 13.3.2023
*
*
* Kuvaus:
 *      Ohjelmassa luetaan tietokantaan kirjastojen tietoja sisältävän
 *  tiedoston tiedot. Ohjelma toteuttaa kirjastoihin liittyvän hakualgoritmin.
 *      Ohjelma kysyy ensin kirjastojen kokoelmiin liittyviä tietoja sisältävää
 *  tiedostoa, jonka sisältö tarkistetaan. Hyväksytyn tiedoston tiedot
 *  tallenetaan sopivaan tietorakenteeseen,
 *  sitten ohjelma antaa käyttäjälle mahdollisuuden
 *  tehdä hakuja kyseiseen tietokantaan ja tulostaa käyttäjän pyytämät
 *  tiedot.
 *      Joka kierroksella käyttäjältä kysytään komentoa. Ohjelma tulostaa,
 *  komennon mukaisesti halutut tiedot kun syöte on: libraries,
 *  material <kirjasto>, books <kirjasto> <tekijä>, loanable tai
 *  reservable <tekijä> <kirjan_nimi>.
 *      Ohjelma tarkistaa, oliko käyttäjän antamat komennot hyväksyttäviä.
 *  Ohjelma päättyy, kun käyttäjä syöttää lopetuskomennon (sanan "quit").
 *

Ohjelman kirjoittajat:
 *
 * Nimi: Nella Forstedt
 * Opiskelijanumero: 150309677
 * Käyttäjätunnus: xknefo
 * E-Mail: nella.forstedt@tuni.fi
 *
 * Nimi: Siri Huhtala
 * Opiskelijanumero: 50031281
 * Käyttäjätunnus: fcsihu
 * E-Mail: siri.huhtala@tuni.fi
 */


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "library.hh"

using namespace std;

// Tietorakenne, jossa string kirjaston nimi ja
// kirjaston nimeen liittyvä Library-olio.
using Database = map<string, Library>;


/*
 * Split jakaa annetun stringin annetulla välimerkillä sana-vektoriksi.
 * Parametreinä jaettava string s, välimerkki delimiter
 * sekä totuusarvo ignore-empty.
 * Ignore-empty on true, sillä syötteen pituus tulee tarkastaa.
 * Palautetaan annetun stringin sanoista tehty vektori result.
*/
vector<string> split(const string& s,
                     const char delimiter,
                     bool ignore_empty = true){

    vector<string> result;
    string line = s;

    while(line.find(delimiter) != string::npos)
    {
        string new_part = line.substr(0, line.find(delimiter));
        line = line.substr(line.find(delimiter)+1, line.size());

        if(not (ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);
        }
    }

    if(not (ignore_empty and line.empty()))
    {
        result.push_back(line);
    }

    return result;
}


/*
 *      Is_row_acceptable tarkistaa, että tiedoston rivissä on aina
 *  tarvittavat 4 tietoa, sekä, että rivin sanat eivät ole
 *  tyhjiä tai välilyöntejä.
 *      Funktion parametrinä elements vektori, jossa rivin sanat vektorissa.
 *      Palauttaa false, jos tiedoston riviä ei hyväksytä,
 *  joka johtaa koko ohjelman pysähtymiseen.
*/
bool is_row_acceptable(vector<string> elements){

    for(int word = 0 ; word < int(elements.size()) ; ++word){

        if (elements.at(word).at(0) == ' '){
            return false;
        }
    }

    //rivissä tulee olla 4 eri tietoa
    if(elements.size() != 4){
        return false;
    }
    return true;
}


/*
 *      Tarkistetaan ohjelmaan syötetyn tiedoston oikeellisuus.
 *  Tiedoston tiedot tallennetaan tietokantaan rivi kerrallaan.
 *      Parametreinä tietokanta database sekä tiedostonnimi filename.
 *      Palautetaan false, jos tiedosto ei ole oikeanlainen.
 *  Palautetaan true, jos tiedosto hyväksytään ja tiedot tallennetaan.
*/
bool read_inputfile(Database &database,
                    const string filename){

    // Luetaan annettu tiedosto inputfile-alkioon
    ifstream inputfile(filename);

    // tarkistetaan, löydetäänkö tiedostoa
    if(not inputfile){
       cout << "Error: input file cannot be opened" << endl;
       return false;
    }

    string row = "";

    // Luetaan tiedoston tiedot
    // ja tallennetaan ne rivi kerrallaan tietorakenteeseen
    while ( getline(inputfile, row) ) {

        // Erotellaan rivin tiedot split-funktiolla
        vector<string> elements = split(row,';');

        // Tarkistetaan rivin oikeellisuus
        if (not is_row_acceptable(elements)){
            cout << "Error: empty field" << endl;
            return false;
        }

        // Eritellään rivin osaset
        string library_name = elements.at(0);
        string author = elements.at(1);
        string title = elements.at(2);

        int reservations = 0;
        string zero_text = "on-the-shelf";

        // Jos varaustieto ei ole zero_text, muutetaan string intiksi
        if((elements.at(3)) != zero_text){
            reservations = stoi(elements.at(3));
        }

        // Jos kirjastoa ei ole vielä luotu tietokantaan,
        // alustetaan uusi kirjasto library_name avulla,
        // ja lisätään uusi kirjasto tietokantaan.
        if(database.find(library_name) == database.end()){

            database.insert({library_name, Library(library_name)});
        }

        // Lisätään kirja oikeaan kirjastoon
        (database.at(library_name)).add_new_book(author,title,reservations);
    }

    inputfile.close();

    return true;
}


/*
 *      Tulostetaan halutun kirjailijan kaikkien kirjojen nimet
 *  varaustietoineen.
 *      Parametreinä tietokanta database,
 *  kirjailija author ja kirjaston nimi name
*/
void print_authors_books(Database database,
                         const string author,
                         const string name ){

    // Käydään läpi kaikki kirjastot
    for(Database::iterator iter = database.begin();
        iter != database.end();iter++){

        // Etsitään haluttu kirjasto, jos löytyy, jatketaan
        if(iter->first == name){
            map<string, vector<Book>> collection =
                    iter->second.get_collection();

            // Käydään läpi halutun kirjaston kirjojen kirjailijat
            for(Collection::iterator iterat = collection.begin();
                iterat != collection.end();iterat++){

                // Etsitään haluttu kirjailija, jos löytyy, jatketaan
                if(iterat->first == author){

                    // Käydään läpi halutun kirjailijan kirjat
                    for(int book = 0 ; book < int((iterat->second).size())
                        ; ++book){

                        // Tulostetaan halutun kirjailijan
                        // kirjojen nimet varaustietoineen.
                        if((iterat->second).at(book).reservations == 0){
                            cout <<  (iterat->second).at(book).title <<
                                     " --- on the shelf" << endl;
                        }

                        else {
                            cout << (iterat->second).at(book).title
                            << " --- " << ((iterat->second)
                                           .at(book)).reservations
                            << " reservations" << endl;
                        }
                    }
                }
            }
        }
    }
}


/*
 *      Tulostetaan kaikki tietorakenteessa olevien kirjastojen nimet
 *      Parametreinä tietokanta database
*/
void print_libraries(Database const &database){

    // Käydään läpi kaikki tietorakenteen kirjastot
    for(Database::const_iterator iter = database.begin();
        iter != database.end();iter++){

        cout << iter->first << endl;
    }
}


/*
 *      Tulostetaan aakkosjärjestyksessä halutun kirjaston kirjoista
 *  kirjailijan ja kirjan nimen,
 *  kun annetaan parametreinä tietorakenne database ja kirjaston nimi.
*/
void print_books(Database database,
                 string library_name){

    // Käydään läpi kaikki tietorakenteen kirjastot
    for(Database::iterator iter = database.begin();
        iter != database.end();iter++){

        // Etsitään haluttu kirjasto
        if(iter->first == library_name){
            map<string, vector<Book>> collection =
                    iter->second.get_collection();

            // Käydään läpi kaikki halutun kirjaston kirjailijat
            for(Collection::iterator iterat = collection.begin();
                iterat != collection.end();iterat++){

                // Käydään läpi kaikki halutun kirjailijan kirjat
                for(int book = 0 ; book < int((iterat->second).size())
                    ; ++book){

                    cout << iterat->first << ": "
                         << (iterat->second).at(book).title << endl;
                }
            }
        }
    }
}


/*
 *      Etsii ja tallentaa reservation_and_library-mappiin
 *  halutun kirjan sijainnin(kirjasto) ja kirjan varaustiedon.
 *      Parametreinä tietorakenne database sekä halutun kirjan
 *  kirjailija ja kirjan nimi.
 *      Palautetaan map, jossa varaustieto sekä kirjaston nimi
*/
map<int, vector<string>> find_reservation(Database database,
                                          string author,
                                          string title){

    map<int, vector<string>> reservation_and_library = {};

    // Käydään tietorakenteen kaikki kirjastot läpi
    for(Database::iterator iter = database.begin();
        iter != database.end();iter++){

         map<string, vector<Book>> collection =
                 iter->second.get_collection();

            // käydään kirjaston kaikkien kirjojen kirjailijat läpi
            for(Collection::iterator iterat = collection.begin();
                iterat != collection.end();iterat++){

                // etsitään haluttu kirjailija
                if(iterat->first == author){

                    //käydään halutun kirjailijan kaikki kirjat läpi
                    for(int book = 0 ; book < int((iterat->second).size())
                        ; ++book){

                        Book &book1 = ((iterat->second).at(book));

                        // etsitään haluttu kirja
                        if((iterat->second).at(book).title == title){

                            // Jos kirjoilla on sama varausmäärä eri
                            // kirjastoissa, tallennetaan molemmat kirjastot
                            // samalla varausnumerolla mappiin
                            if(reservation_and_library.find(book1.reservations)
                                    != reservation_and_library.end()){

                                // Lisätään uusi kirjasto mappiin
                                // samalla varausnumerolla
                               (reservation_and_library.at(book1.reservations))
                                        .push_back(iter->first);
                            }
                            else{
                                // Jos varausnumeroa ei löydy mapista
                                // lisätään uusi pari mappiin
                                reservation_and_library
                                        .insert({book1.reservations,
                                                 {iter->first}});
                            }
                        }
                    }
                }
            }
    }
    return reservation_and_library;
}

/*
 *      Etsitään kaikkien tietokannassa olevien kirjastojen kokoelmissa
 *  olevat kirjat ja niiden kirjailijat. Luodaan kirjojen
 *  nimistä ja kirjailijoista omat vektorit.
 *      Parametreinä tietorakenne database
 *      Palautetaan vector<vector<string>>, jossa omat vektorit
 *  kirjailijoille ja kirjoille
*/
vector<vector<string>> find_authors_and_titles(Database database){

    vector<string> authors = {};
    vector<string> titles = {};

    // Käydään läpi kaikki tietorakenteessa olevat kirjastot
    for(Database::iterator iter = database.begin();
        iter != database.end();iter++){

        map<string, vector<Book>> collection = iter->second.get_collection();

           // käydään kirjaston kaikkien kirjojen kirjailijat läpi
           for(Collection::iterator iterat = collection.begin();
               iterat != collection.end();iterat++){

               // Jos kirjailijaa ei löydy kirjailijoiden vektorista, lisätään
               if(find(authors.begin(), authors.end(), iterat->first)
                       == authors.end()){
                   authors.push_back(iterat->first);
               }

               // käydään kaikki kirjat läpi
               for(int book = 0 ; book < int((iterat->second).size()) ;
                   ++book){

                   // Jos kirjaa ei löydy kirjojen vektorista, lisätään
                   if(find(titles.begin(), titles.end(),
                           iterat->second.at(book).title) == titles.end()){

                       titles.push_back(iterat->second.at(book).title);
                    }
               }
           }
    }
    return {authors, titles};
}


/*
 *      Tulostetaan lyhin varausjono ja kyseessä oleva kirjasto.
 *  Jos sama varausmäärä useassa kirjastossa, tulostetaan
 *  saman varausmäärän omaavat kirjastot.
 *      Parametreinä tietorakenne database,
 *  etsityn kirjan kirjailija ja kirjan nimi
*/
void print_reservation(Database database,
                       string author,
                       string title){

    // Vektori, jossa varaustieto kirjastoa kohti
    map<int, vector<string>> reservation_and_library =
            find_reservation(database, author , title);

    vector<vector<string>> authors_and_titles =
            find_authors_and_titles(database);

    // Vektoreissa kaikki tietorakenteessa olevat kirjailijat ja kirjojen nimet
    vector<string> authors = authors_and_titles.at(0);

    vector<string> titles = authors_and_titles.at(1);

    string zero_text = "on the shelf";

    //jos annettua kirjailijaa tai kirjaa ei löydy
    // kirjailijoiden ja kirjojen listasta.
    if(find(authors.begin(), authors.end() , author) == authors.end() or
            find(titles.begin(), titles.end(), title) == titles.end()){

        cout << "Book is not a library book" << endl;
    }

    // etsitään kaikki varaus&kirjasto- parit
    for(map<int, vector<string>>::iterator iter =
        reservation_and_library.begin();
        iter != reservation_and_library.end() ; ++iter){

        // Jos varausmäärä on liian iso (>100), ei kirjaa voi lainata
        if(iter->first >= 100){
            cout << "Book is not reservable from any library" << endl;
            break;
        }

        // jos varausmäärä on 0, tulostuu on the shelf
        if(iter->first == 0){
            cout << zero_text << endl;
        }

        else {
            cout << iter->first << " reservations" << endl;
        }

        // tulostetaan halutut kirjastot, joista etsitty kirja löytyy
        for(int j = 0 ; j < int(iter->second.size()) ; ++j){
            cout << "--- " << iter->second.at(j) << endl;
        }

        break;
    }
}


/*
 *      Tulostetaan kaikista kirjastoista kirjat, joiden varausmäärä on 0,
 *  eli ne jotka voi lainata.
 *      Parametreinä tietorakenne database
*/
void print_loanables(Database database){

    // Kerätään lainattavissa olevat kirjat mappiin
    map<string,vector<string>> loanables = {};

    //käydään kaikki tietorakenteen kirjastot läpi
    for(Database::iterator iter = database.begin();
        iter != database.end();iter++){

            map<string, vector<Book>> collection =
                    iter->second.get_collection();

            //käydään kirjaston kirjailijat läpi
            for(Collection::iterator iterat = collection.begin();
                iterat != collection.end();iterat++){

                //käydään kirjailijan kirjat läpi
                for(int i = 0 ; i < int((iterat->second).size()) ; ++i){

                    Book &book = (iterat->second).at(i);

                    // jos kirja lainattavissa, tallennetaan kirjan
                    // tiedot mappiin
                    if(book.reservations == 0){

                        // jos kirjailijaa ei vielä löydy, lisätään uusi
                        if(loanables.find(iterat->first) == loanables.end()){
                            loanables.insert({iterat->first , {book.title}});
                        }

                        else {
                        // jos kirjailija löytyy jo,
                        // lisätään kirjailijalle uusi kirja
                            if(find(loanables.at(iterat->first).begin(),
                                loanables.at(iterat->first).end(), book.title)
                                == (loanables.at(iterat->first).end())){

                                loanables.at(iterat->first)
                                        .push_back(book.title);
                            }
                        }
                    }
                }
            }
    }

    // Valmis map tulostetaan aakkosjärjestyksessä
    for(map<string,vector<string>>::iterator iterator =
        loanables.begin(); iterator
        != loanables.end();iterator++){

        sort(iterator->second.begin(), iterator->second.end());

        for(int j = 0; j < int(iterator->second.size()) ; ++j){

            cout << iterator->first << ": " << iterator->second.at(j) << endl;
        }
    }
}


/*
 *      Tarkistetaan onko annetun vektorin pituus annetun pituusehdon mukainen
 *      Parametreinä tarkistettava sana-vektori parts, pituusehto size
 *  sekä not_absolute totuusarvo.
 *      Palautetaan false, jos väärän kokoinen sekä virhetulostus. Muuten true.
*/
bool evaluate_input_size(vector<string> &parts,
                         unsigned long size,
                         bool not_absolute = false){

    if(not_absolute){
        if(parts.size() <size){
            cout << "Error: wrong number of parameters" << endl;
            return false;
        }
    }

    else {
        if(parts.size() != size){
            cout << "Error: wrong number of parameters" << endl;
            return false;
        }
    }
    return true;
}


/*
 *      Muodostetaan käyttäjän antamasta syötteestä
 *  oikean muotoinen käsittelyä varten. Jos syötettä on enemmän kuin 3 sanaa,
 *  viimeiset sanat liitetään yhteen ilman "" merkkejä.
 *      Parametreinä sana-vektori parts sekä palautettava kirjan nimi title.
 *      Palautetaan parametrinäkin oleva kirjan nimi.
*/
string put_title_together(vector<string> parts,
                          string title){

    if(parts.size() > 3){

        for(unsigned long word = 3 ; int(word < parts.size()) ; ++word){

            title += " " + parts.at(word);
        }
    }

    if(title.at(0) == '"'){
        title = title.substr(1, title.length()-2);
    }

    return title;

}


/*
 *      Tarkistetaan, löytyykö annettu kirjaston nimi tietorakenne databasesta.
 *      Parametreinä tietorakenne database sekä kirjaston nimi
 *      Palautetaan true, jos kirjasto löytyy. Muuten false ja virhetulostus.
*/
bool check_library_name(Database database,
                        string library_name){

    for(Database::iterator iter = database.begin();
        iter != database.end();iter++){

        if(iter->first == library_name){
            return true;
        }
    }

    cout << "Error: unknown library" << endl;

    return false;
}


/*
 *      Funktiossa toteutettu itse käyttöliittymä,
 *  jossa käyttäjältä kysytään syötettä. Syötteeksi hyväksytään
 *  libraries, loanable, material, books, reservable tai quit.
 *  Jokainen syöte vaatii myös peräänsä määrätyn määrän syötettä lisätietona.
 *  Jos syötettä ei tunnisteta, tulostuu virhetulostus
 *  ja kysytään uutta syötettä.
 *      Parametreinä tietorakenne database.
 *      Palautetaan false, jos annetaan komento quit.
 *  Tällöin koko ohjelman suoritus loppuu. Muuten true.
*/
bool user_interface(Database database)
{
    string line;

    cout << "lib> ";

    getline(std::cin, line);

    vector< string > parts = split(line, ' ', false);

    string command = parts.at(0);

    // Ohjelman suoritus lopetetaan
    if(command == "quit"){
        return false;
    }

    // Tulostuu kaikki tietorakenteesta löytyvät kirjastojen nimet
    else if(command == "libraries"){

        if(not evaluate_input_size(parts, 1)){
            return true;
        }

        print_libraries(database);

    }

    // Tulostuu kaikki lainattavissa olevat kirjat
    else if(command == "loanable"){

        if(not evaluate_input_size(parts, 1)){
            return true;
        }

        print_loanables(database);


    }

    // Tulostuu annetun kirjaston kaikki kirjat
    else if(command == "material"){

        if(not evaluate_input_size(parts, 2)){
            return true;
        }

        if(not check_library_name(database, parts.at(1))){
            return true;
        }

        print_books(database, parts.at(1));
    }

    // Tulostuu annetun kirjaston ja kirjailijan kirjat
    else if(command == "books"){

        if(not evaluate_input_size(parts, 3)){
            return true;
        }

        if(not check_library_name(database, parts.at(1))){
            return true;
        }

        string library_name = parts.at(1);

        string author = parts.at(2);

        vector<string> authors = (database.at(library_name)).get_authors();

        if(find(authors.begin(), authors.end(), author) == authors.end() ){
            cout << "Error: unknown author" << endl;
            return true;
        }

        print_authors_books(database, author, library_name);
    }

    // Tulostuu halutun kirjan lyhin varausjono ja kirjasto
    else if(command  == "reservable"){

        if(not evaluate_input_size(parts, 3, true)){
            return true;
        }

        string title = put_title_together(parts, parts.at(2));

        print_reservation(database, parts.at(1), title);
    }

    // Jos syötettä ei tunnisteta
    else{
        cout << "Error: unknown command" << endl;
    }

    return true;
}


int main()
{
    // Tietorakenne, johon kirjastojen nimet ja Library-alkiot tallentuu
    Database database;

    cout << "Input file: ";

    string inputfile_name = "";

    getline(cin, inputfile_name);

    if(not read_inputfile(database, inputfile_name)){
        return EXIT_FAILURE;
    }

    // Itse käyttöliittymä
    while(user_interface(database)){
        continue;
    }

    return EXIT_SUCCESS;
}
