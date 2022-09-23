#include "position_table.hpp"
#include <fstream>
#include <sstream>

using namespace pos;

template<typename T>
bool isInVector(T a, std::vector<T> v);

Data::Data(){}

Data::Data(int sofifaId){
    this->sofifaIds.push_back(sofifaId);
}

void Data::operator>>(std::ostream &output){
    for (auto id : this->sofifaIds)
        output << id << std::endl;
}

void Data::append(int sofifaId){
    this->sofifaIds.push_back(sofifaId);
}

Node::Node(){
    this->data = NULL;
}

Node::Node(std::string position, int sofifaId){
    this->pos = position;
    this->data = new Data(sofifaId);
}

void Node::operator>>(std::ostream &output){
    output << this->pos << std::endl;
    *(this->data) >> output;
    output << std::endl;
}

int PositionTable::hash(std::string tag){
    int tagSize = tag.size();
    int a = (int)*(tag.begin());
    int b = (int)*(tag.end()-1);
    int c = (int)*(tag.end()-tagSize/2);
    int d = (int)*(tag.begin()+tagSize/4);
    int e = (int)*(tag.begin()+3*tagSize/4);
    int f = (a*a + b*e + a*e + c*c + e*c + a*c);

    return f % this->size;
}

PositionTable::PositionTable(int size){
    this->size = size;
    for (int i = 0; i < this->size; i ++){
        table.push_back(new std::vector<Node>);
    }
}

PositionTable::PositionTable(){}

void PositionTable::insert(std::string position, int sofifaId){
    int key = hash(position);
    auto linkedList = table[key];
    for (auto i : *linkedList){
        if (i.pos == position){
            i.data->append(sofifaId);
            return;    
        }
    }
    linkedList->push_back(Node(position, sofifaId));
}

void PositionTable::remove(std::string position){

}

Node PositionTable::search(std::string position){
    int key = hash(position);
    std::vector<Node>* list = table[key];
    for (std::vector<Node>::iterator it = list->begin(); it != list->end(); it ++){
        if (it->pos == position){
            return *it;
        }
    }
    return Node();
}

void PositionTable::operator>>(std::ostream &output){
    for (auto it : table){
        if (it->size() > 0){
            for (auto node : *it)
                node >> output;
        }
    }
}

template<typename T>
bool isInVector(T a, std::vector<T> v){
    for (T i : v)
        if (i == a)
            return true;
    
    return false;
}