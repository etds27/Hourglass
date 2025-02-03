#ifdef SIMULATOR
#include "simulator_tools.h"
#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "gl_ring_interface.h"
#include "gl_input_interface.h"
#include "device_manager.h"
#include "hg_display_manager.h"
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
const uint32_t PRESSED_BUTTON_COLOR = 0x777777;
const uint32_t UNPRESSED_BUTTON_COLOR = 0xAAAAAA;

HourglassDisplayManager *displayManager;
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
    logger.debug("Pressing button");
    glPressValue = true;
  }
  else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
  {
    logger.debug("Releasing button");
    glPressValue = false;
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
  struct TimerData data{
      .totalTime = 20000,
      .elapsedTime = (int)(millis() % 20000),
      .isTurnTimeEnforced = true};
  displayManager->updateTimerData(data);
  // Draw the ring
  displayManager->update();
  // Draw the button
  glTranslatef(BUTTON_CENTER_X, BUTTON_CENTER_Y, 0.0f); // Translate left and up

  uint32_t buttonColor = glPressValue ? PRESSED_BUTTON_COLOR : UNPRESSED_BUTTON_COLOR;
  gl_tools::drawCircle(0.0, 0.0, 0.1, buttonColor);

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
  struct BuzzerResultsData data{
      .myPlayerIndex = 1,
      .winningPlayerIndex = 1};
  m_ring->updateBuzzerResultsData(data);

  struct TimerData timerData{
      .totalTime = 20000,
      .elapsedTime = 0,
      .isTurnTimeEnforced = true};

  /*
  m_ring->setAbsoluteOrientation(false);
  m_ring->updateTurnSequenceData(data);
  m_ring->setDisplayMode(DeviceState::State::AwaitingTurn);
  */
  displayManager = new HourglassDisplayManager();
  displayManager->addDisplayInterface(m_ring);
  displayManager->setDisplayMode(DeviceState::State::BuzzerWinnerPeriodTimed);

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