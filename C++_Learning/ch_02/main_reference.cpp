#include <iostream>


int change_value(int* p) {  //int& p = Num
	*p = 320;       //C 방식
	  
	return 0;	
}

int main() {
	int Num = 601;

	std::cout << Num << std::endl;
	change_value(&Num);			// change_value 인자에 Num의 주소값을 전달, Num의 값을 바꾸기
	std::cout << Num << std::endl;

	
	int a = 1;
	int& another_a = a; //C++ 방식/레퍼런스(메모리상에 존재 X) ,  포인터 타입의 참조자 : int*& 가능

	another_a = 5;
	std::cout << " a : " << a  << std::endl;
	std::cout << "another_a: " << another_a << std::endl;

	int b = 3;
	another_a = b;			//&another_a = b; -> X
	std::cout << " another_a : " << another_a << std::endl;



	int arr[3] = { 1, 2, 3 };
	int(&ref)[3] = arr;

	return 0;
}

//레퍼런스와 포인터의 차이
//int& another_a; -> 불가능
//int* p; -> 가능 

