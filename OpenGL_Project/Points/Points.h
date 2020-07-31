#pragma once

#include "../application.h"

class Points : public Application {
public:
  Points();
  void init(GLFWwindow* window) {

  }
  void input(GLFWwindow* window);
  void render();
  void clear();
  void prepare();

  // ¼ÆËãµã
  void calcLine(int start_x, int start_y, int end_x, int end_y);
  void calcCircle(int x, int y, int r);
  void calcTriangle();

  // »­µã
  void drawPoint();
  inline void addPoint(int x, int y);
  void updateWindowSize(int h, int w)
  {
	  this->windowHeight = h;
	  this->windowWidth = w;
  }
  int windowHeight;
  int windowWidth;
private:
  int draw_type;
  int canvas_size;
  bool rasterize;

  float point_size;

  int point_count;
  int vertices[1000000];
  float finalVertices[1000000];

  int triangles_vertices[6];

  int circle_x;
  int circle_y;
  int circle_r;

  ImVec4 bg_color;
  ImVec4 shader_color;
  Shader* point_shader;
};