#include "bitboard.h"
#include "masks.h"
#include "types.h"

uint64_t pawnAdvanceMasks[8][2];
uint64_t neighborFileMasks[8];
uint64_t passedPawnMasks[SQUARE_NUM][2];

uint64_t kingInnerRing[SQUARE_NUM];

void initPawnAdvanceMasks() {
    for (int i = 0; i < 8; ++i) {
        uint64_t wMask = 0ull;
        for (int j = 6; j > i; --j) {
            wMask |= rankMasks[j];
        }
        pawnAdvanceMasks[i][WHITE] = wMask;

        uint64_t bMask = 0ull;
        for (int j = 1; j < i; ++j) {
            bMask |= rankMasks[j];
        }
        pawnAdvanceMasks[i][BLACK] = bMask;
    }
}

void initNeighborFileMasks() {
    for (int i = 0; i < 8; ++i) {
        neighborFileMasks[i] = fileMasks[i] | fileMasks[std::max(0, i - 1)] | fileMasks[std::min(7, i + 1)];
    }
}

void initPassedPawnMasks() {
    for (int sqr = 0; sqr < SQUARE_NUM; ++sqr) {
        int rank = sqr / 8;
	    int file = sqr % 8;
        passedPawnMasks[sqr][WHITE] = pawnAdvanceMasks[rank][WHITE] & neighborFileMasks[file];
        passedPawnMasks[sqr][BLACK] = pawnAdvanceMasks[rank][BLACK] & neighborFileMasks[file];
    }
}

void initKingInnerRing() {
    for (int sqr = 0; sqr < SQUARE_NUM; ++ sqr) {
        uint64_t kingSquare = (1ull << sqr);
	    uint64_t diagonals = ((kingSquare >> 7) & ~fileAMask) | ((kingSquare >> 9) & ~fileHMask) | ((kingSquare << 7) & ~fileHMask) | ((kingSquare << 9) & ~fileAMask);
	    uint64_t cardinals = ((kingSquare >> 1) & ~fileHMask) | ((kingSquare << 1) & ~fileAMask) | (kingSquare >> 8) | (kingSquare << 8);
	    kingInnerRing[sqr] = kingSquare | diagonals | cardinals;
    }
}

void initMasks() {
    initPawnAdvanceMasks();
    initNeighborFileMasks();
    initPassedPawnMasks();

    initKingInnerRing();
}