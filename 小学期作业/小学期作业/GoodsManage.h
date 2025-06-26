#pragma once

#include <vector>
#include <fstream>
#include <json.hpp>
#include<Windows.h>
#include<list>
#include"Goods.h"
#include<json.hpp>
using json = nlohmann::json;

class GoodsManager {
public:
    list<Goods> goods;//动态数组，用来存放每个商品的信息

    bool loadGoods(const string& filename);//读取文件的代码
    Goods* findBookByID(const string& id);//找ID
    void displayAllBooks() const;
    string Utf8ToGbk(const string& utf8);
    void DisplayMessage();
    void SortBooks();
    bool saveBooks(const string& filename);
    string GbkToUtf8(const string& gbk) {
        if (gbk.empty()) return "";

        int wideCharLen = MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), -1, nullptr, 0);
        if (wideCharLen == 0) return "";

        wstring wideStr(wideCharLen, 0);
        MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), -1, &wideStr[0], wideCharLen);

        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (utf8Len == 0) return "";

        string utf8Str(utf8Len, 0);
        WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Len, nullptr, nullptr);

        // 移除末尾的null字符
        if (!utf8Str.empty() && utf8Str.back() == '\0') {
            utf8Str.pop_back();
        }
        return utf8Str;
    }
};
