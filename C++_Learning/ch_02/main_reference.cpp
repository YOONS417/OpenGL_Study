#include <iostream>
#include "func_header.h"

int change_value(int* p) {  //int& p = Num
	*p = 320;       //C 방식
	  
	return 0;	
}

int main() {
	int Num = 601;

	std::cout << Num << std::endl;
	change_value(&Num);			// change_value 인자에 Num의 주소값을 전달, Num의 값을 바꾸기
	std::cout << Num << std::endl << std::endl;

	//레퍼런스는 반드시 초기화 필요, NULL 불가능, 재할당 불가능 
	int a = 1;
	int& another_a = a; //C++ 방식/레퍼런스(메모리상에 존재 X) ,  포인터 타입의 참조자 : int*& 가능
	 
	another_a = 5; // a = 5 레퍼런스를 바꾸면 a도 같이 바뀜 
	std::cout << " a : " << a  << std::endl;
	std::cout << " another_a: " << another_a << std::endl;

	int b = 3;
	another_a = b;		//이미 another_a가 a에 연결되어서 변경 불가,  (  &another_a = b; -> X )
	std::cout << " a : " << a << std::endl << std::endl; //a의 값이 b의 값으로 바뀜


	int arr[3] = { 1, 2, 3 }; //배열들의 레퍼런스(참조하기 위해 반드시 크기를 명시)
	int(&ref)[3] = arr; //ref가 arr를 참조

	ref[0] = 5; 
	ref[1] = 6;
	ref[2] = 7;
	std::cout << arr[0] << arr[1] << arr[2] << std::endl << std:: endl;

	//Function.cpp -> func_header.h -> main_referenec.cpp
	int c = function_01(); //function 안에 a의 값이 c에 복사
						// 함수가 종료되면 a는 메모리에서 사라짐 -> 지역 변수의 레퍼런스를 리턴하지 않게 조심
	std::cout << c << std::endl << std::endl;

	int d = 9; 
	int g = function_02(d);//함수2의 f2는 main의 d를 참조, 레퍼런스를 리턴
	std::cout << " g : " << g << ",  d : "<< d <<std::endl << std::endl; //인자로 받은 레퍼런스를 그대로 리턴
	//g에 d의 24를 대입하는 것과 동일, 함수의 f_2는 복사본이 아니라 d자체

	const int& cc = function_01();   //원래 함수의 리턴값은 해당 문장이 끝난 후 소멸
	std::cout << "cc" << cc << std::endl << std::endl;   
	//예외적으로 상수 레퍼런스로 리턴값을 받으면 해당 리턴값의 생명이 연장(레퍼런스가 사라질 때 까지)

	
	
	return 0;
}

//레퍼런스와 포인터의 차이
//int& another_a; -> 불가능
//int* p; -> 가능 

