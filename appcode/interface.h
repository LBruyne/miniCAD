#ifndef _INTERFACE_H_
#define _INTERFACE_H_

// 用户的显示函数
void display(void);

// 清屏函数，provided in libgraphics
void DisplayClear(void);

// 菜单绘制函数
void drawMenu();

// 按钮绘制函数
void drawButtons();

// 旋转功能函数
void ClockWiseRotateLine(LineT obj, int mode);

void ClockWiseRotateRect(RectT obj, int mode);

void ClockWiseRotateRT(RTriangleT obj, int mode);

void ClockWiseRotateEllipse(EllipseT obj);

#endif // !_INTERFACE_H_

