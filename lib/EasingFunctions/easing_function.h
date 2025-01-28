#pragma once
#include <stdlib.h>
#include <cstdint>

enum class EasingMode
{
  EaseIn,
  EaseOut,
  EaseInAndOut,
  EaseOutAndIn,
};

namespace EasingFunction
{

  class EasingFunction
  {
  public:
    explicit EasingFunction(EasingMode mode);
    virtual ~EasingFunction() = default;

    double ease(double value, double start = 0, double end = 1);

  protected:
    virtual double calculatePointIn(double value);
    virtual double calculatePointOut(double value);

  private:
    EasingMode m_mode;
  };

  class Linear : public EasingFunction
  {
  public:
    Linear(EasingMode mode);

  protected:
    double calculatePointIn(double value) override;
    double calculatePointOut(double value) override;
  };

  class Circular : public EasingFunction
  {
  public:
    Circular(EasingMode mode);

  protected:
    double calculatePointIn(double value) override;
    double calculatePointOut(double value) override;
  };

  class Cubic : public EasingFunction
  {
  public:
    Cubic(EasingMode mode);

  protected:
    double calculatePointIn(double value) override;
    double calculatePointOut(double value) override;
  };

  class Poly : public EasingFunction
  {
  public:
    Poly(uint8_t power, EasingMode mode);

  protected:
    double calculatePointIn(double value) override;
    double calculatePointOut(double value) override;
    uint8_t m_power;
  };

  class Sine : public EasingFunction
  {
  public:
    Sine(EasingMode mode);

  protected:
    double calculatePointIn(double value) override;
    double calculatePointOut(double value) override;
  };

  class Quadratic : public EasingFunction
  {
  public:
    Quadratic(EasingMode mode);

  protected:
    double calculatePointIn(double value) override;
    double calculatePointOut(double value) override;
  };
};