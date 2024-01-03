
#include <iostream>
#include <algorithm>
#include "library.hh"

using namespace std;

// Alustetaan kirjasto-olio, jota voidaan hakea kirjaston nimellä.
Library::Library(string library_name):
    library_name_(library_name) , collection_({})
{
}

// Palautetaan kyseisen kirjaston kokoelmat.
map<string, vector<Book>> Library::get_collection()
{
    return collection_;
}


/*
 *      Tarkistetaan, löytyykö saman kirjailijan saman niminen kirja
 *  jo kirjaston kokoelmasta.
 *      Parametreinä etsittävän kirjan kirjailija sekä kirjan nimi.
 *  Jos kirja löytyy, palautetaan true.
*/
bool Library::does_book_exist(const string author,
                              const string title)
{
    //käydään läpi kaikki kirjailijat
    for(Collection::iterator iter = collection_.begin();
        iter != collection_.end() ; ++iter){

        // jos löydetään etsitty kirjailija, jatketaan
        if(iter->first == author){

            //käydädän läpi etsityn kirjailijan kaikki kirjat
            for(int i = 0 ; i < int(iter->second.size()) ; ++i){

                // jos löydetään etsitty kirja, palautetaan true
                if(iter->second.at(i).title == title){

                    return true;
                }
            }
        }
    }
    return false;
}


/*
 *      Luodaan vektori kaikista kirjaston kirjojen kirjailijoista.
 *      Palautetaan vektori kirjailijoista.
*/
vector<string> Library::get_authors()
{
    vector<string> authors = {};

    // Käydään läpi kaikki kirjaston kirjailijat
    for(Collection::iterator iter = collection_.begin();
        iter != collection_.end();iter++){

        authors.push_back(iter->first);
    }
    return authors;
}


/*
 *      Lisätään tietylle kirjailijalle uusi kirja, jos kirjaa ei vielä löydy.
 *  Jos lisättävää kirjailijaakaan ei löydy,
 *  lisätään kirjailija ja kirja kokoelmiin.
 *  Jos lisättävä kirja löytyy jo kokoelmista,
 *  päivitetään sen varausmäärä uusimman tiedon mukaan.
 *      Parametreinä lisättävä/etsittävä kirjailija,
 *  lisättävä kirja sekä kirjan varausmäärä.
*/
void Library::add_new_book(const string &author,
                           const string &title,
                           int reservations)
{

    // Jos kirjaa ei vielä löydy kokoelmista, luodaan ja lisätään kirja.
    // Add_author tarkistaa, löytyykö kirjailija jo kokoelmista.
    if(not does_book_exist(author,title)){

        Book book{author,title, reservations};

        add_author(author);

        collection_.at(author).push_back(book);
    }

    else{

        // Käydään kirjailijan kirjat läpi
        for(int k = 0; k < int(collection_.at(author).size()); ++k){

            // Jos saman niminen kirja löytyy,
            //päivitetään kirjalle uusi varaustieto.
            if(collection_.at(author).at(k).title == title){

                collection_.at(author).at(k).reservations = reservations;
            }
        }
    }
}


/*
 *      Voidaan lisätä kirjaston kokoelmaan uusi kirjailija.
 *  Ennen lisäystä tarkistetaan, löytyykö kyseinen kirjailija jo kokoelmasta.
 *  Jos löydetään uusi kirjailija, kokoelmaan lisätään kirjailija
 *  ja sen hyötykuormaksi tyhjä vektori.
 *      Parametreinä lisättävä kirjailija.
*/

void Library::add_author(const string &author)
{
    if(collection_.find(author) == collection_.end()){

        collection_.insert({author,{}});
    }
}

