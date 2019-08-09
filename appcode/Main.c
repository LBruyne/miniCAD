//***************************************************************
//  xgliu的大程作业
//  队伍名：刘奶奶和杨柳奶奶队
//  项目名：Simple CAD Project
//  开发者：刘轩铭，宋炜铁，杨锐洁 
//**************************************************************/

#include <graphics.h>
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>

#include "imgui.h"
#include "linkedlist.h"
#include "paint.h"
#include "interface.h"
#include "hardware.h"
#include "basicfun.h"

// 宏定义
#define NLIST 6  // 定义图形链表种类数目

// 全局变量
double winwidth, winheight;   // 窗口尺寸
linkedlistADT list[NLIST] = { NULL, NULL, NULL, NULL, NULL, NULL};// 四类图形元素链表指针

// 用户进入主程序
void Main()
{
	SetWindowTitle("CADProject");
	//SetWindowSize(40, 20);
	InitGraphics();
	//InitConsole();

	// 注册四类回调函数
	registerTimerEvent(TimerEventProcess);      // 定时器
	registerKeyboardEvent(KeyboardEventProcess);// 键盘
	registerMouseEvent(MouseEventProcess);      // 鼠标
	registerCharEvent(CharEventProcess);        // 字符

	// 得到界面尺寸
	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();

	// 创建新的图形链表
	for (int i = 0; i < NLIST; i++) list[i] = NewLinkedList();
}
