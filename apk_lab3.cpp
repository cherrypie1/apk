#include <dos.h>
#include<stdio.h>
#include<stdlib.h>
int attribute = 0x00; //��������� ����

struct VIDEO
{
    unsigned char symb;
    unsigned char attr;
};

void get_reg();         			//�������� ������ �� ���������
void print(int offset, int);		//��������� ��������� ����������������

//IRQ8-15
void interrupt(*int0) (...);// IRQ 8  - ���� ��������� �������
void interrupt(*int1) (...);// IRQ 9  - ���������
void interrupt(*int2) (...);// IRQ 10 - ���������� �������������
void interrupt(*int3) (...);// IRQ 11 - ���������
void interrupt(*int4) (...);// IRQ 12 - ���� PS/2
void interrupt(*int5) (...);// IRQ 13 - �������������� �����������
void interrupt(*int6) (...);// IRQ 14 - ������ ���������� ������� �����
void interrupt(*int7) (...);// IRQ 15 - ������ ���������� ������� �����

//IRQ0-7
void interrupt(*int8) (...);// IRQ 0  - ������
void interrupt(*int9) (...);// IRQ 1  - ����������
void interrupt(*intA) (...);// IRQ 2  - ��������� ������������ ������� �����������
void interrupt(*intB) (...);// IRQ 3  - ���������������� ���� COM 2
void interrupt(*intC) (...);// IRQ 4  - ���������������� ���� COM 1
void interrupt(*intD) (...);// IRQ 5  - ������������ ���� LPT 2
void interrupt(*intE) (...);// IRQ 6  - ���������� ������-��������
void interrupt(*intF) (...);// IRQ 7  - ������������ ���� LPT 1

// new interrupt handlers

void interrupt  new0(...) { get_reg(); int0(); } //int0
void interrupt  new1(...) { get_reg(); int1(); } //int1
void interrupt  new2(...) { get_reg(); int2(); } //int2
void interrupt  new3(...) { get_reg(); int3(); } //int3
void interrupt  new4(...) { get_reg(); int4(); } //int4
void interrupt  new5(...) { get_reg(); int5(); } //int5
void interrupt  new6(...) { get_reg(); int6(); } //int6
void interrupt  new7(...) { get_reg(); int7(); } //int7

void interrupt  new8(...) { get_reg(); int8(); } //int8
void interrupt  new9(...) { get_reg(); int9(); }	//keyboard int9
void interrupt  newA(...) { get_reg(); intA(); } //intA
void interrupt  newB(...) { get_reg(); intB(); } //intB
void interrupt  newC(...) { get_reg(); intC(); } //intC
void interrupt  newD(...) { get_reg(); intD(); } //intD
void interrupt  newE(...) { get_reg(); intE(); } //intE
void interrupt  newF(...) { get_reg(); intF(); } //intF

//��������� ��������� ����������������
void print(int offset, int val)
{
    char temp; //������� ���
    int i;
    VIDEO far* screen = (VIDEO far*)MK_FP(0xB800, 0); //�������� ����������
    screen += offset;
    for (i = 7; i >= 0; i--)        //�������� �� �������� � ���� ���
    {
        temp = val % 2;   			//�������� ��������� ���
        val /= 2;         			//fill the screen
        screen->symb = temp + '0';  //��������� ��� ��� ������('0' ��� '1')
        //screen->attr = attribute;   //������������� ����
        screen++; 					//� ���������� �������
    }
}


//�������� ������ �� ���������
void get_reg()
{
    int reg;
    //������� ����������
    reg = inp(0x21);  	//�������� ������� ����� �������� �����������
    print(0, reg);

    outp(0x20, 0x0B);	//������������ �� ������� ������������ �������� �����������
    reg = inp(0x20);	//�������� ������� ������������ �������� �����������
    print(9, reg);

    outp(0x20, 0x0A);	//������������ �� ������� �������� �������� �����������
    reg = inp(0x20);	//�������� ������� �������� �������� �����������
    print(18, reg);

    reg = inp(0xA1);	//�������� ������� ����� �������� �����������
    print(80, reg);

    outp(0xA0, 0x0B);	//������������ �� ������� ������������ �������� �����������
    reg = inp(0xA0);	//�������� ������� ������������ �������� �����������
    print(80 + 9, reg);

    outp(0xA0, 0x0A);	//������������ �� ������� �������� �������� �����������
    reg = inp(0xA0);	//�������� ������� �������� �������� �����������
    print(80 + 18, reg);
}

void init() {

    //IRQ8-15
    int0 = getvect(0x70); // IRQ 8
    int1 = getvect(0x71); // IRQ 9
    int2 = getvect(0x72); // IRQ 10
    int3 = getvect(0x73); // IRQ 11
    int4 = getvect(0x74); // IRQ 12
    int5 = getvect(0x75); // IRQ 13
    int6 = getvect(0x76); // IRQ 14
    int7 = getvect(0x77); // IRQ 15


    //IRQ0-7
    int8 = getvect(0x08); // IRQ 0
    int9 = getvect(0x09); // IRQ 1
    intA = getvect(0x0A); // IRQ 2
    intB = getvect(0x0B); // IRQ 3
    intC = getvect(0x0C); // IRQ 4
    intD = getvect(0x0D); // IRQ 5
    intE = getvect(0x0E); // IRQ 6
    intF = getvect(0x0F); // IRQ 7

    //��������������� IRQ 8-15 (B0-B7)
    setvect(0x08, new8);
    setvect(0x09, new9);
    setvect(0x0A, newA);
    setvect(0x0B, newB);
    setvect(0x0C, newC);
    setvect(0x0D, newD);
    setvect(0x0E, newE);
    setvect(0x0F, newF);


    //��������������� IRQ 0-7 (08-0F)
    setvect(0x80, new8);
    setvect(0x81, new9);
    setvect(0x82, newA);
    setvect(0x83, newB);
    setvect(0x84, newC);
    setvect(0x85, newD);
    setvect(0x86, newE);
    setvect(0x87, newF);


    _disable();


    outp(0x20, 0x11);	//ICW1 � ������������� �������� �����������
    outp(0x21, 0x08);	//ICW2 � ������� ������ ��� ��������
    outp(0x21, 0x04);	//ICW3 � ���� ��� �������� ����������� (� �������� �������)
    outp(0x21, 0x01);	//ICW4 � �� ���������

    //������������� ���������� ��� �������� �����������
    outp(0xA0, 0x11);  	//ICW1 � ������������� �������� �����������
    outp(0xA1, 0x90);	//ICW2 � ������� ������ ��� ��������
    outp(0xA1, 0x02);	//ICW3 � ���������� ������������ ������ � ��������
    outp(0xA1, 0x01);	//ICW4 � �� ���������

    _enable();
}

int main()
{
    unsigned far* fp; //��������� ���������
    init();

    system("cls");
    printf("                           - MASTER\n");
    printf("                           - SLAVE\n");
    printf("mask     service  request\n");

    FP_SEG(fp) = _psp; //�������� �������
    FP_OFF(fp) = 0x2c; //�������� �������� �������� ������ � ����������� �����
    _dos_freemem(*fp); //������������ ������ ��� ���

    _dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);//��������� �����������, ������ �������� � ���
    //����������, ������ � ����� ������, ������� ������ ���� ��������������
    //��� ��������� ����� � ����������
    return 0;
}
