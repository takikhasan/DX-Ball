#include<windows.h>
#include <GL/glut.h>
#include<bits/stdc++.h>
using namespace std;



/// CONSTANTS   ***     ***     ***

#define PI acos(-1.0)
#define RAD(x) ((x*PI)/180)
const int font=(int)GLUT_BITMAP_HELVETICA_12;
const int font_menu=(int)GLUT_BITMAP_9_BY_15;
const double dx_ball_r = 1.5; /** ball radius */
const double reflector_r = 30, reflector_h = 1; /** Half of relfector width, half of reflector height */
const double brick_width = 15;
const double brick_height = 5.5;
const vector<vector<string>> grid = {
//    {
//        "*#"
//    },
//    {
//        "*#"
//    }
    {
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
    },
    {
        "............",
        ".....##.....",
        ".##.#**#.##.",
        ".#*##**##*#.",
        "..#*.**.*#..",
        "#**.*..*.**#",
        "#**.*..*.**#",
        "..#*.**.*#..",
        ".#*##**##*#.",
        ".##.#**#.##.",
        ".....##.....",
        "............"
    },
    {
        ".#*.....*#.",
        "#*.......*#",
        "*...*.*...*",
        "...*.*.*...",
        "..#.*#*.#..",
        ".#.#.*.#.#.",
        "..#.*#*.#..",
        "...*.*.*...",
        "*...*.*...*",
        "#*.......*#",
        ".#*.....*#."
    }
};
const int dx4[] = { 0, 0, -1, +1 };
const int dy4[] = { +1, -1, 0, 0 };



/// OTHER VARIABLES     ***     ***     ***

int total_options;
double dx_ball_x = 0, dx_ball_y = -98;
double reflector_x = 0, reflector_y = -98;
int direction = 1;
double fps = 200;
double angle = 90 - 22.5;  /** Degrees */
double new_angle = 90 - 22.5;   /** Temporary variable to help adjust angle */
bool ball_is_moving = 0;
int score = 0;
int lives_left = 5;
double each_step = 3.5; /** Increase for more speed */
bool menu_screen = 1;   /** Are we at the menu screen */
bool game_finished_screen = 0;
bool game_over_screen = 0;
bool FPS_screen = 0;
bool save_screen = 0;
bool load_screen = 0;
int menu_highlight = 1; /** Where we at on the menu */
int FPS_highlight = 1;  /** Where we at on the FPS menu */
int save_highlight = 1; /** Where we at on the save menu */
int load_highlight = 1;
bool sound_on = 1;
bool game_running = 0;  /** Is any game running */
int current_level = 0;  /** Current level of the game */
vector<vector<double>> bricks;  /** From the 2d string, actual coordinates are stored here */
vector<bool> done;  /** done[i] = 1 means i'th brick has been destroyed permanently */
queue<int> Q;   /** Queue of bricks to be destroyed */



/// ALL FUNCTION DECLARATIONS   ***     ***     ***

void init(void);    /** Initialize axis */

void circle(double radius_x, double radius_y);  /** Draws the ball, basically */

bool outOfBounds(); /** Collision with boundary */

void reshape(int width, int height);    /** Override reshape function */

void renderBitmapString(double x, double y, void *font, string s);  /** Print any string at any coordinate */

void refresh(); /** For FPS */

void respawn(); /** For dying */

vector<int> queue_bricks(); /** Get id of bricks that just got marked for destruction */

void destroy_bricks();  /** Destroy all the bricks from the queue permanently and update score */

void loadBricks(int level);  /** Load bricks at the start of level from vector */

void print(vector<double> v); /** Print one brick */

void moveLeft(void);    /** Move the reflector left */

void moveRight(void);   /** Move the reflector right */

void keyboard(unsigned char key, int x, int y); /** Keyboard {space, 'a', 'd'} */

void spe_key(int key, int x, int y);    /** Keyboard {left key, right key} */

void displayGame(); /** If we are not in the menu screen, print every object and string of the current game state */

void takeOneStep(); /** Move the ball one step, calculate the effects and change game state accordingly */

void displayMenu(); /** If we are in the menu screen, display options */

void display(void);

void printLives()
{
    int z = lives_left;
    double x = -94;
    double y = 96;
    double unit_len = 1.5;
    int cnt = 0;
    while (z--) {
        vector<pair<double, double>> v;
        v.push_back({x, y});
        v.push_back({x - unit_len, y + unit_len});
        v.push_back({x - 2 * unit_len, y});
        v.push_back({x - 2 * unit_len, y - unit_len});
        v.push_back({x, y - 3 * unit_len});
        v.push_back({x + 2 * unit_len, y - unit_len});
        v.push_back({x + 2 * unit_len, y});
        v.push_back({x + unit_len, y + unit_len});
        glPushMatrix();
            glBegin(GL_POLYGON);
                for (auto p : v) {
                    if (cnt % 2 == 0)
                        glColor3f(0.545, 0.000, 0.000);
                    else
                        glColor3f(1.000, 0.000, 0.000);
                    glVertex2f(p.first, p.second);
                    cnt++;
                }
            glEnd();
        glPopMatrix();
        x += 4 * unit_len + 1;
    }
}

void newGame()
{
    dx_ball_x = 0, dx_ball_y = -98;
    reflector_x = 0, reflector_y = -98;
    direction = 1;
    angle = 90 - 22.5;  /** Degrees */
    new_angle = 90 - 22.5;   /** Temporary variable to help adjust angle */
    ball_is_moving = 0;
    score = 0;
    lives_left = 5;
    each_step = 3.5; /** Increase for more speed */
    menu_screen = false;   /** Are we at the menu screen */
    menu_highlight = 1; /** Where we at on the menu */
    game_running = true;  /** Is any game running */
    current_level = 0;  /** Current level of the game */
    bricks.clear();  /** From the 2d string, actual coordinates are stored here */
    done.clear();  /** done[i] = 1 means i'th brick has been destroyed permanently */
    Q = queue<int>();   /** Queue of bricks to be destroyed */

    loadBricks(current_level);
    done = vector<bool> (bricks.size(), 0);
    respawn();

    glutPostRedisplay();
}

void resumePlaying()
{
    menu_screen = false;
    glutPostRedisplay();
}

void quit()
{
    exit(0);
}

void displaySuccessMessage()
{
    string s1 = "You have finished the game!!!";
    string s2 = "Your score: " + to_string(score) + " (Highest Possible)";
    string s3 = "Press Enter to continue";
    double y = 12;
    glColor3f(1.000, 0.843, 0.000);
    renderBitmapString(-(13.0 / 8.0) * s1.size(), y, (void *)font_menu, s1);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(-(13.0 / 8.0) * s2.size(), y - 8, (void *)font_menu, s2);
    glColor3f(0.980, 0.922, 0.843);
    renderBitmapString(-(13.0 / 8.0) * s3.size(), y - 16, (void *)font_menu, s3);
}

void displayFailureMessage()
{
    string s1 = "You are out of lives!!!";
    string s2 = "Your score: " + to_string(score);
    string s3 = "Press Enter to continue";
    double y = 12;
    glColor3f(1.000, 0.000, 0.000);
    renderBitmapString(-(13.0 / 8.0) * s1.size(), y, (void *)font_menu, s1);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(-(13.0 / 8.0) * s2.size(), y - 8, (void *)font_menu, s2);
    glColor3f(0.980, 0.922, 0.843);
    renderBitmapString(-(13.0 / 8.0) * s3.size(), y - 16, (void *)font_menu, s3);
}

void changeFPS()
{
    FPS_screen = true;
    menu_screen = false;
    glutPostRedisplay();
}

void displayFPSOptions()
{
    total_options = 4;  /** 60, 110, 200, 330 */
    double option_width = 80;
    double option_height = 15;
    double x = -option_width / 2;
    double y = (option_height * total_options) / 2;
    int i = 1;
    while (1) {
        if (FPS_highlight == i) {
            glColor3f(1.000, 0.843, 0.000);
        }
        else {
            glColor3f(0.980, 0.922, 0.843);
        }

        if (i == 1) {
            renderBitmapString(-(13.0 / 8.0) * 2, y - 9, (void *)font_menu, "60");
        }
        else if (i == 2) {
            renderBitmapString(-(13.0 / 8.0) * 3, y - 9, (void *)font_menu, "110");
        }
        else if (i == 3) {
            renderBitmapString(-(13.0 / 8.0) * 3, y - 9, (void *)font_menu, "200");
        }
        else if (i == 4) {
            renderBitmapString(-(13.0 / 8.0) * 3, y - 9, (void *)font_menu, "330");
        }

        if (FPS_highlight == i) {
            glColor3f(1.000, 0.843, 0.000);
        }
        else {
            glColor3f(0.980, 0.922, 0.843);
        }

        glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x, y - option_height);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(x, y - option_height);
            glVertex2f(x + option_width, y - option_height);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(x + option_width, y - option_height);
            glVertex2f(x + option_width, y);
        glEnd();
         glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x + option_width, y);
        glEnd();

        y -= option_height + 2;

        i++;
        if (i > total_options) break;
    }
}

void save()
{
    if (game_running == false) return;
    save_screen = true;
    menu_screen = false;
    glutPostRedisplay();
}

void displaySaveSlots()
{
    total_options = 4;  /** Slot 1, Slot 2, Slot 3, Slot 4 */
    double option_width = 80;
    double option_height = 15;
    double x = -option_width / 2;
    double y = (option_height * total_options) / 2;
    int i = 1;
    while (1) {
        if (save_highlight == i) {
            glColor3f(1.000, 0.843, 0.000);
        }
        else {
            glColor3f(0.980, 0.922, 0.843);
        }

        if (i == 1) {
            renderBitmapString(-(13.0 / 8.0) * 6, y - 9, (void *)font_menu, "Slot 1");
        }
        else if (i == 2) {
            renderBitmapString(-(13.0 / 8.0) * 6, y - 9, (void *)font_menu, "Slot 2");
        }
        else if (i == 3) {
            renderBitmapString(-(13.0 / 8.0) * 6, y - 9, (void *)font_menu, "Slot 3");
        }
        else if (i == 4) {
            renderBitmapString(-(13.0 / 8.0) * 6, y - 9, (void *)font_menu, "Slot 4");
        }

        if (save_highlight == i) {
            glColor3f(1.000, 0.843, 0.000);
        }
        else {
            glColor3f(0.980, 0.922, 0.843);
        }

        glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x, y - option_height);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(x, y - option_height);
            glVertex2f(x + option_width, y - option_height);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(x + option_width, y - option_height);
            glVertex2f(x + option_width, y);
        glEnd();
         glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x + option_width, y);
        glEnd();

        y -= option_height + 2;

        i++;
        if (i > total_options) break;
    }
}

void saveGame(int slot)
{
    char s[] = "SaveFile_ .txt";
    s[9] = '0' + slot;
    ofstream fout;
    fout.open(s);

    fout << setprecision(6) << fixed;
    fout << dx_ball_x << " " << dx_ball_y << endl;
    fout << reflector_x << " " << reflector_y << endl;
    fout << direction << endl;
    fout << angle << endl;
    fout << new_angle << endl;
    fout << ball_is_moving << endl;
    fout << score << endl;
    fout << lives_left << endl;
    fout << current_level << endl;
    fout << (int)bricks.size() << endl;
    for (vector<double> v : bricks) {
        for (double el : v) {
            fout << el << " ";
        }
        fout << endl;
    }
    fout << (int)done.size() << endl;
    for (int el : done) {
        fout << el << " ";
    }
    fout << endl;
    queue<int> temp = Q;
    fout << (int)temp.size() << endl;
    while (!temp.empty()) {
        int top = temp.front();
        temp.pop();
        fout << top << " ";
    }
    fout << endl;

    fout.close();
}

void load()
{
    load_screen = true;
    menu_screen = false;
    glutPostRedisplay();
}

void displayLoadSlots()
{
    total_options = 4;  /** Slot 1, Slot 2, Slot 3, Slot 4 */
    double option_width = 80;
    double option_height = 15;
    double x = -option_width / 2;
    double y = (option_height * total_options) / 2;
    int i = 1;
    while (1) {
        if (load_highlight == i) {
            glColor3f(1.000, 0.843, 0.000);
        }
        else {
            glColor3f(0.980, 0.922, 0.843);
        }

        if (i == 1) {
            renderBitmapString(-(13.0 / 8.0) * 6, y - 9, (void *)font_menu, "Slot 1");
        }
        else if (i == 2) {
            renderBitmapString(-(13.0 / 8.0) * 6, y - 9, (void *)font_menu, "Slot 2");
        }
        else if (i == 3) {
            renderBitmapString(-(13.0 / 8.0) * 6, y - 9, (void *)font_menu, "Slot 3");
        }
        else if (i == 4) {
            renderBitmapString(-(13.0 / 8.0) * 6, y - 9, (void *)font_menu, "Slot 4");
        }

        if (load_highlight == i) {
            glColor3f(1.000, 0.843, 0.000);
        }
        else {
            glColor3f(0.980, 0.922, 0.843);
        }

        glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x, y - option_height);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(x, y - option_height);
            glVertex2f(x + option_width, y - option_height);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(x + option_width, y - option_height);
            glVertex2f(x + option_width, y);
        glEnd();
         glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x + option_width, y);
        glEnd();

        y -= option_height + 2;

        i++;
        if (i > total_options) break;
    }
}

void loadGame(int slot)
{
    char s[] = "SaveFile_ .txt";
    s[9] = '0' + slot;
    ifstream fin;
    fin.open(s);
    if (!fin.fail()) {
        fin >> dx_ball_x >> dx_ball_y;
        fin >> reflector_x >> reflector_y;
        fin >> direction;
        fin >> angle;
        fin >> new_angle;
        fin >> ball_is_moving;
        fin >> score;
        fin >> lives_left;
        fin >> current_level;
        bricks.clear();
        int sz;
        fin >> sz;
        while (sz--) {
            vector<double> curr;
            int z = 5;
            while (z--) {
                double x;
                fin >> x;
                curr.push_back(x);
            }
            bricks.push_back(curr);
        }
        done.clear();
        fin >> sz;
        while (sz--) {
            int x;
            fin >> x;
            done.push_back(x);
        }
        Q = queue<int>();
        fin >> sz;
        while (sz--) {
            int x;
            fin >> x;
            Q.push(x);
        }
    }
    fin.close();
}



int main()
{
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("DX Ball 2D - By Takik Hasan");
	init();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(spe_key);
	glutMainLoop();
	return 0;
}



/// FUNCTION DEFINITIONS    ***     ***     ***

void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
}

void circle(double radius_x, double radius_y)
{
	int i=0;
	double angle = 0.0;
	glBegin(GL_POLYGON);
		for(i = 0; i < 200; i++) {
			angle = 2 * PI * i / 200;
			glVertex2f (cos(angle) * radius_x + dx_ball_x, sin(angle) * radius_y + dx_ball_y);
		}
	glEnd();
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

void reshape(int width, int height)
{
    glutReshapeWindow(600, 600);
}

void renderBitmapString(double x, double y, void *font, string s)
{
    glRasterPos2f(x, y);
    for (char c : s) {
        glutBitmapCharacter(font, c);
    }
}

void refresh()
{
    double curr_fps;
    if (fps == 60) curr_fps = 60;
    else if (fps == 110) curr_fps = 120;
    else if (fps == 200) curr_fps = 240;
    else if (fps == 330) curr_fps = 480;
    double msec = (1.0 / curr_fps) * 1000;
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

vector<int> queue_bricks()
{
    vector<int> ret;
    for (int i = 0; i < (int)bricks.size(); i++) {
        if (!done[i]) {
            double x1 = bricks[i][0];
            double x2 = x1 + brick_width;
            double y2 = bricks[i][1];
            double y1 = y2 - brick_height;
            for (int j = 0; j < 4; j++) {   /** This logic can be improved, not necessary */
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

void loadBricks(int level)
{
    bricks.clear();
    for (auto ss : grid[level]) {
        cout << ss << endl;
    }
    double p = 0.5;
    double top_left_x = -(brick_width * grid[level][0].size()) / 2.0;
    double top_left_y = 50;
    for (int i = 0; i < (int)grid[level].size(); i++) {
        for (int j = 0; j < (int)grid[level][0].size(); j++) {
            double curr_y = top_left_y - i * brick_height;
            double curr_x = top_left_x + j * brick_width;
            /** top left point, bottom right point, type */
            if (grid[level][i][j] == '#') {
                bricks.push_back({curr_x + p, curr_y - p, curr_x + brick_width - p, curr_y - brick_height + p, 1});
            }
            else if (grid[level][i][j] == '*') {
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

void keyboard(unsigned char key, int x, int y)
{
    if (load_screen == true) {
        switch (key) {
            case 13:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (load_highlight == 1) loadGame(1);
                else if (load_highlight == 2) loadGame(2);
                else if (load_highlight == 3) loadGame(3);
                else if (load_highlight == 4) loadGame(4);
                menu_screen = true;
                load_screen = false;
                glutPostRedisplay();
                break;
            case 27:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                menu_screen = true;
                load_screen = false;
                glutPostRedisplay();
                break;
            default:
                break;
        }
    }
    else if (save_screen == true) {
        switch (key) {
            case 13:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (save_highlight == 1) saveGame(1);
                else if (save_highlight == 2) saveGame(2);
                else if (save_highlight == 3) saveGame(3);
                else if (save_highlight == 4) saveGame(4);
                menu_screen = true;
                save_screen = false;
                glutPostRedisplay();
                break;
            case 27:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                menu_screen = true;
                save_screen = false;
                glutPostRedisplay();
                break;
            default:
                break;
        }
    }
    else if (FPS_screen == true) {
        switch (key) {
            case 13:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (FPS_highlight == 1) fps = 60;
                else if (FPS_highlight == 2) fps = 110;
                else if (FPS_highlight == 3) fps = 200;
                else if (FPS_highlight == 4) fps = 330;
                menu_screen = true;
                FPS_screen = false;
                glutPostRedisplay();
                break;
            case 27:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                menu_screen = true;
                FPS_screen = false;
                glutPostRedisplay();
                break;
            default:
                break;
        }
    }
    else if (game_finished_screen == true || game_over_screen == true) {
        switch (key) {
            case 13:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                game_finished_screen = false;
                game_over_screen = false;
                menu_screen = true;
                game_running = false;
                menu_highlight = 1;
                glutPostRedisplay();
                break;
            default:
                break;
        }
    }
    else if (!menu_screen) {
        switch (key) {
            case 'a':
                moveLeft();
                break;
            case 'd':
                moveRight();
                break;
            case 32:
                if (!ball_is_moving) {
                    if (sound_on == true) {
                        PlaySound("smb_fireball.wav", NULL, SND_FILENAME | SND_ASYNC);
                    }
                    ball_is_moving = 1;
                    glutPostRedisplay();
                }
                break;
            case 27:
                if (sound_on) {
                    PlaySound("smb_pause.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                menu_screen = true;
                glutPostRedisplay();
                break;
            default:
                break;
        }
    }
    else {
        switch (key) {
            case 13:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (menu_highlight == 1) {
                    if (sound_on == true) {
                        PlaySound("smb_stage_clear.wav", NULL, SND_FILENAME | SND_ASYNC);
                    }
                    newGame();
                }
                else if (menu_highlight == 7) resumePlaying();
                else if (menu_highlight == 6) quit();
                else if (menu_highlight == 4) changeFPS();
                else if (menu_highlight == 2) save();
                else if (menu_highlight == 3) load();
                else if (menu_highlight == 5) {
                    sound_on = !sound_on;
                    glutPostRedisplay();
                }
                break;
            default:
                break;
        }
    }
}

void spe_key(int key, int x, int y)
{
    if (load_screen == true) {
        switch (key) {
            case GLUT_KEY_UP:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                load_highlight--;
                if (load_highlight == 0)
                    load_highlight = total_options;
                glutPostRedisplay();
                break;
            case GLUT_KEY_DOWN:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                load_highlight++;
                if (load_highlight > total_options)
                    load_highlight = 1;
                glutPostRedisplay();
                break;
          default:
                break;
        }
    }
    else if (save_screen == true) {
        switch (key) {
            case GLUT_KEY_UP:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                save_highlight--;
                if (save_highlight == 0)
                    save_highlight = total_options;
                glutPostRedisplay();
                break;
            case GLUT_KEY_DOWN:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                save_highlight++;
                if (save_highlight > total_options)
                    save_highlight = 1;
                glutPostRedisplay();
                break;
          default:
                break;
        }
    }
    else if (FPS_screen == true) {
        switch (key) {
            case GLUT_KEY_UP:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                FPS_highlight--;
                if (FPS_highlight == 0)
                    FPS_highlight = total_options;
                glutPostRedisplay();
                break;
            case GLUT_KEY_DOWN:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                FPS_highlight++;
                if (FPS_highlight > total_options)
                    FPS_highlight = 1;
                glutPostRedisplay();
                break;
          default:
                break;
        }
    }
    else if (game_finished_screen == true || game_over_screen == true) {
        /// DO NOTHING
    }
    else if (menu_screen) {
        switch (key) {
            case GLUT_KEY_UP:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                menu_highlight--;
                if (menu_highlight == 0)
                    menu_highlight = total_options;
                glutPostRedisplay();
                break;
            case GLUT_KEY_DOWN:
//                PlaySound("menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                menu_highlight++;
                if (menu_highlight > total_options)
                    menu_highlight = 1;
                glutPostRedisplay();
                break;
          default:
                break;
        }
    }
    else {
        switch (key) {
            case GLUT_KEY_LEFT:
                moveLeft();
                break;
            case GLUT_KEY_RIGHT:
                moveRight();
                break;
          default:
                break;
        }
    }
}

void displayGame()
{
    printLives();

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

    glColor3f(0.118, 0.565, 1.000);

    string ss;
    int temp = score;
    while (temp) {
        ss += (temp % 10) + '0';
        temp /= 10;
    }
    reverse(ss.begin(), ss.end());
    if (score < 10) ss = "Current score: 0" + ss;
    else ss = "Current score: " + ss;
    glPushMatrix();
        renderBitmapString(100 - 39, 100 - 6, (void *)font, ss);
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
        renderBitmapString(100 - 39, 100 - 12, (void *)font, ss);
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
        renderBitmapString(100 - 39, 100 - 18, (void *)font, ss);
    glPopMatrix();
}

void destroy_bricks()
{
    bool just_destroyed = 0;
    while (!Q.empty()) {
        int top = Q.front();
        Q.pop();
        if (!done[top]) {
            if (sound_on == true) {
                if (bricks[top][4] == 1)
                    PlaySound("Hard Brick (Brick).wav", NULL, SND_FILENAME | SND_ASYNC);
                else if (bricks[top][4] == 2)
                    PlaySound("smb_coin.wav", NULL, SND_FILENAME | SND_ASYNC);
            }
            if (bricks[top][4] == 1) score += 6;
            else score += 24;
            done[top] = 1;
            just_destroyed = 1;
        }
    }
    if (just_destroyed) {
        bool level_finished = true;
        for (bool check : done) {
            if (check == false) {
                level_finished = false;
                break;
            }
        }
        if (level_finished) {
            if (current_level == (int)grid.size() - 1) {
                /// SUCCESS!! YOU HAVE FINISHED THE GAME! STOP WASTING SO MUCH TIME :) !!
                if (sound_on == true) {
                    PlaySound("smb_world_clear.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                game_finished_screen = true;
                glutPostRedisplay();
            }
            else {
                if (sound_on == true) {
                    PlaySound("smb_stage_clear.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                loadBricks(++current_level);
                done = vector<bool> (bricks.size(), 0);
                respawn();
            }
        }
    }
}

void takeOneStep()
{
    double rad = RAD(angle);

    double perfect_speed_fps = 200;
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

    vector<int> broken = queue_bricks();

    if (broken.size()) {
        for (int in : broken) Q.push(in);
        direction++;
        if (direction == 5) direction = 1;
    }
    else if (outOfBounds()) {
        if (dx_ball_y - dx_ball_r <= -100) {
            lives_left--;
            if (sound_on == true) {
                PlaySound("smb_mariodie.wav", NULL, SND_FILENAME | SND_ASYNC);
            }
            if (lives_left == 0) {
                if (sound_on == true) {
                    PlaySound("smb_gameover.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                game_over_screen = true;
                glutPostRedisplay();
            }
            else {
                respawn();
            }
        }
        else {
//            if (sound_on == true) {
//                PlaySound("smb_bump.wav", NULL, SND_FILENAME | SND_ASYNC);
//            }
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
                swap(angle, new_angle);
                if (sound_on == true) {
                    PlaySound("smb_jump-super.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                direction++;
                if (direction == 5) direction = 1;
            }
            else {
                angle = new_angle;
                destroy_bricks();
            }
        }
        else {
            angle = new_angle;
            destroy_bricks();
        }
    }
}

void displayMenu()
{
    total_options = 6;  /** New Game, Save, Load, Change FPS, Toggle Sound, Quit */
    if (game_running == true) total_options++;  /** Resume Playing */
    double option_width = 80;
    double option_height = 15;
    double x = -option_width / 2;
    double y = (option_height * total_options) / 2;
    int i = 1;
    if (total_options == 7) i = 7;
    while (1) {
        if (menu_highlight == i) {
            glColor3f(1.000, 0.843, 0.000);
        }
        else {
            glColor3f(0.980, 0.922, 0.843);
        }

        if (i == 1) {
            renderBitmapString(-13, y - 9, (void *)font_menu, "New Game");
        }
        else if (i == 2) {
            renderBitmapString(-(13.0 / 8.0) * 4, y - 9, (void *)font_menu, "Save");
        }
        else if (i == 3) {
            renderBitmapString(-(13.0 / 8.0) * 4, y - 9, (void *)font_menu, "Load");
        }
        else if (i == 4) {
            renderBitmapString(-(13.0 / 8.0) * 10, y - 9, (void *)font_menu, "Change FPS");
        }
        else if (i == 5) {
            string ss = sound_on ? "Turn Sound OFF" : "Turn Sound ON";
            renderBitmapString(-(13.0 / 8.0) * (int)ss.size(), y - 9, (void *)font_menu, ss);
        }
        else if (i == 6) {
            renderBitmapString(-(13.0 / 8.0) * 4, y - 9, (void *)font_menu, "Quit");
        }
        else {
            renderBitmapString(-(13.0 / 8.0) * 14, y - 9, (void *)font_menu, "Resume Playing");
        }

        if (menu_highlight == i) {
            glColor3f(1.000, 0.843, 0.000);
        }
        else {
            glColor3f(0.980, 0.922, 0.843);
        }

        glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x, y - option_height);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(x, y - option_height);
            glVertex2f(x + option_width, y - option_height);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(x + option_width, y - option_height);
            glVertex2f(x + option_width, y);
        glEnd();
         glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x + option_width, y);
        glEnd();

        y -= option_height + 2;

        if (total_options != 7) {
            i++;
            if (i > total_options) break;
        }
        else {
            if (i == 7) i = 1;
            else {
                if (i == 6) break;
                else i++;
            }
        }
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (load_screen == true) {
        displayLoadSlots();
        glFlush();
    }
    else if (save_screen == true) {
        displaySaveSlots();
        glFlush();
    }
    else if (FPS_screen == true) {
        displayFPSOptions();
        glFlush();
    }
    else if (game_over_screen == true) {
        displayFailureMessage();
        glFlush();
    }
    else if (game_finished_screen == true) {
        displaySuccessMessage();
        glFlush();
    }
    else if (menu_screen == true) {
        displayMenu();
        glFlush();
    }
    else {
        displayGame();
        glFlush();
        if (ball_is_moving == false) {
            return;
        }
        else {
            takeOneStep();
        }

        refresh();
    }
}
