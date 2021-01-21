/* final_hansenca.cpp
 * Implementing the Secure Hash Algorithm
 * Hansen Cabanero
 * 4/2/2019
 */

#include <iostream>
#include <cstdio>
#include <string.h>

using namespace std;

#define MAX_SIZE 1048576 //defines the max size for character buffer
#define MAX 80 //defines the max size for amount of integers in message digest loop

//function prototypes
unsigned int readFile(unsigned char*);
unsigned int calculateBlocks(unsigned int);
void convertCharArrayToIntArray(unsigned char*, unsigned int*, unsigned int);
void addBitCountToLastBlock(unsigned int*, unsigned int, unsigned int);
void computeMessageDigest(unsigned int*, unsigned int);
unsigned int f(unsigned int, unsigned int);
unsigned int k(unsigned int);
unsigned int circleLeft(unsigned int, unsigned int);

//main function
int main(void){

    //variables, arrays
    unsigned int bytes = 0;
    unsigned char buffer[MAX_SIZE] = {'\0'};
    unsigned int message[MAX_SIZE] = {0};
    unsigned int blockCount = 0;

    //reads input file and puts into buffer array
    cin >> buffer; 

    //returns the file size in bytes to a variable
    bytes = readFile(buffer);

    //returns the amount of blocks from reading the bytes
    blockCount = calculateBlocks(bytes);

    //reads from buffer, converts, and inserts into message array
    convertCharArrayToIntArray(buffer, message, bytes);

    //adds bits to last block
    addBitCountToLastBlock(message, bytes, blockCount);

    //creates message digest
    computeMessageDigest(message, blockCount);

}

//helper function used as a constant depending on which portion of the loop
unsigned int k(unsigned int t){

    unsigned int x = 0;

    if(0 <= t && t <= 19){
    
        x = 0x5A827999;

    } else if(20 <= t && t <= 39){
    
        x = 0x6ED9EBA1;

    } else if(40 <= t && t <= 59){
    
        x = 0x8F1BBCDC;

    } else if(60 <= t && t <= 79){
    
        x = 0xCA62C1D6;

    }

    return x;
}

//helper function for message digest function, producing a 32-bit integer as output
unsigned int f(unsigned int t, unsigned int B, unsigned int C, unsigned int D){

    unsigned int x = 0;

    if(0 <= t && t <= 19){
    
        x = (B & C) | ((~B) & D);

    } else if(20 <= t && t <= 39){
    
        x = B ^ C ^ D;

    } else if(40 <= t && t <= 59){
    
        x = (B & C) | (B & D) | (C & D);

    } else if(60 <= t && t <= 79){
    
        x = B ^ C ^ D;

    }

    return x;
}

//function to create message digest
void computeMessageDigest(unsigned int message[], unsigned int blockCount){

    //variables, arrays
    int count = 0;
    unsigned int words[MAX] = {0};
    unsigned int temp = 0;
    unsigned int H0 = 0x67452301;
    unsigned int H1 = 0xEFCDAB89;
    unsigned int H2 = 0x98BADCFE;
    unsigned int H3 = 0x10325476;
    unsigned int H4 = 0xC3D2E1F0;
    unsigned int A;
    unsigned int B;
    unsigned int C;
    unsigned int D;
    unsigned int E;

    //loops through each block
    for(int x = 1; x <= blockCount; x++){
        
        //inserts the integer into an array to read from current block
        while(count < 16 * x){
    
            words[count % 16] = message[count];

            //prints out each word in current block
            //printf("W[%i] = %X\n", count % 16, words[count % 16]);
            count++;

        }
    
        //process for filling the rest of the loop with an algorithm
        for(int x = 16; x < 80; x++){
    
            words[x] = circleLeft(1, words[x - 3] ^ words[x - 8] ^ words[x - 14] ^ words[x - 16]);

        }

        //giving values to variables
        A = H0;
        B = H1;
        C = H2;
        D = H3;
        E = H4;

        //loops 80 times, constantly updating the variables
        for(int x = 0; x < 80; x++){
    
            temp = circleLeft(5, A) + f(x, B, C, D) + E + words[x] + k(x);

            E = D; 
            D = C;
            C = circleLeft(30, B);
            B = A;
            A = temp;

            //prints out hex values of A, B, C, D, E in a loop
            //printf("t = %i: %X\t%X\t%X\t%X\t%X\n", x, A, B, C, D, E);

        }

        //adds the results for each block for message digest
        H0 = H0 + A;
        H1 = H1 + B;
        H2 = H2 + C;
        H3 = H3 + D;
        H4 = H4 + E;

    }

    /*
    printf("H0 = %X\n", H0);
    printf("H1 = %X\n", H1);
    printf("H2 = %X\n", H2);
    printf("H3 = %X\n", H3);
    printf("H4 = %X\n", H4);
    */

    //prints out message digest
    printf("Message digest = %X %X %X %X %X\n", H0, H1, H2, H3, H4);

}

//circular left shift operation for bits function
unsigned int circleLeft(unsigned int x, unsigned int y){

    unsigned int result = 0;

    return result = (y << x) | (y >> 32-x);

}

//converts bytes to bits, finds end of last block, and assigns size of bits to end
void addBitCountToLastBlock(unsigned int message[], unsigned int sizeOfFileInBytes, unsigned int blockCount){

    int index = 0;
    unsigned int sizeOfFileInBits = (sizeOfFileInBytes - 1) * 8;

    index = (blockCount * 16) - 1;

    message[index] = sizeOfFileInBits;

}

//reads 4 characters at a time, packs into an int, and stores into message array
void convertCharArrayToIntArray(unsigned char buffer[], unsigned int message[], unsigned int sizeOfFileInBytes){

    unsigned int i = 0;
    unsigned int size = 0;
    unsigned int temp = 0;

    size = sizeOfFileInBytes;

    for(int count = 0; count < size; count++){
    
        i = buffer[count] << 8;
        count++;
        i = i | buffer[count];
        i = i << 8;
        count++;
        i = i | buffer[count];
        i = i << 8;
        count++;
        i = i | buffer[count];

        message[temp] = i;
        temp++;

    }
}

/*
 * calculates block by dividing total bits in file by 512
 * adds one to account for one bit that is appended
 * last 64 bits are reserved, adds extra block if  final block is greater than 448 bits
 */
unsigned int calculateBlocks(unsigned int sizeOfFileInBytes){

    unsigned int blockCount = 0;

    blockCount = (((8 * sizeOfFileInBytes) + 1) / 512) + 1;

    if((((8 * sizeOfFileInBytes) + 1) % 512) > (512 - 64)){
    
        blockCount = blockCount + 1;

    }

    return blockCount;
}

//function that finds character amount from file and returns as an unsigned int
unsigned int readFile(unsigned char buffer[]){

    int size = 0;

    size = strlen((char*)buffer);

    buffer[size] = buffer[size] + 0x80;

    size = strlen((char*)buffer);

    return size;
}
