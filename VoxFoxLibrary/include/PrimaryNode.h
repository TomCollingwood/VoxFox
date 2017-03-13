#ifndef PRIMARYNODE_H
#define PRIMARYNODE_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "LeafNode.h"
#include "SecondaryNode.h"
class RootNode;

class PrimaryNode
{
public:
  PrimaryNode();
  PrimaryNode(glm::vec3 _origin);
  void addVoxel(glm::vec3 _position, Voxel _voxel, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor);
  bool isVoxel(glm::vec3 _position, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor);
  glm::vec3 getOrigin() {return m_origin; }
  RootNode * m_parent;
  bool full;
  std::vector<SecondaryNode *> m_secChildren;
  void add(PrimaryNode const& _p);
private:
  float unitChildLength = 1.25;
  glm::vec3 m_origin;


};

#endif // PRIMARYNODE_H
