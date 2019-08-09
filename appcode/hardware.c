/*
 * File: hardware.c
 * -------------
 * 本部分为对硬件的操作及其响应
 * 包括键盘，鼠标，以及在文本编写中对于字符的输入操作
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

//宏定义：系统设置
#define INF 100000000.0  //定义一个无限大
#define TEXTLEN  100     //文本最大长度
#define CURSOR "_"       //光标符号
#define CURSOR_BLINK  1  //光标闪烁定时器事件标志号
#define TIMERSECONDS 500 //光标闪烁间隔
#define SYSCOLOR "Blue"  //默认系统绘制颜色
#define SYSSIZE 1        //默认系统绘制粗细
#define SCALE     0.15   //默认系统基本度量

//宏定义：对于图形对应链表的设置
#define NLIST     6
#define LINE      0
#define RECT      1
#define ELLIPSE   2
#define RTRIANGLE 3
#define STRING    4
#define POINT     5

// 一些基本模式
double winwidth, winheight;     //窗口尺寸
bool isCertainDrawMode = FALSE; 
bool isNormalDrawMode = TRUE;   
bool isEditMode = FALSE;        
int mode = INF;                 
bool isSelected = FALSE;        

// 链表等相关数据结构定义
extern linkedlistADT list[NLIST];
int curList = LINE; /*当前链表序号*/
LineT curLine = NULL;/*直线链表的当前对象指针*/
RectT curRect = NULL;/*矩形链表的当前对象指针*/
EllipseT curEllipse = NULL;/*椭圆链表的当前对象指针*/
RTriangleT curRTriangle = NULL;/*直角三角形链表的当前对象指针*/
PointT curPoint = NULL;/*描点链表的当前对象指针*/
TextT curText = NULL;/*文本链表的当前对象指针*/

// 文本编写需要
TextT tptr; /*当前编辑文本对象指针*/
bool inText = FALSE; /*是否处于当前文本编辑状态*/
char textbuf[TEXTLEN + 1];/*当前文本缓冲区*/;
bool isCursorBlink = FALSE;  /*光标是否闪烁*/
double Fillin = 0;/*是否填充*/

// 宏定义：画图边界
#define DRAW_CELLING (winheight / 1.2)
#define DRAW_FLOOR (winheight / 25)

// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key, event); // GUI获取键盘
	display(); // 刷新显示
	switch (event) {
	case KEY_DOWN:
		switch (key) {
		case VK_F1:
			isEditMode = FALSE;
			isCertainDrawMode = FALSE;
			isNormalDrawMode = TRUE;
			break;
		case VK_F2:
			isEditMode = FALSE;
			isCertainDrawMode = TRUE;
			isNormalDrawMode = FALSE;
			break;
		case VK_F3:
			isEditMode = TRUE;
			isCertainDrawMode = FALSE;
			isNormalDrawMode = FALSE;
			break;
		case VK_F5:
			if (isCertainDrawMode)
				mode = STRING;
		case VK_F6:
			if (isCertainDrawMode)
				mode = LINE;
			break;
		case VK_F7:
			if (isCertainDrawMode)
				mode = RECT;
			break;
		case VK_F8:
			if (isCertainDrawMode)
				mode = ELLIPSE;
			break;
		case VK_F9:
			if (isCertainDrawMode)
				mode = RTRIANGLE;
			break;
		}

	}
	if (isEditMode) {
		switch (event) {
		case KEY_DOWN:
			switch (key) {
			case VK_LEFT:
				if (!isSelected) break;
				switch (curList) {
				case LINE:
					curLine->x1 -= SCALE;
					curLine->x2 -= SCALE;
					break;
				case RECT:
					curRect->x1 -= SCALE;
					curRect->x2 -= SCALE;
					break;
				case ELLIPSE:
					curEllipse->cx -= SCALE;
					break;
				case RTRIANGLE:
					curRTriangle->x1 -= SCALE;
					curRTriangle->x2 -= SCALE;
					break;
				case STRING:
					curText->x -= SCALE;
					break;
				}
				break;
			case VK_RIGHT:
				if (!isSelected) break;
				switch (curList) {
				case LINE:
					curLine->x1 += SCALE;
					curLine->x2 += SCALE;
					break;
				case RECT:
					curRect->x1 += SCALE;
					curRect->x2 += SCALE;
					break;
				case ELLIPSE:
					curEllipse->cx += SCALE;
					break;
				case RTRIANGLE:
					curRTriangle->x1 += SCALE;
					curRTriangle->x2 += SCALE;
					break;
				case STRING:
					curText->x += SCALE;
					break;
				}
				break;
			case VK_UP:
				if (!isSelected) break;
				switch (curList) {
				case LINE:
					curLine->y1 += SCALE;
					curLine->y2 += SCALE;
					break;
				case RECT:
					curRect->y1 += SCALE;
					curRect->y2 += SCALE;
					break;
				case ELLIPSE:
					curEllipse->cy += SCALE;
					break;
				case RTRIANGLE:
					curRTriangle->y1 += SCALE;
					curRTriangle->y2 += SCALE;
					break;
				case STRING:
					curText->y += SCALE;
					break;
				}
				break;
			case VK_DOWN:
				if (!isSelected) break;
				switch (curList) {
				case LINE:
					curLine->y1 -= SCALE;
					curLine->y2 -= SCALE;
					break;
				case RECT:
					curRect->y1 -= SCALE;
					curRect->y2 -= SCALE;
					break;
				case ELLIPSE:
					curEllipse->cy -= SCALE;
					break;
				case RTRIANGLE:
					curRTriangle->y1 -= SCALE;
					curRTriangle->y2 -= SCALE;
					break;
				case STRING:
					curText->y -= SCALE;
					break;
				}
				break;
			case VK_DELETE:/*删除当前对象*/
				DeleteGraphic();
				break;
			case VK_ESCAPE:
				if (!isSelected) break;/*不处于选中状态则不处理*/
				switch (curList) {
				case LINE:
					curLine->isSelected = FALSE;
					curLine->color = GetPenColor();
					break;
				case RECT:
					curRect->isSelected = FALSE;
					curRect->color = GetPenColor();
					break;
				case ELLIPSE:
					curEllipse->isSelected = FALSE;
					curEllipse->color = GetPenColor();
					break;
				case RTRIANGLE:
					curRTriangle->isSelected = FALSE;
					curRTriangle->color = GetPenColor();
					break;
				case STRING:
					curText->isSelected = FALSE;
					curText->color = GetPenColor();
					break;
				default:
					break;
				}
				isSelected = FALSE;
				break;
			case VK_CONTROL://复制
				CopyGraphic();
				break;
			case VK_SHIFT://粘贴
				PasteGraphic();
				break;
			}
			display();
			break;
		default:
			break;
		}
	}
}

// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{

	RectT rptr;
	RTriangleT rtptr;
	EllipseT eptr;
	LineT lptr;
	PointT pptr;
	double x1, y1, x2, y2, dx, dy;
	double mx = ScaleXInches(x);/*pixels --> inches*/
	double my = ScaleYInches(y);/*pixels --> inches*/
	static bool isMove = FALSE; /*移动标志*/
	static bool isChangeSize = FALSE; /*缩放标志*/
	static bool isDraw = FALSE; /*按下状态标志*/
	static double omx, omy;
	uiGetMouse(x, y, button, event); //GUI获取鼠标

	display();
	if (isEditMode) {
		switch (event) {
		case BUTTON_DOWN:
			if (isSelected) { /*已是选中状态*/
				omx = mx;
				omy = my;
				if (button == LEFT_BUTTON) { /*左键按下，进入移动状态*/
					isMove = TRUE;
				}
				else if (button == RIGHT_BUTTON) {/*右键键按下，进入缩放状态*/
					isChangeSize = TRUE;
				}
				break;
			}
			if (button == LEFT_BUTTON) {
				PickNearestNode(list, mx, my);/*选择对象*/
				switch (curList) {
				case LINE:
					curLine->isSelected = TRUE;
					curLine->color = CopyString("GREEN");
					break;
				case RECT:
					curRect->isSelected = TRUE;
					curRect->color = CopyString("GREEN");
					break;
				case ELLIPSE:
					curEllipse->isSelected = TRUE;
					curEllipse->color = CopyString("GREEN");
					break;
				case RTRIANGLE:
					curRTriangle->isSelected = TRUE;
					curRTriangle->color = CopyString("GREEN");
					break;
				case STRING:
					curText->isSelected = TRUE;
					curText->color = CopyString("GREEN");
					break;
				default:
					break;
				}
				if (curList != INF)
					isSelected = TRUE;
				break;
			}
			else if (button == RIGHT_BUTTON) {
			}
			break;
		case BUTTON_UP:
			if (button == LEFT_BUTTON) {
				isMove = FALSE; /*退出移动状态*/
				break;
			}
			else if (button == RIGHT_BUTTON) {
				isChangeSize = FALSE;/*退出缩放状态*/
				break;
			}
			break;
		case BUTTON_DOUBLECLICK:
			break;
		case MOUSEMOVE:
			if (isSelected && (isMove || isChangeSize) && isWithinBound(mx, my)) {
				dx = mx - omx;
				dy = my - omy;
				if (isMove) { /*移动*/
					switch (curList) {
					case LINE:
						curLine->x1 += dx;
						curLine->y1 += dy;
						curLine->x2 += dx;
						curLine->y2 += dy;
						break;
					case RECT:
						x1 = curRect->x1 + dx;
						y1 = curRect->y1 + dy;
						x2 = curRect->x2 + dx;
						y2 = curRect->y2 + dy;
						if (x1 < x2) {
							curRect->x1 = x1;
							curRect->x2 = x2;
						}
						else {
							curRect->x1 = x2;
							curRect->x2 = x1;
						}
						if (y1 < y2) {
							curRect->y1 = y1;
							curRect->y2 = y2;
						}
						else {
							curRect->y1 = y2;
							curRect->y2 = y1;
						}
						break;
					case ELLIPSE:
						curEllipse->cx += dx;
						curEllipse->cy += dy;
						break;
					case RTRIANGLE:
						curRTriangle->x1 += dx;
						curRTriangle->x2 += dx;
						curRTriangle->y1 += dy;
						curRTriangle->y2 += dy;
						break;
					case STRING:
						curText->x += dx;
						curText->y += dy;
						break;
					default:
						break;
					}
				}
				else if (isChangeSize) {
					switch (curList) {
					case LINE:
						if (fabs(omx - curLine->x1) + fabs(omy - curLine->y1) <
							fabs(omx - curLine->x2) + fabs(omy - curLine->y2)) {
							curLine->x1 += dx;
							curLine->y1 += dy;
						}
						else {
							curLine->x2 += dx;
							curLine->y2 += dy;
						}
						break;
					case RECT:
						if (fabs(omx - curRect->x1) < fabs(omx - curRect->x2)) {
							curRect->x1 += dx;
						}
						else {
							curRect->x2 += dx;
						}
						if (fabs(omy - curRect->y1) < fabs(omy - curRect->y2)) {
							curRect->y1 += dy;
						}
						else {
							curRect->y2 += dy;
						}
						break;
					case ELLIPSE:
						curEllipse->rx += dx;
						curEllipse->ry += dy;
						break;
					case RTRIANGLE:
						if (fabs(omx - curRTriangle->x1) < fabs(omx - curRTriangle->x2))
							curRTriangle->x1 += dx;
						else
							curRTriangle->x2 += dx;
						if (fabs(omy - curRTriangle->y1) < fabs(omy - curRTriangle->y2))
							curRTriangle->y1 += dy;
						else
							curRTriangle->y2 += dy;
						break;
					default:
						break;
					}
				}
				omx = mx;
				omy = my;
			}
			break;
		}
	}
	else if (isCertainDrawMode) {
		switch (event) {
		case BUTTON_DOWN:
			if (isWithinBound(mx, my)) {
				if (button == LEFT_BUTTON) {
					isDraw = TRUE;
				}
				else if (button == RIGHT_BUTTON) {
				}
			}
			omx = mx, omy = my;
			break;
		case BUTTON_DOUBLECLICK:
			break;
		case BUTTON_UP:
			if (button == LEFT_BUTTON) {
				if (isDraw && isWithinBound(mx, my)) {
					switch (mode) {
					case LINE:
						lptr = GetBlock(sizeof(*lptr));
						lptr->x1 = omx;
						lptr->y1 = omy;
						lptr->x2 = mx;
						lptr->y2 = my;
						lptr->PenSize = GetPenSize();
						lptr->color = GetPenColor();
						lptr->isSelected = FALSE;
						InsertNode(list[LINE], NULL, lptr);
						DrawLineD(lptr);
						break;
					case RECT:
						rptr = GetBlock(sizeof(*rptr));
						rptr->x1 = omx;
						rptr->y1 = omy;
						rptr->x2 = mx;
						rptr->y2 = my;
						rptr->PenSize = GetPenSize();
						rptr->color = GetPenColor();
						rptr->isSelected = FALSE;
						InsertNode(list[RECT], NULL, rptr);
						DrawRect(rptr);
						break;
					case RTRIANGLE:
						rtptr = GetBlock(sizeof(*rtptr));
						rtptr->x1 = omx;
						rtptr->y1 = omy;
						rtptr->x2 = mx;
						rtptr->y2 = my;
						rtptr->PenSize = GetPenSize();
						rtptr->color = GetPenColor();
						rtptr->isSelected = FALSE;
						InsertNode(list[RTRIANGLE], NULL, rtptr);
						DrawRightTriangle(rtptr);
						break;
					case ELLIPSE:
						eptr = GetBlock(sizeof(*eptr));
						eptr->cx = (omx + mx) / 2; //椭圆中心为两点连线的中点
						eptr->cy = (omy + my) / 2;
						eptr->rx = fabs(omx - mx) / 2; //横坐标距离的一半
						eptr->ry = fabs(omy - my) / 2; //纵坐标距离的一半
						eptr->PenSize = GetPenSize();
						eptr->color = GetPenColor();
						eptr->isSelected = FALSE;
						InsertNode(list[ELLIPSE], NULL, eptr);
						DrawCenteredEllipse(eptr);
						break;
					case STRING:
						if (inText) break;/*若已处于文本编辑状态，则不生成新的空文本*/
						inText = TRUE; /*进入新的文本行输入状态*/
						tptr = GetBlock(sizeof(*tptr));
						tptr->x = mx;
						tptr->y = my;
						MovePen(tptr->x, tptr->y);/*文本显示起始位置*/
						DrawTextString(CURSOR);/*显示光标*/
						textbuf[0] = '\0'; /*形成字符串*/
						tptr->PointSize = GetPointSize();
						tptr->color = GetPenColor();
						tptr->isSelected = FALSE;
						tptr->curPos = 0; /*设置当前光标位置*/
						tptr->isDisplayed = TRUE; /*设置光标闪烁标志*/
						startTimer(CURSOR_BLINK, TIMERSECONDS);/*光标闪烁定时器触发*/
						isCursorBlink = TRUE;/*设置光标闪烁标志*/
						break;
					default:
						break;
					}
					isDraw = FALSE;
				}
			}
			else if (button == RIGHT_BUTTON) {
			}
			break;
		case MOUSEMOVE:
			if (isDraw && isWithinBound(mx, my)) {
				switch (mode) {
				case LINE:
					lptr = GetBlock(sizeof(*lptr));
					lptr->x1 = omx;
					lptr->y1 = omy;
					lptr->x2 = mx;
					lptr->y2 = my;
					lptr->PenSize = GetPenSize();
					lptr->color = GetPenColor();
					lptr->isSelected = FALSE;
					DrawLineD(lptr);
					break;
				case RECT:
					rptr = GetBlock(sizeof(*rptr));
					rptr->x1 = omx;
					rptr->y1 = omy;
					rptr->x2 = mx;
					rptr->y2 = my;
					rptr->PenSize = GetPenSize();
					rptr->color = GetPenColor();
					rptr->isSelected = FALSE;
					DrawRect(rptr);
					break;
				case RTRIANGLE:
					rtptr = GetBlock(sizeof(*rtptr));
					rtptr->x1 = omx;
					rtptr->y1 = omy;
					rtptr->x2 = mx;
					rtptr->y2 = my;
					rtptr->PenSize = GetPenSize();
					rtptr->color = GetPenColor();
					rtptr->isSelected = FALSE;
					DrawRightTriangle(rtptr);
					break;
				case ELLIPSE:
					eptr = GetBlock(sizeof(*eptr));
					eptr->cx = (omx + mx) / 2; //椭圆中心为两点连线的中点
					eptr->cy = (omy + my) / 2;
					eptr->rx = fabs(omx - mx) / 2; //横坐标距离的一半
					eptr->ry = fabs(omy - my) / 2; //纵坐标距离的一半
					eptr->PenSize = GetPenSize();
					eptr->color = GetPenColor();
					eptr->isSelected = FALSE;
					DrawCenteredEllipse(eptr);
					break;
				default:
					break;
				}
			}
			break;
		}
	}
	else if (isNormalDrawMode) {
		if (!isWithinBound(mx, my))
			return;
		switch (event) {
		case BUTTON_DOWN:
			switch (button) {
			case LEFT_BUTTON:
				isDraw = TRUE;
				omx = mx;
				omy = my;
			}
			break;
		case BUTTON_UP:
			switch (button) {
			case LEFT_BUTTON:
				isDraw = FALSE;
			}
			break;
		case MOUSEMOVE:
			if (isDraw && isWithinBound(mx, my)) {
				pptr = GetBlock(sizeof(*pptr));
				pptr->x1 = omx;
				pptr->y1 = omy;
				pptr->x2 = mx;
				pptr->y2 = my;
				pptr->PenSize = GetPenSize();
				pptr->color = GetPenColor();
				pptr->isSelected = FALSE;
				InsertNode(list[POINT], NULL, pptr);
				DrawPoint(pptr);
				omx = mx;
				omy = my;
				break;
			}
		}
	}
}

// 用户的定时器事件响应函数
void TimerEventProcess(int timerID)
{
	switch (timerID) {
	case CURSOR_BLINK: /*100ms光标闪烁定时器*/
		SetEraseMode(!tptr->isDisplayed);
		DrawCursor(textbuf, tptr->curPos, tptr->x, tptr->y);
		SetEraseMode(FALSE);
		MovePen(tptr->x, tptr->y);
		DrawTextString(textbuf);
		tptr->isDisplayed = !tptr->isDisplayed;
		break;
	default:
		break;
	}
}

// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	int len;

	if (!inText) return;
	switch (ch) {
	case 27: /*ESC*/
	case '\r':  /* 注意：回车在这里返回的字符是'\r'，不是'\n'*/
		inText = FALSE;/*退出当前文本输入*/
		SetEraseMode(TRUE);
		MovePen(tptr->x, tptr->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, tptr->curPos, tptr->x, tptr->y);/*擦除当前光标*/
		drawAllImages();//***
		tptr->isDisplayed = FALSE;
		tptr->color = GetPenColor();
		tptr->text = CopyString(textbuf);/*申请字符串空间*/
		InsertNode(list[STRING], NULL, tptr);
		SetEraseMode(FALSE);
		drawAllImages();//***
		cancelTimer(CURSOR_BLINK);/*注销光标闪烁定时器*/
		isCursorBlink = FALSE;
		SetPenColor(SYSCOLOR);
		break;
	case '\b':/*BACKSPACE*/
		if ((len = strlen(textbuf)) == 0) break;
		SetEraseMode(TRUE);
		MovePen(tptr->x, tptr->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, tptr->curPos, tptr->x, tptr->y);/*擦除当前光标*/
		DeleteCharFromString(textbuf, tptr->curPos - 1);
		SetEraseMode(FALSE);
		drawAllImages();
		MovePen(tptr->x, tptr->y);
		DrawTextString(textbuf);
		if (tptr->curPos > 0) tptr->curPos--;
		DrawCursor(textbuf, tptr->curPos, tptr->x, tptr->y);/*显示当前光标*/
		break;
	default:
		if ((len = strlen(textbuf)) >= TEXTLEN) break;
		SetEraseMode(TRUE);
		MovePen(tptr->x, tptr->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, tptr->curPos, tptr->x, tptr->y);/*擦除当前光标*/
		InsertCharToString(textbuf, tptr->curPos, ch);/*将当前字符插入到光标位置*/
		SetEraseMode(FALSE);
		MovePen(tptr->x, tptr->y);
		DrawTextString(textbuf);
		tptr->curPos++;
		DrawCursor(textbuf, tptr->curPos, tptr->x, tptr->y);/*显示当前光标*/
		break;
	}
}

/*画光标*/
void DrawCursor(string str, int curPos, double startx, double starty)
{
	if (curPos < 0 || curPos > strlen(str)) return;
	MovePen(startx + TextStringWidth(SubString(str, 0, curPos - 1)), starty);
	DrawTextString(CURSOR);
	return;
}

/*Insert char c to the string str at the position pos*/
void InsertCharToString(string str, int pos, char c)
{
	int len;

	if (pos < 0 || pos >= TEXTLEN) return;
	len = strlen(str);
	*(str + len + 1) = '\0';/*插入后形成字符串*/
	while (len > pos) {
		*(str + len) = *(str + len - 1);
		len--;
	}
	*(str + len) = c;
	return;
}

/*Delete the character at the pos position of string str*/
void DeleteCharFromString(string str, int pos)
{
	int len;

	len = strlen(str);
	if (pos < 0 || pos >= len) return;

	while (*(str + pos) != '\0') {
		*(str + pos) = *(str + pos + 1);
		pos++;
	}
	return;
}

bool isWithinBound(double mx, double my)
{
	return my >= DRAW_FLOOR && my <= DRAW_CELLING;
}

