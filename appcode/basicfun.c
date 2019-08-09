/*
 * File: basicfun.c
 * -------------
 * 该部分包含CAD系统的基本功能
 * 包括清屏,保存,读取等
 * 以及其他小型功能
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

// 宏定义:系统基本设置
#define INF 100000000.0 //定义一个无限大//
#define COLORLEN 20    //颜色最大长度
#define TEXTLEN  100    //文本最大长度//
#define SCALE     0.15

// 宏定义:链表对应图形设置
#define NLIST     6
#define LINE      0
#define RECT      1
#define ELLIPSE   2
#define RTRIANGLE 3
#define STRING    4
#define POINT     5

extern int isSelected;
extern linkedlistADT list[NLIST];
extern int curList; /*当前链表序号*/
extern LineT curLine;/*直线链表的当前对象指针*/
extern RectT curRect;/*矩形链表的当前对象指针*/
extern EllipseT curEllipse;/*椭圆链表的当前对象指针*/
extern RTriangleT curRTriangle;/*直角三角形链表的当前对象指针*/
extern PointT curPoint;/*描点链表的当前对象指针*/
extern TextT curText;/*文本链表的当前对象指针*/

//复制粘贴需要
double minDistance[NLIST] = { INF, INF, INF, INF, INF, INF };
LineT ledit = NULL;
EllipseT eedit = NULL;
RTriangleT rtedit = NULL;
RectT redit = NULL;
TextT tedit = NULL;

void PreserveLine(void *line)
{
	LineT lptr = (LineT)line;
	FILE *fp;//文件指针

   /*文件的打开*/
	fp = fopen("Line.txt", "a+");//fopen打开文件，这个文件可以是当前不存在的。“w”以写入的形式打开，“r”以读的形式打开
	if (fp == NULL) { //判断如果文件指针为空
		printf("File cannot open! ");
		exit(0);//在以0的形式退出，必须在文件开头有#include <stdlib.h>,stdlib 头文件即standard library标准库头文件
	}
	//写入东西
	fprintf(fp, "%f\n", lptr->x1);//写入指针fp
	fprintf(fp, "%f\n", lptr->y1);
	fprintf(fp, "%f\n", lptr->x2);
	fprintf(fp, "%f\n", lptr->y2);
	fprintf(fp, "%d\n", lptr->PenSize);
	fprintf(fp, "%s.\n", lptr->color);
	//关闭文件
	fclose(fp);
}

void PreserveRect(void *rect)
{
	RectT r = (RectT)rect;
	FILE *fp;

	fp = fopen("Rect.txt", "a+");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}

	fprintf(fp, "%f\n", r->x1);
	fprintf(fp, "%f\n", r->y1);
	fprintf(fp, "%f\n", r->x2);
	fprintf(fp, "%f\n", r->y2);
	fprintf(fp, "%d\n", r->PenSize);
	fprintf(fp, "%f\n", r->Fillin);
	fprintf(fp, "%s.\n", r->color);

	fclose(fp);
}

void PreserveEllipse(void *ellipse)
{
	EllipseT eptr = (EllipseT)ellipse;
	FILE *fp;

	fp = fopen("Ellipse.txt", "a+");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}

	fprintf(fp, "%f\n", eptr->cx);
	fprintf(fp, "%f\n", eptr->cy);
	fprintf(fp, "%f\n", eptr->rx);
	fprintf(fp, "%f\n", eptr->ry);
	fprintf(fp, "%d\n", eptr->PenSize);
	fprintf(fp, "%f\n", eptr->Fillin);
	fprintf(fp, "%s.\n", eptr->color);

	fclose(fp);
}

void PreserveRTriangle(void *rtriangle)
{
	RTriangleT rt = (RTriangleT)rtriangle;
	FILE *fp;

	fp = fopen("RTriangle.txt", "a+");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}

	fprintf(fp, "%f\n", rt->x1);
	fprintf(fp, "%f\n", rt->y1);
	fprintf(fp, "%f\n", rt->x2);
	fprintf(fp, "%f\n", rt->y2);
	fprintf(fp, "%d\n", rt->PenSize);
	fprintf(fp, "%f\n", rt->Fillin);
	fprintf(fp, "%s.\n", rt->color);

	fclose(fp);
}

void PreserveText(void *text)
{
	TextT tptr = (TextT)text;
	FILE *fp;

	fp = fopen("Text.txt", "a+");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}

	fprintf(fp, "%f\n", tptr->x);
	fprintf(fp, "%f\n", tptr->y);
	fprintf(fp, "%d\n", tptr->PointSize);
	fprintf(fp, "%d\n", tptr->curPos);
	fprintf(fp, "%s.\n", tptr->color);
	fprintf(fp, "%s.\n", tptr->text);

	fclose(fp);
}

void PreservePoint(void *point)
{
	PointT pt = (PointT)point;
	FILE *fp;

	fp = fopen("Point.txt", "a+");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}

	fprintf(fp, "%f\n", pt->x1);
	fprintf(fp, "%f\n", pt->y1);
	fprintf(fp, "%f\n", pt->x2);
	fprintf(fp, "%f\n", pt->y2);
	fprintf(fp, "%d\n", pt->PenSize);
	fprintf(fp, "%s.\n", pt->color);
	fclose(fp);
}

void ClearFile()
{
	FILE *fp;

	//Line
	fp = fopen("Line.txt", "w");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	//Rect
	fp = fopen("Rect.txt", "w");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	//Ellipse
	fp = fopen("Ellipse.txt", "w");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	//RTriangle
	fp = fopen("RTriangle.txt", "w");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	//Text
	fp = fopen("Text.txt", "w");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	//Point
	fp = fopen("Point.txt", "w");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
}

void PreserveAllImages()
{
	ClearFile();
	TraverseLinkedList(list[LINE], PreserveLine);
	TraverseLinkedList(list[RECT], PreserveRect);
	TraverseLinkedList(list[ELLIPSE], PreserveEllipse);
	TraverseLinkedList(list[RTRIANGLE], PreserveRTriangle);
	TraverseLinkedList(list[STRING], PreserveText);
	TraverseLinkedList(list[POINT], PreservePoint);
}

void ReadAndGetLine()
{
	LineT lptr;
	FILE *fp;
	int i;
	static char incolor[COLORLEN] = "";

	fp = fopen("Line.txt", "r");

	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	getc(fp);
	if (!feof(fp)) {
		rewind(fp);
		while (!feof(fp)) {
			lptr = GetBlock(sizeof(*lptr));
			fscanf(fp, "%lf", &(lptr->x1));
			fscanf(fp, "%lf", &(lptr->y1));
			fscanf(fp, "%lf", &(lptr->x2));
			fscanf(fp, "%lf", &(lptr->y2));
			fscanf(fp, "%d", &(lptr->PenSize));
			fscanf(fp, "%s", incolor);
			for (i = 0; incolor[i] != '.' && i < strlen(incolor); i++);
			incolor[i] = '\0';
			lptr->color = CopyString(incolor);
			lptr->isSelected = FALSE;
			InsertNode(list[LINE], NULL, lptr);
		}
	}
	fclose(fp);
}

void ReadAndGetRect()
{
	RectT r;
	FILE *fp;
	int i;
	static char incolor[COLORLEN] = "";

	fp = fopen("Rect.txt", "r");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	getc(fp);
	if (!feof(fp)) {
		rewind(fp);
		while (!feof(fp)) {
			r = GetBlock(sizeof(*r));
			fscanf(fp, "%lf", &(r->x1));
			fscanf(fp, "%lf", &(r->y1));
			fscanf(fp, "%lf", &(r->x2));
			fscanf(fp, "%lf", &(r->y2));
			fscanf(fp, "%d", &(r->PenSize));
			fscanf(fp, "%lf", &(r->Fillin));
			fscanf(fp, "%s", incolor);
			for (i = 0; incolor[i] != '.' && i < strlen(incolor); i++);
			incolor[i] = '\0';
			r->color = CopyString(incolor);
			r->isSelected = FALSE;
			InsertNode(list[RECT], NULL, r);
		}
	}
	fclose(fp);
}

void ReadAndGetRTriangle()
{
	RTriangleT rt;
	FILE *fp;
	int i;
	static char incolor[COLORLEN] = "";

	fp = fopen("RTriangle.txt", "r");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	getc(fp);
	if (!feof(fp)) {
		rewind(fp);
		while (!feof(fp)) {
			rt = GetBlock(sizeof(*rt));
			fscanf(fp, "%lf", &(rt->x1));
			fscanf(fp, "%lf", &(rt->y1));
			fscanf(fp, "%lf", &(rt->x2));
			fscanf(fp, "%lf", &(rt->y2));
			fscanf(fp, "%d", &(rt->PenSize));
			fscanf(fp, "%lf", &(rt->Fillin));
			fscanf(fp, "%s", incolor);
			for (i = 0; incolor[i] != '.' && i < strlen(incolor); i++);
			incolor[i] = '\0';
			rt->color = CopyString(incolor);
			rt->isSelected = FALSE;
			InsertNode(list[RTRIANGLE], NULL, rt);
		}
	}
	fclose(fp);
}

void ReadAndGetEllipse()
{
	EllipseT eptr;
	FILE *fp;
	int i;
	static char incolor[COLORLEN] = "";

	fp = fopen("Ellipse.txt", "r");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	getc(fp);
	if (!feof(fp)) {
		rewind(fp);
		while (!feof(fp)) {
			eptr = GetBlock(sizeof(*eptr));
			fscanf(fp, "%lf", &(eptr->cx));
			fscanf(fp, "%lf", &(eptr->cy));
			fscanf(fp, "%lf", &(eptr->rx));
			fscanf(fp, "%lf", &(eptr->ry));
			fscanf(fp, "%d", &(eptr->PenSize));
			fscanf(fp, "%lf", &(eptr->Fillin));
			fscanf(fp, "%s", incolor);
			for (i = 0; incolor[i] != '.' && i < strlen(incolor); i++);
			incolor[i] = '\0';
			eptr->color = CopyString(incolor);
			eptr->isSelected = FALSE;
			InsertNode(list[ELLIPSE], NULL, eptr);
		}
	}
	fclose(fp);
}

void ReadAndGetText()
{
	TextT tptr;
	FILE *fp;
	int i;
	static char incolor[COLORLEN] = "";
	static char intext[TEXTLEN] = "";

	fp = fopen("Text.txt", "r");

	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	getc(fp);
	if (!feof(fp)) {
		rewind(fp);
		while (!feof(fp)) {
			tptr = GetBlock(sizeof(*tptr));
			fscanf(fp, "%lf", &(tptr->x));
			fscanf(fp, "%lf", &(tptr->y));
			fscanf(fp, "%d", &(tptr->PointSize));
			fscanf(fp, "%d", &(tptr->curPos));
			fscanf(fp, "%s", incolor);
			for (i = 0; incolor[i] != '.' && i < strlen(incolor); i++);
			incolor[i] = '\0';
			tptr->color = CopyString(incolor);
			fscanf(fp, "%s", intext);
			for (i = 0; intext[i] != '.' && i < strlen(intext); i++);
			intext[i] = '\0';
			tptr->text = CopyString(intext);
			tptr->isSelected = FALSE;
			InsertNode(list[STRING], NULL, tptr);
		}
	}
	fclose(fp);
}

void ReadAndGetPoint()
{
	PointT pt;
	FILE *fp;
	int i;
	static char incolor[COLORLEN] = "";

	fp = fopen("Point.txt", "r");
	if (fp == NULL) {
		printf("File cannot open! ");
		exit(0);
	}
	getc(fp);
	if (!feof(fp)) {
		rewind(fp);
		while (!feof(fp)) {
			pt = GetBlock(sizeof(*pt));
			fscanf(fp, "%lf", &(pt->x1));
			fscanf(fp, "%lf", &(pt->y1));
			fscanf(fp, "%lf", &(pt->x2));
			fscanf(fp, "%lf", &(pt->y2));
			fscanf(fp, "%d", &(pt->PenSize));
			fscanf(fp, "%s", incolor);
			for (i = 0; incolor[i] != '.' && i < strlen(incolor); i++);
			incolor[i] = '\0';
			pt->color = CopyString(incolor);
			pt->isSelected = FALSE;
			InsertNode(list[POINT], NULL, pt);
		}
	}
	fclose(fp);
}

void ReadAndGetAll()
{
	ReadAndGetLine();
	ReadAndGetRect();
	ReadAndGetPoint();
	ReadAndGetRTriangle();
	ReadAndGetEllipse();
	ReadAndGetText();
}

void CopyGraphic() {
	if (isSelected) {
		switch (curList)
		{
		case LINE:
			ledit = GetBlock(sizeof(*ledit));
			ledit->color = GetPenColor();
			ledit->PenSize = curLine->PenSize;
			ledit->x1 = curLine->x1;
			ledit->x2 = curLine->x2;
			ledit->y1 = curLine->y1;
			ledit->y2 = curLine->y2;
			ledit->isSelected = FALSE;
			break;
		case ELLIPSE:
			eedit = GetBlock(sizeof(*eedit));
			eedit->color = GetPenColor();
			eedit->PenSize = curEllipse->PenSize;
			eedit->cx = curEllipse->cx;
			eedit->cy = curEllipse->cy;
			eedit->rx = curEllipse->rx;
			eedit->ry = curEllipse->ry;
			eedit->Fillin = curEllipse->Fillin;
			eedit->isSelected = FALSE;
			break;
		case RTRIANGLE:
			rtedit = GetBlock(sizeof(*rtedit));
			rtedit->color = GetPenColor();
			rtedit->PenSize = curRTriangle->PenSize;
			rtedit->x1 = curRTriangle->x1;
			rtedit->x2 = curRTriangle->x2;
			rtedit->y1 = curRTriangle->y1;
			rtedit->y2 = curRTriangle->y2;
			rtedit->Fillin = curRTriangle->Fillin;
			rtedit->isSelected = FALSE;
			break;
		case RECT:
			redit = GetBlock(sizeof(*redit));
			redit->color = GetPenColor();
			redit->PenSize = curRect->PenSize;
			redit->x1 = curRect->x1;
			redit->x2 = curRect->x2;
			redit->y1 = curRect->y1;
			redit->y2 = curRect->y2;
			redit->Fillin = curRect->Fillin;
			redit->isSelected = FALSE;
			break;
		case STRING:
			tedit = GetBlock(sizeof(*tedit));
			tedit->text = curText->text;
			tedit->color = GetPenColor();
			tedit->PointSize = curText->PointSize;
			tedit->x = curText->x;
			tedit->y = curText->y;
			tedit->curPos = curText->curPos;
			tedit->isSelected = FALSE;
			tedit->isDisplayed = FALSE;
			break;
		}
	}
}

void PasteGraphic() {
	if (isSelected) {
		switch (curList) {
		case LINE:
			if (ledit) {
				ledit->x1 += SCALE;
				ledit->x2 += SCALE;
				ledit->y1 += SCALE;
				ledit->y2 += SCALE;
				ledit->isSelected = FALSE;
				ledit->color = GetPenColor();
				curLine->color = GetPenColor();
				curLine->isSelected = FALSE;
				InsertNode(list[LINE], NULL, ledit);
				isSelected = !isSelected;
				ledit = NULL;
			}
			break;
		case ELLIPSE:
			if (eedit) {
				eedit->cx += 1.0;
				eedit->cy += 1.0;
				curEllipse->isSelected = FALSE;
				curEllipse->color = GetPenColor();
				eedit->isSelected = FALSE;
				eedit->color = GetPenColor();
				InsertNode(list[ELLIPSE], NULL, eedit);
				isSelected = !isSelected;
				eedit = NULL;
			}
			break;
		case RTRIANGLE:
			if (rtedit) {
				rtedit->x1 += 1.0;
				rtedit->x2 += 1.0;
				rtedit->y1 += 1.0;
				rtedit->y2 += 1.0;
				curRTriangle->isSelected = FALSE;
				curRTriangle->color = GetPenColor();
				rtedit->isSelected = FALSE;
				rtedit->color = GetPenColor();
				InsertNode(list[RTRIANGLE], NULL, rtedit);
				isSelected = !isSelected;
				rtedit = NULL;
			}
			break;
		case RECT:
			if (redit) {
				redit->x1 += 1.0;
				redit->x2 += 1.0;
				redit->y1 += 1.0;
				redit->y2 += 1.0;
				curRect->isSelected = FALSE;
				curRect->color = GetPenColor();
				redit->isSelected = FALSE;
				redit->color = GetPenColor();
				InsertNode(list[RECT], NULL, redit);
				isSelected = !isSelected;
				redit = NULL;
			}
			break;
		case STRING:
			if (tedit) {
				tedit->x += SCALE;
				tedit->y += SCALE;
				curText->isSelected = FALSE;
				curText->color = GetPenColor();
				tedit->isSelected = FALSE;
				tedit->color = GetPenColor();
				InsertNode(list[STRING], NULL, tedit);
				isSelected = !isSelected;
				tedit = NULL;
			}
			break;
		}
	}
}

void DeleteGraphic() {
	if (isSelected) {
		switch (curList) {
		case LINE:
			DeleteNode(list[LINE], curLine, LineEqual);
			break;
		case RECT:
			DeleteNode(list[RECT], curRect, RectEqual);
			break;
		case ELLIPSE:
			DeleteNode(list[ELLIPSE], curEllipse, EllipseEqual);
			break;
		case RTRIANGLE:
			DeleteNode(list[RTRIANGLE], curRTriangle, RTriangleEqual);
			break;
		case STRING:
			DeleteNode(list[STRING], curText, TextEqual);
			break;
		default:
			break;
		}
		isSelected = FALSE;
	}
}

void New_Built()
{
	int Condition;
	Condition = MessageBox(NULL, "是否要保存现有图像？", "保存", MB_YESNOCANCEL);
	if (Condition == 6) PreserveAllImages();//是返回值为6
	else if (Condition == 2 || Condition == 7);//取消返回值为2，否返回值为7
	ClearList();
}

void ClearList()
{
	int i;
	for (i = 0; i < NLIST; i++) {
		FreeLinkedList(list[i]);
		list[i] = NewLinkedList();
	}
}


void PickNearestNode(linkedlistADT list[], double mx, double my)
{

	int i, minlist;
	curList = INF;
	for (i = 1; i < NLIST - 1; i++)
		minDistance[i] = INF;

	curLine = SelectNearestNodeL(list[LINE], mx, my);
	curRect = SelectNearestNodeR(list[RECT], mx, my);
	curEllipse = SelectNearestNodeE(list[ELLIPSE], mx, my);
	curRTriangle = SelectNearestNodeRT(list[RTRIANGLE], mx, my);
	curText = SelectNearestNodeT(list[STRING], mx, my);

	if (!curLine && !curRect && !curEllipse && !curRTriangle && !curText) {
		curList = INF;
		return;
	}

	minlist = 0;

	for (i = 1; i < NLIST - 1; i++) {
		if (minDistance[i] < minDistance[minlist]) minlist = i;
	}
	if (minDistance[minlist] != INF)
		curList = minlist;
	else
		curList = INF;
}

LineT SelectNearestNodeL(linkedlistADT list, double mx, double my)
{
	linkedlistADT nearestnode = NULL, ptr;
	double mindistance, dist;

	ptr = NextNode(list, list);
	if (ptr == NULL) return NULL;
	nearestnode = ptr;
	mindistance = distLine(mx, my, (LineT)NodeObj(list, ptr));
	while (NextNode(list, ptr) != NULL) {
		ptr = NextNode(list, ptr);
		dist = distLine(mx, my, (LineT)NodeObj(list, ptr));
		if (dist < mindistance) {
			nearestnode = ptr;
			mindistance = dist;
		}
	}
	minDistance[LINE] = mindistance;

	return (LineT)NodeObj(list, nearestnode);
}

double distLine(double x, double y, LineT line)
{
	double x0, y0;

	x0 = (line->x1 + line->x2) / 2;
	y0 = (line->y1 + line->y2) / 2;
	if (inBox(x, y, line->x1, line->y1, line->x2, line->y2))
		return sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
	else
		return INF;
}

bool LineEqual(void *line1, void *line2)
{
	LineT lptr1 = (LineT)line1, lptr2 = (LineT)line2;

	return lptr1->x1 == lptr2->x1 && lptr1->y1 == lptr2->y1 &&
		lptr1->x2 == lptr2->x2 && lptr1->y2 == lptr2->y2;
}

RectT SelectNearestNodeR(linkedlistADT list, double mx, double my)
{
	linkedlistADT nearestnode = NULL, ptr;
	double mindistance, dist;

	ptr = NextNode(list, list);
	if (ptr == NULL) return NULL;
	nearestnode = ptr;
	mindistance = distRect(mx, my, (RectT)NodeObj(list, ptr));
	while (NextNode(list, ptr) != NULL) {
		ptr = NextNode(list, ptr);
		dist = distRect(mx, my, (RectT)NodeObj(list, ptr));
		if (dist < mindistance) {
			nearestnode = ptr;
			mindistance = dist;
		}
	}
	minDistance[RECT] = mindistance;

	return (RectT)NodeObj(list, nearestnode);
}

double distRect(double x, double y, RectT rect)
{
	double x0, y0;

	x0 = (rect->x1 + rect->x2) / 2;
	y0 = (rect->y1 + rect->y2) / 2;
	if (inBox(x, y, rect->x1, rect->y1, rect->x2, rect->y2))
		return sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
	else
		return INF;
}

bool RectEqual(void *rect1, void *rect2)
{
	RectT r1, r2;

	r1 = (RectT)rect1;
	r2 = (RectT)rect2;
	return r1->x1 == r2->x1 && r1->y1 == r2->y1 &&
		r1->x2 == r2->x2 && r1->y2 == r2->y2;
}

EllipseT SelectNearestNodeE(linkedlistADT list, double mx, double my)
{
	linkedlistADT nearestnode = NULL, ptr;
	double mindistance, dist;

	ptr = NextNode(list, list);
	if (ptr == NULL) return NULL;
	nearestnode = ptr;
	mindistance = distEllipse(mx, my, (EllipseT)NodeObj(list, ptr));
	while (NextNode(list, ptr) != NULL) {
		ptr = NextNode(list, ptr);
		dist = distEllipse(mx, my, (EllipseT)NodeObj(list, ptr));
		if (dist < mindistance) {
			nearestnode = ptr;
			mindistance = dist;
		}
	}
	minDistance[ELLIPSE] = mindistance;

	return (EllipseT)NodeObj(list, nearestnode);
}

double distEllipse(double x, double y, EllipseT ellipse)
{
	double x2 = ellipse->cx + ellipse->rx;
	double x1 = ellipse->cx - ellipse->rx;
	double y2 = ellipse->cy + ellipse->ry;
	double y1 = ellipse->cy - ellipse->ry;
	if (inBox(x, y, x1, y1, x2, y2))
		return sqrt((x - ellipse->cx)*(x - ellipse->cx) + (y - ellipse->cy)*(y - ellipse->cy));
	else
		return INF;
}

bool EllipseEqual(void *ellipse1, void *ellipse2)
{
	EllipseT eptr1 = (EllipseT)ellipse1, eptr2 = (EllipseT)ellipse2;

	return eptr1->cx == eptr2->cx && eptr1->cy == eptr2->cy &&
		eptr1->rx == eptr2->rx && eptr1->ry == eptr2->ry;

}

RTriangleT SelectNearestNodeRT(linkedlistADT list, double mx, double my)
{
	linkedlistADT nearestnode = NULL, ptr;
	double mindistance, dist;

	ptr = NextNode(list, list);
	if (ptr == NULL) return NULL;
	nearestnode = ptr;
	mindistance = distRTraingle(mx, my, (RTriangleT)NodeObj(list, ptr));
	while (NextNode(list, ptr) != NULL) {
		ptr = NextNode(list, ptr);
		dist = distRTraingle(mx, my, (RTriangleT)NodeObj(list, ptr));
		if (dist < mindistance) {
			nearestnode = ptr;
			mindistance = dist;
		}
	}
	minDistance[RTRIANGLE] = mindistance;

	return (RTriangleT)NodeObj(list, nearestnode);
}

double distRTraingle(double x, double y, RTriangleT RT)
{
	double x0, y0;
	x0 = (RT->x1 + RT->x2) / 2;
	y0 = (RT->y1 + RT->y2) / 2;
	if (inBox(x, y, RT->x1, RT->y1, RT->x2, RT->y2))
		return sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
	else
		return INF;
}

bool RTriangleEqual(void *rt1, void *rt2)
{
	RTriangleT rtptr1 = (RTriangleT)rt1, rtptr2 = (RTriangleT)rt2;
	return rtptr1->x1 == rtptr2->x1 && rtptr1->y1 == rtptr2->y1&&
		rtptr1->x2 == rtptr2->x2&& rtptr1->y2 == rtptr2->y2;
}

TextT SelectNearestNodeT(linkedlistADT list, double mx, double my)
{
	linkedlistADT nearestnode = NULL, ptr;
	double mindistance, dist;

	ptr = NextNode(list, list);
	if (ptr == NULL) return NULL;
	nearestnode = ptr;
	mindistance = distText(mx, my, (TextT)NodeObj(list, ptr));
	while (NextNode(list, ptr) != NULL) {
		ptr = NextNode(list, ptr);
		dist = distText(mx, my, (TextT)NodeObj(list, ptr));
		if (dist < mindistance) {
			nearestnode = ptr;
			mindistance = dist;
		}
	}
	minDistance[STRING] = mindistance;

	return (TextT)NodeObj(list, nearestnode);
}

double distText(double x, double y, TextT text)
{
	double x0 = text->x;
	double y0 = text->y;
	if (inTextBox(x, y, text->x, text->y))
		return sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
	else
		return INF;
}

bool TextEqual(void *text1, void *text2)
{
	TextT tptr1 = (TextT)text1, tptr2 = (TextT)text2;

	return StringEqual(tptr1->text, tptr2->text) &&
		tptr1->x == tptr2->x && tptr1->y == tptr2->y;
}

bool inBox(double x, double y, double x1, double y1, double x2, double y2)
{
	double temp;
	if (x1 > x2)
		temp = x1, x1 = x2, x2 = temp;
	if (y1 > y2)
		temp = y1, y1 = y2, y2 = temp;
	return (x <= x2 && x >= x1 && y >= y1 && y <= y2) ? TRUE : FALSE;
}

bool inTextBox(double x, double y, double x0, double y0)
{
	return x <= x0 + 5*SCALE && x >= x0 - 5*SCALE && y <= y0 + 2*SCALE && y >= y0 - 2*SCALE;
}

