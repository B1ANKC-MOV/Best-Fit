#include<iostream>
#include<stdlib.h>
#include <windows.h>
using namespace std;


#define MainMemory 640//�����С
#define Sleeping 0 //����״̬
#define Working 1 //����״̬
#define Success 1//���
#define Error 0 //����

typedef int Status;


typedef struct SleepingArea
{
	long blocksize;     //������С
	long blockaddress; //������ַ
	int blockstate;   //״̬
}ElemType;

typedef struct DoubleLinkNode//˫������
{
	ElemType data;
	struct DoubleLinkNode* pre;  
	struct DoubleLinkNode* next;
}

DoubleLinkNode, * DoubleLinkList;
DoubleLinkList BlockFirstNode; //�׽��
DoubleLinkList BlockLastNode; //β���

Status MainMemory_Allocation(int);//�ڴ���亯��
Status MainMemory_Recycle(int);//�ڴ���պ���
Status Best_Fit(int);//�����Ӧ�㷨����
Status Init();//��ʼ������

void Display();//չʾ������


Status Init()//��ʼ���ڴ�ռ�����
{
	BlockFirstNode = (DoubleLinkList)malloc(sizeof(DoubleLinkNode));
	BlockLastNode = (DoubleLinkList)malloc(sizeof(DoubleLinkNode));

	BlockFirstNode->pre = NULL;
	BlockFirstNode->next = BlockLastNode;
	BlockLastNode->pre = BlockFirstNode;
	BlockLastNode->next = NULL;

	BlockLastNode->data.blockaddress = 0;//��ʼ��ַΪ0
	BlockLastNode->data.blocksize = MainMemory;//��ʼ������С=�����С
	BlockLastNode->data.blockstate = Sleeping;//��ʼ״̬Ϊ����

	return Success;
}


Status MainMemory_Allocation()//��������
{
	int Allocation_Size = 0;

	HANDLE  hConsole;
	int k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	k = 15;
	SetConsoleTextAttribute(hConsole, k);
	cout << "��������Ҫ����������С(��λ:KB)��" ;
	k = 9;
	SetConsoleTextAttribute(hConsole, k);
	cin >> Allocation_Size;
	cout <<endl<< "\t\t>>>>>>>>>>���������>>>>>>>>>>>"<<endl;
	if (Allocation_Size < 0 || Allocation_Size == 0)
	{
		k = 12;
		SetConsoleTextAttribute(hConsole, k);
		cout << "\t\t[����]�����С�����ʣ������ԣ�" << endl;
		return Error;
	}

	if (Best_Fit(Allocation_Size) == Success) 
		cout << "\t\t\t[�ɹ�]����ɹ���" << endl;
	else {
		k = 12;
		SetConsoleTextAttribute(hConsole, k);
		cout << "\t\t[����]�ڴ治�㣬����ʧ�ܣ�" << endl;
	}
	return Success;
}


Status Best_Fit(int request)//�����Ӧ�㷨
{
	int lowest; //��¼��Сʣ��ռ�
	DoubleLinkList temp = (DoubleLinkList)malloc(sizeof(DoubleLinkNode));
	temp->data.blocksize = request;
	temp->data.blockstate = Working;
	DoubleLinkNode* m = BlockFirstNode->next;
	DoubleLinkNode* n = NULL; //��¼��Ѳ���λ��

	while (m){//��ʼ����С�ռ�����λ��
		if (m->data.blockstate == Sleeping && (m->data.blocksize >= request)){
			if (n == NULL){
				n = m;
				lowest = m->data.blocksize - request;
			}else if (n->data.blocksize > m->data.blocksize){
				n = m;
				lowest = m->data.blocksize - request;
			}
		}
		m = m->next;
	}

	if (n == NULL) 
		return Error;//û���ҵ����п�
	else if (n->data.blocksize == request){
		n->data.blockstate = Working;
		return Success;
	}else{
		temp->pre = n->pre;
		temp->next = n;
		temp->data.blockaddress = n->data.blockaddress;
		n->pre->next = temp;
		n->pre = temp;
		n->data.blockaddress += request;
		n->data.blocksize = lowest;
		return Success;
	}

	return Success;
}


Status MainMemory_Recycle(int flag)//�������
{
	DoubleLinkNode* n = BlockFirstNode;

	for (int i = 0; i <= flag; i++)
		if (n != NULL)
			n = n->next;
		else
			return Error;

	n->data.blockstate = Sleeping;
	//�ڴ��ͷź��ڽ��ڴ�ụ��ϲ���
	if (n->pre != BlockFirstNode && n->pre->data.blockstate == Sleeping){//��ǰ��Ŀ��п�����
		n->pre->data.blocksize += n->data.blocksize;//�ռ�����,�ϲ�Ϊһ��
		n->pre->next = n->next;//ȥ��ԭ�����ϲ���n
		n->next->pre = n->pre;
		n = n->pre;
	}if (n->next != BlockLastNode && n->next->data.blockstate == Sleeping){//�����Ŀ��п�����
		n->data.blocksize += n->next->data.blocksize;//�ռ�����,�ϲ�Ϊһ��
		n->next->next->pre = n;
		n->next = n->next->next;
	}if (n->next == BlockLastNode && n->next->data.blockstate == Sleeping){//�����Ŀ��п�����
		n->data.blocksize += n->next->data.blocksize;
		n->next = NULL;
	}

	return Success;
}


void Display()//��ʾ����������
{
	int BlockID = 0;

	HANDLE  hConsole;
	int k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	k = 10;
	SetConsoleTextAttribute(hConsole, k);
	cout << endl << "\t\t\t������������"<<endl;
	cout << "---------------------------------------------------------------------"<<endl;

	DoubleLinkNode* n = BlockFirstNode->next;

	cout << "\t������\t��ʼ��ַ\t������С\t״̬"<<endl;

	while (n)
	{
		cout << "\t " << BlockID++ << "\t ";
		cout << n->data.blockaddress << "\t\t ";
		cout << n->data.blocksize << "KB\t\t";

		if (n->data.blockstate == Sleeping) 
			cout << "����"<<endl;
		else 
			cout << "�ѷ���"<<endl;

		n = n->next;
	}
	cout << "---------------------------------------------------------------------" << endl;
}

void main(){
	HANDLE  hConsole;
	int k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	k = 14;
	SetConsoleTextAttribute(hConsole, k);
	cout << "------------------ʹ�õ��ڴ�����㷨 * �����Ӧ�㷨------------------" << endl;

	Init(); //��ʼ���ռ��
	int option;

	while (1){
		Display();
		k = 15;
		SetConsoleTextAttribute(hConsole, k);
		cout << "��������ѡ��Ĳ�����1: �����ڴ� 2: �����ڴ� 0: �˳�------:" ;
		k = 9;
		SetConsoleTextAttribute(hConsole, k);
		cin >> option;

		if (option == 1) 
			MainMemory_Allocation(); // �����ڴ�
		else if (option == 2){  // �ڴ����
			int ID;
			k = 15;
			SetConsoleTextAttribute(hConsole, k);
			cout << "����������Ҫ���յķ����ţ�";
			k = 9;
			SetConsoleTextAttribute(hConsole, k);
			cin >> ID;
			cout << endl;
			MainMemory_Recycle(ID);
		}else if (option == 0) 
			break; //�˳�
		else{//�����������
			k = 10;
			SetConsoleTextAttribute(hConsole, 12);
			cout << "[����]�������������ԣ�" << endl;
			continue;
		}
	}
}