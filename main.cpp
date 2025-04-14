#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <cstddef>
#include <cstdio>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "consumer.h"
#include "node.h"

Consumer consumer("from_mbdyn");
Node world = Node(glm::vec3(0., 0., 0.), 
                 glm::quat(1, 0, 0, 0),
                 "WORLD");
std::vector<Node> nodes;
// Node node = Node(glm::vec3(0., 0., 0.), 
//                  glm::quat(1, 0, 0, 0),
//                  "NODE");
//

const int GRID_SIZE = 10; // Размер сетки (количество линий)
const float GRID_STEP = 1.0f; // Шаг между линиями

// Функция для отрисовки горизонтальной сетки
void drawGrid() {
  glLineWidth(1.0f);
  glColor4f(137.f / 255., 137.f / 255., 169.f / 255., 0.1f); // Серый цвет для линий сетки
  glBegin(GL_LINES);
  // Линии вдоль оси X (параллельно оси Y)
  for (int i = -GRID_SIZE; i <= GRID_SIZE; ++i) {
    glVertex3f(-GRID_SIZE * GRID_STEP, i * GRID_STEP, 0.0f);
    glVertex3f(GRID_SIZE * GRID_STEP, i * GRID_STEP, 0.0f);
  }
  // Линии вдоль оси Y (параллельно оси X)
  for (int i = -GRID_SIZE; i <= GRID_SIZE; ++i) {
    glVertex3f(i * GRID_STEP, -GRID_SIZE * GRID_STEP, 0.0f);
    glVertex3f(i * GRID_STEP, GRID_SIZE * GRID_STEP, 0.0f);
  }
  glEnd();
}

void drawNodes(){
  for (int i = 0; i < nodes.size(); ++i){
    nodes[i].draw();
  }
}

// Функция отрисовки сцены
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(5.0f, 5.0f, 5.0f,  // Позиция камеры
            0.0f, 0.0f, 0.0f,  // Цель камеры
            0.0f, 0.0f, 1.0f); // Вектор "вверх"
  // Отрисовка сетки
  drawGrid();

  world.draw();
  drawNodes();
  glutSwapBuffers();
}


// Функция обновления сцены
void update(int value) {
  consumer.update();
  for (size_t i = 0; i < consumer.sbuf->data_size; i = i + 7) {
    glm::vec3 pos(
      (float)consumer.data[i + 0], 
      (float)consumer.data[i + 1], 
      (float)consumer.data[i + 2]
    );
    glm::quat rot = glm::normalize(glm::quat(
      (float)consumer.data[i + 3], 
      (float)consumer.data[i + 4], 
      (float)consumer.data[i + 5], 
      (float)consumer.data[i + 6]
    ));
      
    nodes[i / 7].setTransform(pos, rot);

    nodes[i].printTransform();
  }

  glutPostRedisplay(); // Перерисовываем сцену
  glutTimerFunc(16, update, 0); // Вызываем функцию обновления каждые 16 мс (~60 FPS)
}


// Инициализация OpenGL
void init() {
  for (size_t i = 0; i < consumer.sbuf->data_size / 7; ++i) {
    Node node = Node(glm::vec3(0., 0., 0.), glm::quat(1., 0., 0., 0.), "node");
    nodes.push_back(node);  
  }
  float color[3] = {18.0f / 255, 18.0f / 255, 54.0f / 255};
  glClearColor(color[0], color[1], color[2], 1.0f); // Цвет фона (черный)
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f, 1.0f, 0.1f, 100.0f); // Настройка перспективной проекции
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST); // Включение теста глубины
  glEnable(GL_BLEND); 
  glMatrixMode(GL_PROJECTION);
  glMatrixMode(GL_MODELVIEW);

  if (consumer.sbuf->data_size % 7 != 0){
    printf("INPUT SIZE IS NOT ACCEPTABLE\n");
  }
}


int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("OpenGL with GLM: Moving Coordinate Systems");
  glewInit(); // Инициализация GLEW (если используется)
  init();
  glutDisplayFunc(display);
  glutTimerFunc(16, update, 0); // Запуск таймера для анимации
  glutMainLoop();

  return 0;
}
