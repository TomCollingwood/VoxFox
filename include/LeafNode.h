#ifndef LEAFNODE_H
#define LEAFNODE_H

// 4 * 4 * 4
#include <iostream>
#include "lib/glm/glm/glm.hpp"

class LeafNode
{
public:
  LeafNode();
  LeafNode(glm::vec3 _origin);
  void addVoxel(glm::vec3 _position);
  glm::vec3 getOrigin() {return m_origin; }
private:
  glm::vec3 m_origin;
  char m_VoxelData[64] = "";
  float unitVoxelLength = 0.01953125;
};

#endif // LEAFNODE_H
