#include "BST.h"
#include <iostream>

int main()
{
	BST<int> bst;

	//----WSTAWIANIE
	bst.insert(8);
	bst.insert(10);
	bst.insert(5);
	bst.insert(2);
	bst.insert(6);
	bst.insert(1);
	bst.insert(3);
	bst.insert(9);
	bst.insert(12);
	bst.insert(11);
	bst.insert(15);

	for (auto it = bst.begin(); it != bst.end(); ++it)
		std::cout << *it << " ";

	std::cout << "\n";

	if (bst.is_balanced())
		std::cout << "jest zrownowazone" << "\n";
	else std::cout << "nie jest zrownowazone" << "\n";

	//----USUWANIE
	bst.remove(12);
	bst.remove(44);
	bst.remove(5);

	for (auto it = bst.begin(); it != bst.end(); ++it)
		std::cout << *it << " ";
	std::cout << "\n";

	if (bst.is_balanced())
		std::cout << "jest zrownowazone" << "\n";
	else std::cout << "nie jest zrownowazone" << "\n";

	//----SZUKANIE
	auto n1 = bst.find(8);
	auto n2 = bst.find(21);

	//----KONSTRUKTORY
	BST<int> b1(bst);
	BST<int> b2(6, [](const size_t a) {return a % 3 + 1; });

	b1 = b2;

	BST<int> b3(std::move(b2));

	BST<int> b4(8, [](const size_t a) {return a + 1; });
	b4 = std::move(b3);


	return 0;
}
