// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

//Struct words will store word in id and frequency will store its frequency

struct occurence_list_node{
    occurence_list_node* next ;
    int book_code ;
    int page ;
    int paragraph ;
    int sentence_no ;

};

struct words{
    string id ;
    long long unsigned int frequency ;
    words* next ;
    occurence_list_node* head ;
};


class Dict {
private:
    // You can add attributes/helper functions here
    //hashtable is complete hashtabel
    //size if number of encountered unique words
    //capacity is the number of buckets in hashtable

public: 
    vector<words*>* hash_table ;
    // vector<words*>* hash_table ;
    
    // char seperators[15] ;
    bool seperators[256];
    // char seperators[] = { ' ' ,  '.'  ,   ','  ,   '-'    ,  ':'   ,   '!'  ,  '"'    ,  '\''  , '('   ,  ')'  ,   '?'      ,  '['  ,  ']'    , ';'   ,'@'   };
    // char seperators[] = " .,-:!\"'(?[];@})";
    long long unsigned int  size ;
    long long unsigned int capacity ;
    // pair <int,int> find_in_hash_table (string &word , long long unsigned int key);
    void insert_hash_table(string &word , int book_code , int page , int paragraph , int sentence_no ) ;
    void insert_with_freq(string &word, long long unsigned int freq);
    //hash_val will return the hash value of the particular word
    long long unsigned int hash_val(string &k) ;
    bool seperator_check(char &x) ;
    occurence_list_node* find_all_occurences(string word) ;
    // string seperators = " .,-:!\"'(?[];@})";
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long unsigned int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};