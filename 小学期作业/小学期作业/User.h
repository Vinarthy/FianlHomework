#pragma once
#include <string>
#include <vector>
#include <json.hpp>
#include<iostream>
#include<list>
#include <fstream>
#include <algorithm>

using json = nlohmann::json;
using namespace std;

class User {
public:
    string Name;            // 用户名
    string Password;        // 密码
    string Address;         // 生源
    string Major;
    int Level = 1;
    double pay = 0.0;       // 买商品的费用
    double jianhangCredits = 1000.0; // 建行的余额
    double nonghangCredits = 1000.0;//农行的余额
    list<string> Spcar_Book_ID;    // 购物车商品ID
    list<int> Spcar_Book_Num;      // 购物车商品数量

    // 构造函数
    User(const string& name, const string& password, const string& address, const string& major);
    User() {}//默认构造
    // 转换为JSON对象
    json toJson() const;
    void showInformation() const;
    static User fromJson(const string& name, const json& j);
    static User fromJson2(const string& name, const json& j);//utf-8转utf-8，用来搞数据计算的唉，好烦
    virtual double calculateTotal(double totalPrice) {
        return totalPrice;  // 无折扣的扣钱功能
    }
    virtual bool tryPayjh(double totalPrice) {
        double actualPay = calculateTotal(totalPrice);
        if (jianhangCredits >= actualPay) {
            jianhangCredits -= actualPay;
            pay += actualPay;
            return true;
        }
        return false;
    }
    virtual bool tryPaynh(double totalPrice) {
        double actualPay = calculateTotal(totalPrice);
        if (nonghangCredits >= actualPay) {
            nonghangCredits -= actualPay;
            pay += actualPay;
            return true;
        }
        return false;
    }
    void ChangeSomeInformation(string& Username,string &filename);
    bool loadUsers(json& j, string& filename) {
        ifstream inFile(filename);//作用域才会打开，否则关闭
        if (!inFile.is_open()) {
            j = json::object(); // 新建空对象
            return true;
        }

        try {
            inFile >> j;
            return true;
        }
        catch (...) {
            return false;
        }
    }
};
class Member : public User {
public:
    Member(const User& base) : User(base) {}

    double calculateTotal(double totalPrice) override {
        if (Level == 2) return totalPrice * 0.9;
        if (Level == 3) return totalPrice * 0.85;
        if (Level == 4) return totalPrice * 0.8;
        return totalPrice;
    }
};

class honoured_guest : public User {
public:
    double discount_rate = 1.0;

    honoured_guest(const User& base) : User(base) {}

    void Set_discount_rate() {
        cout << "请输入折扣率（例如输入 0.7 表示 7 折）：";
        cin >> discount_rate;
        if (discount_rate < 0.1 || discount_rate > 1.0) {
            cout << "无效折扣率，已重置为 1（不打折）\n";
            discount_rate = 1.0;
        }
    }

    double calculateTotal(double totalPrice) override {
        return totalPrice * discount_rate;
    }
};