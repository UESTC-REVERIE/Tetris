#include <graphics.h>
#include <conio.h>
#include <time.h>
#include<stdio.h>
#include <windows.h>
#define DOWN 80 //���������
#define LEFT 75 //���������
#define RIGHT 77 //���������
#define UP 72 //���������
#define SPACE 32 //�ո��
#define ESC 27 //Esc��
#define WIDTH 260//�����ܱ�20�����ĳ��Ϳ�
#define LENGHT 500
#define original_speed 100//�ٶȣ���ֵԽ��Խ��
int offset = 5;
int Line[4];
int speed, linenumb;
int type, state, x, y;
int down_low, down_up,down_left, down_right;
int moveable(int type, int state, int x, int y);
int score = 0;//��¼�÷�
//����ȫ��λ�õ�����,������Ϊ20,0�����޷��飬1�����з���
int visit[LENGHT/20][(WIDTH/20)+1]; //WRONG 1
//�ҳ������е���Сֵ
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
//�ҳ������е����ֵ
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
//��ɫ������
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
		setfont(25, 0, _T("����"));
		outtextxy(WIDTH / 3, LENGHT / 6, _T("��SPACE��ͣ"));
		outtextxy(WIDTH / 3, LENGHT / 6 + 30, _T("�������ƣ���������"));
		outtextxy(WIDTH / 3, LENGHT / 6 + 60, _T("������ת"));
		outtextxy(WIDTH / 3, LENGHT / 6 + 90, _T("��ס������"));
		Sleep(1000);
		outtextxy(WIDTH / 3, LENGHT / 6 + 150, _T("�������ʼ��Ϸ"));
		i++;
		if (i == 4)i = 0;
	}
}
//��Ϸ����
void over() {
	cleardevice();
	setcolor(RED);
	setfont(25, 0, _T("����"));
	outtextxy(WIDTH / 3, LENGHT / 2, _T("GAME OVER!"));
	outtextxy(WIDTH / 3, LENGHT / 2 + 30, _T("��SPACE���¿�ʼ"));
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
//���巽��(���淽����Ϣ�����͡�״̬��x��y)
int block[5][4][5][5] = {
	{//I����
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
	{//������
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
	{//L��
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
	{//z����
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
	{//T����
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
//��ָ��λ�û�ͼ�ĺ���
void draw(int type, int state, int x, int y) {
	setfont(20, 0,_T("����"));
	setcolor(choice_color(type));
	int down[4]; int left[4]; int right[4]; int up[4]; int n = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[type][state][i][j] == 1&&moveable(type,state,x,y)==1) {
			outtextxy(x + 20 * j, y + 20 * i, _TEXT("��"));
			//visit[y / 20][x / 20]++;
			down[n] = y + 20 * i;
			right[n] = x + 20 * j;
			left[n] = x+20*j;
			up[n] = y+20*i;
			n++;
			}
		}
	}
	down_low= max_num(down)+20;//���ض˵������
	down_right = max_num(right)+20;
	down_left = min_num(left);
	down_up = min_num(up);
}
//�������
void clear(int type, int state, int x, int y) {
	setcolor(BLACK);
	setfont(20, 0, _T("����"));
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[type][state][i][j] == 1) {
				outtextxy(x + 20 * j, y + 20 * i, _TEXT("��"));
				//visit[y / 20][x / 20]--;
			}
		}
	}

}
//�ж��ܷ��ƶ�
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
//�̶�������
void iniblock(int type, int state, int x, int y) {
	setfont(20, 0, _T("����"));
	setcolor(choice_color(type));
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[type][state][i][j] == 1) {
				outtextxy(x + 20 * j, y + i * 20 , _TEXT("��"));//change
				visit[(y / 20)+i][(x / 20)+j]++;
			}
		}
	}
}
//�жϵ÷�
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
	
	return 0;//û�����з���0
}
//�������
void clear_line(int i) {
	for (int j = 0; j < WIDTH / 20; j++) {
		visit[i][j] = 0;
		setcolor(BLACK);
		setfont(20, 0, _T("����"));
		outtextxy(j * 20, i * 20, _TEXT("��"));
	}
}
//����ϵͳ
void start(int score) {
	char str[20];
	setcolor(BLACK);
	outtextxy(WIDTH + 45, LENGHT / 3, _TEXT("      "));
	setcolor(WHITE);
	setfont(23, 0, _T("����"));
	sprintf_s(str,"%d", score);
	line(0, 80-offset, WIDTH, 80-offset);
	for (int n = 0; n < LENGHT; n += 20) {
		outtextxy(WIDTH + 1, n, _TEXT("||"));
	}
	outtextxy(WIDTH+35, LENGHT/4, _TEXT("�÷֣�"));
	outtextxy(WIDTH + 45, LENGHT / 3,str);
	
}
//���²���
void remake(int line) {
	cleardevice();
	//int mid[LENGHT / 20][WIDTH / 20];
	//memset(mid, 0, sizeof(mid));
	for (int i = line-1; i >0; i--) {
		for (int j=0; j < WIDTH / 20; j++) {
			if (visit[i][j] == 1) {
				//setcolor(YELLOW);
				//setfont(23, 0, _T("����"));
				//outtextxy(j * 20, (i+1)*20+15, _TEXT("��"));
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
				setfont(20, 0, _T("����"));
				outtextxy(j * 20, (i + 1) * 20-offset , _TEXT("��"));
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
				setfont(20, 0, _T("����"));
				outtextxy(j * 20, (i + 1) * 20 - offset, _TEXT("��"));
			}
		}
	}
}
//��Ϸ�����߼�
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
				//��������
				draw(type, state, x, y);
				FlushBatchDraw();
				Sleep(speed);
				speed = original_speed;
				clear(type, state, x, y);
				//���̿���
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
