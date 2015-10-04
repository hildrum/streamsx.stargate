/* Copyright (C) 2015, International Business Machines Corporation  
 * All Rights Reserved  
 */
 
#include "base64.h"
namespace com_ibm_streamsx_base64 {

char mapping[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X',
    'Y','Z','a','b','c','d','e','f',
    'g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v',
    'w','x','y','z','0','1','2','3',
    '4', '5','6','7','8','9','+','/'};

int decodeChar(char c) {
   if (c >= 'A' && c <= 'Z') {
    return c - 'A';
   }
    else if (c >= 'a' && c <= 'z') {
     return c - 'a' + 26;
    }
   else if (c >= '0' && c <= '9') {
     return c - '0' + 26 + 26;
   }
   else if (c == '+') {
     return 62;
   }
   else if (c == '/')  {
     return 63;
   }
   else if (c == '=') {
     return 0;
   }
   else {
	   return -1;
   }
}

// Assumes output is 3/4 of input.
int _base64_decode(const unsigned char * input, int inputLen, unsigned char * output) {
    int inputPos = 0;
    int outputPos = 0;
    while (inputPos +3 < inputLen) {
        unsigned int collectThemAll = 0;
        for (int i = 0; i < 4; i++) {
            int decoded = decodeChar(input[inputPos +i]);
            if (decoded < 0) {
                SPLAPPTRC(L_ERROR,"Char " << input[inputPos+i] << " at position " << inputPos +i << " failed to decode","curlForStreams");
              return -1;
            }
            collectThemAll = (collectThemAll << 6) | decoded;
        }
        unsigned int byte1 = collectThemAll >> 16;
        unsigned int byte2 = (collectThemAll >> 8) & 0xff;
        unsigned int byte3 = collectThemAll & 0xff;
        output[outputPos++] = byte1;
        output[outputPos++] = byte2;
        output[outputPos++] = byte3;
        inputPos +=4;
    }
    if (input[inputLen-1] != '=') {
        return outputPos;
    }
    else if (input[inputLen-2] != '=') {
        return outputPos -1;
    }
    else {
        return outputPos -2;
    }
}

// output had better be long enough, or else
int _base64_encode(const unsigned char * input, int inputLen, unsigned char * output) {

    int pos = 0;
    int outputPos = 0;
    while (pos +2 < inputLen) {
        int firstChar = (input[pos]>>2) & 0x3f;
        // Need bottom two bits of the first, moved up for places, plus top for of second
        int secondChar = ((input[pos] << 4) | (input[pos+1] >> 4))& 0x3f;
        // need bottom four of the second, moved up two places, an dtop 2 of third.
        int thirdChar = ((input[pos+1] << 2) | (input[pos+2] >> 6)) & 0x3f;
        // bottom six of the last.
        int fourthChar = input[pos+2] & 0x3f;
        output[outputPos++] = mapping[firstChar];
        output[outputPos++] = mapping[secondChar];
        output[outputPos++] = mapping[thirdChar];
        output[outputPos++] = mapping[fourthChar];
        pos += 3;
    }
    // next char's always the same.
    if (inputLen % 3 != 0) {
        output[outputPos++] = mapping[ input[pos] >> 2 ];
        if (inputLen % 3 == 2) {
        	output[outputPos++] = mapping[ (input[pos] << 4 | (input[pos+1] >> 4)) & 0x3f];
        	output[outputPos++] = mapping[ (input[pos+1] << 2)&0x3f];
        	output[outputPos++] = '=';
        }
        else  {
        	output[outputPos++] = mapping[(input[pos] << 4) & 0x3f];
        	output[outputPos++] = '=';
        	output[outputPos++] = '=';
        }
    }

    return outputPos;
}

SPL::rstring base64Encode(const SPL::rstring & input, SPL::int32 & error) {
    unsigned char buffer[input.length()*4/3 + 3];
    
    int len = _base64_encode((unsigned char*)input.c_str(),input.length(),buffer);
    if (len < 0) {
        error = len;
        return "";
    }
    else {
        SPL::rstring toReturn((char*)(&buffer[0]),len);
        return toReturn;
    }

}

SPL::rstring base64DecodeToRstring(const SPL::rstring & encoded, SPL::int32 & error) {
    unsigned char buffer[encoded.length()/4 * 3];
    if (encoded.length() % 4 !=0) {
        SPLAPPTRC(L_ERROR,"Encoded length is not a multiple of four","curlForStreams");
        error = -1;
        return "";
    }
    int len = _base64_decode((unsigned char*)encoded.c_str(),encoded.length(), buffer);
    if (len < 0) {
        SPLAPPTRC(L_ERROR,"Error in decode","curlForStreams");
        error = len;
        return "";
    }
    else {
        SPL::rstring aString((char*)(&buffer[0]),len);
        return aString;
    }

}


/*
void testEncoding(std::string input) {
    unsigned char buffer[100];
    unsigned char outbuffer[100];
    int encodedLen = base64_encode((const unsigned char*)input.c_str(),input.length(), buffer);
    std::string myString((char*)&(buffer[0]),encodedLen);
    int decodedLen = base64_decode((const unsigned char*)myString.c_str(),myString.length(),outbuffer);
    std::string outString((char*)&(outbuffer[0]),decodedLen);
    std::cout << input << " -> " << myString << " -> " << outString << std::endl;
}

int main() {
    testEncoding("This is a test\n");
    testEncoding("This is a test");
    testEncoding("This is a tes");
    testEncoding("This is a te");
}
*/

}
