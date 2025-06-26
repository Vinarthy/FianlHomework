#include "GoodsManage.h"
#include <iostream>
#include <Windows.h>
#include<json.hpp>
using json = nlohmann::json;//使用json
using namespace std;

bool GoodsManager::loadGoods(const string& filename) {//这个函数把Json里面的书上传到类里面w
    ifstream in(filename);
    if (!in.is_open()) return false;
    goods.clear();
    json j;
    try {
        in >> j;
        for (const auto& item : j) {
            Goods b(
                Utf8ToGbk(item["ID"].get<string>()),       // 转换ID
                Utf8ToGbk(item["Name"].get<string>()),     // 转换商品名
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
    cout << "1.购买商品" << endl;//结账放在买书功能里面吧，还是要有购物车类的
    cout << "2.展示您的个人信息" << endl;
    cout << "3.商品排序" << endl;//按价格从小往打排序
    cout << "4.修改信息" << endl;//修改ID,密码，生源
    cout << "5.管理员模式" << endl;
    cout << "6.购买不同等级的会员" << endl;
    cout << "9.退出程序" << endl;
}

Goods* GoodsManager::findBookByID(const string& id) {//找Id的（这个还是纯数字找id的好）转编码累死了（不过只要我id都加数字的那么是不是不用转码了）
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
void GoodsManager::SortBooks() {//字符串比较函数
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

    out << j.dump(4); // 使用4空格缩进美化输出
    return true;
}
