#pragma once
#include <iostream>
#include <iomanip>
#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <stdint.h>
#include "position_table.hpp"

using namespace std;

namespace usr{
    class Rating{
    private:
    public:
        int sofifaId;
        float rating;
        Rating(int sofifaId, float rating);
        Rating();
        void operator>>(std::ostream &file);
    };

    class Node{
    public:
        int userId;
        std::vector<Rating*> *rating;
        Node(int userId);
        Node();
        void appendRating(int sofifaId, float rating);
        void operator>>(std::ostream &file);
    };

    class UserTable{
    private:
        vector<vector<Node>> table;
        int hash(int userId);
        int size;
    public:
        UserTable(int size);
        UserTable();
        void insert(int userId);
        void remove(int userId);
        Node search(int userId);
        void appendRating(int userId, int sofifaId, float rating);
        void operator>>(std::ostream &file);
    };
}



namespace ht{
    class Data{
    private:
    public:
        int data;
        std::vector<std::string> player_position;
        std::string name;
        float rating;
        int count;
        Data(string name, string player_position);
        Data();
        void append_rating(float rating);
        void append_positions(string positions);
        void operator>> (std::ostream &output);
    };

    class Node{
    public:
        int key;
        int sofifa_id;
        Data *data;
        Node();
        Node(int key, int sofifa_id, Data *data);
    };

    class HashTable{
    private:
        vector<vector<Node>> table;
        int hash(int sofifa_id);
        usr::UserTable *usrTable;
    public:
        pos::PositionTable *posTable;
        int size;
        HashTable(int size);
        void read_csv(std::ifstream &input);
        void read_rating(std::ifstream &input);
        void insert(int sofifa_id, Data *data);
        void remove(int sofifa_id);
        Node search(int sofifa_id);
        void displayUserRatings(int userId, std::ostream& file);
        void operator>>(std::ostream &file);
        int partition(vector<int> &v, int begin, int end);
        void sort(vector<int> &v);
        void quicksort(vector<int> &v, int begin, int end);
        void selectionSort(vector<int> &v, int n, int min_rating);
        vector<int> searchTop(int N, std::string pos);
    };
}
