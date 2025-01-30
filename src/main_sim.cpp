#ifdef SIMULATOR
#include "simulator_tools.h"
#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "gl_ring_interface.h"
#include "device_manager.h"
#include "device_state.h"
#include "color_converter.h"
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
  uint32_t color = 0x000000;
  uint32_t color2 = 0x000000;

  /*
  if (step == 0)
  {
    color = 0xFF0000;
    color2 = 0x00FF00;
    logger.info("Red");
  }
  if (step == 1)
  {
    color = 0x00FF00;
    color2 = 0x0000FF;
    logger.info("Green");
  }
  if (step == 2)
  {
    color = 0x0000FF;
    color2 = 0xFF0000;
    logger.info("Blue");
  }

  for (int i = 0; i < 16; i++)
  {
    if (i <= 6)
    {
      ColorTransform::DimColor *dimColor = new ColorTransform::DimColor((6 - i) / 6.0f * 255);
      buffer[i] = dimColor->applyTransform(color);
      delete dimColor;
    }
    else
    {
      buffer[i] = 0x000000;
    }
  }
  uint32_t *secondBuffer = new uint32_t[16]{};
  m_ring->copyBuffer(buffer, secondBuffer, 16);
  ColorTransform::SwapRedBlue *swapRedBlue = new ColorTransform::SwapRedBlue();
  m_ring->transformBufferColor(secondBuffer, 16, swapRedBlue);
  delete swapRedBlue;
  m_ring->offsetBuffer(secondBuffer, 8);
  m_ring->printBuffer(secondBuffer);
  m_ring->overlayBuffer(buffer, secondBuffer, 16);
  m_ring->printBuffer(buffer);

  m_ring->offsetBuffer(buffer, rotate);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  struct GameDebugData data{
      .buffer = buffer};
  m_ring->updateGameDebugData(data);
  */
  // Draw the ring
  m_ring->update();
  // Draw the button
  glTranslatef(BUTTON_CENTER_X, BUTTON_CENTER_Y, 0.0f); // Translate left and up
  gl_tools::drawCircle(0.0, 0.0, 0.1, 0xAAAAAA);

  glutSwapBuffers();
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
  struct TurnSequenceData data{
    .totalPlayers = 8,
    .myPlayerIndex = 2,
    .currentPlayerIndex = 4};

  m_ring->setAbsoluteOrientation(false);
  m_ring->updateTurnSequenceData(data);
  m_ring->setDisplayMode(DeviceState::State::AwaitingTurn);

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