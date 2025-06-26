#pragma once
#include<string>
#include<json.hpp>
using json = nlohmann::json;//使用json
using std::string;
class Admin {
public:
	//1.读物品 2.转格式（json->goods,goods->json）3.存物品
	void manageBooks(); // 管理书籍
	void addBook(); // 添加书籍
	void deleteBook(); // 删除书籍

	void manageUsers(); // 管理用户
	void deleteUser(const std::string& userId, const std::string& filename); // 按 ID 删除用户
	void modifyUser(const std::string& userId, const std::string& filename); // 修改用户信息
};