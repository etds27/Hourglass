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
uint32_t *buffer = new uint32_t[16]{};

unsigned long lastMemoryUpdate = millis();

unsigned long lastUpdate = millis();
unsigned long startTime = millis();

void mouseEventHandler(int button, int state, int x, int y)
{
  float relX = (float)x / WINDOW_WIDTH;
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    // std::cout << "Mouse clicked at (" << x << ", " << y << ")" << std::endl;
  }
  else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
  {
    // std::cout << "Mouse released at (" << x << ", " << y << ")" << std::endl;
  }
}

void displayCallback()
{

  glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
  glLoadIdentity();           // Reset the model-view matrix
  uint8_t step = (millis() / 2000) % 3;
  uint8_t rotate = (millis() % 2000) / 2000.0 * 16;
  std::cout << "Rotate: " << rotate << std::endl;
  uint32_t color = 0x000000;

    if (step == 0)
    {
      color = 0xFF0000;
      logger.info("Red");
    }
    if (step == 1)
    {
      color = 0x00FF00;
      logger.info("Green");
    }
    if (step == 2)
    {
      color = 0x0000FF;
      logger.info("Blue");
    }

    for (int i = 0; i < 16; i++)
    {
      if (i <= 6) {
        buffer[i] = m_ring->dimColor(color, (6 - i) / 6.0f * 255);
      } else {
        buffer[i] = 0x000000;
      }
    }
    uint32_t *fullBuffer = new uint32_t[16]{};
    m_ring->offsetBuffer(buffer, rotate);
    m_ring->extendBuffer(buffer, fullBuffer, 8);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    struct GameDebugData data{
        .buffer = fullBuffer
    };
    m_ring->updateGameDebugData(data);

  // Draw the ring
  m_ring->update();
  // Draw the button
  glTranslatef(BUTTON_CENTER_X, BUTTON_CENTER_Y, 0.0f); // Translate left and up
  gl_tools::drawCircle(0.0, 0.0, 0.1, 0xAAAAAA);

  glutSwapBuffers();
  delete[] fullBuffer;

}

void timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(SIMULATOR_REFRESH_DELAY, timer, 0);
}

int main(int argc, char **argv)
{
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
  struct GameDebugData data{
      .buffer = buffer};
  m_ring->updateGameDebugData(data);
  m_ring->setDisplayMode(DeviceState::State::Debug);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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