#include <iostream>
#include "mfrc522.h"

// 우리가 사용하는 rfid = mfrc522이다.
int main(void)
{
    mfrc522 rfid(new SPI(10, 3000000)); // 왜??
    uint8_t byte;
    uint8_t str[MAX_LEN];   // uint8_t = 1byte이다. 여기에 배열값이 들어간다.
    byte = 0x55; // just to check that byte is getting loaded and printed
    delay(1500);

    // check version of the reader
    byte = rfid.mfrc522_read(VersionReg);   //mfrc522라는 칩의 버전 정보를 읽어 온다.

    printf("ver --> %0x\n", byte);
    if (byte == 0x92)   //버전정보를 92를 읽어오면 버전 1
    {
        printf("MIFARE RC522v2\r\n");
        printf("Detected\r\n");
    }
    else if (byte == 0x91 || byte == 0x90) //버전정보를 91,90을 읽어오면 버전 2
    {
        printf("MIFARE RC522v1\r\n");
        printf("Detected\r\n");
    }
    else
    {
        printf("No reader found\r\n");
    }

    // 폴링방식이다. 이제 우리가 이것을 class로 바꿔줘야 한다.
    while (1)
    {
        //1초간격으로 계속 체크해준다.
        byte = rfid.mfrc522_request(PICC_REQALL, str);  //카드가 접촉이 있나없나를 1초간격으로 체크한다.
        // 3이 들어오면 카드번호를 읽어온다. 1이 들어오면 hihi를 출력한다.
        printf("byte-->%d ", byte);
        // 카드값을 읽어오면
        if (byte == CARD_FOUND) //byte에 CARD_FOUND가 들어오면(= byte에 1이 들어오면) 카드번호를 읽어와서 출력한다.
        {
            for (int i = 0; i < MAX_LEN; i++) // MAX_LEN = 16
                str[i] = ' ';
            byte = rfid.mfrc522_get_card_serial(str);   //카드값을 확인한다.

            for (int i = 0; i < 5; i++)
                printf("%02x ", str[i]);
            printf("\n");
        }
        delay(1000);
        //카드값을 안 읽어오면
        printf("hihi\n"); //byte에 3이 들어오면 hihi를 출력한다.
    }
}
