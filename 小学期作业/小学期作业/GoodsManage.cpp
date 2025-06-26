#include "GoodsManage.h"
#include <iostream>
#include <Windows.h>
#include<json.hpp>
using json = nlohmann::json;//ʹ��json
using namespace std;

bool GoodsManager::loadGoods(const string& filename) {//���������Json��������ϴ���������w
    ifstream in(filename);
    if (!in.is_open()) return false;
    goods.clear();
    json j;
    try {
        in >> j;
        for (const auto& item : j) {
            Goods b(
                Utf8ToGbk(item["ID"].get<string>()),       // ת��ID
                Utf8ToGbk(item["Name"].get<string>()),     // ת����Ʒ��
                item["Price"].get<double>()
            );
            goods.push_back(b);
        }
        return true;
    }
    catch (...) {
        return false;
    }
}
void GoodsManager::DisplayMessage() {
    cout << "1.������Ʒ" << endl;//���˷������鹦������ɣ�����Ҫ�й��ﳵ���
    cout << "2.չʾ���ĸ�����Ϣ" << endl;
    cout << "3.��Ʒ����" << endl;//���۸��С��������
    cout << "4.�޸���Ϣ" << endl;//�޸�ID,���룬��Դ
    cout << "5.����Աģʽ" << endl;
    cout << "6.����ͬ�ȼ��Ļ�Ա" << endl;
    cout << "9.�˳�����" << endl;
}

Goods* GoodsManager::findBookByID(const string& id) {//��Id�ģ�������Ǵ�������id�ĺã�ת���������ˣ�����ֻҪ��id�������ֵ���ô�ǲ��ǲ���ת���ˣ�
    for (auto& b : goods) {
        if (b.ID == id) return &b;
    }
    return nullptr;
}

void GoodsManager::displayAllBooks() const {
    for (const auto& b : goods) {
        b.display();
    }
}
string GoodsManager::Utf8ToGbk(const string& utf8) {
    if (utf8.empty()) {
        return "";
    }
    int wideCharLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
    if (wideCharLen == 0) {
        return "";
    }

    std::wstring wideStr(wideCharLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wideStr[0], wideCharLen);

    int gbkLen = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (gbkLen == 0) {
        return "";
    }

    std::string gbkStr(gbkLen, 0);
    WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &gbkStr[0], gbkLen, nullptr, nullptr);

    if (!gbkStr.empty() && gbkStr[gbkStr.size() - 1] == '\0') {
        gbkStr.resize(gbkStr.size() - 1);
    }

    return gbkStr;
}
void GoodsManager::SortBooks() {//�ַ����ȽϺ���
    auto compareByID = [](const Goods& a, const Goods& b) {
        const std::string& id1 = a.ID;
        const std::string& id2 = b.ID;
        return id1 < id2; };
    goods.sort(compareByID);
}
bool GoodsManager::saveBooks(const string& filename) {
    json j;
    for (const auto& book : goods) {
        j.push_back({
            {"ID", GbkToUtf8(book.ID)},
            {"Name", GbkToUtf8(book.Name)},
            {"RemainingQuantity", book.RemainingQuantity},
            {"Price", book.Price}
            });
    }

    ofstream out(filename);
    if (!out.is_open()) return false;

    out << j.dump(4); // ʹ��4�ո������������
    return true;
}
