// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include "Node.h"
// #include <bits/stdc++.h>
using namespace std;
struct sentences{
    string sent ;
    int loc[4] ;
};
class SearchEngine {
private:
    // You can add attributes/helper functions here
    // vector<pair<string,int[4]> >sentences ;
    vector<sentences> text ;
    long long unsigned int no_of_sentences ;
    long long unsigned int text_size ;

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* search(string pattern, int& n_matches);

    /* -----------------------------------------*/
};