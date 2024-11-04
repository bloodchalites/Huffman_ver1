#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <map>

using namespace std;

class Node {
public:
    int a; // Число
    char c; // символ
    Node *left, *right; // указатели на стороны

    Node() : left(nullptr), right(nullptr), a(0), c('\0') {}; // инициализация по умолчанию

    Node(Node *L, Node *R) // конструктор
    {
        left = L;
        right = R;
        a = L->a + R->a; // число становиться суммой двух переменых
        c = '\0'; // символ отсутствует в промежуточных узлах
    }
};

struct MyCompare {
    bool operator() (Node* l, Node* r) const {
        return l->a < r->a;
    }
};

vector<bool> code;
map<char, vector<bool>> table;

// Чтение бинарного файла
void readBin(Node *root) {
    ifstream F("output.bin");
    Node *p = root;
    int count = 0;
    char byte;
    F >> byte;
    while(!F.eof())
    {   bool b = byte & (1 << (7-count) );
        if(b) p=p->right; else p=p->left;
        if(p->left==NULL && p->right==NULL) { cout << p->c; p=root; }
        count++;
        if (count==8) { count=0; F>>byte; }
    }
    F.close();
}

// Рекурсивная функция
void BuildTable(Node *root) {
    if (root->left) {
        code.push_back(0); // в вектор ставим 0
        BuildTable(root->left);
        code.pop_back(); // удаляем последний элемент после возврата
    }

    if (root->right) {
        code.push_back(1); // в вектор ставим 1
        BuildTable(root->right);
        code.pop_back(); // удаляем последний элемент после возврата
    }

    // Если узел — лист, сохраняем код для него
    if (!root->left && !root->right) {
        table[root->c] = code;
    }
}

void Huffman() {
    // Для работы с файлом
    ifstream f("1.txt");
    if (!f.is_open()) {
        wcout << L"Error." << endl;
        return;
        }

    map<char, int> m;

    while(!f.eof()){
        char c = f.get();
        m[c]++; }

    /* для работы со строками
    map<char, int> m;
    string s = "aaabbdddgaabeerdjjo";

     // проходимся по строке
    for (char c : s) {
        m[c]++;
    }


    cout << s << endl;
    // Выводим символы и их количество
    for (const auto& pair : m) {
        cout << pair.first << ": " << pair.second << endl;
    }
    */

    // Лист указателей Node
    list<Node*> t;

    for (auto it = m.begin(); it != m.end(); ++it) {
        Node *p = new Node;
        p->c = it->first;
        p->a = it->second;
        t.push_back(p);
    }

    while (t.size() > 1) {
        t.sort(MyCompare()); // сортируем лист
        Node *SonL = t.front(); // берем первый элемент в списке
        t.pop_front();
        Node *SonR = t.front();
        t.pop_front();
        Node *parent = new Node(SonL, SonR); // создаем родителя
        t.push_back(parent); // родительская переменная идет в лист
    }

    Node *root = t.front(); // последний элемент — корневой

    BuildTable(root);

    // Для работы с файлами
    f.clear(); f.seekg(0); // сбрасывание. перемещение на 0

    ofstream g("output.bin");
    int count=0;
    char buf=0; // буфер 000000000

    while(!f.eof()){
        char c; // считали букву
        c = f.get();
        vector<bool> x = table[c]; // смотрим ее код
        for (int n=0; n < x.size(); n++)
        {
            buf = buf | x[n] << (7-count);
            count++;
            // как только достигли 8 записываем в файл g
            if(count==8) { count=0; g << buf; buf = 0; }
        }

    }
    while(!f.eof())
    {
        char c;
        f >> c;
        vector<bool> x = table[c];
        for (int n=0; n<x.size(); n++) { cout<<x[n]; }
    }


    g.close();
    f.close();

    readBin(root);

    /* Вывод бинарного вида
    for (char c : s) {
        vector<bool> x = table[c];
        for (bool bit : x) {
            cout << bit;
        }
    }
    cout << endl;
    */
}

int main() {

    Huffman();
    return 0;
}
