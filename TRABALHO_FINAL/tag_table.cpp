#include "tag_table.hpp"
#include "parser.hpp"
#include <sstream>
#include "hash_table.hpp"

using namespace tagTable;

Data::Data(int sofifaId) {this->ids.clear();this->ids.push_back(sofifaId);}

Data::Data(){}

void Data::append(int sofifaId){
    this->ids.push_back(sofifaId);
}

Node::Node(std::string tag, Data data){
    this->tag = tag;
    this->data = data;
}

Node::Node(){this->tag = "\0";}

int Table::hash(std::string tag){
    int tagSize = tag.size();
    int a = (int)*(tag.begin());
    int b = (int)*(tag.end()-1);
    int c = (int)*(tag.end()-tagSize/2);
    int d = (int)*(tag.begin()+tagSize/4);
    int e = (int)*(tag.begin()+3*tagSize/4);
    int f = (a*a + b*e + a*e + c*c + e*c + a*c);

    return f % this->size;
}

Table::Table(int size){
    this->size = size;
    for (int i = 0; i < this->size; i ++){
        std::vector<Node*> v;
        table.push_back(v);
    }
    
}

Node* Table::operator[](std::string tag){
    int key = hash(tag);
    std::vector<Node*> list = table[key];
    for (auto it : list){
        if (it->tag == tag){
            return it;
        }
    }
    return NULL;
}

void Table::remove(std::string tag){
    int key = hash(tag);
    std::vector<Node*> list = table[key];
    for (std::vector<Node*>::iterator it = list.begin(); it != list.end(); it ++){
        if ((*it)->tag == tag){
            free(*it);
        }
    }
}

bool isInVector(int a, std::vector<int> v){
    for (int i : v)
        if (i == a)
            return true;
    
    return false;
}

void Table::insert(std::string tag, Data data){
    Node* node = (*this)[tag];
    
    if (node == NULL)
        this->table[hash(tag)].push_back(new Node(tag, data));
    else
        if (!isInVector(data.ids[0], node->data.ids))
            node->data.append(data.ids[0]);
}

void Table::read_csv(std::ifstream &file){
    aria::csv::CsvParser parser(file);
    bool first_line = true;
    for (auto& row : parser) {
        int i = 0;
        std::string tag;
        int sofifaId;
        if (!first_line){
            for (auto& field : row) {
                switch (i) {
                case 0:
                    break;
                case 1:
                    sofifaId = atoi(field.c_str());
                    break;
                case 2: 
                    tag = field;
                    break;
                }
                
                i ++;
            }
            insert(tag, Data(sofifaId));
        }
        else
            first_line = false;
    } 
}


std::vector<int> Table::intersection(std::vector<std::vector<int>> tag_vect){
    std::vector<int> id_vect;
    bool found = true;

    std::vector<int> aux_vect = tag_vect[0];
    tag_vect.erase(tag_vect.begin());

    for (int aux_id : aux_vect)
    {
        found = true;
        for (std::vector<int> vect : tag_vect)
        {
            found = false;
            for (int id : vect)
            {
                if (id == aux_id)
                {   
                    found = true;
                    break;
                }
            }
            if (!found)
                break;
        }
        if (found){
            id_vect.push_back(aux_id);
            if (id_vect.size() >= 20)
                break;
        }
    }
    return id_vect;
}

void Data::operator>>(std::ostream &file){
    for (auto id : this->ids)
        file << id << "\t";
}

void Table::operator>>(std::ostream &file){
    for (auto list : this->table){
        for (auto node : list)
            node->data >> file;
        file << std::endl;    
    }
}

void Table::displayIntersection(std::vector<std::vector<int>> intersec, std::ostream &file, ht::HashTable *hashT){
    int a = 1;
    std::vector<int> inter = intersection(intersec);
    file << "\t" << std::setw(50) << std::right << "Nome" << '|' << setw(25) << std::right << "Posicoes" << '|' << std::setw(10) << std::right << "pontuacao" << '|' << std::endl;
    for (int i : inter){
        std::cout << a++ << ".\t";
        *(hashT->search(i).data) >> std::cout;
        std::cout << std::endl;
    }  
}