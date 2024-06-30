#include<bits/stdc++.h>
#include "Node.h"
#include "qna_tool.h"

using namespace std;

int main(){

    QNA_tool qna_tool;

    int num_books = 98;
    //istream inS;
    std::cout << "Inserting books"<<std::endl;
    for(int i = 1; i <= num_books; i++){

        

        std::string filename = "corpus/mahatma-gandhi-collected-works-volume-";
        filename += to_string(i);
        filename += ".txt";

        std::ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open the input file mahatma-gandhi." << std::endl;
            return 1;
        }

        std::string tuple;
        std::string sentence;


        while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
            // Get a line in the sentence
            tuple += ')';

            std::vector<int> metadata;    
            std::istringstream iss(tuple);

            // Temporary variables for parsing
            std::string token;

            // Ignore the first character (the opening parenthesis)
            iss.ignore(1);

            // Parse and convert the elements to integers
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
                    metadata.push_back(num);
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata.push_back(num);
                }
            }

            // Now we have the string in sentence
            // And the other info in metadata
            // Add to the dictionary

            // Insert in the qna_tool
            qna_tool.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
        }

        inputFile.close();

    }

    string question; //= "Describe mahatma's thoughts on the british raj.";

    /*
        Part-1 : Get the top-k paragraphs
        Checking Correctness
    */

    // Let's try to ask a simple question to the qna_tool
    
    // Node* head = qna_tool.get_top_k_para_modified(question, 10);

    // vector<string> paras;
    // while(head != nullptr){
    //     string res = qna_tool.get_paragraph(head->book_code, head->page, head->paragraph);
    //     paras.push_back(res);
    //     Node* tb=head->right;
    //     if(tb!=nullptr)
    //         tb->left=nullptr;
    //     delete head;
    //     head = tb;
    // }

    // for(int i = 0; i < (int)paras.size(); i++){
    //     cout << paras[i] << endl << endl << endl;
    // }

    // int c;
    // head=qna_tool.s.search("partition",c);
    // vector<string> paras2;
    // while(head != nullptr){
    //     string res = qna_tool.get_paragraph(head->book_code, head->page, head->paragraph);
    //     paras2.push_back(res);
    //     Node* tb=head->right;
    //     if(tb!=nullptr)
    //         tb->left=nullptr;
    //     delete head;
    //     head = tb;
    // }

    // for(int i = 0; i < (int)paras2.size(); i++){
    //     cout << paras2[i] << endl << endl << endl;
    // }

    /*
        Part-2 : Query the LLM
    */
    cout<<"Enter query now: "<<endl;
    bool flag=false;
    while(getline(cin,question))   
    {
        if(flag)
            getline(cin,question);
        flag=true;
        int alpha;
        cout<<"enter minimum paragraphs to be considered: "<<endl;
        cin>>alpha;
        qna_tool.query(question, "api_call.py",alpha);
    }
    return 0;
}