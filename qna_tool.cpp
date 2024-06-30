#include <assert.h>
#include <sstream>
#include <cmath>
#include "qna_tool.h"

using namespace std;

QNA_tool::QNA_tool(){
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    dictionary.insert_sentence(book_code,page,paragraph,sentence_no,sentence);
    //s.insert_sentence(book_code,page,paragraph,sentence_no,sentence);
    if(book_code>=para_score.size())
    {
        para_score.resize(book_code+1);
        para_size.resize(book_code+1);
        
    }
    if(page>=para_score[book_code].size())
    {
        para_score[book_code].resize(page+1);
        para_size[book_code].resize(page+1);
    }
    if(paragraph>=para_score[book_code][page].size())
    {
        para_score[book_code][page].resize(paragraph+1);
        para_size[book_code][page].resize(paragraph+1);
    }
    books=max(books,book_code+1);
    max_pages=max(max_pages,page+1);
    max_paras=max(max_paras,paragraph+1);
    para_size[book_code][page][paragraph]+=get_words(sentence).size();
    return;
}

// void QNA_tool::get_univ_values()
// {
//     ifstream inFile("unigram_freq.csv");
//     if (!inFile.is_open()) {
//         cerr << "Error: Unable to open the input file " << endl;
//         exit(1);
//     }
//     string line;
//     while(getline(inFile,line))
//     {
//         if (line.empty() || line.find("//") == 0 || line.find_first_not_of(" ") == string::npos) {
//             continue;
//         }
//         string word;
//         long long unsigned int count=0;
//         bool check=false;
//         if(line=="word,count")
//             continue;
//         for(auto ch: line)
//         {
//             if(ch!=',' && !check)
//                 word+=ch;
//             else if(ch!=',' && check)
//             {
//                 count=count*10+(ch-'0');
//                 // cout<<count<<endl;
//             }
//             else
//                 check=true;
//         }
//         //cout<<word<<" "<<count<<endl;
//         univ_dic.insert_with_freq(word,count);
//         //break;
//     }
//     inFile.close();
// } 

vector<string> QNA_tool:: get_words(string question)
{
    vector<string> query_tokens;
    string current_word = "";
    for (auto &x : question)
    {
        // bool if_seperator = seperator_check(x);
        if (dictionary.seperators[x])
        {
            if (current_word != "")
            {
                query_tokens.push_back(current_word);
                current_word = "";
            }
        }
        else
        {
            current_word.push_back(x);
        }
    }
    if(current_word!="")
        query_tokens.push_back(current_word);
    return query_tokens;
}

string my_transform_3(string &s)
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

int QNA_tool::get_total_num_of_para()
{
    int count=0;
    for(auto book:para_score)
    {
        for(auto page:book)
        {
            count+=page.size();
        }
    }
    return count;
}

vector<int> QNA_tool::get_num_para(vector<string> &query_tokens, vector<occurence_list_node*> &all_occurences)
{
    int len=query_tokens.size();
    vector<int> to_return (len,0);
    for(int i=0;i<len;i++)
    { 
        vector<vector<vector<bool>>> check_para(books,vector<vector<bool>>(max_pages,vector<bool>(max_paras,0)));
        occurence_list_node* curr=all_occurences[i];
        int count=0;
        while(curr!=nullptr)
        {
            int book_n=curr->book_code;
            int page_n=curr->page;
            int para_n=curr->paragraph;
            if(check_para[book_n][page_n][para_n]==0)
            {
                check_para[book_n][page_n][para_n]=1;
                count++;
            }
            curr=curr->next;
        }
        to_return[i]=count;
    }
    return to_return;
}

pair<Node*,int> QNA_tool::get_top_k_para_modified(string question, int k) {
    minheap q;
    question=my_transform_3(question);
    vector<string> query_tokens=get_words(question);
    int tot_tokens=query_tokens.size();
    double avg_para_length=0;

    //Get avg_para_length   
    int count=1;
    for(int i=0;i<para_score.size();i++)
    {
        int book=i;
        for(int j=0;j<para_score[book].size();j++)
        {
            int page=j;
            for(int l=0;l<para_score[book][page].size();l++)
            {
                int para=l;
                avg_para_length+=(para_size[book][page][para]-avg_para_length)/count;
                count++;                
            }
        }
    }
    //cout<<avg_para_length<<endl;

    
    //Get occurrence of all words
    vector<occurence_list_node*> occurence_of_all_words;
    for(auto token:query_tokens)
    {
        occurence_of_all_words.push_back(dictionary.find_all_occurences(token));
    }

    //Get para counts
    int total_num_para=get_total_num_of_para();
    //cout<<total_num_para<<endl;

    vector<int> num_para=get_num_para(query_tokens,occurence_of_all_words);
    // for(auto &i:num_para)
    // {
    //     cout<<i<<" ";
    // }
    // cout<<endl;

    //Calculate word frequency in paragraph
    vector<vector<vector<vector<int>>>> word_freq_in_para(tot_tokens,vector<vector<vector<int>>>(books,vector<vector<int>>(max_pages,vector<int>(max_paras,0))));
    for(int i=0;i<tot_tokens;i++)
    {
        string token=query_tokens[i];
        occurence_list_node* curr=occurence_of_all_words[i];
        while(curr!=nullptr)
        {
            int book_n=curr->book_code;
            int page_n=curr->page;
            int para_n=curr->paragraph;

            word_freq_in_para[i][book_n][page_n][para_n]++;

            curr=curr->next;
        }
    }

    //Calculate word scores
    vector<double> word_scores (tot_tokens);
    for(int i=0;i<tot_tokens;i++)
    {
        string token=query_tokens[i];
        double idf_i=((total_num_para-num_para[i]+0.5)/(num_para[i]+0.5))+1;
        word_scores[i]=idf_i;
        //cout<<token<<" idf: "<<idf_i<<endl;
    }

    //Scoring paragraphs
    for(int idx=0;idx<tot_tokens;idx++)
    {
        string token=query_tokens[idx];
        for(int i=0;i<para_score.size();i++)
        {
            int book=i;
            for(int j=0;j<para_score[book].size();j++)
            {
                int page=j;
                for(int l=0;l<para_score[book][page].size();l++)
                {
                    int para=l;
                    int freq=word_freq_in_para[idx][book][page][para];
                    double frac=((freq/(freq+(k*(1-b+(b*(para_size[book][page][para]/avg_para_length))))))*(1+k))+delta;
                    para_score[book][page][para]+=frac*word_scores[idx];
                }
            }
        }
    }

    //Finding top K para
    int tot_token_size=0;
    for(int i=0;i<para_score.size();i++)
    {
        int book=i;
        for(int j=0;j<para_score[book].size();j++)
        {
            int page=j;
            for(int l=0;l<para_score[book][page].size();l++)
            {
                int para=l;
                double curr_score=para_score[book][page][para];
                //cout<<curr_score<<endl;
                para_score[book][page][para]=0;
                para_id* newEntry= new para_id();
                newEntry->book=book;
                newEntry->page_no=page;
                newEntry->para_no=para;
                newEntry->score=curr_score;
                if(q.size()<3*k)
                {
                        q.insert(newEntry);
                        // tot_token_size+=para_size[book][page][para];
                }
                else
                {
                    if(q.top()->score<curr_score)
                    {
                        para_id* tbr=q.removeMin();
                        // tot_token_size-=para_size[tbr->book][tbr->page_no][tbr->para_no];
                           delete tbr;
                           // tot_token_size+=para_size[newEntry->book][newEntry->page_no][newEntry->para_no];
                            q.insert(newEntry);
                            
                    }
                    else
                    {
                        delete newEntry;
                    }
                }
            }
        }
    }
    int total_k=0;
    //Making the linked list
    Node* head= new Node();
    head->right=nullptr;
    head->left=nullptr;
    int count_big_para=0;
    vector<para_id*> stk;
    while(!q.empty())
    {
        stk.push_back(q.removeMin());        
    }
    while(!stk.empty() && count_big_para<k)
    {
        total_k++;
        para_id* entry=stk.back();
        stk.pop_back();
        Node* newEn= new Node();
        newEn->page=entry->page_no;
        newEn->book_code=entry->book;
        newEn->paragraph=entry->para_no;
        if(head->right){
            head->right->left=newEn; 
        }
        newEn->right=head->right;
        head->right=newEn;
        newEn->left=head;
        if(para_size[entry->book][entry->page_no][entry->para_no]>=15)
            count_big_para++;
        delete entry;
        //cout<<newEn->book_code<<" "<<newEn->page<<" "<<newEn->paragraph<<" "<<para_score[newEn->book_code][newEn->page][newEn->paragraph]<<endl;
        // for(int i=0;i<tot_tokens;i++)
        // {
        //     //cout<<"Frequency of "<<query_tokens[i]<<" is " <<word_freq_in_para[i][newEn->book_code][newEn->page][newEn->paragraph]<<endl;
        // }
    }

    //Returning the list
    Node* return_node=head->right;
    if(head->right)
        head->right->left=nullptr;
    delete head;
    return {return_node,total_k};
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    
    minheap q;
    question=my_transform_3(question);
    vector<string> query_tokens=get_words(question);
    vector<double> word_scores;
    vector<int> num_paragraphs;
    //cout<<"wow\n";
    for(auto token:query_tokens)
    {
        long long unsigned int u_freq=univ_dic.get_word_count(token)+1;
        long long unsigned int freq=dictionary.get_word_count(token)+1;
        //cout<<token<<" "<<freq<<" "<<u_freq<<endl;
        word_scores.push_back((double)freq/u_freq);
    }
    //cout<<"wow1\n";

    int num_words=word_scores.size();

    vector<occurence_list_node*> occurence_of_all_words;
    for(auto token:query_tokens)
    {
        occurence_of_all_words.push_back(dictionary.find_all_occurences(token));
    }
    //cout<<"wow2\n";
    for(int i=0;i<num_words;i++)
    {
        occurence_list_node* curr=occurence_of_all_words[i];
        while(curr!=nullptr)
        {
            int book_n=curr->book_code;
            int page_n=curr->page;
            int para_n=curr->paragraph;

            para_score[book_n][page_n][para_n]+=word_scores[i];

            curr=curr->next;
        }
    }
    //cout<<"wow3\n";

    for(int i=0;i<para_score.size();i++)
    {
        int book=i;
        for(int j=0;j<para_score[book].size();j++)
        {
            int page=j;
            for(int l=0;l<para_score[book][page].size();l++)
            {
                int para=l;
                double curr_score=para_score[book][page][para];

                para_id* newEntry= new para_id();
                newEntry->book=book;
                newEntry->page_no=page;
                newEntry->para_no=para;
                newEntry->score=curr_score;

                if(q.size()<k)
                {
                    q.insert(newEntry);
                }
                else
                {
                    if(q.top()->score<curr_score)
                    {
                        para_id* tbr=q.removeMin();
                        delete tbr;
                        q.insert(newEntry);
                    }
                    else
                    {
                        delete newEntry;
                    }
                }
            }
        }
    }
    //cout<<"wow4\n";
    Node* head= new Node();
    head->right=nullptr;
    head->left=nullptr;
    while(!q.empty())
    {
        para_id* entry=q.removeMin();
        Node* newEn= new Node();
        newEn->page=entry->page_no;
        newEn->book_code=entry->book;
        newEn->paragraph=entry->para_no;
        if(head->right){
            head->right->left=newEn; 
        }
        newEn->right=head->right;
        head->right=newEn;
        newEn->left=head;
        delete entry;
        //cout<<newEn->book_code<<" "<<newEn->page<<" "<<newEn->paragraph<<" "<<para_score[newEn->book_code][newEn->page][newEn->paragraph]<<endl;
    }
    //cout<<"wow5\n";
    Node* return_node=head->right;
    if(head->right)
        head->right->left=nullptr;
    delete head;
    return return_node;
}

void QNA_tool::query(string question, string filename,int k){
    // Inmplement your function here  
   // std::cout << "Q: " << question << std::endl;
    //std::cout << "A: ";

    // int k;
    // cout<<"Enter least number of paragraphs to be considered: "<<endl;
    // cin>>k; 
    string api_key="";
    pair<Node*,int> temp = get_top_k_para_modified(question,k);
    Node * root= temp.first;
    k= temp.second;
    query_llm(filename,root,k,api_key,question);
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    //cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "corpus/mahatma-gandhi-collected-works-volume-";
    //cout<<filename<<endl;
    filename += to_string(book_code);
    //cout<<filename<<endl;
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    //You will be given a set of paragraphs, not necessarily in any logical order or semantically correct. These paragraphs are excerpts from Mahatma Gandhi's books. Now I will give you a query. Answer the query with reference to the paragraphs I have provided. Do not mention if you do not find the exact answer in the paragraphs I have provided, instead make reasonable assumptions to generate an answer.
    // outfile <<"I am giving you a set of 20 paragraphs, this are from mahatma gandhi's autobiography and I am having a query and I want to find the best possible answer of this query from this paragraphs I am giving. Paragraphs are according to decreasing order of relevance. But give the best possible answer for the query.\n Here is the query : \n" ;
    outfile << "You will be given a set of paragraphs, not necessarily in any logical order or semantically correct. These paragraphs are excerpts from Mahatma Gandhi's books and in decreasing priority of relevance. Now I will give you a query.Answer the query using the paragraphs I have provided. If you do not find the exact answer in the paragraphs, feel free to make assumptions.\nHere is the query: \n";
    outfile << "Q: "<<question;
    // outfile << "\nHere are the paragraphs:\n";
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}