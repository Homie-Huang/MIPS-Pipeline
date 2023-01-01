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
        if (current_state.IF_stage.implement)
        {
            bitset<32> instruction = IM.read(current_state.IF_stage.PC);
            next_state.ID_stage.ins = instruction;

            //* 32bits個1的指令表示結束
            if (instruction.to_string<char, std::string::traits_type, std::string::allocator_type>() == "11111111111111111111111111111111")
            {
                current_state.IF_stage.implement = 0;
            }
            next_state.ID_stage.implement = current_state.IF_stage.implement;

            //* Load-Use Data Hazard
            if (current_state.EX_stage.type && current_state.EX_stage.MemRead) // type=1 -> I-type，MemRead只有lw指令為1
            {
                //* 判斷是否為sw指令，sw的opcode[31~26]為101011->43
                if ((bitset<6>(shift_bits(current_state.ID_stage.ins, 26))).to_ulong() == 43)
                {
                    //* 因為lw為I-type指令，因此write_register為rt
                    //* 當lw還沒從記憶體讀取data，sw便無法從rt取得資料
                    // TODO
                    if (current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // 向右移21bits再取最小的5個bit，也就是rt
                    {
                        next_state.ID_stage = current_state.ID_stage; // 維持在ID stage
                    }
                }
                else //* R-type指令
                {
                    //* 當lw還沒從記憶體讀取data，R-type指令便無法從rs和rt取得資料
                    if (current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 21)) && current_state.EX_stage.implement && bitset<6>(shift_bits(current_state.ID_stage.ins, 26)) == bitset<6>("000000")) // opcode全為0 -> R-type
                    {
                        next_state.ID_stage = current_state.ID_stage; // 維持在ID stage
                    }
                    if (current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 16)) && current_state.EX_stage.implement && bitset<6>(shift_bits(current_state.ID_stage.ins, 26)) == bitset<6>("000000")) // opcode全為0 -> R-type
                    {
                        next_state.ID_stage = current_state.ID_stage; // 維持在ID stage
                    }
                }
            }

            //* Branch Hazard
            //* beq的opcode[31~26]為000100->4，且當rs=rt時[Register comparator在ID Stage]，執行beq指令
            if (bitset<6>(shift_bits(current_state.ID_stage.ins, 26)).to_ulong() == 4 && RF.read(bitset<5>(shift_bits(current_state.ID_stage.ins, 21))).to_ulong() == RF.read(bitset<5>(shift_bits(current_state.ID_stage.ins, 16))).to_ulong())
            {
                //* 執行beq指令，則會停止下一個指令繼續執行，跳去別的指令
                if (current_state.ID_stage.implement)
                {
                    next_state.ID_stage.implement = 0;
                }
            }
        }
        else
        {
            next_state.ID_stage.implement = current_state.IF_stage.implement;
        }

        //! ID Stage: Instruction decode & register read
        //* ID Stage執行時
        if (current_state.ID_stage.implement)
        {
            bitset<6> opcode = bitset<6>(shift_bits(current_state.ID_stage.ins, 26));
            bitset<6> funct = bitset<6>(shift_bits(current_state.ID_stage.ins, 0));

            bitset<5> reg1 = bitset<5>(shift_bits(current_state.ID_stage.ins, 21));
            bitset<5> reg2 = bitset<5>(shift_bits(current_state.ID_stage.ins, 16));
            next_state.EX_stage.rs = reg1;
            next_state.EX_stage.rt = reg2;
            bitset<16> iaddr = bitset<16>(shift_bits(current_state.ID_stage.ins, 0));
            next_state.EX_stage.I_address = iaddr;
            //* 若為R-type指令: write_register = rd
            if (opcode.to_ulong() == 0)
            {
                next_state.EX_stage.Write_reg = bitset<5>(shift_bits(current_state.ID_stage.ins, 11));
                next_state.EX_stage.type = 0;
            }
            else //* 反之為I-type指令: write_register = rt
            {
                next_state.EX_stage.Write_reg = reg2;
                next_state.EX_stage.type = 1;
            }

            //* ALU input
            next_state.EX_stage.Read_data1 = RF.read(reg1);
            next_state.EX_stage.Read_data2 = RF.read(reg2);

            //* lw、sw、add -> add，sub -> sub
            if (opcode.to_ulong() == 35 || opcode.to_ulong() == 43 || funct.to_ulong() == 32)
            {
                next_state.EX_stage.ALUOp = 1;
            }
            else
            {
                next_state.EX_stage.ALUOp = 0;
            }

            //* Control Sign
            if (opcode.to_ulong() == 43 || funct.to_ulong() == 32) // sw、beq
            {
                next_state.EX_stage.RegDst = 'X';
                next_state.EX_stage.MemtoReg = 'X';
                next_state.EX_stage.RegWrite = 0;
            }
            else if (opcode.to_ulong() == 35) // lw
            {
                next_state.EX_stage.RegDst = '0';
                next_state.EX_stage.MemtoReg = '1';
                next_state.EX_stage.RegWrite = 1;
            }
            else // R-type
            {
                next_state.EX_stage.RegDst = '1';
                next_state.EX_stage.MemtoReg = '0';
                next_state.EX_stage.RegWrite = 1;
            }
            next_state.EX_stage.ALUSrc = (opcode.to_ulong() == 35 || opcode.to_ulong() == 43) ? 1 : 0;
            next_state.EX_stage.Branch = (opcode.to_ulong() == 4) ? 1 : 0;
            next_state.EX_stage.MemRead = (opcode.to_ulong() == 35) ? 1 : 0;
            next_state.EX_stage.MemWrite = (opcode.to_ulong() == 43) ? 1 : 0;
            //? newState.EX.wrt_enable = (IsStore.to_ulong() || IsBranch.to_ulong() || JType.to_ulong()) ? 0 : 1;

            next_state.EX_stage.implement = current_state.ID_stage.implement;

            //* Load-Use Data Hazard
            if (current_state.EX_stage.type && current_state.EX_stage.MemRead) // type=1 -> I-type，MemRead只有lw指令為1
            {
                //* 判斷是否為sw指令，sw的opcode[31~26]為101011->43
                if ((bitset<6>(shift_bits(current_state.ID_stage.ins, 26))).to_ulong() == 43)
                {
                    //* 因為lw為I-type指令，因此write_register為rt
                    //* 當lw還沒從記憶體讀取data，sw便無法從rt取得資料
                    // TODO
                    if (current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // 向右移21bits再取最小的5個bit，也就是rt
                    {
                        next_state.EX_stage.implement = 0; // 維持在ID stage
                    }
                }
                else //* R-type指令
                {
                    //* 當lw還沒從記憶體讀取data，R-type指令便無法從rs和rt取得資料
                    if (current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 21)) && current_state.EX_stage.implement && bitset<6>(shift_bits(current_state.ID_stage.ins, 26)) == bitset<6>("000000")) // opcode全為0 -> R-type
                    {
                        next_state.EX_stage.implement = 0; // 維持在ID stage
                    }
                    if (current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 16)) && current_state.EX_stage.implement && bitset<6>(shift_bits(current_state.ID_stage.ins, 26)) == bitset<6>("000000")) // opcode全為0 -> R-type
                    {
                        next_state.EX_stage.implement = 0; // 維持在ID stage
                    }
                }
            }

            //TODO
            //* Branch Hazard
            //* beq的opcode[31~26]為000100->4，且當rs=rt時[Register comparator在ID Stage]，執行beq指令
            if (opcode.to_ulong() == 4)
            {
                next_state.EX_stage.Write_reg = bitset<5>("00000");
            }
            if (opcode.to_ulong() == 4 && RF.read(reg1).to_ulong() != RF.read(reg2).to_ulong())
            {
                current_state.IF_stage.PC = bitset<32>(current_state.IF_stage.PC.to_ulong() - 4);
            }
        }
        else
        {
            next_state.EX_stage.implement = current_state.ID_stage.implement;
        }

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