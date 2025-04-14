#ifndef NODE
#define NODE

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

class Node {
public:
  Node(float x, float y, float z, 
       float w, float i, float j, float k,
       std::string label);
  Node(glm::vec3 position, glm::quat rotation, std::string label);
  Node();

  ~Node();
  void draw();
  void setPos(glm::vec3 pos);
  void setRot(glm::quat rot);
  void printTransform();

private:
  std::string label;
  float AXIS_LENGTH = 1.0f;
  glm::mat4 transform;

};


#endif // !NODE
