// Do NOT add any other includes
#include "search.h"

SearchEngine::SearchEngine(){
    // Implement your function here  
}

SearchEngine::~SearchEngine(){
    // Implement your function here  
}

string my_transform_2(string &s)
{
    string ans = "";
  
    for (int i = 0; i < s.size(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            ans.push_back(s[i] + 32);

        }
        else{
            ans.push_back(s[i]);
        }

    }
    return ans;
}

void proc_bad(string& m , vector<int> &b_table)

{
     int size = m.size();
    
    for (int i = 0; i < size - 1; i++) {
        b_table[(m[i])] = size - i - 1;
    }
   
}




Node* bm_search(vector<sentences> &text, string &m, int &n_matches)
{
    
    
    string m2 = my_transform_2(m);
   

    int msize = m2.size();
    int s2 = 1;
    for(int i = msize-2 ; i >=0 ; i--)
    {
        if(m2[i]==m2[msize-1])
        break;
        s2++;

    }

    vector<int> b_table(256,msize);
    proc_bad(m2,b_table);
    
    Node* matches = new Node();
    matches->right = NULL;
    Node* traversor = matches;

    for(auto &T:text)
    {
       
        int tsize = T.sent.size();
        int shift = 0 ;

        while( (shift <= (tsize - msize)))
        {
            int i = msize -1;

                if (m2[i] == T.sent[shift + i]) {    
                while (i >= 0 && (m2[i] == T.sent[shift + i])) {
                    i--;
                }
            }
            
            if( i < 0)
            {
                 Node* match = new Node(T.loc[0],T.loc[1],T.loc[2],T.loc[3],shift);
                match->right = NULL;
                    n_matches++;
                    traversor->right = match;
                     match->left = traversor;
                    traversor = match;
                    shift += s2;
                    
           

            }
            else
            {
          
                shift += b_table[T.sent[shift + msize-1]];


            }
            
 

        }

    }
     if(matches->right!=NULL)
     {
        Node* final_node = matches->right ;
        delete matches ;
         final_node->left = NULL;
        return final_node ;
     }
     delete matches ;
    return NULL;
}



void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
    
{
    // Implement your function here
    // int b[] = {book_code , page , paragraph , sentence_no} ;
    sentences a;
    string sente = my_transform_2(sentence);
    a.sent = sente;
    a.loc[0] = book_code;
    a.loc[1] = page;
    a.loc[2] = paragraph;
    a.loc[3] = sentence_no;
    no_of_sentences++;
    text.push_back(a);
    text_size += sentence.size();
    return;
}


Node *SearchEngine::search(string pattern, int &n_matches)

{
    n_matches = 0 ;
    // return boyer_moore(text,pattern,n_matches);
    return bm_search(text, pattern, n_matches);

}


// int main()
// {
//     // SearchEngine* a = new SearchEngine() ;
//     // for(int i = 0 ; i<1000 ; i++)
//     // {
//     //     a->insert_sentence(0,0,0,0,to_string(i)) ;
//     // }
//     // int b =  0;
//     // Node* temp = a->search("1" , b) ;
//     // for(int i = 0 ; i<b ; i++)
//     // {
//     //     cout<<temp->offset<<endl;
//     //     temp = temp->right ;
//     // }
//     // cout<<"j"<<endl;
//     // cout<<b<<endl;
//     // delete a ;
//     // cout<<temp->offset<<endl;

//     SearchEngine* b  = new SearchEngine() ;
//     b->insert_sentence(0,0,0,0,"The hghjdTHE tHe thE !THE! !the! .....the.... ");
//     int nmatches = 0;
//     Node* temp = b->search("tHe" , nmatches);
//     for(int i = 0 ; i<nmatches ; i++)
//     {
//         cout<<temp->offset<<endl;
//         temp = temp->right ;
//     }
//     cout<<"namtches : "<<nmatches<<endl;
//     delete b;

// }
