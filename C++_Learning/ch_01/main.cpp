#include <iostream>
#include "NameSpace.h" 
//"" 직접 만든 프로젝트 폴더 내의 헤더를 가져올 때

int main() {
	std::cout <<  "hey"  << ns::Num << std::endl;  // std = namespace , cout/endl = iostream의 stddp 저장
	
	int user_input;
	
	while (1) {
		std::cout << "password : ";
		std::cin >> user_input;  //사용자로 부터 입력 받기, 

		if (ns::Password == user_input) {
			std::cout << "Welcome" << std::endl;
			break;
		}
		else {
			std::cout << "Please try again" << std::endl;
			 
		}
	}
	
	
	
	return 0;   // ex) std::cout << output << output ...
}


//This is first day of c++ 
//Creating headerfile and use it on main.cpp
//변수 선언 및 구조문은 C와 동일