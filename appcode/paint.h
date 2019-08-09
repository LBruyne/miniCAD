#ifndef _PAINT_H_
#define _PAINT_H_

// 以下对几类图形元素结构化定义
typedef struct { //直线
	double x1, y1; //第一个点坐标，下同
	double x2, y2; //第二个点坐标，下同
	int PenSize;
	string color;
	bool isSelected;
} *LineT;

typedef struct { //矩形
	double x1, y1; //第一个点坐标，下同
	double x2, y2; //第二个点坐标，下同
	int PenSize;
	double Fillin; //是否填充
	string color;
	bool isSelected;
} *RectT;

typedef struct { //椭圆和圆
	double cx, cy; //圆心
	double rx, ry; //长短半径
	int PenSize;
	double Fillin;
	string color;
	bool isSelected;
} *EllipseT;

typedef struct { //直角三角形
	double x1, y1;
	double x2, y2;
	int PenSize;
	double Fillin;
	string color;
	bool isSelected;
}*RTriangleT;

typedef struct { //点
	double x1, y1;
	double x2, y2;
	int PenSize;
	string color;
	bool isSelected;
}*PointT;

typedef struct {/*文本类型*/
	string text;/*文本指针*/
	double x, y;/*文本显示起始位置坐标*/
	int PointSize; /*文字大小*/
	string color; /*颜色*/
	bool isSelected; /*选中*/
	int curPos; /*光标位置*/
	bool isDisplayed; /*光标是否处于显示状态*/
} *TextT;

// 重绘所有图像
void drawAllImages();

// 对各类图形的绘制函数
void DrawCenteredEllipse(void *ellipse);/*画椭圆*/

void DrawLineD(void *line);

void DrawRect(void *rect);

void DrawRightTriangle(void *rtriangle);

void drawRightTriangle(double x, double y, double w, double h);

void DrawPoint(void *point);

void DrawTextT(void *text); /*显示文本*/

#endif // !_PAINT_H_

