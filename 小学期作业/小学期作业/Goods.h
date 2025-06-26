#pragma once

#include <string>
using namespace std;

class Goods {
public:
    string ID;
    string Name;
    double Price;
    int RemainingQuantity;//默认是10，会随着购买减少

    Goods() = default;
    Goods(string id, string name,double price);//构造
    void display() const;
};



