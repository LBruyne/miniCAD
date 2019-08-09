/*
 * File: interface.c
 * -------------
 * 该部分定义了对于菜单和按钮的绘制
 * 按钮和菜单对功能起触发作用
 */

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

// 宏定义:系统设定
#define INF 100000000.0 
#define SYSCOLOR "Blue"  /*默认系统绘制颜色*/
#define SYSSIZE 1 /*默认系统绘制粗细*/
#define CHANGEHEIGHT 1.7/*默认按钮距离*/

// 宏定义:对于链表对应图形的设置
#define NLIST     6
#define LINE      0
#define RECT      1
#define ELLIPSE   2
#define RTRIANGLE 3
#define STRING    4
#define POINT     5

// 宏定义:模式选项
#define EDITMODE 0
#define DRAWMODE 1
#define NORMALMODE 2

// 宏定义:以下为菜单中功能对应编号
#define NEWBUILT_FUNC 1
#define OPEN_FUNC 2
#define PRESERVE_FUNC 3
#define CLEAR_FUNC 4
#define EXIT_FUNC 5

#define CHOOSE_FUNC 1
#define COPY_FUNC 2
#define PASTE_FUNC 3
#define DELETE_FUNC 4

#define LINE_FUNC 1
#define RECT_FUNC 2
#define ELLIPSE_FUNC 3
#define RTRIANGLE_FUNC 4
#define TEXT_FUNC 5

#define ABOUTBOX 1
#define ILLUSTRATION 2

// 宏定义:颜色定义
#define RED 0
#define BLUE 1
#define MAGENTA 2
#define BLACK 3
#define BROWN 4
#define GREEN 5
#define GRAY 6
#define YELLOW 7

// 宏定义:顺时针和逆时针旋转对应选项
#define CK 1
#define ACK -1

extern double winwidth, winheight;   // 窗口尺寸

// 宏定义:画图边界
#define DRAW_CELLING (winheight / 1.2)
#define DRAW_FLOOR (winheight / 25)

extern bool isCertainDrawMode;
extern bool isNormalDrawMode;
extern bool isEditMode;
extern int mode; // 画图模式下画图形的种类

extern linkedlistADT list[NLIST];/*四类图形元素链表指针*/
extern int curList; /*当前链表序号*/
extern LineT curLine;/*直线链表的当前对象指针*/
extern RectT curRect;/*矩形链表的当前对象指针*/
extern EllipseT curEllipse;/*椭圆链表的当前对象指针*/
extern RTriangleT curRTriangle ;/*直角三角形链表的当前对象指针*/
extern PointT curPoint;/*描点链表的当前对象指针*/
extern TextT curText;/*文本链表的当前对象指针*/

extern bool isSelected; /*图形元素选中状态*/
extern int ColorSelected;//颜色

void display()
{
	// 清屏
	DisplayClear();

	drawMenu();

	drawButtons();

	drawAllImages();
	// 绘制和处理菜单
}

// 按钮演示程序
void drawButtons()
{
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 18;
	double y = winheight / 1.105;
	double w = 3 * GetFontHeight(); // 控件宽度
	static bool isDraw = FALSE; /*按下状态标志*/

	double mx = ScaleXInches(x);/*pixels --> inches*/
	double my = ScaleYInches(y);/*pixels --> inches*/

	static double omx, omy;

	int color = ColorSelected;
	int size = GetPenSize();
	SetPenSize(SYSSIZE);
	SetPenColor(SYSCOLOR);

	SetPenSize(1);
	drawLabel(x - winwidth / 22, y - winheight / 100, "图案");

	//矩形
	if (button(GenUIID(0), x, y, w, h, "直线")) {
		isEditMode = FALSE;
		isCertainDrawMode = TRUE;
		isNormalDrawMode = FALSE;
		mode = LINE;
	}

	//三角
	if (button(GenUIID(0), x, y - winheight / 18, w, h, "三角")) {
		isEditMode = FALSE;
		isCertainDrawMode = TRUE;
		isNormalDrawMode = FALSE;
		mode = RTRIANGLE;
	}
	//矩形
	if (button(GenUIID(0), x + winwidth / 13, y, w, h, "矩形")) {
		isEditMode = FALSE;
		isCertainDrawMode = TRUE;
		isNormalDrawMode = FALSE;
		mode = RECT;
	}

	//椭圆
	if (button(GenUIID(0), x + winwidth / 13, y - winheight / 18, w, h, "椭圆")) {
		isEditMode = FALSE;
		isCertainDrawMode = TRUE;
		isNormalDrawMode = FALSE;
		mode = ELLIPSE;
	}

	//文本
	if (button(GenUIID(0), x + winwidth / 6.6, y, w, h, "文本")) {
		isEditMode = FALSE;
		isCertainDrawMode = TRUE;
		isNormalDrawMode = FALSE;
		mode = STRING;
	}

	//画笔
	if (button(GenUIID(0), x + winwidth / 6.6, y - winheight / 18, w, h, "画笔")) {
		isEditMode = FALSE;
		isCertainDrawMode = FALSE;
		isNormalDrawMode = TRUE;
	}

	//粗细
	SetPenColor("Blue");

	drawLabel(x + winwidth / 4.4, y - winheight / 100, "粗细");
	setButtonColors("Blue", "Blue", "Red", "Red", 1);

	if (button(GenUIID(0), x + winwidth / 3.6, y - winheight / 19.5, CHANGEHEIGHT*w, 0.3*h, "")) {
		size = 8;
	}
	if (button(GenUIID(0), x + winwidth / 3.6, y - winheight / 33, CHANGEHEIGHT*w, 0.25*h, "")) {
		size = 6;
	}
	if (button(GenUIID(0), x + winwidth / 3.6, y - winheight / 88, CHANGEHEIGHT*w, 0.2*h, "")) {
		size = 4;
	}
	if (button(GenUIID(0), x + winwidth / 3.6, y + winheight / 100, CHANGEHEIGHT*w, 0.15*h, "")) {
		size = 2;
	}
	if (button(GenUIID(0), x + winwidth / 3.6, y + winheight / 35, CHANGEHEIGHT*w, 0.12*h, "")) {
		size = 0.5;
	}

	//颜色
	SetPenColor("Blue");
	drawLabel(x + winwidth / 2.55, y - winheight / 100, "颜色");

	setButtonColors("Red", "Black", "Red", "White", 1);
	if (button(GenUIID(0), x + winwidth / 2.25, y, w, h, "红色")) {
		color = RED;
	}

	setButtonColors("Blue", "Black", "Blue", "White", 1);
	if (button(GenUIID(0), x + winwidth / 2.25, y - winheight / 18, w, h, "蓝色")) {
		color = BLUE;
	}

	setButtonColors("Black", "Red", "Black", "White", 1);
	if (button(GenUIID(0), x + winwidth / 1.95, y, w, h, "黑色")) {
		color = BLACK;
	}

	setButtonColors("Yellow", "Black", "Yellow", "White", 1);
	if (button(GenUIID(0), x + winwidth / 1.95, y - winheight / 18, w, h, "黄色")) {
		color = YELLOW;
	}

	setButtonColors("Green", "Black", "Green", "White", 1);
	if (button(GenUIID(0), x + winwidth / 1.71, y, w, h, "绿色")) {
		color = GREEN;
	}

	setButtonColors("Gray", "Black", "Gray", "White", 1);
	if (button(GenUIID(0), x + winwidth / 1.71, y - winheight / 18, w, h, "灰色")) {
		color = GRAY;
	}

	setButtonColors("Brown", "Black", "Brown", "White", 1);
	if (button(GenUIID(0), x + winwidth / 1.525, y - winheight / 18, w, h, "棕色")) {
		color = BROWN;
	}

	setButtonColors("Magenta", "Black", "Magenta", "White", 1);
	if (button(GenUIID(0), x + winwidth / 1.525, y, w, h, "紫色")) {
		color = MAGENTA;
	}
	setButtonColors("Blue", "Blue", "Red", "Red", 0);
	//填充
	if (button(GenUIID(0), x + winwidth / 1.35, y - winheight / 18, w, h, "填充")) {
		if (isEditMode) {
			if (isSelected) {
				switch (curList)
				{
				case RECT:
					curRect->Fillin = TRUE;
					break;
				case ELLIPSE:
					curEllipse->Fillin = TRUE;
					break;
				case RTRIANGLE:
					curRTriangle->Fillin = TRUE;
					break;
				default:
					break;
				}
			}
		}
	}
	if (button(GenUIID(0), x + winwidth / 1.35, y, w, h, "删除")) {
		if (isEditMode) {
			if (isSelected) {
				DeleteGraphic();
			}
		}
	}


	//向右旋转
	if (button(GenUIID(0), x + winwidth / 1.22, y, 2 * w, h, "向右旋转90°")) {
		//按下顺时针旋转按钮，旋转90度
		if (isSelected) {
			switch (curList) {
			case LINE:
				ClockWiseRotateLine(curLine, CK);
				break;
			case RECT:
				ClockWiseRotateRect(curRect, CK);
				break;
			case ELLIPSE:
				ClockWiseRotateEllipse(curEllipse);
				break;
			case RTRIANGLE:
				ClockWiseRotateRT(curRTriangle, CK);
				break;
			}
			display();
		}
	}

	//向左旋转
	if (button(GenUIID(0), x + winwidth / 1.22, y - winheight / 18, 2 * w, h, "向左旋转90°")) {
		//按下逆时针旋转按钮，旋转90度
		if (isSelected) {
			switch (curList) {
			case LINE:
				ClockWiseRotateLine(curLine, ACK);
				break;
			case RECT:
				ClockWiseRotateRect(curRect, ACK);
				break;
			case ELLIPSE:
				ClockWiseRotateEllipse(curEllipse);
				break;
			case RTRIANGLE:
				ClockWiseRotateRT(curRTriangle, ACK);
				break;
			}
			display();
		}
	}
	SetPenSize(size);
	ColorSelected = color;
}

void drawMenu()
{
	static char * menuListFile[] = { "File",
	 "New-build  | Ctrl-N", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
	 "Open  | Ctrl-I",
	 "Preserve  | Ctrl-P",
	 "Clear | Ctrl-F",
	 "Exit   | Ctrl-E" };
	static char * menuListEdit[] = { "Edit",
	 "Select",
	 "Copy  | Ctrl-C",
	 "Paste  | Ctrl-V",
	 "Delete" };
	static char * menuListDraw[] = { "Draw",
	 "Line",
	 "Rectangle",
	 "Ellipse",
	 "RightTriangle",
	 "Text" };
	static char * menuListHelp[] = { "Help",
	 "About | Ctrl-A",
	 "Usage| Ctrl-U" };


	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH * 1.5; // 控件高度
	double w = TextStringWidth(menuListHelp[0]) * 2; // 控件宽度
	double wlist = TextStringWidth(menuListFile[0]) * 6;//点击后的列表宽度
	double xindent = winheight / 20; // red缩进
	static char *bar[3] = { "编辑状态","画图状态","普通状态" };
	static char *Color[8] = { "红色","蓝色","紫色","黑色","棕色","绿色","灰色","黄色" };
	int n = 2;
	int EditSelection;
	int FileSelection;
	int DrawSelection;
	int HelpSelection;
	int color = ColorSelected;
	int size = GetPenSize();
	SetPenSize(SYSSIZE);
	SetPenColor(SYSCOLOR);
	//??
	setMenuColors("Blue", "White", "Blue", "Red", 1);

	//File菜单
	FileSelection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	//以上给子菜单命名
	if (FileSelection > 0)
		display();
	switch (FileSelection) {
	case NEWBUILT_FUNC:
		New_Built();
		break;
	case OPEN_FUNC:
		ReadAndGetAll();
		drawAllImages();
		break;
	case PRESERVE_FUNC:
		PreserveAllImages();
		break;
	case CLEAR_FUNC:
		ClearList();
		break;
	case EXIT_FUNC:
		exit(-1);
		break;
	default:
		break;
	}

	// Edit 菜单
	EditSelection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
	if (EditSelection > 0) display();
	switch (EditSelection) {
	case CHOOSE_FUNC:
		isEditMode = TRUE;
		isCertainDrawMode = FALSE;
		isNormalDrawMode = FALSE;
		break;
	case COPY_FUNC:
		CopyGraphic();
		break;
	case PASTE_FUNC:
		PasteGraphic();
		break;
	case DELETE_FUNC:
		DeleteGraphic();
		break;
	default:
		break;
	}

	// Draw 菜单
	DrawSelection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListDraw, sizeof(menuListDraw) / sizeof(menuListDraw[0]));
	if (DrawSelection > 0) {
		isEditMode = FALSE;
		isCertainDrawMode = TRUE;
		isNormalDrawMode = FALSE;
	}
	if (DrawSelection == LINE_FUNC) mode = LINE;
	else if (DrawSelection == RECT_FUNC) mode = RECT;
	else if (DrawSelection == ELLIPSE_FUNC) mode = ELLIPSE;
	else if (DrawSelection == RTRIANGLE_FUNC) mode = RTRIANGLE;
	else if (DrawSelection == TEXT_FUNC) mode = STRING;

	// Help 菜单
	HelpSelection = menuList(GenUIID(0), x + 3 * w, y - h, w, wlist, h, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	if (HelpSelection > 0)  display();
	if (HelpSelection == ABOUTBOX)
		MessageBox(NULL, "        CAD（Computer Aided Design）——计算机辅助设计，指利用计算机及其图形设备帮助设计人员进行设计工作。\n        “二维CAD绘图系统”基于libgraphics，主要运用交互技术、图形变换技术以实现二维图形的输入、编辑功能，并提供文件保存和读取功能。", "关于本软件", MB_OK);
	else if (HelpSelection == ILLUSTRATION)
		MessageBox(NULL, "一、用户界面\n界面由菜单栏、图表工具栏、画图区、状态信息栏四大部分组成，配有《Hello World》背景音乐。\n1.菜单栏包含文件、编辑、画图、帮助四个部分，各部分所含功能如下：\n文件：新建、打开、保存、清除、退出；\n编辑：选择、复制、粘贴、删除；\n画图：直线、矩形、椭圆、直角三角形、文本；\n帮助：关于、用户手册。\n2.操作栏含有图案选择、画笔粗细选择、颜色选择、删除、填充、旋转等功能。\n3.状态信息栏含有当前颜色、当前模式等。\n\n二、三大模式\nF1 普通模式 可任意画线\nF2 画图模式 可画出指定图形或文本\nF3 编辑模式 可对图形进行编辑\n\n三、具体操作\n1.在画图模式下(F2)以下各键可画的图形为：\nF6 直线\nF7 矩形\nF8 椭圆\nF9 直角三角形\n\n2.在编辑模式下以下各键代表的操作为（仅针对已被左键选中的图形）：\nUp 向上移动 \nDown 向下移动\nLeft 向左移动\nRight 向右移动\nDel 删除\nEsc 去除选择\nCtrl 复制\nShift 粘贴\n左键 拖移\n右键 放缩\n\n四、快捷键\nCtrl+N 新建\nCtrl+I 打开\nCtrl+P 保存\nCtrl+F 清除\nCtrl+C 复制\nCtrl+V 粘贴\nCtrl+E 退出\nCtrl+A 关于\nCtrl+U 用户手册\n\n祝您每天好心情！", "用户手册", MB_OK);

	//设置菜单栏范围
	SetPenColor("Blue");
	MovePen(0, winheight / 1.2);
	DrawLine(winwidth, 0);
	MovePen(0, winheight / 1.037);
	DrawLine(winwidth, 0);

	//设置菜单状态栏
	SetPenColor("Blue");
	MovePen(0, winheight / 25);
	DrawLine(winwidth, 0);
	drawLabel(winwidth / 40, winheight / 80, "当前颜色：");
	drawLabel(winwidth / 7, winheight / 80, Color[color]);
	drawLabel(winwidth / 1.45, winheight / 80, "当前模式：");
	if (isEditMode) {
		n = EDITMODE;
	}
	if (isCertainDrawMode) {
		n = DRAWMODE;
	}
	if (isNormalDrawMode) {
		n = NORMALMODE;
	}
	drawLabel(winwidth / 1.15, winheight / 80, bar[n]);

	ColorSelected = color;
	SetPenSize(size);
}


//使用函数对对象旋转
void ClockWiseRotateLine(LineT obj, int mode)
{
	double dx = obj->x2 - obj->x1;
	double dy = obj->y2 - obj->y1;
	obj->x2 = obj->x1 + dy * mode;
	obj->y2 = obj->y1 - dx * mode;
}

void ClockWiseRotateRect(RectT obj, int mode)
{
	double dx = obj->x2 - obj->x1;
	double dy = obj->y2 - obj->y1;
	obj->x2 = obj->x1 + dy * mode;
	obj->y2 = obj->y1 - dx * mode;
}

void ClockWiseRotateRT(RTriangleT obj, int mode)
{
	double dx = obj->x2 - obj->x1;
	double dy = obj->y2 - obj->y1;
	obj->x2 = obj->x1 + dy * mode;
	obj->y2 = obj->y1 - dx * mode;
}

void ClockWiseRotateEllipse(EllipseT obj)
{
	double temp;
	temp = obj->rx;
	obj->rx = obj->ry;
	obj->ry = temp;
}
