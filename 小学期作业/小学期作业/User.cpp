#include "User.h"
#include<iostream>
#include"GoodsManage.h"
#include<list>
#include<json.hpp>
#include<bits/stdc++.h>
using json = nlohmann::json;//使用json
User::User(const string& name, const string& password, const string& address,const string& major)
    : Name(name), Password(password), Address(address),Major(major) {
}


json User::toJson() const {//buyer转json格式,我要在这个基础上做一个加密
    return {
        {"Name", Name},
        {"Password", Password},
        {"Address", Address},
        {"Major", Major},
        {"pay", pay},
        {"jianhangCredits", jianhangCredits},
        {"nonghangCredits", nonghangCredits},
        {"Level", Level},
        {"Spcar_Book_ID", Spcar_Book_ID},
        {"Spcar_Book_Num", Spcar_Book_Num}
    };
}

GoodsManager x;
User User::fromJson(const string& name, const json& j) {
    User b;
    b.Name = x.Utf8ToGbk(name);
    b.Password = x.Utf8ToGbk(j.at("Password").get<string>());//utf-gbk要的
    b.Address = x.Utf8ToGbk(j.at("Address").get<string>());
    b.Major = x.Utf8ToGbk(j.at("Major").get<string>());
    b.pay = j.at("pay").get<double>();
    b.Level = j.at("Level").get<int>();
    b.jianhangCredits = j.at("jianhangCredits").get<double>();
    b.nonghangCredits = j.at("nonghangCredits").get<double>();
    b.Spcar_Book_ID = j.at("Spcar_Book_ID").get<list<string>>();
    b.Spcar_Book_Num = j.at("Spcar_Book_Num").get<list<int>>();
    return b;
}
User User::fromJson2(const string& name, const json& j) {
    User b;
    b.Name = name;
    b.Password = j.at("Password").get<string>();//utf-gbk要的
    b.Address = j.at("Address").get<string>();
    b.Major = j.at("Major").get<string>();
    b.pay = j.at("pay").get<double>();
    b.Level = j.at("Level").get<int>();
    b.jianhangCredits = j.at("jianhangCredits").get<double>();
    b.nonghangCredits = j.at("nonghangCredits").get<double>();
    b.Spcar_Book_ID = j.at("Spcar_Book_ID").get<list<string>>();
    b.Spcar_Book_Num = j.at("Spcar_Book_Num").get<list<int>>();
    return b;
}
void User::showInformation() const {
    string levelStr;
    if (Level == 1) levelStr = "普通用户";
    else if (Level >= 2 && Level <= 4) levelStr = "会员";
    else if (Level == 5) levelStr = "mhy贵宾";
    else levelStr = "未知等级";
    cout << "========= 用户信息 =========\n";
    cout << "用户名：" << Name << endl;
    cout << "生源：" << Address << endl;
    cout << "专业：" << Major << endl;
    cout << "等级：" << Level << endl;
    cout << "建行余额：" << jianhangCredits << " 元\n";
    cout << "农行余额：" << nonghangCredits << "元\n";
}


void User::ChangeSomeInformation(string& Username, string& filename) {
    cout << "请选择自己想修改的信息" << endl;
    json users;
    ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> users;
        }
        catch (...) {
            cout << "读取用户数据失败！" << endl;
            return;
        }
    }
    else {
        cout << "无法打开用户数据文件！" << endl;
        return;
    }

    while (true) {
        // 显示菜单
        cout << "1.修改名字" << endl;
        cout << "2.修改密码" << endl;
        cout << "3.修改生源地" << endl;
        cout << "4.修改专业" << endl;
        cout << "5.按q退出" << endl;

        string t;
        cin >> t;

        if (!users.contains(Username)) {
            cout << "用户不存在！" << endl;
            return;
        }

        // 获取当前用户数据
        User b = User::fromJson2(Username, users[Username]);
        GoodsManager miao;

        if (t == "1") {
            cout << "请输入新名字：";
            string x;
            cin >> x;
            // 移除旧用户，添加新用户
            users.erase(Username);
            Username = miao.GbkToUtf8(x); // 更新用户名
            b.Name = Username;
            users[Username] = b.toJson();
        }
        else if (t == "2") {
            cout << "请输入新密码：";
            string x;
            cin >> x;
            b.Password = miao.GbkToUtf8(x);
            users[Username] = b.toJson();
        }
        else if (t == "3") {
            cout << "请输入新生源地：";
            string x;
            cin >> x;
            b.Address = miao.GbkToUtf8(x);
            users[Username] = b.toJson();
        }
        else if (t == "4") {
            cout << "请输入新专业：";
            string x;
            cin >> x;
            b.Major = miao.GbkToUtf8(x);
            users[Username] = b.toJson();
        }
        else if (t == "q") {
            break;
        }
        else {
            cout << "无效输入请重试！" << endl;
            continue;
        }

        // 保存整个用户列表
        ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << setw(4) << users;
            cout << "修改成功！" << endl;
        }
        else {
            cout << "保存失败！" << endl;
        }
    }
}