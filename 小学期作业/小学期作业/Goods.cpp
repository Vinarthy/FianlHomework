#include "Goods.h"
#include <iostream>
using namespace std;

Goods::Goods(string id, string name, double price)
    : ID(id), Name(name),Price(price) {
}

void Goods::display() const {
    cout << "商品编号: " << ID << ", 商品名: " << Name
        <<" 价格: " << Price << "元" << endl;
}