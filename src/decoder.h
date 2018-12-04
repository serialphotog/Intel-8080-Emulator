#ifndef __ENCODER_H__
#define __ENCODER_H__

// Gets called when an unimplemented instruction is encountered
void unimplementedInstruction(CPUState *state);

// Decodes CPU instructions
int decode(CPUState *state);

#endif
