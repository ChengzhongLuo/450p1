//
// Created by Kevin Luo on 9/23/23.
//

#ifndef INC_450P1_CRC_H
#define INC_450P1_CRC_H

#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <fstream>
#include <vector>

using namespace std;

char exor(char a,char b);
bool crc_decode(char data[], char crc[], char key[]);
char* crc_encode(char data[], char key[]);
void printcrcresult(char* crc, bool result);




#endif //INC_450P1_CRC_H
