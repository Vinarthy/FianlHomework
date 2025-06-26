#include "User.h"
#include<iostream>
#include"GoodsManage.h"
#include<list>
#include<json.hpp>
#include<bits/stdc++.h>
using json = nlohmann::json;//ʹ��json
User::User(const string& name, const string& password, const string& address,const string& major)
    : Name(name), Password(password), Address(address),Major(major) {
}


json User::toJson() const {//buyerתjson��ʽ,��Ҫ�������������һ������
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
    b.Password = x.Utf8ToGbk(j.at("Password").get<string>());//utf-gbkҪ��
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
    b.Password = j.at("Password").get<string>();//utf-gbkҪ��
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
    if (Level == 1) levelStr = "��ͨ�û�";
    else if (Level >= 2 && Level <= 4) levelStr = "��Ա";
    else if (Level == 5) levelStr = "mhy���";
    else levelStr = "δ֪�ȼ�";
    cout << "========= �û���Ϣ =========\n";
    cout << "�û�����" << Name << endl;
    cout << "��Դ��" << Address << endl;
    cout << "רҵ��" << Major << endl;
    cout << "�ȼ���" << Level << endl;
    cout << "������" << jianhangCredits << " Ԫ\n";
    cout << "ũ����" << nonghangCredits << "Ԫ\n";
}


void User::ChangeSomeInformation(string& Username, string& filename) {
    cout << "��ѡ���Լ����޸ĵ���Ϣ" << endl;
    json users;
    ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> users;
        }
        catch (...) {
            cout << "��ȡ�û�����ʧ�ܣ�" << endl;
            return;
        }
    }
    else {
        cout << "�޷����û������ļ���" << endl;
        return;
    }

    while (true) {
        // ��ʾ�˵�
        cout << "1.�޸�����" << endl;
        cout << "2.�޸�����" << endl;
        cout << "3.�޸���Դ��" << endl;
        cout << "4.�޸�רҵ" << endl;
        cout << "5.��q�˳�" << endl;

        string t;
        cin >> t;

        if (!users.contains(Username)) {
            cout << "�û������ڣ�" << endl;
            return;
        }

        // ��ȡ��ǰ�û�����
        User b = User::fromJson2(Username, users[Username]);
        GoodsManager miao;

        if (t == "1") {
            cout << "�����������֣�";
            string x;
            cin >> x;
            // �Ƴ����û���������û�
            users.erase(Username);
            Username = miao.GbkToUtf8(x); // �����û���
            b.Name = Username;
            users[Username] = b.toJson();
        }
        else if (t == "2") {
            cout << "�����������룺";
            string x;
            cin >> x;
            b.Password = miao.GbkToUtf8(x);
            users[Username] = b.toJson();
        }
        else if (t == "3") {
            cout << "����������Դ�أ�";
            string x;
            cin >> x;
            b.Address = miao.GbkToUtf8(x);
            users[Username] = b.toJson();
        }
        else if (t == "4") {
            cout << "��������רҵ��";
            string x;
            cin >> x;
            b.Major = miao.GbkToUtf8(x);
            users[Username] = b.toJson();
        }
        else if (t == "q") {
            break;
        }
        else {
            cout << "��Ч���������ԣ�" << endl;
            continue;
        }

        // ���������û��б�
        ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << setw(4) << users;
            cout << "�޸ĳɹ���" << endl;
        }
        else {
            cout << "����ʧ�ܣ�" << endl;
        }
    }
}