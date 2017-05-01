#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <time.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdio.h>

int aux_counter;
int hour_counter = 0;

time_t now = time(0);
time_t hour = time(0);
time_t time1;
tm *ltm2;
tm *ltm = localtime(&now);

class game
{
  public:
    int OrthoWid;
    int OrthoHei;
    int WinWid;
    int WinHei;
    int winXPos;
    int winYPos;
    float FieldSizeX;
    float FieldSizeY;
    int delay;
    float PThickness;
    float BallSize;
    float BorderT;
    float MLineT;
    int ScoreL;
    int ScoreR;
    float TextPosX;
    float TextPosY;
    float BallSpeedX;
    float PSpeedY;
    game()
    {
        WinWid = 300;
        WinHei = 300;
        OrthoWid = 300;
        OrthoHei = 300;
        winXPos = 100;
        winYPos = 100;
        delay = 1;
        PThickness = 10;
        BallSize = 10;
        FieldSizeX = 200;
        FieldSizeY = 150;
        BorderT = 10;
        MLineT = 5;
        ScoreL = (ltm->tm_hour);
        ScoreR = (ltm->tm_min);
        TextPosX = 0;
        TextPosY = FieldSizeY + 10;
        BallSpeedX = 2;
        PSpeedY = 5;
    }
    void start_settings();
    void increase_score(int score);
    void DrawField();
    void DrawScore();
} settings;

class ball
{
  public:
    float x;
    float y;
    float vx;
    float vy;
    void move();
    void reflection();
    void draw();
    void ai();
    void adjust(int flag);
} ball;

class reflector
{
  public:
    float x, y;
    float vy;
    float size;
    bool Up, Down;
    reflector()
    {
        vy = 0;
        y = 0;
        Up = false;
        Down = false;
    }
    void draw();
    void compensate();
} left, right, reflector;

/*
 void game::KeyControl()
 {
 if ((left.Up) && (!left.Down))
 left.vy = PSpeedY;
 if ((!left.Up) && (left.Down))
 left.vy = -PSpeedY;
 if ((right.Up) && (!right.Down))
 right.vy = PSpeedY;
 if ((!right.Up) && (right.Down))
 right.vy = -PSpeedY;
 }
 */

void game::start_settings()
{
    aux_counter = 2;
    now = time(0);

    left.size = 60;
    right.size = 60;
    left.x = -185;
    right.x = 185;
    ball.vx = 2.3;
    ball.vy = 2.3;
    ball.x = 0;
    ball.y = 0;
}

void game::increase_score(int score)
{
    if (score == 1)
    {
        ScoreR++;
        if (ScoreR == 60)
            ScoreR = 0;
    }
    else
    {
        ScoreL++;
        if (ScoreL == 24)
            ScoreL = 0;
    }
}

void game::DrawField()
{
    glColor3f(1, 1, 0);
    glVertex2f(-FieldSizeX - BorderT, -FieldSizeY - BorderT);
    glVertex2f(FieldSizeX + BorderT, -FieldSizeY - BorderT);
    glVertex2f(FieldSizeX + BorderT, -FieldSizeY);
    glVertex2f(-FieldSizeX - BorderT, -FieldSizeY);

    glVertex2f(-FieldSizeX - BorderT, FieldSizeY + BorderT);
    glVertex2f(FieldSizeX + BorderT, FieldSizeY + BorderT);
    glVertex2f(FieldSizeX + BorderT, FieldSizeY);
    glVertex2f(-FieldSizeX - BorderT, FieldSizeY);

    glVertex2f(-FieldSizeX - BorderT, -FieldSizeY - BorderT);
    glVertex2f(-FieldSizeX, -FieldSizeY - BorderT);
    glVertex2f(-FieldSizeX, FieldSizeY + BorderT);
    glVertex2f(-FieldSizeX - BorderT, FieldSizeY + BorderT);

    glVertex2f(FieldSizeX, -FieldSizeY - BorderT);
    glVertex2f(FieldSizeX + BorderT, -FieldSizeY - BorderT);
    glVertex2f(FieldSizeX + BorderT, FieldSizeY + BorderT);
    glVertex2f(FieldSizeX, FieldSizeY + BorderT);

    for (float i = -FieldSizeY; i <= FieldSizeY; i += 4 * MLineT)
    {
        glVertex2f(-MLineT, i + MLineT);
        glVertex2f(MLineT, i + MLineT);
        glVertex2f(MLineT, i - MLineT);
        glVertex2f(-MLineT, i - MLineT);
    }
}

void game::DrawScore()
{
    time_t t = time(0);
    tm *ltm3 = localtime(&now);

    //garantia de que o relógio se mantém  sincronizado
    ScoreL = ltm3->tm_hour;
    ScoreR = ltm3->tm_min;

    glColor3f(1, 1, 1);
    glRasterPos2f(TextPosX - 50, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '0' + ScoreL / 10);
    glRasterPos2f(TextPosX - 40, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '0' + ScoreL % 10);

    glRasterPos2f(TextPosX, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ':');

    glRasterPos2f(TextPosX + 30, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '0' + ScoreR / 10);
    glRasterPos2f(TextPosX + 45, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '0' + ScoreR % 10);
}

void reflector::draw()
{
    glColor3f(1, 0, 1);
    glVertex2f(x + settings.PThickness, y + size / 2);
    glVertex2f(x + settings.PThickness, y - size / 2);
    glVertex2f(x - settings.PThickness, y - size / 2);
    glVertex2f(x - settings.PThickness, y + size / 2);
}

void reflector::compensate()
{

    if (left.y > 120)
        left.y = 120;
    if (left.y < -120)
        left.y = -120;
    if (right.y < -120)
        right.y = -120;
    if (right.y > 120)
        right.y = 120;
}

void ball::reflection()
{
    if ((y <= -settings.FieldSizeY + 5) || (y >= settings.FieldSizeY - 5))
        vy = -vy;
    if ((x <= left.x + settings.PThickness) && (fabs(double(y - left.y)) <= left.size / 2 + fabs(vy)))
    {
        vx = -vx;
        vy += left.vy;
        system("afplay pipe.wav");
    }
    if ((x >= right.x - settings.PThickness) && (fabs(double(y - right.y)) <= right.size / 2 + fabs(vy)))
    {
        vx = -vx;
        vy += right.vy;
        system("afplay pipe.wav");
    }
}

void ball::ai()
{
    if (x < 0)
    {
        if (vx < 0)
        {
            if (y > left.y)
                left.y = left.y + 3;

            if (y < left.y)
                left.y = left.y - 3;
        }
    }

    if (x > 0)
    {
        if (vx >= 0)
        {
            if (y > right.y)
                right.y = right.y + 3;

            if (y < right.y)
                right.y = right.y - 3;
        }
    }
}

void ball::adjust(int flag)
{

    if (flag == 1)
    {
        if (left.y - y < 1 || left.y + y > 1)
        {
            if (left.y < y)
                left.y = left.y - 3;
            if (left.y >= y)
                left.y = left.y + 3;
        }
    }
    else
    {
        if (right.y - y < 1 || right.y + y > 1)
        {
            if (right.y < y)
                right.y = right.y - 3;
            if (right.y >= y)
                right.y = right.y - 3;
        }
    }
}

void ball::draw()
{
    glVertex2f(x + settings.BallSize, y + settings.BallSize);
    glVertex2f(x + settings.BallSize, y - settings.BallSize);
    glVertex2f(x - settings.BallSize, y - settings.BallSize);
    glVertex2f(x - settings.BallSize, y + settings.BallSize);
}

void ball::move()
{
    x += vx;
    y += vy;
}

/*
 void keyboard(unsigned char key, int x, int y)
 {
 switch (key)
 {
 case 'q':
 left.Up = true;
break;
case 'a':
left.Down = true;
break;
case 'z':
if (left.hold)
{
    left.hold = false;
    ball.vx = settings.BallSpeedX;
}
break;
case 'p':
right.Up = true;
break;
case 'l':
right.Down = true;
break;
case 'm':
if (right.hold)
{
    right.hold = false;
    ball.vx = -settings.BallSpeedX;
    break;
}
}
}
void keyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'esc':
        return 0;
        break;
    case 'a':
        left.Down = false;
        break;
    case 'p':
        right.Up = false;
        break;
    case 'l':
        right.Down = false;
        break;
    }
}
*/
void Timer(int value)
{
    if (value == 1)
    {
        if (time1 - hour >= 20)
        {
            if (ball.x < right.x)
            {
                ball.ai();
                ball.adjust(2);
                reflector.compensate();
            }
            else
            {
                system("afplay beep-10.wav");
                settings.increase_score(2);
                settings.start_settings();
                hour = time(0);
                value = 0;
            }
        }
        else if (ball.x > left.x)
        {

            ball.ai();
            ball.adjust(1);
            reflector.compensate();
        }
        else
        {
            system("afplay beep-10.wav");
            settings.increase_score(1);
            settings.start_settings();
            value = 0;
        }
    }

    if (value == 0)
    {
        time1 = time(0);
        if (time1 - (now) >= 10)
            value = 1;

        else
        {
            ball.ai();
            reflector.compensate();
        }
    }

    ball.move();
    ball.reflection();
    glutPostRedisplay();
    glutTimerFunc(1, Timer, value);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    right.draw();
    left.draw();
    ball.draw();
    settings.DrawField();
    glEnd();
    settings.DrawScore();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    system("afplay timer.wav");
    srand(time(NULL));
    settings.start_settings();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(settings.WinWid, settings.WinHei);
    glutInitWindowPosition(settings.winXPos, settings.winYPos);
    glutCreateWindow("***PongClock***");
    glutDisplayFunc(draw);
    glutTimerFunc(settings.delay, Timer, 0);
    //glutKeyboardFunc(keyboard);
    //glutKeyboardUpFunc(keyboardUp);
    glClearColor(1, 0.75, 1, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-settings.OrthoWid, settings.OrthoWid, -settings.OrthoHei, settings.OrthoHei);
    glutMainLoop();
    return (0);
}
