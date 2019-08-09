#ifndef _HARDWARE_H_
#define _HARDWARE_H_

// 注册消息函数
void KeyboardEventProcess(int key, int event);

void MouseEventProcess(int x, int y, int button, int event);

void TimerEventProcess(int timerID);

void CharEventProcess(char ch);

// 字符输入辅助函数
void InsertCharToString(string str, int pos, char c);/*将字符c插入到字符串str的pos位置*/

void DeleteCharFromString(string str, int pos);/*删除字符串str的pos位置字符*/

void DrawCursor(string str, int curPos, double startx, double starty);/*显示文本光标*/

// 区域判断辅助函数
bool isWithinBound(double mx, double my);

#endif // !_HARDWARE_H_

