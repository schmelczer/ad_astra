#ifndef BITWISE_H
#define BITWISE_H

#define BV(x) (1 << (x))
#define modifyBit(P, B, V) ((P) = ((P) & ~BV(B)) | ((V) << B))
#define setBit(P, B) ((P) |= BV(B))
#define clearBit(P, B) ((P) &= ~BV(B))
#define toggleBit(P, B) ((P) ^= BV(B))
#define getBit(P, B) (((P) & BV(B)) >> (B))

#endif
