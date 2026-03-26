#include <iostream>		//C++에서 메모리 할당 및 해제 new, delete

int main() {

	int* p = new int; //int영역 할당, 주소값을 p에 넣음
	*p = 325;
	
	std::cout << " Addres of int memory : " << p << std::endl;

	delete p;	//할당된 메모리 공간 해제

	int arr_size;
	std::cout << " Input array size : ";
	std::cin >> arr_size;	//배열의 크기를 입력 받음
	int* list = new int[arr_size];	//int 배열 생성

	for (int i = 0; i < arr_size; i++) {
		std::cout << " Input any Number : ";	
		std::cin >> list[i];	//각 원소를 입력
	}
	std::cout << std::endl;
	for (int i = 0; i < arr_size; i++) {
		std::cout << " The element of list["<< i <<"] : " << list[i] << std::endl;
	}		// 각 i번쨰의 원소를 출력

	delete[] list;		//int 배열 해제

	return 0;
}