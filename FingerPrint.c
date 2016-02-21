#include <stdio.h>
#include "UART.h"
#include "Command.h"

#define FALSE -1
#define TRUE 0

int main(int argc, char *argv[])
{
    int fd = FALSE;
    int ret;
    if (argc != 3){
        printf("Usage: %s /dev/ttySn command\n", argv[0]);
        return FALSE;
    }
    fd = UART_Open(fd, argv[1]);
    if (FALSE == fd){
        printf("open error\n");
        exit(1);
    }
    ret = UART_Init(fd);
    if (FALSE == fd){
        printf("Set Port Error\n");
        exit(1);
    }
    //TurnOnLED(fd);

    switch(*argv[2]){
        case 'V':
            Verify(fd);
        break;
        case 'I':
            Identify(fd);
        break;
        case 'E':
            Enroll(fd);
        break;
        case 'e':
            EnrollOneTime(fd);
        break;
        case 'C':
            ClearTemplate(fd);
        break;
        case 'c':
            ClearAllTemplate(fd);
        break;
        case 'G':
            GetEmptyID(fd);
        break;
        case 'g':
            GetTemplateStatus(fd);
        break;
        case 'B':
            GetBrokenTemplate(fd);
        break;
        case 'R':
            ReadTemplate(fd);
        break;
        case 'W':
            WriteTemplate(fd);
        break;
        case 'S':
            SetSecurityLevel(fd);
        break;
        case 's':
            GetSecurityLevel(fd);
        break;
        case 'F':
            SetFingerTimeOut(fd);
        break;
        case 'f':
            GetFingerTimeOut(fd);
        break;
        case 'D':
            SetDeviceID(fd);
        break;
        case 'd':
            GetDeviceID(fd);
        break;
        case 'v':
            GetFWVersion(fd);
        break;
        case 'A':
            FingerDetect(fd);
        break;
        case 'b':
            SetBaudrate(fd);
        break;
        case 'U':
            SetDuplicationCheck(fd);
        break;
        case 'u':
            GetDuplicationCheck(fd);
        break;
        case 'a':
            EnterStandbyMode(fd);
        break;
        case 'N':
            GetDeviceName(fd);
        break;
        case 'i':
            IdentifyFree(fd);
        break;
        case 'P':
            SetDevicePassword(fd);
        break;
        case 'p':
            VerifyDevicePassword(fd);
        break;
        case 'n':
            GetEnrollCount(fd);
        break;
        case 'H':
            ChangeTemplate(fd);
        break;
        case 'M':
            SetOperationMode(fd);
        break;
        case 'm':
            GetOperationMode(fd);
        break;
        case 'T':
            TestConnect(fd);
        break;
        case 'r':
            EnrollAndStoreinRAM(fd);
        break;
        case 'J':
            GetEnrollData(fd);
        break;
        case 'j':
            GetFeatureDataofCapturedFP(fd);
        break;
        default:
            printf("%s is not a exist parameter!",argv[2]);
        break;
    }

    UART_Close(fd);
    return 0;
}
