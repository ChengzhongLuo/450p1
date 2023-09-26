# include "crc.h"

char exor(char a,char b)// perform exor operation
{
    if(a==b)
        return '0';
    else
        return '1';
}

char* binaryDivision(char dividend[], char divisor[]){
    int dividendLength = strlen(dividend);//length of dividend
    int divisorLength = strlen(divisor);//length of divisor
    char* remainder = new char[64];
    char* temp = new char[64];//two char array for recording the remainder
    for(int i=0;i<divisorLength;i++){
        remainder[i] = dividend[i]; //initiate remainder equal to the left most bits of dividend, like what we do in long division
    }
    for(int j=divisorLength;j<=dividendLength;j++){
        for(int i=0;i<divisorLength;i++){
            temp[i]=remainder[i]; //store the remainder in temp to use it later as dividend
        }
        if(remainder[0]=='0') {
            for (int i = 0; i < divisorLength - 1; i++) {
                remainder[i] = temp[i + 1];  //if first bit of remainder is 0 than shift to right by 1 bit
            }
        }
        else {
            for (int i = 0; i < divisorLength - 1; i++) {
                remainder[i] = exor(temp[i + 1], divisor[i + 1]);  //exor the dividend with divisor
            }
        }
        if (j != dividendLength)
            remainder[divisorLength - 1] = dividend[j];  //appending next bit of dividend to remainder
        else
            remainder[divisorLength - 1] = '\0';
    }
    return remainder;
}

bool crc_decode(char data[], char crc[], char key[]) {

    int dataLen = strlen(data);
    int crcGenLen = strlen(key);
    char* crcCode = binaryDivision(data,key);

    for (int i = 0; i < crcGenLen - 1; i++) {
        if (crcCode[i] != '0') {
            return false; // CRC verification failed
        }
    }
    return true;
}

char* crc_encode(char data[], char key[])
{
    int dataLen = strlen(data);
    int crcGenLen = strlen(key);

    for(int i=0;i<crcGenLen-1;i++) //appending n-1 zeroes to data
        data[dataLen+i]='0';
    data[dataLen+crcGenLen-1]='\0';

    int codeLen = dataLen+crcGenLen-1; //length of appended data word

    char *crcCode;
    crcCode = binaryDivision(data, key);

    for(int i=0;i<crcGenLen-1;i++)
        data[dataLen+i]=crcCode[i]; //replace zeros with CRC
    data[codeLen]='\0';
//    for (int i= 0; i < codelen; i++)
//        cout<<data[i];
//    cout<<endl;
    return crcCode;
}

void printCrcResult(char* crc, bool result){
    int crcGenLen = strlen(crc);
    //cout<<"crcCount: "<<crcGenLen<<endl;
    if(crcGenLen==8) {
        cout << "CRC-8" << endl;
        if(result)
            cout<<"CRC: "<<crc<<"\t\t\t Result: Pass"<<endl;
        else
            cout<<"CRC: "<<crc<<"\t\t\t Result: Not Pass"<<endl;
    }
    else if (crcGenLen==16) {
        cout << "CRC-16" << endl;
        if (result)
            cout << "CRC: " << crc << "\t\t Result: Pass"<<endl;
        else
            cout << "CRC: " << crc << "\t\t Result: Not Pass"<<endl;
    }
}
