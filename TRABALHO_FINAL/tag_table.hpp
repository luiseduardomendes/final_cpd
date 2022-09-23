#pragma once
#include<vector> 
#include<iostream>
#include<fstream>
#include <string>
#include "hash_table.hpp"

namespace tagTable{
    class Data{
        public:
            std::vector<int> ids;
            Data(int sofifaId);
            Data();
            void append(int sofifaId);
            void operator>>(std::ostream &file);
    };

    class Node{
        public:
            Data data;
            std::string tag;
            Node();
            Node(std::string tag, Data data);
    };

    class Table{
        private:
            int hash(std::string);
            std::vector<std::vector<Node*>> table;
            int size;
        public:
            Table(int size);
            Node* operator[](std::string);
            void remove(std::string);
            void insert(std::string, Data data);
            std::vector<int> intersection(std::vector<std::vector<int>>);
            void read_csv(std::ifstream &file);
            void operator>>(std::ostream &file);
            void displayIntersection(std::vector<std::vector<int>>, std::ostream &file, ht::HashTable *hashT);
    };
}