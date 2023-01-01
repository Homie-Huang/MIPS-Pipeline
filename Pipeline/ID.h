#include <iostream>
#include <bitset>
using namespace std;

class ID
{
public:
    ID()
    {
        implement = 0; // implement初始化為0
    }

    //* instruction
    bitset<32> ins;

    //* 在當前clock cycle，此stage是否在執行
    bool implement;
};