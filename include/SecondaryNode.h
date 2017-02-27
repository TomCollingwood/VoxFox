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
  bool addVoxel(glm::vec3 _position, LeafNode * _leafAccessor);
  bool isVoxel(glm::vec3 _position);
  void draw(std::vector<float> * _vertexes, glm::vec3 _DOF);
  glm::vec3 getOrigin() {return m_origin; }
  PrimaryNode * m_parent;
  std::vector<LeafNode *> m_leafChildren;
private:
  bool full;
  glm::vec3 m_origin;
  float unitSecondaryLength = 1.25;
  float unitChildLength = 0.15625;

  RootNode * m_grandparent;

};

#endif // SECONDARYNODE_H
