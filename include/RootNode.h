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
  void draw(ngl::Mat4 MV);
  void addVoxel(glm::vec3 _position);
  bool isVoxel(glm::vec3 _position);
  void printVertexes();
  std::vector<float> * getVertexes();
  float getFloat(int i);
  int getVertexSize();
  void createSphere(glm::vec3 _position, int _radius);
  bool full;
private:
  const float unitChildLength = 10.0;
  const int depth = 3;
  float unitVoxelLength = 0.01953125;
  std::vector<LeafNode *> m_leafTable;
  std::vector<PrimaryNode *> m_primChildren;
  LeafNode * m_leafAccessor;
  SecondaryNode * m_secAccessor;
  PrimaryNode * m_primAccessor;
  float numberOfVoxels;
  std::vector<float> * m_vertexes;
};

#endif // ROOTNODE_H
