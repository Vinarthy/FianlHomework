#pragma once
#include<string>
#include<json.hpp>
using json = nlohmann::json;//ʹ��json
using std::string;
class Admin {
public:
	//1.����Ʒ 2.ת��ʽ��json->goods,goods->json��3.����Ʒ
	void manageBooks(); // �����鼮
	void addBook(); // ����鼮
	void deleteBook(); // ɾ���鼮

	void manageUsers(); // �����û�
	void deleteUser(const std::string& userId, const std::string& filename); // �� ID ɾ���û�
	void modifyUser(const std::string& userId, const std::string& filename); // �޸��û���Ϣ
};