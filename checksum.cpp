//
// Created by Kevin Luo on 9/23/23.
//


#include "checksum.h"



int binaryToDecimal(const string& bin) {
    int decimal = 0;
    for (int i = 0; i < bin.size(); ++i) {
        decimal = decimal * 2 + (bin[i] - '0');
    }
    return decimal;
}

string decimalToBinary(int decimal) {
    string binary = "";
    for (int i = 0; i < 8; ++i) {
        binary.insert(binary.begin(), (decimal % 2) + '0');
        decimal /= 2;
    }
    return binary;
}

string computeChecksum(const string& data) {
    vector<string> chunks;
    int datalen = data.length();

    // Divide data into 8 chunks of 8 bits each
    for (int i = 0; i < datalen; i += 8) {
        chunks.push_back(data.substr(i, 8));
    }

    int sum = binaryToDecimal(chunks[0]);
    for (int i = 1; i < 8; ++i) {
        int current = binaryToDecimal(chunks[i]);
        sum += current;
    }

    while (sum > 255) {
        sum -= 255;
    }


    // Convert the sum back to binary
    string binarySum = decimalToBinary(sum % 256); // Ensure it fits into 8 bits

    // Flip the bits
    for (int i = 0; i < 8; ++i) {
        binarySum[i] = (binarySum[i] == '0') ? '1' : '0';
    }

    return binarySum;
}

bool checksum_decode(string data){
    int datalen = data.length();

    string data1 = data.substr(0, datalen-8);
    string data2 = data.substr(datalen-8, datalen);
    string newchecksum = computeChecksum(data1);
    if(newchecksum == data2)
        return true;
    else
        return false;
}


void printchecksumresult(string checksum, bool result){
    cout<<"Checksum"<<endl;
    if(result)
        cout<<"Checksum: "<<checksum<<"\t\t\tResult: Pass"<<endl;
    else
        cout<<"Checksum: "<<checksum<<"\t\t\tResult: Not Pass"<<endl;
}