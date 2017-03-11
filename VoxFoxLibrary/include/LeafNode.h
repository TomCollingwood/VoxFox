#ifndef LEAFNODE_H
#define LEAFNODE_H

// 4 * 4 * 4
#include <iostream>
#include <vector>
#include <deque>
#include <cstring>
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Text.h>
#include <glm/glm.hpp>
#include "DataStructs.h"

class SecondaryNode;
class RootNode;

class LeafNode
{
public:
  LeafNode();
  LeafNode(glm::vec3 _origin, int _index) : m_origin(_origin), m_index(_index) {}
  bool addVoxel(glm::vec3 _position, Voxel _voxel);
  bool isVoxel(glm::vec3 _position);
  glm::vec3 getOrigin();
  void draw(std::vector<float> * _vertexes, glm::vec3 _DOF);
  bool full;
  SecondaryNode * m_parent;
  RootNode * m_root;
  glm::vec3 m_origin;
  char m_VoxelMap[64]="";
  std::deque<Voxel> m_VoxelData;
  int getIndex() { return m_index; }
private:
  //int m_voxNumber;
  int m_index;
  float unitVoxelLength = 0.01953125;
  float unitLeafLength = 0.15625;
};

#endif // LEAFNODE_H
