#include "dict.h"

//To use call find_all_occurences function it will return occurence_list_node* data type which is head of list storing all occurences 
long long unsigned int initial_size = 0;
// long long unsigned int initial_size = 0;
long long unsigned int initial_capacity = 8192;
long long unsigned int choose_prime_for_polynomial_hash_function = 31;
long long unsigned int choose_prime_for_taking_mod = 31;
int load_factor_num = 4;
int load_factor_den = 5;

Dict ::Dict()
{
    size = initial_size;
    capacity = initial_capacity;
    hash_table = new vector<words *>(capacity, NULL);
    // (*hash_table) = new words *[initial_capacity];
    // for (int i = 0; i < initial_capacity; i++)
    // {
    //     (*hash_table)[i] = NULL;
    // }

    for (int i = 0; i < 256; i++)
    {
        seperators[i] = false;
    }
    // seperators = new char[15] ;
    // seperators[0] = ' ';
    seperators[' '] = true;
    seperators['.'] = true;
    seperators[','] = true;
    seperators['-'] = true;
    seperators[':'] = true;
    seperators['!'] = true;
    seperators['"'] = true;
    seperators['\''] = true;
    seperators['('] = true;
    seperators[')'] = true;
    seperators['?'] = true;
    seperators['['] = true;
    seperators[']'] = true;
    seperators[';'] = true;
    seperators['@'] = true;
    // seperators  =  { ' ' ,  '.'  ,   ','  ,   '-'    ,  ':'   ,   '!'  ,  '"'    ,  '\''  , '('   ,  ')'  ,   '?'      ,  '['  ,  ']'    , ';'   ,'@'   };
}

Dict::~Dict()
{
    // Implement your function here
    for (int i = 0; i < capacity; i++)
    {
        words *temp = (*hash_table)[i];
        while (temp)
        {
            words *temp2 = temp->next;
            occurence_list_node* temp_node_occurence_list = temp->head ;
            while(temp_node_occurence_list)
            {
                occurence_list_node* temp1 = temp_node_occurence_list->next ;
                delete temp_node_occurence_list ;
                temp_node_occurence_list = temp1;
            }
            delete temp;
            temp = temp2;
        }
    }
    delete (hash_table);
}

void my_transform(string &s)
{
    // string ans ;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
        {
            s[i] = (s[i] - 'A' + 'a');
        }
    }
    // return ans ;
}

bool Dict ::seperator_check(char &x)
{
    for (auto &i : seperators)
    {
        if (x == i)
        {
            return true;
        }
    }
    return false;
}

void Dict ::insert_hash_table(string &word , int book_code , int page , int paragraph , int sentence_no )
{
    my_transform(word);
    long long unsigned int key = hash_val(word);
    words *temp = (*hash_table)[key];
    //If word found already in hash table occurence will be added in while loop and function will get return 
    while (temp)
    {
        if (temp->id == word)
        {
            temp->frequency++;
            occurence_list_node* older_head = temp->head  ;
            occurence_list_node* new_head = new occurence_list_node() ;
            new_head->next = NULL ;
            new_head->book_code = book_code ;
            new_head->page = page ;
            new_head->paragraph = paragraph ;
            new_head->sentence_no = sentence_no ;
            new_head->next = older_head ;
            temp->head = new_head ;
            return;
        }
        temp = temp->next;
    }

    //This parts inserts new word in hash table
    temp = (*hash_table)[key];
    words *new_word = new words();
    new_word->id = word;
    new_word->frequency = 1;
    new_word->next = temp;
    (*hash_table)[key] = new_word;
    size++;
    //Till here

    //This parts adds occurence list node for that newly inserted word
    occurence_list_node* new_word_occurence = new occurence_list_node() ;
    
    new_word_occurence->book_code = book_code ;
    new_word_occurence->page = page ;
    new_word_occurence->paragraph = paragraph ;
    new_word_occurence->sentence_no = sentence_no ;
    new_word_occurence->next = NULL ;
    
    
    new_word->head = new_word_occurence ;
    //Till here
    

    if (size * load_factor_den >= capacity * load_factor_num)
    {
        capacity = 2 * capacity;
        // cout<<"resizing and new capacity is"<<capacity<<endl;
        // words** new_hash_table[capacity];
        vector<words*>* new_hash_table = new vector<words*>(capacity,NULL);
        
        

        //In resizing we were not deleting the older words in hash table they were just getting reallocated so there occurence 
        //list will also automatically get reallocated 

        for (int i = 0; i < capacity / 2; i++)
        {
            words *temp = (*hash_table)[i];
            while (temp)
            {
                words *temp2 = temp->next;
                long long unsigned int key_new = hash_val(temp->id);
                words *to_store = (*new_hash_table)[key_new];
                temp->next = to_store;
                (*new_hash_table)[key_new] = temp;
                temp = temp2;
            }
        }
        // for(int i = 0 ; i<capacity/2 ; i++)
        // {
        //     words* temp = (*hash_table)[i] ;
        //     while(temp)
        //     {
        //         words* temp2 = temp ;
        //         temp = temp->next ;
        //         delete temp2;
        //     }
        // }
        delete (hash_table);
        hash_table = new_hash_table;
    }
    return;
}

void Dict:: insert_with_freq(string &word,long long unsigned int freq)
{
    my_transform(word);
    long long unsigned int key = hash_val(word);
    words *temp = (*hash_table)[key];
    //If word found already in hash table occurence will be added in while loop and function will get return 
    while (temp)
    {
        if (temp->id == word)
        {
            // temp->frequency++;
            // occurence_list_node* older_head = temp->head  ;
            // occurence_list_node* new_head = new occurence_list_node() ;
            // new_head->next = NULL ;
            // new_head->book_code = book_code ;
            // new_head->page = page ;
            // new_head->paragraph = paragraph ;
            // new_head->sentence_no = sentence_no ;
            // new_head->next = older_head ;
            // temp->head = new_head ;
            return;
        }
        temp = temp->next;
    }

    //This parts inserts new word in hash table
    temp = (*hash_table)[key];
    words *new_word = new words();
    new_word->id = word;
    new_word->frequency =freq;
    new_word->next = temp;
    (*hash_table)[key] = new_word;
    size++;
    //Till here
    if (size * load_factor_den >= capacity * load_factor_num)
    {
        capacity = 2 * capacity;
        // cout<<"resizing and new capacity is"<<capacity<<endl;
        // words** new_hash_table[capacity];
        vector<words*>* new_hash_table = new vector<words*>(capacity,NULL);
        
        

        //In resizing we were not deleting the older words in hash table they were just getting reallocated so there occurence 
        //list will also automatically get reallocated 

        for (int i = 0; i < capacity / 2; i++)
        {
            words *temp = (*hash_table)[i];
            while (temp)
            {
                words *temp2 = temp->next;
                long long unsigned int key_new = hash_val(temp->id);
                words *to_store = (*new_hash_table)[key_new];
                temp->next = to_store;
                (*new_hash_table)[key_new] = temp;
                temp = temp2;
            }
        }
        // for(int i = 0 ; i<capacity/2 ; i++)
        // {
        //     words* temp = (*hash_table)[i] ;
        //     while(temp)
        //     {
        //         words* temp2 = temp ;
        //         temp = temp->next ;
        //         delete temp2;
        //     }
        // }
        delete (hash_table);
        hash_table = new_hash_table;
    }
    return;

}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    // Implement your function here
    string current_word = "";
    for (auto &x : sentence)
    {
        // bool if_seperator = seperator_check(x);
        if (seperators[x])
        {
            if (current_word != "")
            {
                insert_hash_table(current_word, book_code , page , paragraph , sentence_no);
                current_word = "";
            }
        }
        else
        {
            current_word.push_back(x);
        }
    }
    if (current_word == "")
    {
    }
    else
    {
        insert_hash_table(current_word , book_code , page , paragraph , sentence_no );
    }
    return;
}

occurence_list_node* Dict :: find_all_occurences(string word) 
{
    long long unsigned int key = hash_val(word) ;
    words* temp = (*hash_table)[key] ;

    while(temp)
    {
        if(temp->id == word)
        {
            break ;
        }
        temp = temp->next ;
    }
    if(temp == NULL)
    {
        return NULL ;
    }
    return temp->head ;
}

long long unsigned int Dict::get_word_count(string word)
{
    // Implement your function here
    long long unsigned int key = hash_val(word);
    words *temp = (*hash_table)[key];
    while (temp)
    {
        if (temp->id == word)
        {
            return temp->frequency;
        }
        temp = temp->next;
    }
    return 0;
}

void Dict::dump_dictionary(string filename)
{
    // Implement your function here
    std::ofstream output_file(filename, std::ios::out);
     if (!output_file.is_open()) {
        return;
    }
  
    for (int i = 0; i < capacity; i++)
    {
        words *temp = (*hash_table)[i];
        while (temp)
        {
            output_file << temp->id << ", " << temp->frequency << endl;
            temp = temp->next;
        }
    }
   
    return;
}

long long unsigned int Dict ::hash_val(string &k)
{

    // IMPLEMENT YOUR CODE HERE
    long long unsigned int hashval = 0;
    long long unsigned int p = choose_prime_for_polynomial_hash_function;
    long long unsigned int x = 1;
    for (auto &c : k)
    {
        hashval = (hashval + ((c)*x));
        x = (x * p);
    }

    hashval = hashval % capacity;
    return hashval;

    // unsigned long long hashval = 5381; // Initial hash value

    // for(auto &c : k) {
    //     // int c = *str;
    //     hashval = ((hashval << 5) + hashval) + c; 
    //     // str++;
    // }

    // hashval = hashval%capacity;
    // return hashval;
    // Placeholder return value
}
// int main()
// {
//     Dict* d = new Dict() ;
//     // cout<<"hi"<<endl;
//     d->insert_sentence(12,123,69,69,to_string(5)) ;
//     d->insert_sentence(112,123,69,69,to_string(5)) ;
//     d->insert_sentence(12,1233,69,69,to_string(5)) ;
//     d->insert_sentence(12,123,65339,69,to_string(5)) ;
//     d->insert_sentence(12,123,69,69574,to_string(5)) ;
//     d->insert_sentence(12,123,269,69,to_string(5)) ;
//     d->insert_sentence(12,12343,69,69,to_string(5)) ;
//     for(int i = 0 ; i<1000000 ; i++)
//     {
//         d->insert_sentence(0,0,746,0,to_string(i)) ;

//     }
//     // cout<<"gonna start dumping"<<endl;

//     occurence_list_node* temp = d->find_all_occurences("5") ;
//     while(temp)
//     {
//         cout<<temp->book_code<<" "<<temp->page<<" "<<temp->paragraph<<" "<<temp->sentence_no<<endl;
//         temp = temp->next ;
//     }
//     // d->dump_dictionary("check.txt") ;
//     delete d ;

// }