#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

//! Instruction fetch from memory
class IF
{
public:
    IF(bitset<32> PC)
        : PC(PC)
    {
        implement = 1; // implement初始化為1，必定皆會從抓取指令開始
    }

protected:
    //* Program counter
    bitset<32> PC;

    //* 在當前clock cycle，此stage是否在執行
    bool implement;
};