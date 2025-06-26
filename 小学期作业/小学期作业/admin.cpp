#include<bits/stdc++.h>
#include"admin.h"
#include<Windows.h>
#include"Goods.h"
#include"GoodsManage.h"
#include<json.hpp>
#include"User.h"
using json = nlohmann::json;//ʹ��json
using namespace std;
//��Ʒ���ĸ�Ԫ�أ�ID�����֣��۸�ʣ������
void Admin::manageBooks() {
    while (true) {
        cout << "\n-- ��Ʒ���� --\n";
        cout << "1. �����Ʒ\n";
        cout << "2. ɾ����Ʒ\n";
        cout << "q. �����ϼ��˵�\n";
        string choice;
        cin >> choice;
        if (choice == "1") addBook();
        else if (choice == "2") deleteBook();
        else if (choice == "q") break;
        else cout << "��Чѡ������ԡ�\n";
    }
}


void Admin::addBook() {
    GoodsManager bo;
    bo.loadGoods("Data/book.json"); // ����������Ʒ
    Goods b;
    cout << "��������Ʒ��: ";
    cin >> b.ID;
    cout << "��������Ʒ��: ";
    cin.ignore();
    getline(cin, b.Name);
    cout << "������۸�: ";
    cin >> b.Price;
    cout << "��������Ʒ����: ";
    cin>>b.RemainingQuantity;
    bo.goods.push_back(b);
    bo.saveBooks("Data/book.json");
    cout << "��ӳɹ���\n";
}

void Admin::deleteBook() {
    string id;
    cout << "������Ҫɾ�������: ";
    cin >> id;
    GoodsManager bo;
    bo.loadGoods("Data/book.json"); // ����������Ʒ

    // ���Ҳ�ɾ����Ʒ
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
        cout << "ɾ���ɹ���\n";
    }
    else {
        cout << "δ�ҵ�����š�\n";
    }
}
void Admin::manageUsers() {
    GoodsManager wd;
    while (true) {
        cout << "\n-- �û����� --\n";
        cout << "1. ɾ���û�\n";
        cout << "2. �޸��û���Ϣ\n";
        cout << "q. �������˵�\n";
        string choice;
        cin >> choice;
        if (choice == "1") {
            cout << "������Ҫɾ�����û�ID: ";
            string userId;
            cin >> userId;
            userId = wd.GbkToUtf8(userId);
            deleteUser(userId, "Data/users.json");
        }
        else if (choice == "2") {
            cout << "������Ҫ�޸���Ϣ���û�ID: ";
            string userId;
            cin >> userId;
            userId = wd.GbkToUtf8(userId);
            modifyUser(userId, "Data/users.json");
        }
        else if (choice == "q") break;
        else cout << "��Чѡ�������ԡ�\n";
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
                    cout << "�û�ɾ���ɹ���\n";
                }
            }
            else {
                cout << "δ�ҵ����û���\n";
            }
        }
        catch (...) {
            cout << "��ȡ�û�����ʧ�ܡ�\n";
        }
    }
    else {
        cout << "�޷����û������ļ���\n";
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

                cout << "��ѡ������Ҫ�޸ĵ���Ϣ" << endl;
                cout << "1. �޸�����" << endl;
                cout << "2. �޸�����" << endl;
                cout << "3. �޸ĵ�ַ" << endl;
                cout << "4. �޸�רҵ" << endl;
                cout << "5.�޸Ľ������" << endl;
                cout << "6.�޸�ũ�����" << endl;
                cout << "7.�޸ĵȼ�" << endl;
                cout << "8. ��q�˳�" << endl;
                while (1) {
                    string t;
                    cin >> t;

                    if (t == "1") {
                        cout << "�������µ�������";
                        string x;
                        cin >> x;
                        x = miao.GbkToUtf8(x);
                        user.Name = x;
                    }
                    else if (t == "2") {
                        cout << "�������µ����룺";
                        string x;
                        cin >> x;
                        x = miao.GbkToUtf8(x);
                        user.Password = x;
                    }
                    else if (t == "3") {
                        cout << "�������µĵ�ַ��";
                        string x;
                        cin >> x;
                        x = miao.GbkToUtf8(x);
                        user.Address = x;
                    }
                    else if (t == "4") {
                        cout << "�������µ�רҵ��";
                        string x;
                        cin >> x;
                        x = miao.GbkToUtf8(x);
                        user.Major = x;
                    }
                    else if (t == "5") {
                        cout << "�������µĽ�����";
                        double x;
                        cin >> x;
                        user.jianhangCredits = x;
                    }
                    else if (t == "6") {
                        cout << "�������µ�ũ����";
                        double x;
                        cin >> x;
                        user.nonghangCredits = x;
                    }
                    else if (t == "7") {
                        cout << "�������µĵȼ���";
                        int x;
                        cin >> x;
                        user.Level = x;
                    }
                    else if (t == "q") {
                        break;
                    }
                    else {
                        cout << "��Чѡ�����������룡";
                        continue;
                    }

                    users[userId] = user.toJson();
                    ofstream outFile(filename);
                    if (outFile.is_open()) {
                        outFile << users.dump(4);
                    }
                }
                cout << "�û���Ϣ�޸ĳɹ���\n";
            }
            else {
                cout << "δ�ҵ����û���\n";
            }
        }
        catch (...) {
            cout << "��ȡ�û�����ʧ�ܡ�\n";
        }
    }
    else {
        cout << "�޷����û������ļ���\n";
    }
}
