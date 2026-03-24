#include <iostream>
#include "func_header.h"

int function_01() {
	int f = 10;
	return f;
}

int& function_02(int& f_2) {
	f_2 = 24;
	return f_2;
}