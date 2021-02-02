#include "experimentsetup.h"


// List of all detectors, sorted by the address. Needs to be edited by
// the user whenever the addresses are changed.

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


DetectorInfo_t pDetector[] =
{
    {0, f000MHz, unused, 0, 0},
    {1, f000MHz, unused, 0, 0},
    {2, f000MHz, unused, 0, 0},
    {3, f000MHz, unused, 0, 0},
    {4, f000MHz, unused, 0, 0},
    {5, f000MHz, unused, 0, 0},
    {6, f000MHz, unused, 0, 0},
    {7, f000MHz, unused, 0, 0},
    {8, f000MHz, unused, 0, 0},
    {9, f000MHz, unused, 0, 0},
    {10, f000MHz, unused, 0, 0},
    {11, f000MHz, unused, 0, 0},
    {12, f000MHz, unused, 0, 0},
    {13, f000MHz, unused, 0, 0},
    {14, f000MHz, unused, 0, 0},
    {15, f000MHz, unused, 0, 0},
    {16, f000MHz, unused, 0, 0},
    {17, f000MHz, unused, 0, 0},
    {18, f000MHz, unused, 0, 0},
    {19, f000MHz, unused, 0, 0},
    {20, f000MHz, unused, 0, 0},
    {21, f000MHz, unused, 0, 0},
    {22, f000MHz, unused, 0, 0},
    {23, f000MHz, unused, 0, 0},
    {24, f000MHz, unused, 0, 0},
    {25, f000MHz, unused, 0, 0},
    {26, f000MHz, unused, 0, 0},
    {27, f000MHz, unused, 0, 0},
    {28, f000MHz, unused, 0, 0},
    {29, f000MHz, unused, 0, 0},
    {30, f000MHz, unused, 0, 0},
    {31, f000MHz, unused, 0, 0},
    {32, f500MHz, labr, 0, 0},
    //{33, f500MHz, labr, 1, 0},
    {33, f500MHz, unused, 0, 0},
    {34, f500MHz, labr, 2, 0},
    {35, f500MHz, labr, 3, 0},
    {36, f500MHz, labr, 4, 0},
    {37, f500MHz, labr, 5, 0},
    {38, f500MHz, labr, 6, 0},
    {39, f500MHz, labr, 7, 0},
    {40, f500MHz, labr, 8, 0},
    {41, f500MHz, labr, 9, 0},
    {42, f500MHz, labr, 10, 0},
    {43, f500MHz, labr, 11, 0},
    {44, f500MHz, labr, 12, 0},
    {45, f500MHz, labr, 13, 0},
    {46, f500MHz, labr, 14, 0},
    {47, f500MHz, labr, 15, 0},
    {48, f500MHz, labr, 16, 0},
    {49, f500MHz, labr, 17, 0},
    {50, f500MHz, labr, 18, 0},
    {51, f500MHz, labr, 19, 0},
    {52, f500MHz, labr, 20, 0},
    {53, f500MHz, labr, 21, 0},
    {54, f500MHz, labr, 22, 0},
    {55, f500MHz, labr, 23, 0},
    {56, f500MHz, labr, 24, 0},
    {57, f500MHz, labr, 25, 0},
    {58, f500MHz, labr, 26, 0},
    {59, f500MHz, labr, 27, 0},
    {60, f500MHz, labr, 28, 0},
    {61, f500MHz, labr, 29, 0},
    {62, f500MHz, labr, 30, 0},
    {63, f500MHz, labr, 31, 0},
    //{64, f250MHz, deDet, 0, 0},
    {64, f250MHz, labr, 1, 0},
    {65, f250MHz, deDet, 1, 0},
    {66, f250MHz, deDet, 2, 0},
    {67, f250MHz, deDet, 3, 0},
    {68, f250MHz, deDet, 4, 0},
    {69, f250MHz, deDet, 5, 0},
    {70, f250MHz, deDet, 6, 0},
    {71, f250MHz, deDet, 7, 0},
    {72, f250MHz, deDet, 0, 1},
    {73, f250MHz, deDet, 1, 1},
    {74, f250MHz, deDet, 2, 1},
    {75, f250MHz, deDet, 3, 1},
    {76, f250MHz, deDet, 4, 1},
    {77, f250MHz, deDet, 5, 1},
    {78, f250MHz, deDet, 6, 1},
    {79, f250MHz, deDet, 7, 1},
    {80, f250MHz, deDet, 0, 2},
    {81, f250MHz, deDet, 1, 2},
    {82, f250MHz, deDet, 2, 2},
    {83, f250MHz, deDet, 3, 2},
    {84, f250MHz, deDet, 4, 2},
    {85, f250MHz, deDet, 5, 2},
    {86, f250MHz, deDet, 6, 2},
    {87, f250MHz, deDet, 7, 2},
    {88, f250MHz, deDet, 0, 3},
    {89, f250MHz, deDet, 1, 3},
    {90, f250MHz, deDet, 2, 3},
    {91, f250MHz, deDet, 3, 3},
    {92, f250MHz, deDet, 4, 3},
    {93, f250MHz, deDet, 5, 3},
    {94, f250MHz, deDet, 6, 3},
    {95, f250MHz, deDet, 7, 3},
    {96, f250MHz, deDet, 0, 4},
    {97, f250MHz, deDet, 1, 4},
    {98, f250MHz, deDet, 2, 4},
    {99, f250MHz, deDet, 3, 4},
    {100, f250MHz, deDet, 4, 4},
    {101, f250MHz, deDet, 5, 4},
    {102, f250MHz, deDet, 6, 4},
    {103, f250MHz, deDet, 7, 4},
    {104, f250MHz, deDet, 0, 5},
    {105, f250MHz, deDet, 1, 5},
    {106, f250MHz, deDet, 2, 5},
    {107, f250MHz, deDet, 3, 5},
    {108, f250MHz, deDet, 4, 5},
    {109, f250MHz, deDet, 5, 5},
    {110, f250MHz, deDet, 6, 5},
    {111, f250MHz, deDet, 7, 5},
    {112, f250MHz, deDet, 0, 6},
    {113, f250MHz, deDet, 1, 6},
    {114, f250MHz, deDet, 2, 6},
    {115, f250MHz, deDet, 3, 6},
    {116, f250MHz, deDet, 4, 6},
    {117, f250MHz, deDet, 5, 6},
    {118, f250MHz, deDet, 6, 6},
    {119, f250MHz, deDet, 7, 6},
    {120, f250MHz, deDet, 0, 7},
    {121, f250MHz, deDet, 1, 7},
    {122, f250MHz, deDet, 2, 7},
    {123, f250MHz, deDet, 3, 7},
    {124, f250MHz, deDet, 4, 7},
    {125, f250MHz, deDet, 5, 7},
    {126, f250MHz, deDet, 6, 7},
    {127, f250MHz, deDet, 7, 7},
    {128, f250MHz, eGuard, 0, 0},
    {129, f250MHz, eDet, 0, 0},
    {130, f250MHz, eGuard, 1, 0},
    {131, f250MHz, eDet, 1, 0},
    {132, f250MHz, eGuard, 2, 0},
    {133, f250MHz, eDet, 2, 0},
    {134, f250MHz, eGuard, 3, 0},
    {135, f250MHz, eDet, 3, 0},
    {136, f250MHz, eGuard, 4, 0},
    {137, f250MHz, eDet, 4, 0},
    {138, f250MHz, eGuard, 5, 0},
    {139, f250MHz, eDet, 5, 0},
    {140, f250MHz, eGuard, 6, 0},
    {141, f250MHz, eDet, 6, 0},
    {142, f250MHz, eGuard, 7, 0},
    {143, f250MHz, eDet, 7, 0},
    {144, f000MHz, unused, 0, 0},
    {145, f000MHz, unused, 0, 0},
    {146, f000MHz, unused, 0, 0},
    {147, f000MHz, unused, 0, 0},
    {148, f000MHz, unused, 0, 0},
    {149, f000MHz, unused, 0, 0},
    {150, f000MHz, unused, 0, 0},
    {151, f000MHz, unused, 0, 0},
    {152, f000MHz, unused, 0, 0},
    {153, f000MHz, unused, 0, 0},
    {154, f000MHz, unused, 0, 0},
    {155, f000MHz, unused, 0, 0},
    {156, f000MHz, unused, 0, 0},
    {157, f000MHz, unused, 0, 0},
    {158, f000MHz, unused, 0, 0},
    {159, f000MHz, unused, 0, 0},
    {160, f000MHz, unused, 0, 0},
    {161, f000MHz, unused, 0, 0},
    {162, f000MHz, unused, 0, 0},
    {163, f000MHz, unused, 0, 0},
    {164, f000MHz, unused, 0, 0},
    {165, f000MHz, unused, 0, 0},
    {166, f000MHz, unused, 0, 0},
    {167, f000MHz, unused, 0, 0},
    {168, f000MHz, unused, 0, 0},
    {169, f000MHz, unused, 0, 0},
    {170, f000MHz, unused, 0, 0},
    {171, f000MHz, unused, 0, 0},
    {172, f000MHz, unused, 0, 0},
    {173, f000MHz, unused, 0, 0},
    {174, f000MHz, unused, 0, 0},
    {175, f000MHz, unused, 0, 0},
};


DetectorInfo_t GetDetector(uint16_t address)
{
    return (address < TOTAL_NUMBER_OF_ADDRESSES) ? pDetector[address] : pDetector[0];
}

enum ADCSamplingFreq GetSamplingFrequency(uint16_t address)
{
    return (address < TOTAL_NUMBER_OF_ADDRESSES) ? pDetector[address].sfreq : f000MHz;
}

#ifdef __cplusplus
}
#endif // __cplusplus
