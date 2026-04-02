#include <iostream>  
/* c++에서 구조체와 클레스 차이
	구조체 : 단순한 데이터 묶음 ( vertex, color, vextor3)
	클레스 : 기능과 로직이 포함될 때 ( shader, camera, renderer)
 */

class Animal {
private:
	int food;
	int weight;

public:
	void set_animal(int _food, int _weight) {
		food = _food;
		weight = _weight;
	}
	void increase_food(int inc) {
		food += inc;
		weight += (inc / 3);
		}
	void view_state()const {
		std::cout << " Food : " << food << std::endl;
		std::cout << " Weight : " << weight << " kg" << std::endl;
	}
};

int main() {
	Animal animl;
	animl.set_animal(100, 80);
	animl.increase_food(50);

	animl.view_state();

	return 0;
}