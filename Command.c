#include "Command.h"
#include "UART.h"
#include <stdio.h>

#define FALSE -1
#define TRUE 0

void TurnOnLED(int fd){
    int ret;
    char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x24;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    send_buf[6] = 0x01;
    send_buf[22] = 0x27;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
}

void TurnOffLED(int fd){
    int ret;
    char send_buf[24];
    char rcv_buf[24];
    int i = 0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x24;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    send_buf[22] = 0x26;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
}

void Identify(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    unsigned char rcv_buf1[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x02;
    send_buf[3] = 0x01;
    send_buf[22] = 0x02;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    printf("Please input your finger.\n");
    ret = UART_Recv(fd,rcv_buf,24);
    ret = UART_Recv(fd,rcv_buf1,24);
    if(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x12) printf("ERR_IDENTIFY\n");
        else if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
    }
    else{
        order = rcv_buf1[9]*256+rcv_buf1[8];
        printf("The order is %d\n",order);
    }
}

void Verify(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    unsigned char rcv_buf1[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x01;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the template number: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>3000 || tem<1){
        printf("Please input a template number between 1 and 3000\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        printf("ERR_TMPL_EMPTY\n");
        return;
    }
    printf("Please input your finger.\n");
    ret = UART_Recv(fd,rcv_buf1,24);
    if(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x11) printf("ERR_VERIFY\n");
        else if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        else printf("ERR_VERIFY\n");
    }
    else{
        printf("success\n");
        order = rcv_buf1[9]*256+rcv_buf1[8];
        printf("The order is %d\n",order);
    }
}

void Enroll(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    unsigned char rcv_buf1[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x03;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the template number: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>3000 || tem<1){
        printf("Please input a template number between 1 and 3000\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        printf("ERR_TMPL_NOT_EMPTY\n");
        return;
    }
    ret = UART_Recv(fd,rcv_buf1,24);
    printf("First finger!\n");
    while(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        ret = UART_Recv(fd,rcv_buf1,24);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    printf("Second finger!\n");
    ret = UART_Recv(fd,rcv_buf1,24);
    while(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        ret = UART_Recv(fd,rcv_buf1,24);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    printf("Third finger!\n");
    ret = UART_Recv(fd,rcv_buf1,24);
    while(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        ret = UART_Recv(fd,rcv_buf1,24);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        if(rcv_buf[8] == 0x19){
            printf("ERR_DUPLICATION_ID\n");
            order = rcv_buf[11]*256+rcv_buf[10];
            printf("The order is %d\n",order);
        }
        else if(rcv_buf[8] == 0x30){
            printf("ERR_GENERALIZE\n");
        }
    }
    else{
        printf("Success\n");
        order = rcv_buf[9]*256+rcv_buf[8];
        printf("The order is %d\n",order);
    }
}

void EnrollOneTime(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    unsigned char rcv_buf1[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x04;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the template number: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>3000 || tem<1){
        printf("Please input a template number between 1 and 3000\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        printf("ERR_TMPL_NOT_EMPTY\n");
        return;
    }
    ret = UART_Recv(fd,rcv_buf1,24);
    //printf("First finger!\n");
    if(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        else if(rcv_buf1[8] == 0x19){
            printf("ERR_DUPLICATION_ID\n");
            order = rcv_buf1[11]*256+rcv_buf1[10];
            printf("The order is %d\n",order);
        }
    }
    else{
        printf("success\n");
        order = rcv_buf1[9]*256+rcv_buf1[8];
        printf("The order is %d\n",order);
    }
}

void ClearTemplate(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x05;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the template number: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>3000 || tem<1){
        printf("Please input a template number between 1 and 3000\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        printf("ERR_TMPL_EMPTY\n");
    }
    else{
        printf("Success to delete template %d!\n",tem);
    }
}

void ClearAllTemplate(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x06;
    send_buf[3] = 0x01;
    send_buf[22] = 0x06;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        printf("success\n");
        order = rcv_buf[9]*256+rcv_buf[8];
        printf("%d templates have been deleted!\n",order);
    }
}

void GetEmptyID(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x07;
    send_buf[3] = 0x01;
    send_buf[22] = 0x07;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        printf("success\n");
        order = rcv_buf[9]*256+rcv_buf[8];
        printf("Template %d is empty.\n",order);
    }
    else{printf("ERR_EMPTY_ID_NPEXIST\n");}
}

void GetTemplateStatus(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x08;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the template number: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>3000 || tem<1){
        printf("Please input a template number between 1 and 3000\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6]==0x00){
        if(rcv_buf[8]==0x01) printf("GD_TEMPLATE_NOT_EMPTY\n");
        else printf("GD_TEMPLATE_EMPTY\n");
    }
}

void GetBrokenTemplate(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x09;
    send_buf[3] = 0x01;
    send_buf[22] = 0x09;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        order = rcv_buf[9]*256+rcv_buf[8];
        printf("%d templates are broken\n",order);
        order = rcv_buf[11]*256+rcv_buf[10];
        printf("the first broken temple is %d\n",order);
    }
}

void ReadTemplate(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24],rcv_buf1[510];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x0A;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the template number: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>3000 || tem<1){
        printf("Please input a template number between 1 and 3000\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        printf("ERR_EMPTY_TMPL\n");
    }
    else{
        ret = UART_Recv(fd,rcv_buf1,510);
        order = rcv_buf1[9]*256+rcv_buf1[8];
        printf("read template %d:\n",order);
        for(i=10;i<508;i++) printf("%x ",rcv_buf1[i]);
        printf("\n");
    }
}

void WriteTemplate(int fd){}

void SetSecurityLevel(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x0C;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the security level: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>5 || tem<1){
        printf("Please input a number between 1 and 5\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00) printf("Success, the security level is %d\n",tem);
    else printf("failure\n");
}

void GetSecurityLevel(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x0D;
    send_buf[3] = 0x01;
    send_buf[22] = 0x0D;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        order = rcv_buf[9]*256+rcv_buf[8];
        printf("security level is %d\n",order);
    }
}

void SetFingerTimeOut(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x0E;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the time out: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>60 || tem<1){
        printf("Please input a number between 1 and 60\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00) printf("Success, time  out is %d\n",tem);
    else printf("failure\n");
}

void GetFingerTimeOut(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x0F;
    send_buf[3] = 0x01;
    send_buf[22] = 0x0F;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        order = rcv_buf[9]*256+rcv_buf[8];
        printf("time out is %d\n",order);
    }
}

void SetDeviceID(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x10;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the device id: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>254 || tem<1){
        printf("Please input a number between 1 and 254\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00) printf("Success, the device id is %d\n",tem);
    else printf("failure\n");
}

void GetDeviceID(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x11;
    send_buf[3] = 0x01;
    send_buf[22] = 0x11;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        order = rcv_buf[8]*256+rcv_buf[7]; //unknow mistake
        printf("the device id is %d\n",order);
    }
}

void GetFWVersion(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x12;
    send_buf[3] = 0x01;
    send_buf[22] = 0x12;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        printf("the version is %d.%d\n",rcv_buf[8],rcv_buf[9]);
    }
}

//some mistakes
void FingerDetect(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    TurnOnLED(fd);
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x13;
    send_buf[3] = 0x01;
    send_buf[22] = 0x13;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    sleep(3);
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        if(rcv_buf[8] == 0x01) printf("exist finger\n");
        else printf("no finger\n");
    }
    TurnOffLED(fd);
}

void SetBaudrate(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x14;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the baudrate index(1-9600 2-19200 3-38400 4-57600 5-115200): ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>5 || tem<1){
        printf("Please input a number between 1 and 5\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00) printf("Success, the baudrate index is %d\n",tem);
    else printf("failure\n");
}

void SetDuplicationCheck(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x15;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the baudrate index(0-not check; 1-check): ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>1 || tem<0){
        printf("Please input a number 0 or 1\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00) printf("Success\n");
    else printf("failure\n");
}

void GetDuplicationCheck(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x16;
    send_buf[3] = 0x01;
    send_buf[22] = 0x16;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        if(rcv_buf[8] == 0x01) printf("check\n");
        else printf("not check\n");
    }
}

void EnterStandbyMode(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x17;
    send_buf[3] = 0x01;
    send_buf[22] = 0x17;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        if(rcv_buf[8] == 0x01) printf("success\n");
        else printf("fail\n");
    }
}

void GetDeviceName(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x21;
    send_buf[3] = 0x01;
    send_buf[22] = 0x21;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        for(i=8;i<18;i++) printf("%c",rcv_buf[i]);
        printf("\n");
    }
}

void IdentifyFree(int fd){}

void SetDevicePassword(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x26;
    send_buf[3] = 0x01;
    send_buf[4] = 0x0E;
    send_buf[5] = 0x00;
    printf("Please input 14 bytes passwords: ");
    for(int i=6;i<20;i++)
        scanf("%c",&rcv_buf[i]);
    printf("\n");
    for(i=0;i<20;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00) printf("Success\n");
    else printf("failure\n");
}

void VerifyDevicePassword(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x27;
    send_buf[3] = 0x01;
    send_buf[4] = 0x0E;
    send_buf[5] = 0x00;
    printf("Please input 14 bytes passwords: ");
    for(int i=6;i<20;i++)
        scanf("%c",&rcv_buf[i]);
    printf("\n");
    for(i=0;i<20;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00) printf("Success\n");
    else printf("failure\n");
}

void GetEnrollCount(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x28;
    send_buf[3] = 0x01;
    send_buf[22] = 0x28;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        order = rcv_buf[9]*256+rcv_buf[8];
        printf("the number of templates is:%d\n",order);
    }
}

void ChangeTemplate(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    unsigned char rcv_buf1[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x29;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the template number: ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>3000 || tem<1){
        printf("Please input a template number between 1 and 3000\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    ret = UART_Recv(fd,rcv_buf1,24);
    printf("First finger!\n");
    while(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        ret = UART_Recv(fd,rcv_buf1,24);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    printf("Second finger!\n");
    ret = UART_Recv(fd,rcv_buf1,24);
    while(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        ret = UART_Recv(fd,rcv_buf1,24);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    printf("Third finger!\n");
    ret = UART_Recv(fd,rcv_buf1,24);
    while(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        ret = UART_Recv(fd,rcv_buf1,24);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        if(rcv_buf[8] == 0x19){
            printf("ERR_DUPLICATION_ID\n");
            order = rcv_buf[11]*256+rcv_buf[10];
            printf("The order is %d\n",order);
        }
        else if(rcv_buf[8] == 0x30){
            printf("ERR_GENERALIZE\n");
        }
    }
    else{
        printf("Success\n");
        order = rcv_buf[9]*256+rcv_buf[8];
        printf("The order is %d\n",order);
    }
}

void SetOperationMode(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    int tem,sum=0;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x1D;
    send_buf[3] = 0x01;
    send_buf[4] = 0x02;
    send_buf[5] = 0x00;
    printf("Please input the baudrate index(1-access reader; 12-command mode): ");
    scanf("%d",&tem);
    printf("\n");
    if(tem>2 || tem<1){
        printf("Please input a number 1 or 2\n");
        return;
    }
    send_buf[6] = tem%256;
    send_buf[7] = tem/256;
    for(i=0;i<8;i++){sum=sum+send_buf[i];}
    send_buf[22] = sum%256;
    send_buf[23] = sum/256;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00) printf("Success\n");
    else printf("failure\n");
}

void GetOperationMode(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x1E;
    send_buf[3] = 0x01;
    send_buf[22] = 0x1E;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        if(rcv_buf[8] == 0x01) printf("access mode\n");
        else printf("command mode\n");
    }
}

void FPCancel(int fd){}

void TestConnect(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x50;
    send_buf[3] = 0x01;
    send_buf[22] = 0x50;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x00){
        if(rcv_buf[8] == 0x00) printf("success\n");
        else printf("fail\n");
    }
}

void EnrollAndStoreinRAM(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    unsigned char rcv_buf1[24];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x18;
    send_buf[3] = 0x01;
    send_buf[22] = 0x18;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    ret = UART_Recv(fd,rcv_buf1,24);
    printf("First finger!\n");
    while(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        ret = UART_Recv(fd,rcv_buf1,24);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    printf("Second finger!\n");
    ret = UART_Recv(fd,rcv_buf1,24);
    while(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        ret = UART_Recv(fd,rcv_buf1,24);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    printf("Third finger!\n");
    ret = UART_Recv(fd,rcv_buf1,24);
    while(rcv_buf1[6] == 0x01){
        if(rcv_buf1[8] == 0x23) printf("ERR_TIME_OUT\n");
        else if(rcv_buf1[8] == 0x21) printf("ERR_BAD_QUALITY\n");
        ret = UART_Recv(fd,rcv_buf1,24);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        if(rcv_buf[8] == 0x30){
            printf("ERR_GENERALIZE\n");
        }
    }
    else{
        printf("Success\n");
    }
}

void GetEnrollData(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    unsigned char rcv_buf1[508];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x19;
    send_buf[3] = 0x01;
    send_buf[22] = 0x19;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        printf("ERR_FAIL\n");
    }
    else{
        ret = UART_Recv(fd,rcv_buf1,508);
        printf("the template is:\n");
        for(i=8;i<506;i++) printf("%x ",rcv_buf1[i]);
        printf("\n");
    }
}

void GetFeatureDataofCapturedFP(int fd){
    int ret;
    unsigned char send_buf[24];
    unsigned char rcv_buf[24];
    unsigned char rcv_buf1[508];
    int i = 0;
    int order;
    for(i=0;i<24;i++){send_buf[i]=0x00;}
    send_buf[0] = 0x55;
    send_buf[1] = 0xAA;
    send_buf[2] = 0x1A;
    send_buf[3] = 0x01;
    send_buf[22] = 0x1A;
    send_buf[23] = 0x01;
    ret = UART_Send(fd,send_buf,24);
    if(FALSE==ret){
        printf("write error!\n");
        exit(1);
    }
    ret = UART_Recv(fd,rcv_buf,24);
    if(rcv_buf[6] == 0x01){
        printf("ERR_FAIL\n");
    }
    else{
        ret = UART_Recv(fd,rcv_buf1,508);
        printf("the template is:\n");
        for(i=8;i<506;i++) printf("%x ",rcv_buf1[i]);
        printf("\n");
    }
}
