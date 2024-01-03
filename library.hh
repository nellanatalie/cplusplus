/*
 * Luokka nimeltä kirjasto. Kirjasto-olio määritellään kirjaston nimen avulla.
 * Jokaisella kirjasto-oliolla on collection, jossa kirjastosta löytyvien
 * kirjojen tiedot.
 * Kirjastoon voidaan lisätä kirja, sekä
 * kirjastosta voidaan hakea kirjaston kokoelmat ja
 * kirjaston kirjojen kirjailijat.
 * Jokainen kirjastosta löytyvä kirja on määritelty Book-olioina,
 * josta voidaan hakea kirjan kirjailija
 * kirjan nimi sekä kirjan varausjono.
 */


#ifndef LIBRARY_HH
#define LIBRARY_HH

#include <string>
#include <vector>
#include <map>

using namespace std;

struct Book;

using Collection = map<string, vector<Book>>;

struct Book
{
    string author;
    string title;
    int reservations;
};

class Library{
public:

    Library(string library_name);

    map<string, vector<Book>> get_collection();

    vector<string> get_authors();

    void add_new_book(const string &author,
                      const string &title,
                      int reservations);

private:

    string library_name_ = "";

    Collection collection_;

    void add_author(string const &author);

    bool does_book_exist(const string author,
                         const string title);


};

#endif // LIBRARY_HH

