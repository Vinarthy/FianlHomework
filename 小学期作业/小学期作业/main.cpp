/*思路：1.用户类，储存姓名，专业，生源，农行和建行的信息，并加一个administer的json文件（二进制读写试试看）
 2.商品类：储存商品，包含商品编号，商品名字（string），商品价格,剩余商品数量
 然后就是正常结账，不过可以选择结账方式是哪个账户然后往对应账户里面扣钱
 改动：现在是默认直接注册10个用户了先，然后再强制选择一个用户登录（），然后付钱,记录订单号，放到json文件里面
 格式：用户名下面【1. ........】【2. ......】
 用户注册了之后可以改名字，密码，地址，专业（1 2 3 4 5 6....这样排序）
 管理员类：可以删除和增加用户和商品，改变用户的任意数据。

 加密：获取字符的二进制形式然后加密，再存储。
*/
#include<bits/stdc++.h>
#include"User.h"
#include <Windows.h> // 引入 Windows 系统
#include"GoodsManage.h"
#include"Goods.h"
#include<list>
#include"admin.h"
#include<json.hpp>
using json = nlohmann::json;//使用json
using namespace std;

void LogOrSign();
void check();
void signup();
bool loadUsers(json& j);
void saveUsers(const json& j);
string CurrentUser = "";//用户全局变量


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
string safe_input_utf8() {//登录时输入字符串，自动处理中文输入乱码问题，输入一个字符串然后gbk->utf-8自动去零）
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
    // 显示商品列表，用户输入购买内容
    while (true) {
        cout << "======= 商品列表 =======\n";
        bookManager.displayAllBooks();
        cout << "请输入 商品ID 数量 进行添加，例如: 1 2\n";
        cout << "输入 q 开始结算，展示购物车并付款\n";

        string input;
        cin >> input;

        if (input == "q") break;

        string bookID = input;
        int quantity;
        cin >> quantity;

        Goods* selectedBook = bookManager.findBookByID(bookID);
        if (!selectedBook) {
            cout << "未找到该商品，请重试。\n";
            continue;
        }

        if (quantity <= 0) {
            cout << "数量必须为正数\n";
            continue;
        }

        realBuyer.Spcar_Book_ID.push_back(bookID);
        realBuyer.Spcar_Book_Num.push_back(quantity);
        cout << "已加入购物车：" << selectedBook->Name << " x" << quantity << endl;
    }

    // ====== 结算逻辑 ======
    cout << "\n==== 结算 ====\n";

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
    cout << "用户等级：" << realBuyer.Level << "，原价合计：" << total << " 元，折扣后应付：" << discountedTotal << " 元\n";

    if (realBuyer.tryPayjh(discountedTotal) || realBuyer.tryPaynh(discountedTotal)) {
        cout << "支付成功！剩余建行余额：" << realBuyer.jianhangCredits
            << " 元，农行余额：" << realBuyer.nonghangCredits << " 元\n";

        // 清空购物车
        realBuyer.Spcar_Book_ID.clear();
        realBuyer.Spcar_Book_Num.clear();

        // 保存用户状态
        users[realBuyer.Name] = realBuyer.toJson();
        ofstream out("Data/users.json");
        if (out.is_open()) {
            out << setw(4) << users;
        }

    }
    else {
        cout << "支付失败（余额不足），购物车已清空。\n";
        realBuyer.Spcar_Book_ID.clear();
        realBuyer.Spcar_Book_Num.clear();
    }
}
int main() {
    cout << "欢迎进入信息管理系统！" << endl;
    LogOrSign();
    if (Success_Log == true) {
        GoodsManager bookManager;
        bookManager.loadGoods(bookfilename);

        cout << "当前商品列表：" << endl;
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

                    // 如果是贵宾，还可以让他输入折扣
                    if (temp.Level == 5) {
                        dynamic_cast<honoured_guest*>(realUser.get())->Set_discount_rate();
                    }

                    buyBooks(bookManager, *realUser, users); // 传引用
                }
            }
            else if (t == "2") {//显示用户信息CurrentUser当前用户
                json users;
                if (loadUsers(users) && users.contains(CurrentUser)) {
                    User b = User::fromJson(CurrentUser, users[CurrentUser]);
                    b.showInformation();
                }
                else {
                    cout << "用户信息读取失败！\n";
                }
            }
            else if (t == "3") {
                bookManager.SortBooks();
                if (bookManager.saveBooks("Data/book.json")) {
                    cout << "商品排序成功并已保存！下面展示已排序的商品" << endl;
                    bookManager.displayAllBooks();
                }
                else {
                    cout << "排序成功，但保存文件失败！" << endl;
                }
            }
            else if (t == "4") {//修改用户信息：我的建议是1，2，3，4展示所有信息，然后按数字修改
                json users;
                if (loadUsers(users) && users.contains(CurrentUser)) {
                    User b = User::fromJson2(CurrentUser, users[CurrentUser]);
                    b.ChangeSomeInformation(CurrentUser, filename);
                }
                else {
                    cout << "用户信息读取失败！\n";
                }
            }
            else if (t == "5") {//去管理员类里面写
                Admin admin;
                while (1) {
                    cout << "请输入你的操作："<<endl;
                    cout << "1.管理商品" << endl;
                    cout << "2.管理用户" << endl;
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
                        cout << "无效选择，请重新输入。\n";
                    }
                }

            }
            else if (t == "6") {//购买不同等级的会员
                cout << "1.一级会员：98元" << endl;//level->2然后可以打折
                cout << "2.二级会员：198元" << endl;
                cout << "3.三级会员：328元" << endl;
                cout << "4.mhy超级会员：648元" << endl;
                string choice;
                cin >> choice;
                if (choice == "q") continue;

                // 获取用户信息
                json users;
                if (loadUsers(users) && users.contains(CurrentUser)) {
                    User b = User::fromJson2(CurrentUser, users[CurrentUser]);

                    int newLevel = b.Level; // 默认不升级
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
                        cout << "无效选择。\n";
                        continue;
                    }

                    // 判断是否需要升级 + 折扣
                    if (newLevel <= b.Level) {
                        cout << "当前等级已高于或等于所选等级，无法重复升级\n";
                    }
                    else if (b.jianhangCredits + b.nonghangCredits < cost) {
                        cout << "积分不足，升级失败，当前积分：" << b.jianhangCredits + b.nonghangCredits << "元\n";
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
                            json newUserData = b.toJson();  // 生成新的用户数据
                            users[CurrentUser] = newUserData;//更新用户数据
                            saveUsers(users);
                        }
                        catch (const std::exception& e) {
                            cerr << "写入用户数据失败，错误：" << e.what() << endl;
                        }
                        cout << "恭喜成为 ";
                        if (newLevel == 2) cout << "一级会员！\n";
                        else if (newLevel == 3) cout << "二级会员！\n";
                        else if (newLevel == 4) cout << "三级会员！\n";
                        else if (newLevel == 5) cout << "mhy超级会员！\n";

                        cout << "当前积分：" << b.jianhangCredits + b.nonghangCredits << "元\n";
                    }
                }
                else {
                    cout << "用户信息读取失败！\n";
                }
            }
            else if (t == "9") {//退出
                break;
            }
            else {
                cout << "无效输入，请重试";
            }
        }
    }
}


void LogOrSign() {
    while (true) {
        if (Success_Log == true) {
            break;
        }
        cout << "========== 开始操作 ==========\n";
        cout << "输入 0 登录，输入 1 注册，输入 q 退出程序\n";
        string choice;
        cin >> choice;

        if (choice == "0") {
            check();
        }
        else if (choice == "1") {
            signup();
        }
        else if (choice == "q" || choice == "Q") {
            cout << "退出程序...\n";
            return;
        }
        else {
            cout << "无效选择，请重试。\n";
        }
    }
}
void check() {
    json users;
    if (!loadUsers(users)) {
        cout << "用户数据加载失败。\n";
        return;
    }

    while (true) {
        string codeInput;
        cout << "\n==== 登录 ====\n";
        cout << "输入 q 返回开始操作\n";
        cout << "请输入用户名: ";
        cin.ignore();
        string name = safe_input_utf8();
        if (name == "q") return;

        cout << "请输入密码:";
        cin >> codeInput;
        if (codeInput == "q") return;

        try {//如果发生name的contains操作，说明Code已经在json文件中，所以不用再转码
            if (users.contains(name) && users[name]["Password"] == codeInput) {
                cout << "登录成功！欢迎使用本系统~~~！\n";
                Success_Log = true;
                CurrentUser = name;//将用户名存进去
                break;//登录之后跳出循环
            }
            else {
                cout << "用户名或密码错误，请重试。\n";
            }
        }
        catch (...) {
            cout << "输入应为数字。\n";
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
        cout << "\n==== 注册 ====\n";
        cout << "输入 q 返回开始操作\n";
        cout << "请输入用户名: ";
        cin >> name;
        name = GbkToUtf8(name);
        name.erase(remove(name.begin(), name.end(), '\0'), name.end());
        if (name == "q") return;

        if (users.contains(name)) {
            cout << "用户名已存在，请重新输入。\n";
            continue;
        }

        cout << "请输入密码: ";
        cin >> password;
        if (password == "q") return;

        cout << "请输入地址: ";
        cin.ignore(); // 清空缓冲区
        getline(cin, address);
        address = GbkToUtf8(address);
        address.erase(remove(address.begin(), address.end(), '\0'), address.end());

        cout << "请输入专业: ";
        cin.ignore(); // 清空缓冲区
        getline(cin, Major);
        Major = GbkToUtf8(Major);
        Major.erase(remove(Major.begin(), Major.end(), '\0'), Major.end());

        try {
            // 创建新用户
            User newBuyer(name, password, address,Major);

            // 将用户转换为JSON对象
            users[name] = newBuyer.toJson();

            saveUsers(users);
            cout << "注册成功！现在可以登录了。\n";
            return;
        }
        catch (const exception& e) {
            cerr << "注册错误: " << e.what() << endl;
            cout << "注册失败，请重试。\n";
        }
    }
}

bool loadUsers(json& j) {
    ifstream inFile(filename);//文件是否打开，没打开则创建
    if (!inFile.is_open()) {
        j = json::object(); // 创建空对象
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
    ofstream outFile(filename);//json写入文件，自动保存
    if (outFile.is_open()) {
        outFile << setw(4) << j;
    }
}