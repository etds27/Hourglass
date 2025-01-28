#ifdef SIMULATOR

#include "gl_tools.h"
#include <cmath>
#include <math.h>

float M_PI = 3.1415926f;

void gl_tools::drawEllipse(GLfloat x, GLfloat y, GLfloat radiusX, GLfloat radiusY, uint32_t color, GLint vertices)
{
    float r = ((color >> 16) & 0xFF) / 255.0f;
    float g = ((color >> 8) & 0xFF) / 255.0f;
    float b = ((color) & 0xFF) / 255.0f;
    /*
    std::cout << "Color: " << color << std::endl;
    std::cout << "r: " << r << std::endl;
    std::cout << "g: " << g << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "X: " << x << std::endl;
    std::cout << "Y: " << y << std::endl;
    */
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b);
    glVertex2f(x, y);
    for (int i = 0; i < vertices + 1; i++)
    {
        float angle = 2.0f * M_PI * float(i % vertices) / float(vertices);
        float dx = radiusX * cosf(angle);
        float dy = radiusY * sinf(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}


void gl_tools::drawCircle(GLfloat x, GLfloat y, GLfloat radius, uint32_t color, GLint vertices)
{
    drawEllipse(x, y, radius, radius, color, vertices);
}

#endif