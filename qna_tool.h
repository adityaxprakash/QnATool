#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"

using namespace std;

struct para_id
{
    int book;
    int page_no;
    int para_no;
    double score;
};

class minheap
{
    private:
    vector<para_id*> heap;
    void HeapifyUp(int i)
    {
        if(i==0)
            return;
        int parent=(i-1)/2;
        if((heap[i])->score<(heap[parent])->score)
        {
            std::swap(heap[parent],heap[i]);
            HeapifyUp(parent);
        }
    }

    void HeapifyDown(int index)
    {
        if(2*index+1>=heap.size())
            return;
        int minindex=index;
        int left=2*index+1;
        int right=2*index+2;
        if(heap[left]->score<heap[minindex]->score)
            minindex=left;
        if(right<heap.size() && heap[right]->score<heap[minindex]->score)
            minindex=right;
        if (minindex != index) {
            std::swap(heap[minindex], heap[index]);
            HeapifyDown(minindex);
        }
    }

    public:
    void insert(para_id* k) 
    {
        heap.push_back(k);
        HeapifyUp(heap.size()-1);
    }

    para_id* removeMin()
    {
        para_id* tbr=heap[0];
        heap[0]=heap[heap.size()-1];
        heap.pop_back();
        if(heap.size()!=0)
            HeapifyDown(0);
        return tbr;
    }

    para_id* top()
    {
        return heap[0];
    }

    int size(){
        return heap.size();
    }
    bool empty()
    {
        return heap.size()==0;
    }
};



class QNA_tool {

private:
   
    Dict dictionary;
    Dict univ_dic;
    vector<string> get_words(string question);
    void get_univ_values();
    vector<vector<vector<double>>> para_score;
    vector<vector<vector<long long unsigned int>>> para_size;
    int get_total_num_of_para();
    vector<int> get_num_para(vector<string> &query_tokens, vector<occurence_list_node*> &all_occurences);
    int books=0;
    int max_pages=0;
    int max_paras=0;
    double k=1.2;
    double b=0.5;
    double delta=1;
    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename,int k);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
    pair<Node*,int> get_top_k_para_modified(string question, int k);
    //SearchEngine s;
};