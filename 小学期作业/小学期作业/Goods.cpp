#include "Goods.h"
#include <iostream>
using namespace std;

Goods::Goods(string id, string name, double price)
    : ID(id), Name(name),Price(price) {
}

void Goods::display() const {
    cout << "��Ʒ���: " << ID << ", ��Ʒ��: " << Name
        <<" �۸�: " << Price << "Ԫ" << endl;
}