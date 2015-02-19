#pragma once

class MachineCode;
class Argument;
class LabelTable;

void myNegInstruction(MachineCode&);
void myCplInstruction(MachineCode&);
void myLdInstruction(const Argument&, const Argument&, MachineCode&, const LabelTable&);
void outInstruction(const Argument& arg1, MachineCode& code, const LabelTable&);
void jpInstruction(const Argument& arg1, MachineCode& code, const LabelTable&);
