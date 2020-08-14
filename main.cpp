#include<windows.h>
#include <GL/glut.h>
#include <math.h>
#include <chrono>
#include<bits/stdc++.h>
using namespace std;

#define PI acos(-1.0)
#define RAD(x) ((x*PI)/180)

double dx_ball_x = 0, dx_ball_y = -98;
double dx_ball_r = 1.5;
double reflector_x = 0, reflector_y = -98;
double reflector_r = 30, reflector_h = 1;
int direction = 1;
//chrono::steady_clock::time_point lastTime;
double fps = 240;
double angle = 90 - 22.5;  /** Degrees */
double new_angle = 90 - 22.5;
bool ball_is_moving;
const int font=(int)GLUT_BITMAP_9_BY_15;
int score = 0;
vector<vector<double>> bricks;
vector<string> grid = {
    "..##...##..",
    ".###...###.",
    "##*#####*##",
    "#**#####**#",
    "##*#***#*##",
    ".###***###.",
    "##*#***#*##",
    "#**#####**#",
    "##*#####*##",
    ".###...###.",
    "..##...##.."
};
int dx4[] = { 0, 0, -1, +1 };
int dy4[] = { +1, -1, 0, 0 };
vector<bool> done;
// 63.35835023

void renderBitmapString(float x, float y, void *font, string s){
    glRasterPos2f(x, y);
    for (char c : s) {
        glutBitmapCharacter(font, c);
    }
}

bool outOfBounds()
{
    int x1 = dx_ball_x - dx_ball_r;
    int x2 = x1 + 2 * dx_ball_r;
    int y1 = dx_ball_y - dx_ball_r;
    int y2 = y1 + 2 * dx_ball_r;
    if (x1 <= -100 || x2 >= 100) return 1;
    if (y1 <= -100 || y2 >= 100) return 1;
    return 0;
}

void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
}

void circle(float radius_x, float radius_y)
{
	int i=0;
	float angle = 0.0;
	glBegin(GL_POLYGON);
		for(i = 0; i < 200; i++) {
			angle = 2 * PI * i / 200;
			glVertex2f (cos(angle) * radius_x + dx_ball_x, sin(angle) * radius_y + dx_ball_y);
		}
	glEnd();
}

void refresh2()
{
    double msec = (1.0 / fps) * 1000;
    Sleep(msec);
    glutPostRedisplay();
}

void respawn()
{
    dx_ball_x = reflector_x;
    dx_ball_y = reflector_y + reflector_h + dx_ball_r + 1;
    direction = 1;
    ball_is_moving = 0;
    angle = 90 - 22.5;
    new_angle = 90 - 22.5;
}

double brick_width = 15;
double brick_height = 5.5;

vector<int> break_brick()
{
    vector<int> ret;
    for (int i = 0; i < bricks.size(); i++) {
        if (!done[i]) {
            double x1 = bricks[i][0];
            double x2 = x1 + brick_width;
            double y2 = bricks[i][1];
            double y1 = y2 - brick_height;
            for (int j = 0; j < 4; j++) {
                double nx = dx_ball_x + dx4[j] * dx_ball_r;
                double ny = dx_ball_y + dy4[j] * dx_ball_r;
                if (nx >= x1 && nx <= x2 && ny >= y1 && ny <= y2) {
                    ret.push_back(i);
                    break;
                }
            }
        }
    }
    return ret;
}

void g()
{
    for (auto ss : grid) {
        cout << ss << endl;
    }
    double p = 0.5;
    double top_left_x = -(brick_width * grid[0].size()) / 2.0;
    double top_left_y = 50;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            double curr_y = top_left_y - i * brick_height;
            double curr_x = top_left_x + j * brick_width;
            if (grid[i][j] == '#') {
                bricks.push_back({curr_x + p, curr_y - p, curr_x + brick_width - p, curr_y - brick_height + p, 1});
            }
            else if (grid[i][j] == '*') {
                bricks.push_back({curr_x + p, curr_y - p, curr_x + brick_width - p, curr_y - brick_height + p, 2});
            }
        }
    }
}

void print(vector<double> v)
{
    glPushMatrix();
//        if (v[4] == 1) glColor3f(0.412, 0.412, 0.412);
//        else glColor3f(0.502, 0.000, 0.000);
        if (v[4] == 2) glColor3f(1.000, 0.549, 0.000);
        else glColor3f(0.663, 0.663, 0.663);
        glRectf(v[0], v[1], v[2], v[3]);
    glPopMatrix();
}

queue<int> Q;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
        glColor3f(0.753, 0.753, 0.753);
        circle(dx_ball_r, dx_ball_r);
    glPopMatrix();

    glPushMatrix();
        glBegin(GL_POLYGON);
            glColor3f(0.118, 0.565, 1.000);
            glVertex2f(reflector_x - reflector_r, reflector_y + reflector_h);
            glVertex2f(reflector_x - reflector_r, reflector_y - reflector_h);
            glVertex2f(reflector_x + reflector_r, reflector_y - reflector_h);
            glVertex2f(reflector_x + reflector_r, reflector_y + reflector_h);
        glEnd();
    glPopMatrix();

    int cnt = 0;
    for (vector<double> v : bricks) {
        if (!done[cnt++]) print(v);
    }

    string ss;
    int temp = score;
    while (temp) {
        ss += (temp % 10) + '0';
        temp /= 10;
    }
    reverse(ss.begin(), ss.end());
    ss = "Current score: 0" + ss;
    glPushMatrix();
        renderBitmapString(100 - 65, 100 - 8, (void *)font, ss);
    glPopMatrix();

    ss.clear();
    temp = (int)angle;
    while (temp) {
        ss += (temp % 10) + '0';
        temp /= 10;
    }
    reverse(ss.begin(), ss.end());
    if (angle != (int)angle) ss += ".5";
    ss = "Current angle: " + ss;
    glPushMatrix();
        renderBitmapString(100 - 65, 100 - 15, (void *)font, ss);
    glPopMatrix();

    ss.clear();
    temp = fps;
    while (temp) {
        ss += (temp % 10) + '0';
        temp /= 10;
    }
    reverse(ss.begin(), ss.end());
    ss = "FPS: " + ss;
    glPushMatrix();
        renderBitmapString(100 - 65, 100 - 22, (void *)font, ss);
    glPopMatrix();

    glFlush();

    if (!ball_is_moving) return;

    double rad = RAD(angle);

    double perfect_speed_fps = 240;
    double fps_fix = perfect_speed_fps / fps;

    double incx = (cos(rad) / 2) * fps_fix;
    double incy = (sin(rad) / 2) * fps_fix;

    if (direction == 1) {
        dx_ball_x = dx_ball_x + incx;
        dx_ball_y = dx_ball_y + incy;
    }
    else if (direction == 2) {
        dx_ball_x = dx_ball_x - incx;
        dx_ball_y = dx_ball_y + incy;
    }
    else if (direction == 3) {
        dx_ball_x = dx_ball_x - incx;
        dx_ball_y = dx_ball_y - incy;
    }
    else {
        dx_ball_x = dx_ball_x + incx;
        dx_ball_y = dx_ball_y - incy;
    }

    vector<int> broken = break_brick();

    if (broken.size()) {
        for (int in : broken) Q.push(in);
        direction++;
        if (direction == 5) direction = 1;
    }
    else if (outOfBounds()) {
        if (dx_ball_y - dx_ball_r <= -100) {
            respawn();
        }
        else {
            direction++;
            if (direction == 5) direction = 1;
        }
    }
    else {
        if (dx_ball_x > (reflector_x - reflector_r) && dx_ball_x < (reflector_x + reflector_r)) {
            if (dx_ball_y - dx_ball_r <= reflector_y + reflector_h) {
                double segment = (dx_ball_x - (reflector_x - reflector_r)) / (reflector_r * 2);
                new_angle = angle;
                if (segment >= .30 && segment <= 0.70) {
                    angle += 22.5;
                }
                else  {
                    angle -= 22.5;
                }
                if (angle > 90 - 22.5) angle = 90 - 22.5;
                if (angle < 22.5) angle = 22.5;
//                cout << angle << endl;
                swap(angle, new_angle);
                direction++;
                if (direction == 5) direction = 1;
//                dx_ball_y = reflector_y + reflector_h + dx_ball_r + 1;
            }
            else {
                angle = new_angle;
                while (!Q.empty()) {
                    int top = Q.front();
                    Q.pop();
                    if (!done[top]) {
                        score++;
                        done[top] = 1;
                    }
                }
            }
        }
        else {
            angle = new_angle;
            while (!Q.empty()) {
                int top = Q.front();
                Q.pop();
                if (!done[top]) {
                    score++;
                    done[top] = 1;
                }
            }
        }
    }

    refresh2();
}

double each_step = 3.5;

void moveLeft(void)
{
    if (reflector_x >= -100 + reflector_r) {
        reflector_x -= each_step;
        if (!ball_is_moving) respawn();
        glutPostRedisplay();
    }
}

void moveRight(void)
{
    if (reflector_x <= 100 - reflector_r) {
        reflector_x += each_step;
        if (!ball_is_moving) respawn();
        glutPostRedisplay();
    }
}

void slower(void)
{
    glutPostRedisplay();
}

void faster(void)
{
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'a':
			moveLeft();
			break;
        case 'd':
			moveRight();
            break;
        case 32:
            if (!ball_is_moving) {
                ball_is_moving = 1;
                glutPostRedisplay();
            }
            break;
        case GLUT_KEY_UP:
            glutIdleFunc(faster);
        default:
            break;
	}
}

int main()
{
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("DX Ball 2D");
	init();
//	lastTime = std::chrono::steady_clock::now();
    g();
    done = vector<bool> (bricks.size(), 0);
    respawn();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
