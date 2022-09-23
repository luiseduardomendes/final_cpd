#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <string>
#include <ctime>
//#include <windows.h>
#include "ternary_search_tree.hpp"
#include "hash_table.hpp"
#include "tag_table.hpp"
#include "position_table.hpp"

using namespace std;

//LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void coutVector(std::vector<std::string> vec);
/*
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow){

    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"MyWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc))
        return -1;

    CreateWindowW(L"MyWindowClass", L"My Window", WS_OVERLAPPED | WS_VISIBLE, 100, 100, 640, 480, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while(GetMessage(&msg, NULL, NULL, NULL)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
    switch (msg){
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        DefWindowProcW(hWnd, msg, wp, lp);
        break;
    }
}
*/

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
        std::smatch matches;
        std::regex player_reg ("player ([^.]+)");
        std::regex user_reg ("user ([^.]+)");
        std::regex tags_reg ("tags ([^.]+)");
        std::regex top10_reg ("top([0-9]+) '([^.]+)'");
        std::regex help_reg ("help([^.]?)");
        if (std::regex_search(user_input, matches, player_reg)){
            std::vector<int> ids;
            std::vector<std::string> temp = tree.search_by_radix(matches.str(1), &ids);

            if (temp.size() > 1){
                std::cout << std::endl << "First 20 search results:" << std::endl;
                std::cout << "\t" << std::setw(50) << std::right << "Nome" << '|' << setw(25) << std::right << "Posicoes" << '|' << std::setw(10) << std::right << "pontuacao" << '|' << std::endl;
                int a = 1;
                for (auto id : ids){
                    std::cout << a++ << ".\t";
                    *(table.search(id).data) >> std::cout;
                    std::cout << std::endl;
                }
            }
            else if (temp.size() == 1){
                int id = ids[0];
                ht::Node node = table.search(id);
                std::cout << std::setw(50) << std::right << "Nome" << '|' << setw(25) << std::right << "Posicoes" << '|' << std::setw(10) << std::right << "pontuacao" << '|' << std::endl;
                *(node.data) >> std::cout;
            }
            else{
                std::cout << "There's no result to your search!" << std::endl;
            }
        }
        else if (std::regex_search(user_input, matches, user_reg)){
            table.displayUserRatings(atoi(matches.str(1).c_str()), std::cout);
        }
        else if (std::regex_search(user_input, matches, tags_reg)){
            std::vector<std::vector<int>> search_result;
            std::string str =  matches.str(1);
	        std::regex tags_split ("\'([^\'\"]+)\'");
            std::sregex_iterator it (str.begin(), str.end(), tags_split);
            std::sregex_iterator lastMatch;
            while (it != lastMatch){
                tagTable::Node* node;
                if ((node = tag_table[it->str(1)]) != NULL)
                    search_result.push_back(node->data.ids);
                else
                    std::cout << it->str(1) << " tag no found" << std::endl;
                it ++;
            }
            tag_table.displayIntersection(search_result, std::cout, &table);

        }
        else if (std::regex_search(user_input, matches, top10_reg)){
            int p = matches.str().find(' ');
            int n = atoi(matches.str().substr(3, p).c_str());
            string pos =  matches.str().substr(p+2);
            pos.pop_back();

            vector<int> v = table.searchTop(n, pos);
            std::cout << "\t" << std::setw(50) << std::right << "Nome" << '|' << setw(25) << std::right << "Posicoes" << '|' << std::setw(10) << std::right << "pontuacao" << '|' << std::endl;
            for (int id = 0; id < v.size(); id ++){
                cout << id + 1 << "\t";
                *(table.search(v[id]).data) >> std::cout;
                std::cout << endl;
            }
        }
        else if (std::regex_search(user_input, matches, help_reg)){
            std::cout << "player player_name" << std::endl;
            std::cout << "user user_id" << std::endl;
            std::cout << "tags 'tag1' 'tag2' 'tag3' ... 'tagn'" << std::endl;
            std::cout << "top10 'pos'" << std::endl;
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



