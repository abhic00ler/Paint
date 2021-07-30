#ifndef POINT__H
#define POINT__H

#include <iostream>
#include <string>
using namespace std;

class Point
{
public:
  Point();
  Point(int x, int y, float r, float g, float b,int s);
  ~Point();

  int getX();
  int getY();
  float getR();
  float getG();
  float getB();
  int getSize();
  void setPosition(int x, int y);
  void setColour(float r, float g, float b);

private:
  int newX;
  int newY;
  float newR;
  float newG;
  float newB;
  int s;
};

#endif

