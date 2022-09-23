#include "ternary_search_tree.hpp"
#include "parser.hpp"

tst::Data::Data(int user_id, int sofifa_id, float rating){
    this->user_id = user_id;
    this->sofifa_id = sofifa_id;
    this->rating = rating;
}

tst::Data::Data(){
    this->user_id = -1;
    this->sofifa_id = -1;
}

tst::Node::Node(){
    this->key = '\0';
}

tst::Node::Node(Data *data, char key){
    this->key = key;
    this->data = data;
    this->next.assign(0x60, NULL);
}

tst::Tree::Tree(){
    this->size = 0;
    root = new Node(NULL, '\0');
}

bool tst::Tree::isEmpty(){
    return root == NULL;
}

int to_int(char a){
    if (0x41 <= a && a <= 0x5A)
        return a;
    return a - 0x20;
}

void tst::Tree::insert(std::string word, Data *data){
    char ch;
    Node *node = this->root;

    for (std::string::iterator it = word.begin(); it != (word.end()); it ++){        
        ch = *it;
        int c = to_int(ch);
        if (node->next[c] == NULL){
            node->next[c] = new Node(NULL, ch);
        }
        node = node->next[c];
    }
    if (node->data == NULL){
        node->data = data; 
        this->size ++;
    }
}

void tst::Tree::remove(std::string word){

}

int tst::Tree::search(std::string word){
    char ch;
    Node *node = this->root;
    for (std::string::iterator it = word.begin(); it != word.end(); it ++){
        ch = *it;
        int c = to_int(ch);
        if (node->next[c] == NULL)
            return -1;
        node = node->next[c];
    }
    if (node->data == NULL)
        return -1;
    else
        return node->data->sofifa_id;
}

void tst::Tree::read_csv(std::ifstream &input){
    std::vector<std::string> names;
    aria::csv::CsvParser parser(input);
    for (auto& row : parser) {
        int i = 0;
        std::string name;
        int sofifaId;
        for (auto& field : row) {
 
            switch (i) {
            case 0:
                sofifaId = atoi(field.c_str());
                break;
            case 1:
                name = field;
                break;
            case 2: 
                break;
            }
            
            i ++;
        }
        insert(name, new tst::Data(0,sofifaId, 0));
    } 
}

bool isInVector(std::vector<tst::Node*> v, tst::Node* node){
    for (std::vector<tst::Node*>::iterator  it = v.begin(); it != v.end(); it ++)
        if ((*it) == node)
            return true;
    return false;
}

void tst::Tree::find_words_by_radix(Node *node, std::string current_word, std::vector<tst::Node*> *visited, std::vector<std::string> *word_list, std::vector<int> *ids){
    
    visited->push_back(node);
    current_word.push_back(node->key);
    
    if (word_list->size() >= 20)
        return;
 
    if (node->data != NULL) {
        word_list->push_back(current_word);
        ids->push_back(node->data->sofifa_id);
    }
    
    for (std::vector<Node*>::iterator it = node->next.begin(); it != node->next.end(); it ++)
        if (*it != NULL && !isInVector(*visited, *it))
            find_words_by_radix(*it, current_word, visited, word_list, ids);
        
    
}

std::vector<std::string> tst::Tree::search_by_radix(std::string radix, std::vector<int> *sofifa_ids) {
    std::vector<std::string> *wl = new std::vector<std::string>; 
    std::vector<std::string> word_list = *wl;

    std::string current_word = "";
    std::vector<tst::Node*> visited;
    
    tst::Node *node = this->root;
     
    for (std::string::iterator it = radix.begin(); it != radix.end(); it ++){
        char ch = *it;
 
        current_word.push_back(ch); 
        if (node->next[to_int(ch)] == NULL)
            return word_list;

        node = node->next[to_int(ch)];
    }
    current_word.pop_back();
    find_words_by_radix(node, current_word, &visited, wl, sofifa_ids);
    return *wl;

}