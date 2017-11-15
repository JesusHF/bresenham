#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

/* Function that returns -1,0,1 depending on whether x */
/* is <0, =0, >0 respectively */
#define sign(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))
#define x_off x - cx
#define y_off y - cy

/* Function to plot a point */
void setPixel(GLint x, GLint y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

/* Function to plot 4 points */
void set4Pixel(GLint x, GLint y)
{
    glBegin(GL_POINTS);

    //draw vertex
    glVertex2i(x, y);  //  0, R
    glVertex2i(x, -y); //  0,-R
    glVertex2i(y, x);  //  R, 0
    glVertex2i(-y, x); // -R, 0
    glEnd();
}

void set4Pixel(GLint x, GLint y, GLint cx, GLint cy)
{
    glBegin(GL_POINTS);

    //draw vertex
    glVertex2i(x + cx, y + cy);  //  0, R
    glVertex2i(y + cx, x + cy);  //  R, 0
    glVertex2i(x + cx, -y + cy); //  0, -R
    glVertex2i(-x + cx, y + cy); // -R,  0
    glEnd();
}

//draws 8 pixels for circle function
void set8Pixel(GLint x, GLint y)
{
    glBegin(GL_POINTS);

    //draw vertex
    glVertex2i(x, y);   //  1,  2
    glVertex2i(-x, y);  // -1,  2
    glVertex2i(x, -y);  //  1, -2
    glVertex2i(-x, -y); // -1, -2
    glVertex2i(y, x);   //  2,  1
    glVertex2i(-y, x);  // -2,  1
    glVertex2i(y, -x);  //  2, -1
    glVertex2i(-y, -x); // -2, -1
    glEnd();
}

void set8Pixel(GLint x, GLint y, GLint cx, GLint cy)
{
    glBegin(GL_POINTS);

    glVertex2i(x + cx, y + cy);   //  1,  2
    glVertex2i(y + cx, x + cy);   //  2,  1
    glVertex2i(-x + cx, y + cy);  // -1,  2
    glVertex2i(-y + cx, x + cy);  // -2,  1
    glVertex2i(x + cx, -y + cy);  //  1, -2
    glVertex2i(y + cx, -x + cy);  //  2, -1
    glVertex2i(-x + cx, -y + cy); // -1, -2
    glVertex2i(-y + cx, -x + cy); // -2, -1

    glEnd();
}

void set8Pixel(GLint x, GLint y, GLint cx, GLint cy, GLint type)
{
    switch (type)
    {
        case 0:
        //type 0
        glBegin(GL_POINTS);
        glVertex2i(-x + cx, -y + cy); // -1, -2
        glVertex2i(-y + cx, -x + cy); // -2, -1
        glEnd();
        break;

        case 1:
        //type 1
        glBegin(GL_POINTS);
        glVertex2i(x + cx, -y + cy); //  1, -2
        glVertex2i(y + cx, -x + cy); //  2, -1
        glEnd();
        break;

        case 2:
        //type 2
        glBegin(GL_POINTS);
        glVertex2i(x + cx, y + cy); //  1,  2
        glVertex2i(y + cx, x + cy); //  2,  1
        glEnd();
        break;

        case 3:
        //type 3
        glBegin(GL_POINTS);
        glVertex2i(-x + cx, y + cy); // -1,  2
        glVertex2i(-y + cx, x + cy); // -2,  1
        glEnd();
        break;
    }
}

void setRefillLine(GLint x0, GLint y0)
{
    GLint x = x0 * -1;

    while (x < x0)
    {
        setPixel(x, y0);
        x++;
    }
}

void set4RefillLine(GLint x0, GLint y0)
{
    setRefillLine(x0, y0);
    setRefillLine(y0, x0);
    setRefillLine(x0, -y0);
    setRefillLine(y0, -x0);
}

void setHalfRefillLine(GLint x0, GLint y0)
{
    GLint x = 0;
    if (x0 < 0)
    {
        x = x0;
        x0 = 0;
    }

    while (x < x0)
    {
        setPixel(x, y0);
        x++;
    }
}

void set4HalfRefillLine(GLint x0, GLint y0, GLint type)
{
    if (type == 0)
    {
        setHalfRefillLine(x0, y0);
        setHalfRefillLine(y0, x0);
        setHalfRefillLine(-x0, -y0);
        setHalfRefillLine(-y0, -x0);
    }
    else if(type == 1)
    {
        setHalfRefillLine(-x0, y0);
        setHalfRefillLine(-y0, x0);
        setHalfRefillLine(x0, -y0);
        setHalfRefillLine(y0, -x0);
    }
}

/* Generalized Bresenham's Algorithm */
void bres_general(int x1, int y1, int x2, int y2)
{
    int dx, dy, x, y, d, s1, s2, swap = 0, temp;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    s1 = sign(x2 - x1);
    s2 = sign(y2 - y1);

    /* Check if dx or dy has a greater range */
    /* if dy has a greater range than dx swap dx and dy */
    if (dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        swap = 1;
    }

    /* Set the initial decision parameter and the initial point */
    d = 2 * dy - dx;
    x = x1;
    y = y1;

    int i;
    for (i = 1; i <= dx; i++)
    {
        setPixel(x, y);

        while (d >= 0)
        {
            if (swap)
                x = x + s1;
            else
            {
                y = y + s2;
                d = d - 2 * dx;
            }
        }
        if (swap)
            y = y + s2;
        else
            x = x + s1;
        d = d + 2 * dy;
    }
    glFlush();
}

/**
 * Function to draw a circle with center is in 0,0
*/
void draw_circle(GLint radius)
{
    GLint x, y, d, dE, dSE;

    x = 0;
    y = radius;
    d = 1 - radius;
    dE = 3;
    dSE = 5 - 2 * radius;
    set4Pixel(x, y);

    //while painting painting top half of circle
    while (y > x)
    {

        if (d < 0)
        {
            x++;
            dE += 2;
            dSE += 2;
            d += dE;
        }
        else
        {
            x++;
            y--;
            dE += 2;
            dSE += 4;
            d += dSE;
        }
        set8Pixel(x, y);
    }
}

/**
 * Function to draw a circle with center is in given position
*/
void draw_circle(GLint radius, GLint cx, GLint cy)
{
    GLint x, y, d, dE, dSE;

    x = 0;
    y = radius;
    d = 1 - radius;
    dE = 3;
    dSE = 5 - 2 * radius;
    set4Pixel(x, y, cx, cy);

    //while painting top eighth part of circle
    while (y > x)
    {

        if (d < 0)
        {
            x++;
            dE += 2;
            dSE += 2;
            d += dE;
        }
        else
        {
            x++;
            y--;
            dE += 2;
            dSE += 4;
            d += dSE;
        }
        set8Pixel(x, y, cx, cy);
    }
}

/**
 * Draws a circle in the giben center and sector
 * radius: radius of the circle
 * cx: coordinate x of the center
 * cy: coordinate y of the center
 * part: quarter of the circle to draw (0 bot-left, 1 bot-right, 2 top-right, 3 top-left)
*/
void draw_circle(GLint radius, GLint cx, GLint cy, GLint part)
{
    GLint x, y, d, dE, dSE;

    x = 0;
    y = radius;
    d = 1 - radius;
    dE = 3;
    dSE = 5 - 2 * radius;
    set8Pixel(x, y, cx, cy, part);

    //while painting top eighth part of circle
    while (y > x)
    {

        if (d < 0)
        {
            x++;
            dE += 2;
            dSE += 2;
            d += dE;
        }
        else
        {
            x++;
            y--;
            dE += 2;
            dSE += 4;
            d += dSE;
        }
        set8Pixel(x, y, cx, cy, part);
    }
}

void draw_circle_refilled(GLint radius)
{
    GLint x, y, d, dE, dSE;

    x = 0;
    y = radius;
    d = 1 - radius;
    dE = 3;
    dSE = 5 - 2 * radius;
    set4Pixel(x, y);
    setRefillLine(radius, 0);
    //while painting top eighth part of circle
    while (y > x)
    {

        if (d < 0)
        {
            x++;
            dE += 2;
            dSE += 2;
            d += dE;
        }
        else
        {
            x++;
            y--;
            dE += 2;
            dSE += 4;
            d += dSE;
        }
        set4RefillLine(x, y);
    }
}

void draw_airscrew_refilled(GLint radius, GLint type)
{
    GLint x, y, d, dE, dSE;

    x = 0;
    y = radius;
    d = 1 - radius;
    dE = 3;
    dSE = 5 - 2 * radius;
    set4Pixel(x, y);
    setRefillLine(radius, 0);                   //draws initial line of circle
    //while painting top eighth part of circle
    while (y > x)
    {

        if (d < 0)
        {
            x++;
            dE += 2;
            dSE += 2;
            d += dE;
        }
        else
        {
            x++;
            y--;
            dE += 2;
            dSE += 4;
            d += dSE;
        }
        set8Pixel(x, y);
        set4HalfRefillLine(x, y, type);
    }
}

void draw_ellipse(GLint a, GLint b)
{
    GLint x_increment = a * a * 2, y_increment = b * b * 2;
    GLint comp1 = x_increment, comp2 = y_increment;

    GLint x = 0, y = b;
    while (comp1 < comp2)
    {
        //first region
        comp1 += x_increment;
        x++;
    }

    while (y > 0)
    {
        //second region
        comp2 += y_increment;

        y--;
    }
}

void draw_fibonacci(int n)
{
    GLint cx = 1, cy = 0;
    GLint lastradius = 0, radius = 1;
    GLint aux;

    int cuarter_type = 0;
    draw_circle(radius, cx, cy, 0);
    cuarter_type++;

    //while n-1 is different from 0, draw
    while (--n != 0)
    {
        aux = radius;
        radius += lastradius;
        lastradius = aux;
        draw_circle(radius, cx, cy, cuarter_type);

        switch (cuarter_type)
        {
        case 0:
            //draw first cuarter (bottom left)
            cy+=lastradius;
            cuarter_type++;
            break;

        case 1:
            //draw second cuarter (bottom right)
            cx -= lastradius;
            cuarter_type++;
            break;

        case 2:
            //draw third cuarter (top left)
            cy-=lastradius;
            cuarter_type++;
            break;

        case 3:
            //draw fourth cuarter (top right)
            cx+=lastradius;
            cuarter_type = 0;
            break;
        }
    }
}

void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    /**draws a circle of 50 pixels radius*/
    //draw_circle(50);

    /**draws a circle of 50 pixels radius*/
    //draw_circle(50, 50, 150);

    /**draws a airscrew of 2 types*/
    draw_airscrew_refilled(50, 0);

    /**draws a ellipse of 50 and 100 pixels axis*/
    //draw_ellipse(50, 100);

    /**draws fibonacci secuence*/
    //draw_fibonacci(15);

    glFlush();
}

void init()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // position of the window in the screen (0,0= top left corner)
    glutInitWindowPosition(350, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Bresenham test");
    glClearColor(0.0, 0.0, 0.0, 0);
    glColor3f(1.0, 1.0, 1.0);

    // set coordinates of screen 0,0 in the center of screen
    gluOrtho2D(-320, 320, -240, 240);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    init();
    glutDisplayFunc(draw);
    glutMainLoop();
    return 0;
}