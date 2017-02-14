#ifndef ROOTNODE_H
#define ROOTNODE_H

#include <iostream>
#include <vector>
#include "lib/glm/glm/glm.hpp"

#include "include/LeafNode.h"
#include "include/PrimaryNode.h"

class RootNode
{
public:
  RootNode();
  void addVoxel(glm::vec3 _position);

private:
  const float unitChildLength = 10.0;
  const int depth = 3;

  std::vector<LeafNode *> m_leafTable;
  std::vector<PrimaryNode *> m_primChildren;
  std::vector<LeafNode *> m_accessors;
};

#endif // ROOTNODE_H
