#ifndef _BASICFUN_H_
#define _BASICFUN_H_

//新建函数
void New_Built();

//复制图形函数
void CopyGraphic();

//粘贴图形函数
void PasteGraphic();

//删除图形函数
void DeleteGraphic();

//保存函数
void PreserveAllImages();

void PreserveLine(void *line);

void PreserveRect(void *rect);

void PreserveEllipse(void *ellipse);

void PreserveRTriangle(void *rtriangle);

void PreservePoint(void *point);

//清除文件函数
void ClearFile();

//清除屏幕函数
void ClearList();

//读取函数
void ReadAndGetLine();

void ReadAndGetRect();

void ReadAndGetPoint();

void ReadAndGetRTriangle();

void ReadAndGetEllipse();

void ReadAndGetAll();

//以下为选择操作需要用到的函数
void PickNearestNode(linkedlistADT list[], double mx, double my);

/*椭圆处理函数*/
EllipseT SelectNearestNodeE(linkedlistADT list, double mx, double my);/*选择靠(mx,my)最近的结点*/

bool EllipseEqual(void *ellipse1, void *ellipse2);/*比较两个椭圆相等*/

double distEllipse(double x, double y, EllipseT ellipse);

/*直线处理函数*/
LineT SelectNearestNodeL(linkedlistADT list, double mx, double my);

bool LineEqual(void *line1, void *line2);

double distLine(double x, double y, LineT line);

/*矩形处理函数*/
RectT SelectNearestNodeR(linkedlistADT list, double mx, double my);

bool RectEqual(void *rect1, void *rect2);

double distRect(double x, double y, RectT rect);

/*直角三角形处理函数*/
RTriangleT SelectNearestNodeRT(linkedlistADT list, double mx, double my);

bool RTriangleEqual(void *rt1, void *rt2);

double distRTraingle(double x, double y, RTriangleT RT);

/*文本处理函数*/
TextT SelectNearestNodeT(linkedlistADT list, double mx, double my);

bool TextEqual(void *text1, void *text2);/*比较两个文本是否相同*/

double distText(double x, double y, TextT text);

/*选中判断函数(对于选择功能的辅助)*/
bool inBox(double x, double y, double x1, double y1, double x2, double y2);

bool inTextBox(double x, double y, double x0, double y0);

#endif // !_BASICFUN_H_
