#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
/*
任何階段的 nop 位指示它是否在當前時鐘週期內執行有效操作。
IF 級的 nop 位初始化為 0，所有其他級的 nop 位初始化為 1。
（這是因為在第一個時鐘週期中，只有 IF 級執行有效操作。）
*/
/*
在沒有危險的情況下，
當前時鐘週期中一個階段的 nop 位的值等於前一個時鐘週期中前一個階段的 nop 位。
*/
using namespace std;

#define RegNum 32
#define MemSize 1000

struct IF{
    IF()
    bitset<RegNum> PC;

};