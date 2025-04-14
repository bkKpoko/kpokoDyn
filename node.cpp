#include "node.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ostream>
#include <string>


Node::Node(float x, float y, float z, 
           float w, float i, float j, float k,
           std::string label) : label(label){
}

Node::Node(glm::vec3 pos, 
           glm::quat rot,
           std::string label) : label(label){
  transform = glm::translate(glm::mat4(1.0f), pos) * glm::mat4_cast(rot);
}

Node::Node(){
  transform = glm::mat4(1.0f);
}

Node::~Node(){

}


void Node::draw() {
  glLineWidth(5.0f);
  glPushMatrix();
  glMultMatrixf(glm::value_ptr(transform)); // Применяем преобразование
  
  glBegin(GL_LINES);
  // Ось X (красная)
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(AXIS_LENGTH, 0.0f, 0.0f);

  // Ось Y (зеленая)
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, AXIS_LENGTH, 0.0f);

  // Ось Z (синяя)
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, AXIS_LENGTH);
  glEnd();

  // Подпись системы координат
  glColor3f(1.0f, 1.0f, 1.0f); // Белый цвет текста
  glRasterPos3f(0.0f, 0.0f, 0.1f); // Позиция текста
  for (char c : label) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
  }
  glPopMatrix();
}


void Node::setPos(glm::vec3 pos){
  transform = glm::translate(transform, pos);
}

void Node::setRot(glm::quat rot){
  transform = transform * glm::mat4_cast(rot);
}


void Node::setTransform(glm::vec3 pos, glm::quat rot){
  transform = glm::mat4_cast(rot);
  transform[3] = glm::vec4(pos, 1.0f);
}

void Node::printTransform(){
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::cout << transform[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
