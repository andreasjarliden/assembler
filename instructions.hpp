#pragma once

class MachineCode;
class Argument;

void myNegInstruction(MachineCode&);
void myCplInstruction(MachineCode&);
void myLdInstruction(const Argument&, const Argument&, MachineCode&);
