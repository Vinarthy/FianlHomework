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
    string Name;            // �û���
    string Password;        // ����
    string Address;         // ��Դ
    string Major;
    int Level = 1;
    double pay = 0.0;       // ����Ʒ�ķ���
    double jianhangCredits = 1000.0; // ���е����
    double nonghangCredits = 1000.0;//ũ�е����
    list<string> Spcar_Book_ID;    // ���ﳵ��ƷID
    list<int> Spcar_Book_Num;      // ���ﳵ��Ʒ����

    // ���캯��
    User(const string& name, const string& password, const string& address, const string& major);
    User() {}//Ĭ�Ϲ���
    // ת��ΪJSON����
    json toJson() const;
    void showInformation() const;
    static User fromJson(const string& name, const json& j);
    static User fromJson2(const string& name, const json& j);//utf-8תutf-8�����������ݼ���İ����÷�
    virtual double calculateTotal(double totalPrice) {
        return totalPrice;  // ���ۿ۵Ŀ�Ǯ����
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
        ifstream inFile(filename);//������Ż�򿪣�����ر�
        if (!inFile.is_open()) {
            j = json::object(); // �½��ն���
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
        cout << "�������ۿ��ʣ��������� 0.7 ��ʾ 7 �ۣ���";
        cin >> discount_rate;
        if (discount_rate < 0.1 || discount_rate > 1.0) {
            cout << "��Ч�ۿ��ʣ�������Ϊ 1�������ۣ�\n";
            discount_rate = 1.0;
        }
    }

    double calculateTotal(double totalPrice) override {
        return totalPrice * discount_rate;
    }
};