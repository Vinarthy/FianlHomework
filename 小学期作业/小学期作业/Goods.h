#pragma once

#include <string>
using namespace std;

class Goods {
public:
    string ID;
    string Name;
    double Price;
    int RemainingQuantity;//Ĭ����10�������Ź������

    Goods() = default;
    Goods(string id, string name,double price);//����
    void display() const;
};



