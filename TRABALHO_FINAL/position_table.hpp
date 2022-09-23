#pragma once
#include <vector>
#include <string>
#include <iostream>

namespace pos{
    class Data{
    public:
        std::vector<int> sofifaIds;
        Data();
        Data(int sofifaId);
        void append(int sofifaId);
        void operator>>(std::ostream &output);
    };
    class Node{
    public:
        Data* data;
        std::string pos;
        Node();
        Node(std::string position, int sofifaId);
        void operator>>(std::ostream &output);
    };
    class PositionTable {
    private:
    public:
        int size;
        int hash(std::string position);
        std::vector<std::vector<Node>*> table;
        PositionTable(int size);
        PositionTable();
        void insert(std::string position, int sofifaId);
        void remove(std::string position);
        Node search(std::string position);
        void operator>>(std::ostream &file);
    };

}

std::vector<std::string> split(std::string str);