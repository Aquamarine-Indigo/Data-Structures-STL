//provided by ivy

// #include "vector.hpp"
// #include "class-matrix.hpp"
// #include "class-bint.hpp"
#include "v.hpp"
#include "class_matrix.hpp"
#include "class_bint.hpp"
#include <iostream>

void complex_test()
{
	freopen("four_out.txt", "w", stdout);
	std::cout << "Supplementary test for large amounts of data ..." << std::endl;
	sjtu::vector<Diamond::Matrix<Util::Bint>> myVec;
	for (int i = 1; i <= 1926; ++i)
	{
		// std::cerr << "i = " << i << "\t";
		myVec.push_back(Diamond::Matrix<Util::Bint>(i % 8 + 1, i % 17 + 1, Util::Bint(i * 817)));
		// if(i > 1)
		// std::cerr << "push val[" << i - 2 << "] = " << myVec[i - 2] << std::endl;
	}
	std::cerr << "Cin finished\nsize() = " << myVec.size() << std::endl;
	int o = 1234;
	while (o--)
		myVec.pop_back();
	std::cerr << "pop finished\nsize() = " << myVec.size() << std::endl;

	// for(int i = 0; i < myVec.size(); i++)
	// {
	// 	std::cerr << "val[" << i << "] = " << myVec[i] << std::endl;
	// }
	myVec = myVec;
	std::cerr << "copy finished\n";
	int _ = 20, __ = myVec.size();
	while (_--)
	{
		// std::cerr << "_ = " << _ << ", __ - _ = " << __ - _ << "\tval = " << myVec[__ - _] << std::endl;
		if (_ % 2 == 0)
		std::cout << myVec[_][0][0] << std::endl;
		else
		std::cout << myVec[__ - _][0][0] << std::endl;
	}
	std::cout << "Finished!" << std::endl;
}

int main()
{
    complex_test();
}