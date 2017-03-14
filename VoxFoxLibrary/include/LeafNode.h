/// \file LeafNode.h
/// \brief LeafNode   Class for the lowest level node. Contains the voxel data.
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox

#ifndef LEAFNODE_H
#define LEAFNODE_H

#include <iostream>
#include <vector>
#include <cstring>
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Text.h>

#include "glm/glm.hpp"
#include "DataStructs.h"

class SecondaryNode;
class RootNode;

class LeafNode
{
public:
  LeafNode();
  LeafNode(glm::vec3 _origin);
  ~LeafNode();
  bool addVoxel(glm::vec3 _position, Voxel _voxel);
  bool isVoxel(glm::vec3 _position);
  glm::vec3 getOrigin() {return m_origin; }
//  /void draw(std::vector<float> * _vertexes, glm::vec3 _DOF);
  bool full;
  SecondaryNode * m_parent;
  RootNode * m_root;
  glm::vec3 m_origin;
  char * m_VoxelMap;
  std::vector<Voxel> m_VoxelData;
  void add(LeafNode *_l);
  int idx, idy, idz;
private:
  int m_voxNumber;

  float unitVoxelLength = 0.01953125;
  float unitLeafLength = 0.15625;
  const float m_primUnit = 10.0;
  const float m_secUnit = 1.25;
  const float m_leafUnit = 0.15625;
  const float m_voxUnit = 0.01953125;

};

#endif // LEAFNODE_H
