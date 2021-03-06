// testjson.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "json.hpp"


using namespace std;
using json = nlohmann::json;
int main()
{
	json j; 
	j["pi"] = 3.141;
	j["happy"] = true;
	j["name"] = "Niels";
	j["notihg"] = nullptr;
	j["answer"]["everything"] = 42;
	j["list"]={1, 0, 2};
	j["object"] =
	{
		{"currency","USD"},
	{"value",42.99}
	};
	json j2 = {
		{"pi", 3.141},
		{ "happy" , true },
		{ "name" , "Niels" },
		{"notihg", nullptr },
		{"answer",{"everything", 42} },
		{"list", { 1, 0, 2 } },
		{"object",{
			{ "currency","USD" },
			{ "value",42.99 }
		}
		}
	};
	j["pi"] = "pi";
	//json객체를 json으로 만들어준다
	std::string jsonStr = j.dump();
	std::string jsonStr2 = j2.dump();
	char ms[1024];
	char ps[1024];
	
	//json객체에서 오브젝트로 가져온다 
	std::string name = j["name"].get<std::string>();

	printf_s(jsonStr.c_str());
	printf_s("\n");

	printf_s(jsonStr2.c_str());
	printf_s("\n");
	printf_s(name.c_str());
	printf_s("\n");


	strcpy_s(ms, j2["name"].dump().c_str());
	
	json s = ms;
	
	std::string name3 = j2["currency"].get<std::string>();
	printf_s(s.dump().c_str());
	printf_s("\n");
	printf_s(name3.c_str());
	printf_s("\n");
	printf_s("\n");
	return 0;
}

