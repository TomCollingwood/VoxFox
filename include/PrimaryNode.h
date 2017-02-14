#ifndef PRIMARYNODE_H
#define PRIMARYNODE_H

#include <iostream>
#include <vector>
#include "lib/glm/glm/glm.hpp"

#include "include/LeafNode.h"
#include "include/SecondaryNode.h"
class RootNode;

class PrimaryNode
{
public:
  PrimaryNode();
  PrimaryNode(glm::vec3 _origin);
  void addVoxel(glm::vec3 _position);
  void draw(std::vector<float> * _vertexes);
  glm::vec3 getOrigin() {return m_origin; }
private:
  float unitChildLength = 1.25;
  glm::vec3 m_origin;
  RootNode * m_parent;
  std::vector<SecondaryNode *> m_secChildren;

};

#endif // PRIMARYNODE_H
