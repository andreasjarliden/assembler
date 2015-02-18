#pragma once

class MachineCode;
class Argument;

void myNegInstruction(MachineCode&);
void myCplInstruction(MachineCode&);
void myLdInstruction(const Argument&, const Argument&, MachineCode&);
void outInstruction(const Argument& arg1, MachineCode& code);
void jpInstruction(const Argument& arg1, MachineCode& code);
