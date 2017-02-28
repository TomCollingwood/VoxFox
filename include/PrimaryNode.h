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
  void addVoxel(glm::vec3 _position, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor);
  void draw(std::vector<float> * _vertexes, glm::vec3 _DOF);
  bool isVoxel(glm::vec3 _position, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor);
  glm::vec3 getOrigin() {return m_origin; }
  RootNode * m_parent;
  bool full;
  std::vector<SecondaryNode *> m_secChildren;
private:
  float unitChildLength = 1.25;
  glm::vec3 m_origin;


};

#endif // PRIMARYNODE_H
