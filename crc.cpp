# include "crc.h"

char exor(char a,char b)// perform exor operation
{
    if(a==b)
        return '0';
    else
        return '1';
}

bool crc_decode(char data[], char crc[], char key[]) {
    int dataLen = strlen(data);
    int crcGenLen = strlen(key);

    char temp[128], rem[128];

    for (int i = 0; i < crcGenLen; i++)
        rem[i] = data[i];

    for (int j = crcGenLen; j <= dataLen; j++) {
        for (int i = 0; i < crcGenLen; i++)
            temp[i] = rem[i];                // remainder of previous step is divident of current step

        if (rem[0] == '0')                //if 1's bit of remainder is 0 then shift the rem by 1 bit
        {
            for (int i = 0; i < crcGenLen - 1; i++)
                rem[i] = temp[i + 1];
        } else                    //else exor the divident with generator key
        {
            for (int i = 0; i < crcGenLen - 1; i++)
                rem[i] = exor(temp[i + 1], key[i + 1]);

        }
        if (j != dataLen)
            rem[crcGenLen - 1] = data[j];        //appending next bit of data to remainder obtain by division
        else
            rem[crcGenLen - 1] = '\0';
    }
    for (int i= 0; i < crcGenLen - 1; i++)
        cout<<rem[i];
    cout<<endl;
    for (int i = 0; i < crcGenLen - 1; i++) {
        if (rem[i] != '0') {
            return false; // CRC verification failed
        }
    }
    return true;
}

char* crc_encode(char data[], char key[])
{
    int dataLen = strlen(data);
    int crcGenLen = strlen(key);

    for(int i=0;i<crcGenLen-1;i++)                //appending n-1 zeroes to data
        data[dataLen+i]='0';
    data[dataLen+crcGenLen-1]='\0';

    int codelen = dataLen+crcGenLen-1;                //lenght of appended data word

    char* rem = new char[128];
    char temp[128];

    for(int i=0;i<crcGenLen;i++)
        rem[i]=data[i];                    //considering n bits of data for each step of binary division/EXOR

    for(int j=crcGenLen;j<=codelen;j++)
    {
        for(int i=0;i<crcGenLen;i++)
            temp[i]=rem[i];                // remainder of previous step is divident of current step

        if(rem[0]=='0')                //if 1's bit of remainder is 0 then shift the rem by 1 bit
        {
            for(int i=0;i<crcGenLen-1;i++)
                rem[i]=temp[i+1];
        }
        else                    //else exor the divident with generator key
        {
            for(int i=0;i<crcGenLen-1;i++)
                rem[i]=exor(temp[i+1],key[i+1]);

        }
        if(j!=codelen)
            rem[crcGenLen-1]=data[j];        //appending next bit of data to remainder obtain by division
        else
            rem[crcGenLen-1]='\0';
    }

    for(int i=0;i<crcGenLen-1;i++)
        data[dataLen+i]=rem[i];                //replace n-1 zeros with n-1 bit CRC
    data[codelen]='\0';
//    for (int i= 0; i < codelen; i++)
//        cout<<data[i];
//    cout<<endl;
    return rem;
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
