#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>

#include "State.h"
#include "Instruction_Memory.h"
#include "Register_File.h"
#include "Data_Memory.h"

using namespace std;

void save_state(int cycle, State state)
{
    ofstream file("ouput.txt", ios_base::out | ios_base::app);

    file << "Cycle " << cycle << endl;

    file << "\n[IF_stage]" << endl;
    file << "implement:\t" << state.IF_stage.implement << endl;
    file << "PC:\t" << state.IF_stage.PC.to_ulong() << endl;

    file << "\n[ID_stage]" << endl;
    file << "implement:\t" << state.ID_stage.implement << endl;
    file << "ins:\t" << state.ID_stage.ins << endl;

    file << "\n[EX_stage]" << endl;
    file << "implement:\t" << state.EX_stage.implement << endl;
    file << "rs:\t" << state.EX_stage.rs << endl;
    file << "rt:\t" << state.EX_stage.rt << endl;
    file << "Write_reg:\t" << state.EX_stage.Write_reg << endl;
    file << "I_address:\t" << state.EX_stage.I_address << endl;
    file << "Read_data1:\t" << state.EX_stage.Read_data1 << endl;
    file << "Read_data2:\t" << state.EX_stage.Read_data2 << endl;
    file << "ALUp:\t" << state.EX_stage.ALUOp << endl;
    file << "type:\t" << state.EX_stage.type << endl;
    file << "RegDst:\t" << state.EX_stage.RegDst << endl;
    file << "ALUSrc:\t" << state.EX_stage.ALUSrc << endl;
    file << "Branch:\t" << state.EX_stage.Branch << endl;
    file << "MemRead:\t" << state.EX_stage.MemRead << endl;
    file << "MemWrite:\t" << state.EX_stage.MemWrite << endl;
    file << "RegWrite:\t" << state.EX_stage.RegWrite << endl;
    file << "MemtoReg:\t" << state.EX_stage.MemtoReg << endl;

    file << "\n[MEM_stage]" << endl;
    file << "implement:\t" << state.MEM_stage.implement << endl;
    file << "rs:\t" << state.MEM_stage.rs << endl;
    file << "rt:\t" << state.MEM_stage.rt << endl;
    file << "Write_reg:\t" << state.MEM_stage.Write_reg << endl;
    file << "ALU_result:\t" << state.MEM_stage.ALU_result << endl;
    file << "Write_data:\t" << state.MEM_stage.Write_data << endl;
    file << "Branch:\t" << state.MEM_stage.Branch << endl;
    file << "MemRead:\t" << state.MEM_stage.MemRead << endl;
    file << "MemWrite:\t" << state.MEM_stage.MemWrite << endl;
    file << "RegWrite:\t" << state.MEM_stage.RegWrite << endl;
    file << "MemtoReg:\t" << state.MEM_stage.MemtoReg << endl;

    file << "\n[WB_stage]" << endl;
    file << "implement:\t" << state.WB_stage.implement << endl;
    file << "rs:\t" << state.WB_stage.rs << endl;
    file << "rt:\t" << state.WB_stage.rt << endl;
    file << "Write_reg:\t" << state.WB_stage.Write_reg << endl;
    file << "Write_data:\t" << state.WB_stage.Write_data << endl;
    file << "RegWrite:\t" << state.WB_stage.RegWrite << endl;
    file << "MemtoReg:\t" << state.WB_stage.MemtoReg << endl;

    file.close();
}

unsigned long shift_bits(bitset<32> ins, int start)
{
    return ((ins.to_ulong()) >> start);
}

bitset<32> Sign_extend(bitset<16> i_address)
{
    string extended_address;
    //* 若最大位數為0，則extend 16個0
    if (i_address[15] == 0)
    {
        extended_address = "0000000000000000" + i_address.to_string<char, std::string::traits_type, std::string::allocator_type>();
    }
    else
    {
        extended_address = "1111111111111111" + i_address.to_string<char, std::string::traits_type, std::string::allocator_type>();
    }
    return (bitset<32>(extended_address));
}

int main()
{
    Register_File RF;
    Instruction_Memory IM;
    Data_Memory DM;

    State current_state;
    State next_state;

    int cycle;

    while (true)
    {
        //! IF Stage: Instruction fetch from memory
        //* IF Stage執行時
        if(current_state.IF_stage.implement)
        {
            bitset<32> instruction = IM.read(current_state.IF_stage.PC);
            next_state.ID_stage.ins = instruction;

            if(instruction.to_string<char, std::string::traits_type, std::string::allocator_type>() == "11111111111111111111111111111111")
            {
                current_state.IF_stage.implement = 0;
            }
            else
            {
                next_state.ID_stage.implement = current_state.IF_stage.implement;
            }
            
        }

        //! ID Stage: Instruction decode & register read


        //! EX Stage: Execute operation or calculate address


        //! MEM Stage: Access memory operand


        //! WB Stage: Write result back to register


        //* 儲存每個cycle結束後的狀態
        save_state(cycle, next_state); 

        //* 進入下一cycle，並更新當前狀態
        cycle = cycle + 1;
		current_state = next_state;
    }

    //* 儲存 register result
    RF.save_file();

    //* 儲存 memory result
    DM.save_file();

    return 0;
}