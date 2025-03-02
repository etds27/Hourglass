#pragma once

#ifdef SIMULATOR

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdint>
namespace gl_tools {
    /// @brief Draw a filled ellipse on the GLU display
    /// @param x X position of the center of the ellipse
    /// @param y Y position of the center of the ellipse
    /// @param radiusX X radius of the ellipse
    /// @param radiusY Y radius of the ellipse
    /// @param color Color to fill the ellipse
    /// @param vertices Number of vertices to draw the ellipse with
    void drawEllipse(GLfloat x, GLfloat y, GLfloat radiusX, GLfloat radiusY, uint32_t color, GLint vertices = 30);
    
    /// @brief Draw a filled circle on the GLU display
    /// @param x X position of the center of the circle
    /// @param y Y position of the center of the circle
    /// @param radius Radius of the circle
    /// @param color Color to fill the circle
    /// @param vertices Number of vertices to draw the ellipse circle
    void drawCircle(GLfloat x, GLfloat y, GLfloat radius, uint32_t color, GLint vertices = 30);
}

#endif