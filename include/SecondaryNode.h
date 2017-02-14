#ifndef SECONDARYNODE_H
#define SECONDARYNODE_H

#include <iostream>
#include <vector>

#include "include/LeafNode.h"

class RootNode;
class PrimaryNode;

class SecondaryNode
{
public:
  SecondaryNode();
  SecondaryNode(glm::vec3 _origin);
  void addVoxel(glm::vec3 _position);
  glm::vec3 getOrigin() {return m_origin; }
private:
  glm::vec3 m_origin;
  float unitChildLength = 0.15625;
  PrimaryNode * m_parent;
  RootNode * m_grandparent;
  std::vector<LeafNode *> m_leafChildren;
};

#endif // SECONDARYNODE_H
