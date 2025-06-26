/*˼·��1.�û��࣬����������רҵ����Դ��ũ�кͽ��е���Ϣ������һ��administer��json�ļ��������ƶ�д���Կ���
 2.��Ʒ�ࣺ������Ʒ��������Ʒ��ţ���Ʒ���֣�string������Ʒ�۸�,ʣ����Ʒ����
 Ȼ������������ˣ���������ѡ����˷�ʽ���ĸ��˻�Ȼ������Ӧ�˻������Ǯ
 �Ķ���������Ĭ��ֱ��ע��10���û����ȣ�Ȼ����ǿ��ѡ��һ���û���¼������Ȼ��Ǯ,��¼�����ţ��ŵ�json�ļ�����
 ��ʽ���û������桾1. ........����2. ......��
 �û�ע����֮����Ը����֣����룬��ַ��רҵ��1 2 3 4 5 6....��������
 ����Ա�ࣺ����ɾ���������û�����Ʒ���ı��û����������ݡ�

 ���ܣ���ȡ�ַ��Ķ�������ʽȻ����ܣ��ٴ洢��
*/
#include<bits/stdc++.h>
#include"User.h"
#include <Windows.h> // ���� Windows ϵͳ
#include"GoodsManage.h"
#include"Goods.h"
#include<list>
#include"admin.h"
#include<json.hpp>
#include "List_.h"
using json = nlohmann::json;//ʹ��json
using namespace std;

void LogOrSign();
void check();
void signup();
bool loadUsers(json& j);
void saveUsers(const json& j);
string CurrentUser = "";//�û�ȫ�ֱ���


string GbkToUtf8(const string& gbkStr) {
    int wideLen = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, nullptr, 0);
    if (wideLen == 0) return "";

    wstring wideStr(wideLen, 0);
    MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, &wideStr[0], wideLen);

    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Len == 0) return "";

    string utf8Str(utf8Len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Len, nullptr, nullptr);

    return utf8Str;
}
string safe_input_utf8() {//��¼ʱ�����ַ������Զ��������������������⣬����һ���ַ���Ȼ��gbk->utf-8�Զ�ȥ�㣩
    string gbk;
    getline(cin, gbk);
    string utf8 = GbkToUtf8(gbk);
    utf8.erase(remove(utf8.begin(), utf8.end(), '\0'), utf8.end());
    return utf8;
}


string filename = "Data/users.json";
string bookfilename = "Data/book.json";
bool Success_Log = false;

void buyBooks(GoodsManager& bookManager, User& realBuyer, json& users) {
    // ��ʾ��Ʒ�б��û����빺������
    while (true) {
        cout << "======= ��Ʒ�б� =======\n";
        bookManager.displayAllBooks();
        cout << "������ ��ƷID ���� ������ӣ�����: 1 2\n";
        cout << "���� q ��ʼ���㣬չʾ���ﳵ������\n";

        string input;
        cin >> input;

        if (input == "q") break;

        string bookID = input;
        int quantity;
        cin >> quantity;

        Goods* selectedBook = bookManager.findBookByID(bookID);
        if (!selectedBook) {
            cout << "δ�ҵ�����Ʒ�������ԡ�\n";
            continue;
        }

        if (quantity <= 0) {
            cout << "��������Ϊ����\n";
            continue;
        }

        realBuyer.Spcar_Book_ID.push_back(bookID);
        realBuyer.Spcar_Book_Num.push_back(quantity);
        cout << "�Ѽ��빺�ﳵ��" << selectedBook->Name << " x" << quantity << endl;
    }

    // ====== �����߼� ======
    cout << "\n==== ���� ====\n";

    double total = 0.0;
    auto idIt = realBuyer.Spcar_Book_ID.begin();
    auto numIt = realBuyer.Spcar_Book_Num.begin();

    while (idIt != realBuyer.Spcar_Book_ID.end() && numIt != realBuyer.Spcar_Book_Num.end()) {
        string id = *idIt;
        int qty = *numIt;

        Goods* book = bookManager.findBookByID(id);
        if (book) {
            double price = book->Price;
            total += price * qty;
        }

        ++idIt;
        ++numIt;
    }

    double discountedTotal = realBuyer.calculateTotal(total);
    cout << "�û��ȼ���" << realBuyer.Level << "��ԭ�ۺϼƣ�" << total << " Ԫ���ۿۺ�Ӧ����" << discountedTotal << " Ԫ\n";

    // ���֧����ʽѡ��
    string paymentMethod;
    bool paymentSuccess = false;

    while (true) {
        cout << "��ѡ��֧����ʽ��\n";
        cout << "1. �������� (���: " << realBuyer.jianhangCredits << " Ԫ)\n";
        cout << "2. ũҵ���� (���: " << realBuyer.nonghangCredits << " Ԫ)\n";
        cout << "3. ȡ��֧��\n";

        cin >> paymentMethod;

        if (paymentMethod == "1") {
            if (realBuyer.tryPayjh(discountedTotal)) {
                paymentSuccess = true;
                cout << "ʹ�ý�������֧���ɹ���\n";
            }
            else {
                cout << "�����������㣡\n";
            }
            break;
        }
        else if (paymentMethod == "2") {
            if (realBuyer.tryPaynh(discountedTotal)) {
                paymentSuccess = true;
                cout << "ʹ��ũҵ����֧���ɹ���\n";
            }
            else {
                cout << "ũҵ�������㣡\n";
            }
            break;
        }
        else if (paymentMethod == "3") {
            cout << "��ȡ��֧����\n";
            break;
        }
        else {
            cout << "��Чѡ�����������롣\n";
        }
    }

    if (paymentSuccess) {
        cout << "֧���ɹ���ʣ�ཨ����" << realBuyer.jianhangCredits
            << " Ԫ��ũ����" << realBuyer.nonghangCredits << " Ԫ\n";

        // ��չ��ﳵ
        realBuyer.Spcar_Book_ID.clear();
        realBuyer.Spcar_Book_Num.clear();

        // �����û�״̬
        users[realBuyer.Name] = realBuyer.toJson();
        ofstream out("Data/users.json");
        if (out.is_open()) {
            out << setw(4) << users;
        }
    }
    else {
        cout << "֧��δ��ɣ����ﳵ����ա�\n";
        realBuyer.Spcar_Book_ID.clear();
        realBuyer.Spcar_Book_Num.clear();
    }
}
// main.cpp
#include "List_.h" // �����Զ�������ͷ�ļ�
// ... �������� ...

void createInitialUsers() {
    // ���������û�
    json users;
    ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> users;
        }
        catch (...) {
            users = json::object();
        }
        inFile.close();
    }
    else {
        users = json::object();
    }

    cout << "==== �ֶ������û� ====" << endl;
    cout << "����Ҫ�ֶ�����10���û�����Ϣ" << endl;
    GoodsManager converter;

    // ʹ���Զ�������洢�û�
    List_<User> userList;

    // �ֶ�����10���û�����ӵ�����
    for (int i = 1; i <= 10; i++) {
        cout << "\n==== ������ " << i << " ���û� ====" << endl;

        string username, password, address, major;

        // �ֶ������û���
        while (true) {
            cout << "�������û���: ";
            getline(cin, username);

            // ��������Ƿ�Ϊ��
            if (username.empty()) {
                cout << "�û�������Ϊ�գ�����������" << endl;
                continue;
            }

            // ת��ΪUTF-8
            string utf8Username = converter.GbkToUtf8(username);

            // ����û����Ƿ��Ѵ���
            if (users.contains(utf8Username)) {
                cout << "�û����Ѵ��ڣ�����������" << endl;
            }
            else {
                username = utf8Username;
                break;
            }
        }

        // �ֶ���������
        cout << "����������: ";
        getline(cin, password);
        password = converter.GbkToUtf8(password);

        // �ֶ������ַ
        cout << "�������ַ: ";
        getline(cin, address);
        address = converter.GbkToUtf8(address);

        // �ֶ�����רҵ
        cout << "������רҵ: ";
        getline(cin, major);
        major = converter.GbkToUtf8(major);

        // �ֶ������û�����
        User newUser(username, password, address, major);

        // �ֶ���ӵ�����
        userList.push_back(newUser);

        // ʹ��ԭʼ������ʾ�û���������ת�����⣩
        cout << "�û� " << i << " (" << username << ") �����ɹ���" << endl;
    }

    // �ֶ���������
    List_<User>::Node* current = userList.get_head();
    while (current != nullptr) {
        // �ֶ����ʽڵ�����
        User& user = current->data;

        // �ֶ���ӵ�JSON
        users[user.Name] = user.toJson();

        // ֱ�����UTF-8�û���������ת������
        cout << "�ѽ��û���ӵ�ϵͳ: " << user.Name << endl;

        // �ֶ��ƶ�����һ���ڵ�
        current = current->next;
    }

    // �����û�����
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << setw(4) << users;
        cout << "\n==== �û�������� ====" << endl;
        cout << "�ѳɹ����� 10 ���û������浽ϵͳ" << endl;
    }
    else {
        cerr << "�޷������û�����" << endl;
    }
}

// ... main() �������ֲ��� ...
int main() {
    cout << "��ӭ������Ϣ����ϵͳ��" << endl;
    cout << "���濪ʼ������ʼ�û���~" << endl;

    // ������ʼ�û�
    createInitialUsers();
    LogOrSign();
    if (Success_Log == true) {
        GoodsManager bookManager;
        bookManager.loadGoods(bookfilename);

        cout << "��ǰ��Ʒ�б�" << endl;
        bookManager.displayAllBooks();
        GoodsManager a;
        while (1) {
            a.DisplayMessage();
            string t;
            cin >> t;
            if (t == "1") {
                json users;
                if (loadUsers(users) && users.contains(CurrentUser)) {
                    User temp = User::fromJson2(CurrentUser, users[CurrentUser]);

                    unique_ptr<User> realUser;
                    if (temp.Level >= 2 && temp.Level <= 4)
                        realUser = make_unique<Member>(temp);
                    else if (temp.Level == 5)
                        realUser = make_unique<honoured_guest>(temp);
                    else
                        realUser = make_unique<User>(temp);

                    // ����ǹ�������������������ۿ�
                    if (temp.Level == 5) {
                        dynamic_cast<honoured_guest*>(realUser.get())->Set_discount_rate();
                    }

                    buyBooks(bookManager, *realUser, users); // ������
                }
            }
            else if (t == "2") {//��ʾ�û���ϢCurrentUser��ǰ�û�
                json users;
                if (loadUsers(users) && users.contains(CurrentUser)) {
                    User b = User::fromJson(CurrentUser, users[CurrentUser]);
                    b.showInformation();
                }
                else {
                    cout << "�û���Ϣ��ȡʧ�ܣ�\n";
                }
            }
            else if (t == "3") {
                bookManager.SortBooks();
                if (bookManager.saveBooks("Data/book.json")) {
                    cout << "��Ʒ����ɹ����ѱ��棡����չʾ���������Ʒ" << endl;
                    bookManager.displayAllBooks();
                }
                else {
                    cout << "����ɹ����������ļ�ʧ�ܣ�" << endl;
                }
            }
            else if (t == "4") {//�޸��û���Ϣ���ҵĽ�����1��2��3��4չʾ������Ϣ��Ȼ�������޸�
                json users;
                if (loadUsers(users) && users.contains(CurrentUser)) {
                    User b = User::fromJson2(CurrentUser, users[CurrentUser]);
                    b.ChangeSomeInformation(CurrentUser, filename);
                }
                else {
                    cout << "�û���Ϣ��ȡʧ�ܣ�\n";
                }
            }
            else if (t == "5") {//ȥ����Ա������д
                Admin admin;
                while (1) {
                    cout << "��������Ĳ�����"<<endl;
                    cout << "1.������Ʒ" << endl;
                    cout << "2.�����û�" << endl;
                    string adminChoice;
                    cin >> adminChoice;
                    if (adminChoice == "1") {
                        admin.manageBooks();
                    }
                    else if (adminChoice == "2") {
                        admin.manageUsers();
                    }
                    else if (adminChoice == "q") {
                        break;
                    }
                    else {
                        cout << "��Чѡ�����������롣\n";
                    }
                }

            }
            else if (t == "6") {//����ͬ�ȼ��Ļ�Ա
                cout << "1.һ����Ա��98Ԫ" << endl;//level->2Ȼ����Դ���
                cout << "2.������Ա��198Ԫ" << endl;
                cout << "3.������Ա��328Ԫ" << endl;
                cout << "4.mhy������Ա��648Ԫ" << endl;
                string choice;
                cin >> choice;
                if (choice == "q") continue;

                // ��ȡ�û���Ϣ
                json users;
                if (loadUsers(users) && users.contains(CurrentUser)) {
                    User b = User::fromJson2(CurrentUser, users[CurrentUser]);

                    int newLevel = b.Level; // Ĭ�ϲ�����
                    double cost = 0.0;

                    if (choice == "1") {
                        newLevel = 2;
                        cost = 98.0;
                    }
                    else if (choice == "2") {
                        newLevel = 3;
                        cost = 198.0;
                    }
                    else if (choice == "3") {
                        newLevel = 4;
                        cost = 328.0;
                    }
                    else if (choice == "4") {
                        newLevel = 5;
                        cost = 648.0;
                    }
                    else {
                        cout << "��Чѡ��\n";
                        continue;
                    }

                    // �ж��Ƿ���Ҫ���� + �ۿ�
                    if (newLevel <= b.Level) {
                        cout << "��ǰ�ȼ��Ѹ��ڻ������ѡ�ȼ����޷��ظ�����\n";
                    }
                    else if (b.jianhangCredits + b.nonghangCredits < cost) {
                        cout << "���ֲ��㣬����ʧ�ܣ���ǰ���֣�" << b.jianhangCredits + b.nonghangCredits << "Ԫ\n";
                    }
                    else {
                        if (b.jianhangCredits >= cost) {
                            b.jianhangCredits -= cost;
                        }
                        else {
                            cost -= b.jianhangCredits;
                            b.jianhangCredits = 0;
                            b.nonghangCredits -= cost;
                        }
                        b.Level = newLevel;
                        try {
                            json newUserData = b.toJson();  // �����µ��û�����
                            users[CurrentUser] = newUserData;//�����û�����
                            saveUsers(users);
                        }
                        catch (const std::exception& e) {
                            cerr << "д���û�����ʧ�ܣ�����" << e.what() << endl;
                        }
                        cout << "��ϲ��Ϊ ";
                        if (newLevel == 2) cout << "һ����Ա��\n";
                        else if (newLevel == 3) cout << "������Ա��\n";
                        else if (newLevel == 4) cout << "������Ա��\n";
                        else if (newLevel == 5) cout << "mhy������Ա��\n";

                        cout << "��ǰ���֣�" << b.jianhangCredits + b.nonghangCredits << "Ԫ\n";
                    }
                }
                else {
                    cout << "�û���Ϣ��ȡʧ�ܣ�\n";
                }
            }
            else if (t == "9") {//�˳�
                break;
            }
            else {
                cout << "��Ч���룬������";
            }
        }
    }
}


void LogOrSign() {
    while (true) {
        if (Success_Log == true) {
            break;
        }
        cout << "========== ��ʼ���� ==========\n";
        cout << "���� 0 ��¼������ 1 ע�ᣬ���� q �˳�����\n";
        string choice;
        cin >> choice;

        if (choice == "0") {
            check();
        }
        else if (choice == "1") {
            signup();
        }
        else if (choice == "q" || choice == "Q") {
            cout << "�˳�����...\n";
            return;
        }
        else {
            cout << "��Чѡ�������ԡ�\n";
        }
    }
}
void check() {
    json users;
    if (!loadUsers(users)) {
        cout << "�û����ݼ���ʧ�ܡ�\n";
        return;
    }

    while (true) {
        string codeInput;
        cout << "\n==== ��¼ ====\n";
        cout << "���� q ���ؿ�ʼ����\n";
        cout << "�������û���: ";
        cin.ignore();
        string name = safe_input_utf8();
        if (name == "q") return;

        cout << "����������:";
        cin >> codeInput;
        if (codeInput == "q") return;

        try {//�������name��contains������˵��Code�Ѿ���json�ļ��У����Բ�����ת��
            if (users.contains(name) && users[name]["Password"] == codeInput) {
                cout << "��¼�ɹ�����ӭʹ�ñ�ϵͳ~~~��\n";
                Success_Log = true;
                CurrentUser = name;//���û������ȥ
                break;//��¼֮������ѭ��
            }
            else {
                cout << "�û�����������������ԡ�\n";
            }
        }
        catch (...) {
            cout << "����ӦΪ���֡�\n";
        }
    }
}

void signup() {
    json users;
    if (!loadUsers(users)) {
        users = json::object();
    }

    while (true) {
        string name, address, password,Major;
        cout << "\n==== ע�� ====\n";
        cout << "���� q ���ؿ�ʼ����\n";
        cout << "�������û���: ";
        cin >> name;
        name = GbkToUtf8(name);
        name.erase(remove(name.begin(), name.end(), '\0'), name.end());
        if (name == "q") return;

        if (users.contains(name)) {
            cout << "�û����Ѵ��ڣ����������롣\n";
            continue;
        }

        cout << "����������: ";
        cin >> password;
        if (password == "q") return;

        cout << "�������ַ: ";
        cin.ignore(); // ��ջ�����
        getline(cin, address);
        address = GbkToUtf8(address);
        address.erase(remove(address.begin(), address.end(), '\0'), address.end());

        cout << "������רҵ: ";
        cin.ignore(); // ��ջ�����
        getline(cin, Major);
        Major = GbkToUtf8(Major);
        Major.erase(remove(Major.begin(), Major.end(), '\0'), Major.end());

        try {
            // �������û�
            User newBuyer(name, password, address,Major);

            // ���û�ת��ΪJSON����
            users[name] = newBuyer.toJson();

            saveUsers(users);
            cout << "ע��ɹ������ڿ��Ե�¼�ˡ�\n";
            return;
        }
        catch (const exception& e) {
            cerr << "ע�����: " << e.what() << endl;
            cout << "ע��ʧ�ܣ������ԡ�\n";
        }
    }
}

bool loadUsers(json& j) {
    ifstream inFile(filename);//�ļ��Ƿ�򿪣�û���򴴽�
    if (!inFile.is_open()) {
        j = json::object(); // �����ն���
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
void saveUsers(const json& j) {
    ofstream outFile(filename);//jsonд���ļ����Զ�����
    if (outFile.is_open()) {
        outFile << setw(4) << j;
    }
}