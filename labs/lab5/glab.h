#pragma once

#include "geometry.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>

#include <gl/GL.h>
#include <gl/freeglut.h>


void glut_init(int argc, char **argv, int numberOfPoints, int limitOfPointsValue);
void wrong_input(std::istream &in);