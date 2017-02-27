#ifndef LEAFNODE_H
#define LEAFNODE_H

// 4 * 4 * 4
#include <iostream>
#include <vector>
#include <cstring>
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Text.h>
#include "lib/glm/glm/glm.hpp"

class SecondaryNode;
class RootNode;

class LeafNode
{
public:
  LeafNode();
  LeafNode(glm::vec3 _origin);
  bool addVoxel(glm::vec3 _position);
  bool isVoxel(glm::vec3 _position);
  glm::vec3 getOrigin() {return m_origin; }
  void draw(std::vector<float> * _vertexes, glm::vec3 _DOF);
  bool full;
  SecondaryNode * m_parent;
  RootNode * m_root;
  glm::vec3 m_origin;
  char * m_VoxelData;
private:
  int m_voxNumber;

  float unitVoxelLength = 0.01953125;
  float unitLeafLength = 0.15625;
};

#endif // LEAFNODE_H
