#include<bits/stdc++.h>
#include"admin.h"
#include<Windows.h>
#include"Goods.h"
#include"GoodsManage.h"
#include<json.hpp>
#include"User.h"
using json = nlohmann::json;//使用json
using namespace std;
//商品，四个元素：ID，名字，价格，剩余数量
void Admin::manageBooks() {
    while (true) {
        cout << "\n-- 商品管理 --\n";
        cout << "1. 添加商品\n";
        cout << "2. 删除商品\n";
        cout << "q. 返回上级菜单\n";
        string choice;
        cin >> choice;
        if (choice == "1") addBook();
        else if (choice == "2") deleteBook();
        else if (choice == "q") break;
        else cout << "无效选项，请重试。\n";
    }
}


void Admin::addBook() {
    GoodsManager bo;
    bo.loadGoods("Data/book.json"); // 加载现有商品
    Goods b;
    cout << "请输入商品号: ";
    cin >> b.ID;
    cout << "请输入商品名: ";
    cin.ignore();
    getline(cin, b.Name);
    cout << "请输入价格: ";
    cin >> b.Price;
    cout << "请输入商品数量: ";
    cin>>b.RemainingQuantity;
    bo.goods.push_back(b);
    bo.saveBooks("Data/book.json");
    cout << "添加成功！\n";
}

void Admin::deleteBook() {
    string id;
    cout << "请输入要删除的书号: ";
    cin >> id;
    GoodsManager bo;
    bo.loadGoods("Data/book.json"); // 加载现有商品

    // 查找并删除商品
    bool found = false;
    for (auto it = bo.goods.begin(); it != bo.goods.end(); ++it) {
        if (it->ID == id) {
            bo.goods.erase(it);
            found = true;
            break;
        }
    }

    if (found) {
        bo.saveBooks("Data/book.json");
        cout << "删除成功！\n";
    }
    else {
        cout << "未找到该书号。\n";
    }
}
void Admin::manageUsers() {
    GoodsManager wd;
    while (true) {
        cout << "\n-- 用户管理 --\n";
        cout << "1. 删除用户\n";
        cout << "2. 修改用户信息\n";
        cout << "q. 返回主菜单\n";
        string choice;
        cin >> choice;
        if (choice == "1") {
            cout << "请输入要删除的用户ID: ";
            string userId;
            cin >> userId;
            userId = wd.GbkToUtf8(userId);
            deleteUser(userId, "Data/users.json");
        }
        else if (choice == "2") {
            cout << "请输入要修改信息的用户ID: ";
            string userId;
            cin >> userId;
            userId = wd.GbkToUtf8(userId);
            modifyUser(userId, "Data/users.json");
        }
        else if (choice == "q") break;
        else cout << "无效选择，请重试。\n";
    }
}

void Admin::deleteUser(const std::string& userId, const std::string& filename) {
    json users;
    ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> users;
            if (users.contains(userId)) {
                users.erase(userId);
                ofstream outFile(filename);
                if (outFile.is_open()) {
                    outFile << users.dump(4);
                    cout << "用户删除成功！\n";
                }
            }
            else {
                cout << "未找到该用户。\n";
            }
        }
        catch (...) {
            cout << "读取用户数据失败。\n";
        }
    }
    else {
        cout << "无法打开用户数据文件。\n";
    }
}

void Admin::modifyUser(const std::string& userId, const std::string& filename) {
    json users;
    ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> users;
            if (users.contains(userId)) {
                User user = User::fromJson2(userId, users[userId]);
                GoodsManager miao;

                cout << "请选择你想要修改的信息" << endl;
                cout << "1. 修改姓名" << endl;
                cout << "2. 修改密码" << endl;
                cout << "3. 修改地址" << endl;
                cout << "4. 修改专业" << endl;
                cout << "5.修改建行余额" << endl;
                cout << "6.修改农行余额" << endl;
                cout << "7.修改等级" << endl;
                cout << "8. 按q退出" << endl;
                while (1) {
                    string t;
                    cin >> t;

                    if (t == "1") {
                        cout << "请输入新的姓名：";
                        string x;
                        cin >> x;
                        x = miao.GbkToUtf8(x);
                        user.Name = x;
                    }
                    else if (t == "2") {
                        cout << "请输入新的密码：";
                        string x;
                        cin >> x;
                        x = miao.GbkToUtf8(x);
                        user.Password = x;
                    }
                    else if (t == "3") {
                        cout << "请输入新的地址：";
                        string x;
                        cin >> x;
                        x = miao.GbkToUtf8(x);
                        user.Address = x;
                    }
                    else if (t == "4") {
                        cout << "请输入新的专业：";
                        string x;
                        cin >> x;
                        x = miao.GbkToUtf8(x);
                        user.Major = x;
                    }
                    else if (t == "5") {
                        cout << "请输入新的建行余额：";
                        double x;
                        cin >> x;
                        user.jianhangCredits = x;
                    }
                    else if (t == "6") {
                        cout << "请输入新的农行余额：";
                        double x;
                        cin >> x;
                        user.nonghangCredits = x;
                    }
                    else if (t == "7") {
                        cout << "请输入新的等级：";
                        int x;
                        cin >> x;
                        user.Level = x;
                    }
                    else if (t == "q") {
                        break;
                    }
                    else {
                        cout << "无效选择，请重新输入！";
                        continue;
                    }

                    users[userId] = user.toJson();
                    ofstream outFile(filename);
                    if (outFile.is_open()) {
                        outFile << users.dump(4);
                    }
                }
                cout << "用户信息修改成功！\n";
            }
            else {
                cout << "未找到该用户。\n";
            }
        }
        catch (...) {
            cout << "读取用户数据失败。\n";
        }
    }
    else {
        cout << "无法打开用户数据文件。\n";
    }
}
