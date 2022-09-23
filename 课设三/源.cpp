#include<iostream>
#include<stdlib.h>
#include <windows.h>
using namespace std;


#define MainMemory 640//主存大小
#define Sleeping 0 //空闲状态
#define Working 1 //已用状态
#define Success 1//完成
#define Error 0 //出错

typedef int Status;


typedef struct SleepingArea
{
	long blocksize;     //分区大小
	long blockaddress; //分区地址
	int blockstate;   //状态
}ElemType;

typedef struct DoubleLinkNode//双向链表
{
	ElemType data;
	struct DoubleLinkNode* pre;  
	struct DoubleLinkNode* next;
}

DoubleLinkNode, * DoubleLinkList;
DoubleLinkList BlockFirstNode; //首结点
DoubleLinkList BlockLastNode; //尾结点

Status MainMemory_Allocation(int);//内存分配函数
Status MainMemory_Recycle(int);//内存回收函数
Status Best_Fit(int);//最佳适应算法函数
Status Init();//初始化函数

void Display();//展示分配结果


Status Init()//初始化内存空间链表
{
	BlockFirstNode = (DoubleLinkList)malloc(sizeof(DoubleLinkNode));
	BlockLastNode = (DoubleLinkList)malloc(sizeof(DoubleLinkNode));

	BlockFirstNode->pre = NULL;
	BlockFirstNode->next = BlockLastNode;
	BlockLastNode->pre = BlockFirstNode;
	BlockLastNode->next = NULL;

	BlockLastNode->data.blockaddress = 0;//初始地址为0
	BlockLastNode->data.blocksize = MainMemory;//初始分区大小=主存大小
	BlockLastNode->data.blockstate = Sleeping;//初始状态为空闲

	return Success;
}


Status MainMemory_Allocation()//分配主存
{
	int Allocation_Size = 0;

	HANDLE  hConsole;
	int k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	k = 15;
	SetConsoleTextAttribute(hConsole, k);
	cout << "请输入需要分配的主存大小(单位:KB)：" ;
	k = 9;
	SetConsoleTextAttribute(hConsole, k);
	cin >> Allocation_Size;
	cout <<endl<< "\t\t>>>>>>>>>>主存分配中>>>>>>>>>>>"<<endl;
	if (Allocation_Size < 0 || Allocation_Size == 0)
	{
		k = 12;
		SetConsoleTextAttribute(hConsole, k);
		cout << "\t\t[警告]分配大小不合适，请重试！" << endl;
		return Error;
	}

	if (Best_Fit(Allocation_Size) == Success) 
		cout << "\t\t\t[成功]分配成功！" << endl;
	else {
		k = 12;
		SetConsoleTextAttribute(hConsole, k);
		cout << "\t\t[错误]内存不足，分配失败！" << endl;
	}
	return Success;
}


Status Best_Fit(int request)//最佳适应算法
{
	int lowest; //记录最小剩余空间
	DoubleLinkList temp = (DoubleLinkList)malloc(sizeof(DoubleLinkNode));
	temp->data.blocksize = request;
	temp->data.blockstate = Working;
	DoubleLinkNode* m = BlockFirstNode->next;
	DoubleLinkNode* n = NULL; //记录最佳插入位置

	while (m){//初始化最小空间和最佳位置
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
		return Error;//没有找到空闲块
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


Status MainMemory_Recycle(int flag)//主存回收
{
	DoubleLinkNode* n = BlockFirstNode;

	for (int i = 0; i <= flag; i++)
		if (n != NULL)
			n = n->next;
		else
			return Error;

	n->data.blockstate = Sleeping;
	//内存释放后邻近内存会互相合并：
	if (n->pre != BlockFirstNode && n->pre->data.blockstate == Sleeping){//与前面的空闲块相连
		n->pre->data.blocksize += n->data.blocksize;//空间扩充,合并为一个
		n->pre->next = n->next;//去掉原来被合并的n
		n->next->pre = n->pre;
		n = n->pre;
	}if (n->next != BlockLastNode && n->next->data.blockstate == Sleeping){//与后面的空闲块相连
		n->data.blocksize += n->next->data.blocksize;//空间扩充,合并为一个
		n->next->next->pre = n;
		n->next = n->next->next;
	}if (n->next == BlockLastNode && n->next->data.blockstate == Sleeping){//与最后的空闲块相连
		n->data.blocksize += n->next->data.blocksize;
		n->next = NULL;
	}

	return Success;
}


void Display()//显示主存分配情况
{
	int BlockID = 0;

	HANDLE  hConsole;
	int k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	k = 10;
	SetConsoleTextAttribute(hConsole, k);
	cout << endl << "\t\t\t主存分配情况表"<<endl;
	cout << "---------------------------------------------------------------------"<<endl;

	DoubleLinkNode* n = BlockFirstNode->next;

	cout << "\t分区号\t起始地址\t分区大小\t状态"<<endl;

	while (n)
	{
		cout << "\t " << BlockID++ << "\t ";
		cout << n->data.blockaddress << "\t\t ";
		cout << n->data.blocksize << "KB\t\t";

		if (n->data.blockstate == Sleeping) 
			cout << "空闲"<<endl;
		else 
			cout << "已分配"<<endl;

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
	cout << "------------------使用的内存分配算法 * 最佳适应算法------------------" << endl;

	Init(); //初始化空间表
	int option;

	while (1){
		Display();
		k = 15;
		SetConsoleTextAttribute(hConsole, k);
		cout << "请输入您选择的操作：1: 分配内存 2: 回收内存 0: 退出------:" ;
		k = 9;
		SetConsoleTextAttribute(hConsole, k);
		cin >> option;

		if (option == 1) 
			MainMemory_Allocation(); // 分配内存
		else if (option == 2){  // 内存回收
			int ID;
			k = 15;
			SetConsoleTextAttribute(hConsole, k);
			cout << "请输入您想要回收的分区号：";
			k = 9;
			SetConsoleTextAttribute(hConsole, k);
			cin >> ID;
			cout << endl;
			MainMemory_Recycle(ID);
		}else if (option == 0) 
			break; //退出
		else{//输入操作有误
			k = 10;
			SetConsoleTextAttribute(hConsole, 12);
			cout << "[警告]输入有误，请重试！" << endl;
			continue;
		}
	}
}