#include <graphics.h>
#include <conio.h>
#include <time.h>
#include<stdio.h>
#include <windows.h>
#define DOWN 80 //方向键：下
#define LEFT 75 //方向键：左
#define RIGHT 77 //方向键：右
#define UP 72 //方向键：上
#define SPACE 32 //空格键
#define ESC 27 //Esc键
#define WIDTH 260//设置能被20整除的长和宽
#define LENGHT 500
#define original_speed 100//速度，此值越大越慢
int offset = 5;
int Line[4];
int speed, linenumb;
int type, state, x, y;
int down_low, down_up,down_left, down_right;
int moveable(int type, int state, int x, int y);
int score = 0;//记录得分
//访问全局位置的数组,■长度为20,0代表无方块，1代表有方块
int visit[LENGHT/20][(WIDTH/20)+1]; //WRONG 1
//找出数组中的最小值
int min_num(int* a)
{

	int min = a[0];
	for (int i = 1; i < 4; i++) {
		if (a[i] < min) {
			min = a[i];
		}
	}
	return min;
}
//找出数组中的最大值
int max_num(int*a)
{
	
	int max = a[0];
	for (int i = 1; i < 4; i++) {
		if (a[i] > max) {
			max = a[i];
		}
	}
	return max;
}
//颜色处理函数
int choice_color(int type) {
	int color;
	switch (type) {
	case 0:color = YELLOW; break;
	case 1:color = BLUE; break;
	case 2:color = RED; break;
	case 3:color = MAGENTA; break;
	case 4:color = CYAN; break;
	}
	return color;
}
void beginning() {
	initgraph(WIDTH + 150, LENGHT);
	//setbkcolor(WHITE);
	int i = 0;
	while (!_kbhit()) {
		setcolor(choice_color(i));
		setfont(25, 0, _T("楷体"));
		outtextxy(WIDTH / 3, LENGHT / 6, _T("按SPACE暂停"));
		outtextxy(WIDTH / 3, LENGHT / 6 + 30, _T("按←左移，按→右移"));
		outtextxy(WIDTH / 3, LENGHT / 6 + 60, _T("按↑旋转"));
		outtextxy(WIDTH / 3, LENGHT / 6 + 90, _T("按住↓加速"));
		Sleep(1000);
		outtextxy(WIDTH / 3, LENGHT / 6 + 150, _T("任意键开始游戏"));
		i++;
		if (i == 4)i = 0;
	}
}
//游戏结束
void over() {
	cleardevice();
	setcolor(RED);
	setfont(25, 0, _T("楷体"));
	outtextxy(WIDTH / 3, LENGHT / 2, _T("GAME OVER!"));
	outtextxy(WIDTH / 3, LENGHT / 2 + 30, _T("按SPACE重新开始"));
	if (_getch() == SPACE) {
		
		cleardevice();
		score = 0;
		//setbkcolor(BLACK);
		memset(visit, 0, sizeof(visit));
		for (int m = 0; m < LENGHT / 20; m++) {
			visit[m][WIDTH / 20] = 1;//wrong
		}
		for (int k = 0; k < WIDTH / 20; k++) {//change
			visit[0][k] = -2;
		}
		speed = original_speed;
		return;
	}
	system("pause");
}
//定义方块(储存方块信息：类型、状态、x、y)
int block[5][4][5][5] = {
	{//I字形
		{
			0,0,0,0,0,
			0,1,0,0,0,
			0,1,0,0,0,
			0,1,0,0,0,
			0,1,0,0,0 
		},
		{
			0,0,0,0,0,
			0,0,0,0,0,
			1,1,1,1,0,
			0,0,0,0,0,
			0,0,0,0,0
		},
		{
			0,0,0,0,0,
			0,1,0,0,0,
			0,1,0,0,0,
			0,1,0,0,0,
			0,1,0,0,0
		},
		{
			0,0,0,0,0,
			0,0,0,0,0,
			1,1,1,1,0,
			0,0,0,0,0,
			0,0,0,0,0
		}
	},
	{//田字形
		{
			0,0,0,0,0,
			0,1,1,0,0,
			0,1,1,0,0,
			0,0,0,0,0,
			0,0,0,0,0
		},
		{
			0,0,0,0,0,
			0,1,1,0,0,
			0,1,1,0,0,
			0,0,0,0,0,
			0,0,0,0,0
		},
		{
			0,0,0,0,0,
			0,1,1,0,0,
			0,1,1,0,0,
			0,0,0,0,0,
			0,0,0,0,0
		},
		{
			0,0,0,0,0,
			0,1,1,0,0,
			0,1,1,0,0,
			0,0,0,0,0,
			0,0,0,0,0
		}
	
	},
	{//L形
		{
			0,0,0,0,0,
			0,1,0,0,0,
			0,1,0,0,0,
			0,1,1,0,0,
			0,0,0,0,0
		},
		{
			0,0,0,0,0,
			0,0,0,0,0,
			1,1,1,0,0,
			1,0,0,0,0,
			0,0,0,0,0
		},
		{
			0,0,0,0,0,
			1,1,0,0,0,
			0,1,0,0,0,
			0,1,0,0,0,
			0,0,0,0,0
		},
		{
			0,0,0,0,0,
			0,0,1,0,0,
			1,1,1,0,0,
			0,0,0,0,0,
			0,0,0,0,0
		}
	},
	{
	{//z字形
			0,0,0,0,0,
			1,1,0,0,0,
			0,1,1,0,0,
			0,0,0,0,0,
			0,0,0,0,0
		},
		{
			0,0,0,0,0,
			0,0,1,0,0,
			0,1,1,0,0,
			0,1,0,0,0,
			0,0,0,0,0
		},
		{

			0,0,0,0,0,
			1,1,0,0,0,
			0,1,1,0,0,
			0,0,0,0,0,
			0,0,0,0,0		},
		{
			0,0,0,0,0,
			0,0,1,0,0,
			0,1,1,0,0,
			0,1,0,0,0,
			0,0,0,0,0
		}
	},
	{//T字形
		{

			0,0,0,0,0,
			1,1,1,0,0,
			0,1,0,0,0,
			0,0,0,0,0,
			0,0,0,0,0
		},
		{
			0,0,0,0,0,
			0,0,1,0,0,
			0,1,1,0,0,
			0,0,1,0,0,
			0,0,0,0,0
		},
		{

			0,0,0,0,0,
			0,0,0,0,0,
			0,1,0,0,0,
			1,1,1,0,0,
			0,0,0,0,0 },
		{
			0,0,0,0,0,
			1,0,0,0,0,
			1,1,0,0,0,
			1,0,0,0,0,
			0,0,0,0,0
		}
	}
};
//在指定位置画图的函数
void draw(int type, int state, int x, int y) {
	setfont(20, 0,_T("楷体"));
	setcolor(choice_color(type));
	int down[4]; int left[4]; int right[4]; int up[4]; int n = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[type][state][i][j] == 1&&moveable(type,state,x,y)==1) {
			outtextxy(x + 20 * j, y + 20 * i, _TEXT("■"));
			//visit[y / 20][x / 20]++;
			down[n] = y + 20 * i;
			right[n] = x + 20 * j;
			left[n] = x+20*j;
			up[n] = y+20*i;
			n++;
			}
		}
	}
	down_low= max_num(down)+20;//返回端点的坐标
	down_right = max_num(right)+20;
	down_left = min_num(left);
	down_up = min_num(up);
}
//清除函数
void clear(int type, int state, int x, int y) {
	setcolor(BLACK);
	setfont(20, 0, _T("楷体"));
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[type][state][i][j] == 1) {
				outtextxy(x + 20 * j, y + 20 * i, _TEXT("■"));
				//visit[y / 20][x / 20]--;
			}
		}
	}

}
//判断能否移动
int moveable(int type, int state, int x, int y) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[type][state][i][j] == 1) {
				visit[(y / 20)+i][(x / 20)+j]++;
			}
		}
	}
	for (int i = 0; i < LENGHT / 20; i++) {
		for (int j = 0; j <=WIDTH / 20; j++) { //WRONG 2
			if (visit[i][j] >1 ) { 
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {
						if (block[type][state][i][j] == 1) {
							visit[(y / 20) + i][(x / 20) + j]--;
						}
					}
				}
				return 0; 
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[type][state][i][j] == 1) {
				visit[(y / 20) + i][(x / 20) + j]--;
			}
		}
	}
	return 1;
}
//固定化方块
void iniblock(int type, int state, int x, int y) {
	setfont(20, 0, _T("楷体"));
	setcolor(choice_color(type));
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[type][state][i][j] == 1) {
				outtextxy(x + 20 * j, y + i * 20 , _TEXT("■"));//change
				visit[(y / 20)+i][(x / 20)+j]++;
			}
		}
	}
}
//判断得分
int judge() {
	int target = 0;
	int n = 0;
	int sumtarget=0;
	memset(Line, -1, sizeof(Line));
	for (int i = 0; i < LENGHT / 20; i++) {
		for (int j = 0; j < WIDTH / 20; j++) {
			if (visit[i][j] == 1)target++;
		}
		if (target == WIDTH / 20) { Line[n] = i; n++; sumtarget += target;}
		target = 0;
	}
	if (sumtarget == WIDTH / 20 || sumtarget == 2*(WIDTH / 20) || sumtarget == 3 * (WIDTH / 20) || sumtarget == 4 * (WIDTH / 20)) {
		linenumb = sumtarget / (WIDTH / 20);
		return score += (linenumb * (linenumb + 1) / 2)*100;
	}
	
	return 0;//没有满行返回0
}
//清除此行
void clear_line(int i) {
	for (int j = 0; j < WIDTH / 20; j++) {
		visit[i][j] = 0;
		setcolor(BLACK);
		setfont(20, 0, _T("楷体"));
		outtextxy(j * 20, i * 20, _TEXT("■"));
	}
}
//分数系统
void start(int score) {
	char str[20];
	setcolor(BLACK);
	outtextxy(WIDTH + 45, LENGHT / 3, _TEXT("      "));
	setcolor(WHITE);
	setfont(23, 0, _T("楷体"));
	sprintf_s(str,"%d", score);
	line(0, 80-offset, WIDTH, 80-offset);
	for (int n = 0; n < LENGHT; n += 20) {
		outtextxy(WIDTH + 1, n, _TEXT("||"));
	}
	outtextxy(WIDTH+35, LENGHT/4, _TEXT("得分："));
	outtextxy(WIDTH + 45, LENGHT / 3,str);
	
}
//重新布局
void remake(int line) {
	cleardevice();
	//int mid[LENGHT / 20][WIDTH / 20];
	//memset(mid, 0, sizeof(mid));
	for (int i = line-1; i >0; i--) {
		for (int j=0; j < WIDTH / 20; j++) {
			if (visit[i][j] == 1) {
				//setcolor(YELLOW);
				//setfont(23, 0, _T("楷体"));
				//outtextxy(j * 20, (i+1)*20+15, _TEXT("■"));
				visit[i][j] = 0;
				visit[i+1][j] = 1;
			}
		}
	}
	//memset(visit, 0, sizeof(visit));
	//memcpy(visit, mid, sizeof(mid));
	for (int i = 0; i <LENGHT/20; i++) {
		for (int j = 0; j < WIDTH / 20; j++) {
			if (visit[i][j] == 1) {
				setcolor(choice_color(type));
				setfont(20, 0, _T("楷体"));
				outtextxy(j * 20, (i + 1) * 20-offset , _TEXT("■"));
			}
		}
	}
}
void remakex() {
	cleardevice();
	start(score);
	for (int i = 0; i < LENGHT / 20; i++) {
		for (int j = 0; j < WIDTH / 20; j++) {
			if (visit[i][j] == 1) {
				setcolor(choice_color(type));
				setfont(20, 0, _T("楷体"));
				outtextxy(j * 20, (i + 1) * 20 - offset, _TEXT("■"));
			}
		}
	}
}
//游戏主体逻辑
	int main(){
		beginning();
		cleardevice();
		//setbkcolor(BLACK);
		memset(visit, 0, sizeof(visit));
		for (int m = 0; m < LENGHT / 20; m++) {
			visit[m][WIDTH / 20] = 1;//wrong
		}
		for (int k=0; k < WIDTH / 20; k++) {//change
			visit[0][k] = -2;
		}
		speed = original_speed;
		//system("mode con lines=25 cols=35");
		
		start(0);
		srand(time(NULL));
		while(1) {
			type = rand() % 5;
			state = rand() % 4;
			BeginBatchDraw();
			for (x = 120, y = 0; moveable(type, state, x, y) == 1; y += offset) {
				//方块下落
				draw(type, state, x, y);
				FlushBatchDraw();
				Sleep(speed);
				speed = original_speed;
				clear(type, state, x, y);
				//键盘控制
				if (_kbhit()) {
					char a = _getch();
					switch (a) {
					case LEFT:if (moveable(type, state, x - 20, y) == 1 && down_left >= 20)   x -= 20;  break;
					case RIGHT:if(moveable(type, state, x + 20, y ) == 1 && down_right <= WIDTH-20 ) x += 20;  break;
					case UP:if(state==3&& moveable(type, 0, x , y) == 1)state = 0;else if(state!=3&&moveable(type, state+1, x, y) == 1) state++;  break;
					case DOWN:if(speed> original_speed-1)speed -= original_speed-1;  break;
					case SPACE:draw(type, state, x, y+offset); system("pause>nul"); break;
					}
				}
				
				if (down_low >= LENGHT) break; 
				start(score);
			}
			EndBatchDraw();
			iniblock(type, state, x, y-offset);
			remakex();
			//change
			int max = visit[0][0];
				for (int i = 1; i < WIDTH/20; i++) {
					if (visit[1][i] > max) {
						max = visit[1][i];
					}
				}
				if (max == 1) { over(); }
			
			if (judge()) {
				
				for (int n = 0; n < 4 && Line[n]!=-1; n++) {
					clear_line(Line[n]);
					remake(Line[n]);
				}
				start(judge());
			}
		//change
			Sleep(100);
		}
			closegraph();
			system("pause");
	}
