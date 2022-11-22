#ifndef GLUT_HPP
#include <string>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#define GLUT_HPP

void point(GLfloat x, GLfloat y, GLfloat z);

void color(GLfloat r, GLfloat g, GLfloat b);

void color(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha);

void writeText(float x, float y, std::string text, float r, float g, float b, void *font);

void writeText(float x, float y, std::string text, void *font);

void getPreviousColors(float rgba[4]);

void setColor(float rgba[4]);

#endif
