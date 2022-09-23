#include "hash_table.hpp"
#include "parser.hpp"

ht::Data::Data(std::string name, std::string player_position){
    this->count = 0;
    this->name = name;
    this->rating = 0;
    append_positions(player_position);
}

ht::Data::Data(){}

void ht::Data::operator>>(std::ostream &output){
    output << std::setw(50) << std::left << this->name << '|';
    std::string pos;
    for (std::vector<std::string>::iterator it = this->player_position.begin(); it != this->player_position.end(); it ++){
        pos.append(*it);
        pos.append(", ");
    }
    pos.pop_back();
    pos.pop_back();
    output << setw(25) << std::right << pos;
    
    output << '|' << std::setw(10) << std::right << std::setprecision(3) << this->rating << '|';
}

void ht::Data::append_positions(std::string positions){
    size_t start = 0, end = 0;

    while ((end = positions.find(',', start)) != string::npos){
        this->player_position.push_back(positions.substr(start, end - start));
        start = end + 1;
    }

    this->player_position.push_back(positions.substr(start));
}

void ht::Data::append_rating(float rating){
    this->rating = ((this->rating * this->count) + rating) / (this->count + 1);
    this->count ++;
}

ht::Node::Node(int key, int sofifa_id, ht::Data *data){
    this->key = key;
    this->data = data;
    this->sofifa_id = sofifa_id;
}

ht::Node::Node(){
    this->key = -1;
}

ht::Node ht::HashTable::search(int sofifa_id){
    int key = hash(sofifa_id);
    vector<ht::Node> list = table[key];
    for (vector<Node>::iterator it = list.begin(); it != list.end(); it ++){
        if (it->sofifa_id == sofifa_id){
            return *it;
        }
    }
    return ht::Node();
}

void ht::HashTable::remove(int sofifa_id){
    int key = hash(sofifa_id);
    vector<Node> list = table[key];
    for (vector<Node>::iterator it = list.begin(); it != list.end(); it ++){
        if (it->sofifa_id == sofifa_id){
            list.erase(it);
        }
    }
}

void ht::HashTable::insert(int sofifa_id, Data *data){
    int key = hash(sofifa_id);
    table[key].push_back(Node(key, sofifa_id, data));
}

int ht::HashTable::hash(int sofifa_id){
    return sofifa_id % this->size;
}

void ht::HashTable::read_csv(std::ifstream &input){
    std::vector<std::string> names;
    aria::csv::CsvParser parser(input);
    bool first_line = true;
    for (auto& row : parser) {
        int i = 0;
        std::string name;
        std::string player_position;
        int sofifaId;
        
        if (!first_line){
            for (auto& field : row) {
                switch (i) {
                case 0:
                    sofifaId = atoi(field.c_str());
                    break;
                case 1:
                    name = field;
                    break;
                case 2: 
                    player_position = field;
                    break;
                }
                
                i ++;
            }
            insert(sofifaId, new ht::Data(name, player_position));
        }
        else
            first_line = false;
    } 
}

void ht::HashTable::read_rating(std::ifstream &input){
    std::vector<std::string> names;
    aria::csv::CsvParser parser(input);
    bool first_line = true;
    for (auto& row : parser) {
        int i = 0;
        int userId;
        float rating;
        int sofifaId;        
        
        if (!first_line){

            for (auto& field : row) {
                switch (i) {
                case 0:
                    userId = atoi(field.c_str());
                    break;
                case 1:
                    sofifaId = atoi(field.c_str());
                    break;
                case 2: 
                    rating = atof(field.c_str());
                    break;
                }
                i ++;
            }
            int key = hash(sofifaId);
            
            std::vector<ht::Node>::iterator it;
            std::vector<ht::Node> list = table[key];
            for (it = list.begin(); it != list.end() && it->sofifa_id != sofifaId; it ++){}
            if(it != list.end()){
                it->data->append_rating(rating);
                usrTable->appendRating(userId, sofifaId, rating);
            }
        }
        else
            first_line = false;
    } 
}

ht::HashTable::HashTable(int size){
    this->size = size;
    this->usrTable = new usr::UserTable(this->size * 5);
    for (int i = 0; i < this->size; i ++){
        vector<Node> v;
        v.clear();
        table.push_back(v);
    }
}


void ht::HashTable::operator>>(ostream &file) {
    for (vector<vector<Node>>::iterator it1 = table.begin(); it1 !=  table.end(); it1 ++){
        vector<Node> list = *it1;
        if (list.size() != 0){
            for (vector<Node>::iterator it2 = list.begin(); it2 != list.end(); it2 ++){
                file << 
                    std::setw(10) << std::setfill('.') << right << it2->key << '|' << 
                    std::setw(10) << std::setfill('.') << right << it2->sofifa_id << '\t';
                    *(it2->data) >> file;
                    file << endl;
            }
        }
    }
}

void ht::HashTable::displayUserRatings(int userId, std::ostream& file){
    usr::Node node = usrTable->search(userId);
    file << std::setw(40) << std::left << "Player name" << "rating" << std::endl;
    for (std::vector<usr::Rating*>::iterator it = node.rating->begin(); it != node.rating->end(); it ++){
        file << std::setw(40) << std::left << this->search((*it)->sofifaId).data->name << (*it)->rating << std::endl;
    }
}

usr::Rating::Rating(){}

usr::Rating::Rating(int sofifaId, float rating){
    this->sofifaId = sofifaId;
    this->rating = rating;  
}

usr::Node::Node(){
    this->rating = new std::vector<Rating*>;
}

usr::Node::Node(int userId){
    this->userId = userId;
    this->rating = new std::vector<Rating*>;
}

void usr::Node::appendRating(int sofifaId, float rating){
    this->rating->push_back(new Rating(sofifaId, rating));
}

int usr::UserTable::hash(int userId){
    return userId % this->size;
}

usr::UserTable::UserTable(){}

usr::UserTable::UserTable(int size){
    this->size = size;
    for (int i = 0; i < this->size; i ++){
        std::vector<Node> v;
        table.push_back(v);
    }
}

void usr::UserTable::insert(int userId){
    int key = hash(userId);
    table[key].push_back(Node(userId));
}

void usr::UserTable::remove(int userId){
    int key = hash(userId);
    vector<Node> list = table[key];
    for (vector<Node>::iterator it = list.begin(); it != list.end(); it ++){
        if (it->userId == userId){
            list.erase(it);
        }
    }
}

usr::Node usr::UserTable::search(int userId){
    int key = hash(userId);
    vector<Node> list = table[key];
    for (vector<Node>::iterator it = list.begin(); it != list.end(); it ++){
        if (it->userId == userId){
            return *it;
        }
    }
    return Node();
}

void usr::UserTable::appendRating(int userId, int sofifaId, float rating){
    int key = hash(userId);
    std::vector<Node> list = table[key];
    vector<Node>::iterator it;
    for (it = list.begin(); it != list.end(); it ++){
        if (it->userId == userId){
            it->appendRating(sofifaId, rating);
            return;
        }
    }
    insert(userId);
    this->appendRating(userId, sofifaId, rating);
}

void usr::UserTable::operator>>(std::ostream &file){
    for (vector<vector<Node>>::iterator it1 = table.begin(); it1 !=  table.end(); it1 ++){
        vector<Node> list = *it1;
        if (list.size() != 0){
            for (vector<Node>::iterator it2 = list.begin(); it2 != list.end(); it2 ++){
                *it2 >> file;
            }
        }
    }
}

void usr::Node::operator>>(std::ostream &file){
    file << this->userId << std::endl;
    int cont = 0;
    for (auto it = this->rating->begin(); it != this->rating->end(); it ++){
        *(*it) >> file;
        cont ++;
        if (cont >= 20)
            break;
    }
}

void usr::Rating::operator>>(std::ostream &file){
    file << std::setw(15) << std::left << this->sofifaId << this->rating << std::endl;
}
