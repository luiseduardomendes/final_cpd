#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include "ternary_search_tree.hpp"
#include "hash_table.hpp"
#include "tag_table.hpp"
#include "position_table.hpp"

using namespace std;
bool startsWith(std::string str, std::string prefix);
void coutVector(std::vector<std::string> vec);
std::vector<std::string> split_str(std::string str, char sep);
std::vector<std::string> split_tag(std::string str);

int main(int argv, char **argc){
    tst::Tree tree;
    clock_t start, end;

    start = clock();

    std::ifstream input("INF01124_FIFA21_clean/players.csv");
    tree.read_csv(input);
    input.close();

    input.open("INF01124_FIFA21_clean/players.csv");
    ht::HashTable table(tree.size);

    table.read_csv(input);
    std::cout << "players.csv read successfull" << std::endl;
    input.close();

    input.open("INF01124_FIFA21_clean/rating.csv");
    table.read_rating(input);
    std::cout << "rating.csv read successfull" << std::endl;

    std::ifstream input2("INF01124_FIFA21_clean/tags.csv");
    tagTable::Table tag_table(table.size);
    tag_table.read_csv(input2);
    std::cout << "tags.csv read successfull" << std::endl;

    end = clock();

    std::cout << "loading time: " << double(end-start) / CLOCKS_PER_SEC << std::endl;

    getc(stdin);
    
    std::string user_input;
    while (user_input != "exit"){
        system("clear");
        
        auto start = user_input.find(' ');
        std::string cmd = user_input;
        std::string arg = "";
        std::vector<std::string> data;

        if (start != std::string::npos){
            cmd = user_input.substr(0, start);
            arg = user_input.substr(start+1);
            data = split_tag(arg);
        }

        if (cmd == "player"){
            std::vector<int> ids = tree.search_by_radix(arg);

            if (ids.size() > 1){
                std::cout << std::endl << "First 20 search results:" << std::endl;
                std::cout << "\t" << std::setw(50) << std::right << "Nome" << '|' << setw(25) << std::right << "Posicoes" << '|' << std::setw(10) << std::right << "pontuacao" << '|' << std::endl;
                int a = 1;
                for (auto id : ids){
                    std::cout << a++ << ".\t";
                    *(table.search(id).data) >> std::cout;
                    std::cout << std::endl;
                    if (a >= 20)
                        break;
                }
            }
            else if (ids.size() == 1){
                int id = ids[0];
                ht::Node node = table.search(id);
                std::cout << std::setw(50) << std::right << "Nome" << '|' << setw(25) << std::right << "Posicoes" << '|' << std::setw(10) << std::right << "pontuacao" << '|' << std::endl;
                *(node.data) >> std::cout;
            }
            else{
                std::cout << "There's no result to your search!" << std::endl;
            }
        }
        
        else if (cmd == "user"){
            table.displayUserRatings(atoi(arg.c_str()), std::cout);
        }
        
        else if (cmd == "tags"){
            std::vector<std::vector<int>> search_result;
            std::vector<std::string>::iterator it = data.begin();
            while (it != data.end()){
                tagTable::Node* node;
                if ((node = tag_table[*it]) != NULL)
                    search_result.push_back(node->data.ids);
                else
                    std::cout << *it << " tag no found" << std::endl;
                it ++;
            }
            if (search_result.size() >= 1)
                tag_table.displayIntersection(search_result, std::cout, &table);

        }
        else if (startsWith(cmd, "top")){
            std::vector<std::string>::iterator it = data.begin();
            int n = atoi(cmd.substr(3).c_str());

            vector<int> v = table.searchTop(n, *it);

            std::cout << "\t" << std::setw(50) << std::right << "Nome" << '|' << setw(25) << std::right << "Posicoes" << '|' << std::setw(10) << std::right << "pontuacao" << '|' << std::endl;
            for (int id = 0; id < v.size(); id ++){
                cout << id + 1 << "\t";
                *(table.search(v[id]).data) >> std::cout;
                std::cout << endl;
            }
        }

        else if (cmd == "roleTag"){
            std::vector<std::vector<int>> search_result;
            std::vector<std::string>::iterator it = data.begin();
            
            search_result.push_back(table.posTable->search(*it).data->sofifaIds);
            it ++;
            
            while (it != data.end()){
                tagTable::Node* node;
                if ((node = tag_table[*it]) != NULL)
                    search_result.push_back(node->data.ids);
                else
                    std::cout << *it << " tag no found" << std::endl;
                it ++;
            }
            if (search_result.size() >= 1)
                tag_table.displayIntersection(search_result, std::cout, &table);

        }

        else if (cmd == "help"){
            std::cout << "player player_name" << std::endl;
            std::cout << "user user_id" << std::endl;
            std::cout << "tags 'tag1' 'tag2' 'tag3' ... 'tagn'" << std::endl;
            std::cout << "top10 'pos'" << std::endl;
            std::cout << "roleTag 'pos' 'tag1' 'tag2' 'tag3' ... 'tagn'" << std::endl;
            std::cout << "exit" << std::endl;
        }
        std::cout << std::endl << std::endl << "[pressione ENTER para continuar]" << std::endl;
        std::cin.get();
        system("clear");
        getline(std::cin, user_input);
    }


    return 0;
}

void coutVector(std::vector<std::string> vec){
    int i = 1;
    cout << "\t" << std::setw(50) << std::right << "Nome" << '|' << setw(25) << std::right << "Posicoes" << '|' << std::setw(10) << std::right << "pontuacao" << '|' << std::endl;
    for (vector<string>::iterator it = vec.begin(); it != vec.end(); it ++){
        cout << i << ".\t";
        cout << *it << endl;
        i ++;
    }
}

std::vector<std::string> split_str(std::string str, char sep){
    size_t start = 0, end = 0;
    std::vector<std::string> tokens;

    while ((end = str.find(sep, start)) != string::npos){
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

std::vector<std::string> split_tag(std::string str){
    size_t start = 0, end = 0;
    std::vector<std::string> tokens;
    std::string temp;

    while ((end = str.find('\'', start)) != string::npos){
        temp = str.substr(start, end - start);
        if (temp != " " && temp != "")
            tokens.push_back(temp);
        start = end + 1;
    }
    temp = str.substr(start);
    if (temp != " " && temp != "")
        tokens.push_back(temp);
    return tokens;
}

bool startsWith(std::string str, std::string prefix){
    int i = 0;
    if (str.size() < prefix.size()) return false;
    while(str[i] == prefix[i] && i < prefix.size()) i ++;
    if (i == prefix.size()) return true; else return false;
}
