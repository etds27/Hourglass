#ifdef SIMULATOR
#include "simulator_tools.h"
#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "gl_ring_interface.h"
#include "device_manager.h"
#include "device_state.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "gl_tools.h"
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int SIMULATOR_REFRESH_DELAY = 10;    

const float BUTTON_CENTER_X = 0.85f;
const float BUTTON_CENTER_Y = 0.0f;

DeviceManager *deviceManager;
GLRingInterface *m_ring;
unsigned long lastMemoryUpdate = millis();

void mouseEventHandler(int button, int state, int x, int y)
{
    float relX = (float)x / WINDOW_WIDTH;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
      // std::cout << "Mouse clicked at (" << x << ", " << y << ")" << std::endl;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
      // std::cout << "Mouse released at (" << x << ", " << y << ")" << std::endl;
    }
}

void displayCallback() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
    glLoadIdentity();                // Reset the model-view matrix

    // Draw the ring
    m_ring->update();
    // Draw the button
    glTranslatef(BUTTON_CENTER_X, BUTTON_CENTER_Y, 0.0f); // Translate left and up
    gl_tools::drawCircle(0.0, 0.0, 0.1, 0xAAAAAA);
    glFlush();
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(SIMULATOR_REFRESH_DELAY, timer, 0);
}

int main(int argc, char** argv) {
  switch (LOGGER_LEVEL)
  {
  case 0:
    loggerLevel = Logging::LoggerLevel::DEBUG;
    break;
  case 1:
    loggerLevel = Logging::LoggerLevel::INFO;
    break;
  case 2:
    loggerLevel = Logging::LoggerLevel::WARNING;
    break;
  case 4:
    loggerLevel = Logging::LoggerLevel::OFF;
    break;
  default:
    loggerLevel = Logging::LoggerLevel::OFF;
    break;
  }

    // deviceManager = new DeviceManager();
    /// deviceManager->start();
    m_ring = new GLRingInterface(RING_LED_COUNT);
    m_ring->setDisplayMode(DeviceState::AwaitingConnecion);
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); 
    glutInitWindowPosition(0, 0); 
    glutCreateWindow("Hourglass Simulator"); 
    glutTimerFunc(0, timer, 0);
    glutMouseFunc(mouseEventHandler);
    glutDisplayFunc(displayCallback);
    glutMainLoop();
    return EXIT_SUCCESS;
}

#endif