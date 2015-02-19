#pragma once

class MachineCode;
class Argument;
class LabelTable;

void negInstruction(MachineCode&);
void cplInstruction(MachineCode&);
void ldInstruction(const Argument&, const Argument&, MachineCode&, const LabelTable&);
void outInstruction(const Argument& arg1, MachineCode& code, const LabelTable&);
void jpInstruction(const Argument& arg1, MachineCode& code, const LabelTable&);
