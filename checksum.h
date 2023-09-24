//
// Created by Kevin Luo on 9/23/23.
//

#ifndef INC_450P1_CHECKSUM_H
#define INC_450P1_CHECKSUM_H

#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <fstream>
#include <vector>

using namespace std;

int binaryToDecimal(const string& bin);
string decimalToBinary(int decimal);
bool checksum_decode(string data);
string computeChecksum(const string& data);
void printchecksumresult(string checksum, bool result);


#endif //INC_450P1_CHECKSUM_H
