// Author: João Alexandre Aguiar Amaral Santos
//Git: github/orangelash/pongclock

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "irrKlang.h"

#include <time.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <string.h>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

ISoundEngine *SoundEngine = createIrrKlangDevice();
time_t now = time(0);
time_t hour = time(0);
time_t time1;
tm *ltm = localtime(&now);

class game
{
  public:
    float score1, score2, score3,
        ref1, ref2, ref3,
        field1, field2, field3,
        ball1, ball2, ball3,
        back1, back2, back3, back4;

    int OrthoWid,
        OrthoHei;

    int WinWid,
        WinHei;

    int winXPos,
        winYPos;

    float FieldSizeX,
        FieldSizeY;

    float PThickness;
    float BallSize;
    float BorderT;
    float MLineT;

    int ScoreL,
        ScoreR;

    float TextPosX,
        TextPosY;
    void changeColor(int i);
    game()
    {
        score1 = 1, score2 = 1, score3 = 1;
        ref1 = 1, ref2 = 1, ref3 = 1;
        field1 = 1, field2 = 1, field3 = 1;
        ball1 = 1, ball2 = 1, ball3 = 1;
        back1 = 0, back2 = 0, back3 = 0, back4 = 0;
        WinWid = 300;
        WinHei = 300;
        OrthoWid = 300;
        OrthoHei = 300;
        winXPos = 100;
        winYPos = 100;
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
    reflector()
    {
        vy = 0;
        y = 0;
    }
    void draw();
    void compensate();
} left, right, reflector;

<<<<<<< HEAD

=======
void game::changeColor(int i)
{
    if (i == 1)
    {
        score1 = 1, score2 = 1, score3 = 1;
        ref1 = 1, ref2 = 1, ref3 = 1;
        field1 = 1, field2 = 1, field3 = 1;
        ball1 = 1, ball2 = 1, ball3 = 1;
        glClearColor(0, 0, 0, 1);
    }
    if (i == 2)
    {
        score1 = 1, score2 = 1, score3 = 0.6;
        ref1 = 0.5, ref2 = 0.5, ref3 = 1;
        field1 = 0, field2 = 1, field3 = 1;
        ball1 = 0.5, ball2 = 0.5, ball3 = 1;
        glClearColor(0.1, 0.1, 0.3, 1);
    }
    if (i == 3)
    {
        score1 = 0.3, score2 = 0.6, score3 = 0.6;
        ref1 = 0.2, ref2 = 0.5, ref3 = 0.8;
        field1 = 0.5, field2 = 1, field3 = 0.3;
        ball1 = 0.2, ball2 = 0.5, ball3 = 0.8;
        glClearColor(0.2, 0.3, 0.3, 1);
    }
}
>>>>>>> marco

void game::start_settings()
{
    now = time(0);
    left.size = 60;
    right.size = 60;
    left.x = -185;
    right.x = 185;
    ball.vx =5;
    ball.vy = 5;
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
    glColor3f(field1, field2, field3);
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

    glColor3f(score1, score2, score3);
    glRasterPos2f(TextPosX - 60, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + ScoreL / 10);
    glRasterPos2f(TextPosX - 40, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + ScoreL % 10);

    glRasterPos2f(TextPosX, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');

    glRasterPos2f(TextPosX + 30, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + ScoreR / 10);
    glRasterPos2f(TextPosX + 50, TextPosY + 20);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + ScoreR % 10);
}

void reflector::draw()
{
    glColor3f(settings.ref1, settings.ref2, settings.ref3);
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
    {
        vy = -vy;
        SoundEngine->play2D("./audio/bleep.wav", GL_FALSE);
    }

    if ((x <= left.x + settings.PThickness) && (fabs(double(y - left.y)) <= left.size / 2 + fabs(vy)))
    {
        vx = -vx;
        vy += left.vy;
        SoundEngine->play2D("./audio/pipe.wav", GL_FALSE);
    }
    if ((x >= right.x - settings.PThickness) && (fabs(double(y - right.y)) <= right.size / 2 + fabs(vy)))
    {
        vx = -vx;
        vy += right.vy;
        SoundEngine->play2D("./audio/pipe.wav", GL_FALSE);
    }
}

void ball::ai()
{
    if (x < 0)
    {
        if (vx < 0)
        {
            if (y > left.y)
                left.y = left.y + 6;

            if (y < left.y)
                left.y = left.y - 6;
        }
    }

    if (x > 0)
    {
        if (vx >= 0)
        {
            if (y > right.y)
                right.y = right.y + 6;

            if (y < right.y)
                right.y = right.y - 6;
        }
    }
}

void ball::adjust(int flag)
{

    if (flag == 1)
    {
        if (vx < 0 && x < -50)
        {
            if (y < 0)
                left.y = left.y + 3;
            if (y >= 0)
                left.y = left.y - 3;
        }
    }
    else
    {
        if (vx > 0 && x > 50)
        {
            if (y < 0)
                right.y = right.y + 3;
            if (y >= 0)
                right.y = right.y - 3;
        }
    }
}

void ball::draw()
{
    glColor3f(settings.ball1, settings.ball2, settings.ball3);
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

<<<<<<< HEAD
=======
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {

    case 'f':
        glutFullScreenToggle();
        break;
    case 27:
        exit(0);
        break;

    case '1':
        SoundEngine->stopAllSounds();
        SoundEngine->play2D("./audio/breakout.mp3", GL_TRUE);
        settings.changeColor(1);
        break;
    case '2':
        SoundEngine->stopAllSounds();
        SoundEngine->play2D("./audio/MF-W-90.XM", GL_TRUE);
        settings.changeColor(2);
        break;
    case '3':
        SoundEngine->stopAllSounds();
        SoundEngine->play2D("./audio/MF-3DAYS.S3M", GL_TRUE);
        settings.changeColor(3);
        break;
    }
}
>>>>>>> marco
void Timer(int value)
{
    if (value == 1)
    {
<<<<<<< HEAD
        if (time1 - hour >= 3597)
=======
        if (time1 - hour >=20)
>>>>>>> marco
        {
            if (ball.x < right.x)
            {
                ball.ai();
                ball.adjust(2);
                reflector.compensate();
            }
            else
            {
                SoundEngine->play2D("./audio/beep-10.wav", GL_FALSE);
                settings.increase_score(2);
                settings.start_settings();
                hour = time(0);
                value = 0;
            }
        }
        else if (ball.x > left.x)
        {
<<<<<<< HEAD
           
            left.y = -ball.y;
            right.y = ball.y;
=======
>>>>>>> marco

            ball.ai();
            ball.adjust(1);
            reflector.compensate();
        }
        else
        {
            SoundEngine->play2D("./audio/bleep.mp3", GL_FALSE);
            settings.increase_score(1);
            settings.start_settings();
            value = 0;
        }
    }

    if (value == 0)
    {
        time1 = time(0);
        if (time1 - (now) >= 57)
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
    puts("\t\nPongclock\n\nAuthor: João Alexandre Aguiar Amaral Santos\nGithub: www.github.com/orangelash/\nEmail: joaoaaasantos [at] gmail.com");
    SoundEngine->play2D("./audio/breakout.mp3", GL_TRUE);
    srand(time(NULL));
    puts("Configuring stats...");
    settings.start_settings();
    puts("Initializing glut...");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    puts("Creating window...");
    glutInitWindowSize(settings.WinWid, settings.WinHei);
    glutInitWindowPosition(settings.winXPos, settings.winYPos);
    glutCreateWindow("***PongClock***");
    glutDisplayFunc(draw);
    glutTimerFunc(1, Timer, 0);
    glutKeyboardFunc(keyboard);
    glClearColor(settings.back1, settings.back2, settings.back3, settings.back4);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-settings.OrthoWid, settings.OrthoWid, -settings.OrthoHei, settings.OrthoHei);
    puts("\n  > Setup Complete <");
    puts("\nInstructions:\n1- Classic Theme\n2- Midnight Theme\n3- Jungle Theme\nf- Toggle Fullscreen\nESC- Exit\n");
    glutMainLoop();
    return (0);
}
