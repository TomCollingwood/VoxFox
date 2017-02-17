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
  int getSize();
  void draw();
  void addVoxel(glm::vec3 _position);
  void printVertexes();
  std::vector<float> * getVertexes();
  float getFloat(int i);
  int getVertexSize();

private:
  const float unitChildLength = 10.0;
  const int depth = 3;

  std::vector<LeafNode *> m_leafTable;
  std::vector<PrimaryNode *> m_primChildren;
  std::vector<LeafNode *> m_accessors;
  float numberOfVoxels;
  std::vector<float> * m_vertexes;
};

#endif // ROOTNODE_H
