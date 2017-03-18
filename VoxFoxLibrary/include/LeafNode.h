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
#include <array>
#include <cstring>
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Text.h>

#include "glm/glm.hpp"
#include "Voxel.h"

class SecondaryNode;
class VoxFoxTree;

class LeafNode
{
public:
  LeafNode();
  LeafNode(glm::vec3 _origin);
  LeafNode(LeafNode const &l);

  LeafNode & operator=(LeafNode const &_l) ;

  LeafNode operator+(LeafNode const & _l);
  LeafNode operator|(LeafNode _l) ;
  LeafNode operator-(LeafNode _l) ;


  void moveX(int const &_shift);
  void moveY(int const &_shift);
  void moveZ(int const &_shift);

  bool addVoxel(glm::vec3 _position, Voxel _voxel);
  bool isVoxel(glm::vec3 _position);
  glm::vec3 getOrigin() const {return m_origin; }
  void shiftOrigin(int _x, int _y, int _z);

  std::array<char,64> m_VoxelMap;
  std::vector<Voxel> m_VoxelData;
  int idx, idy, idz;

private:
  glm::vec3 m_origin;
  float unitVoxelLength = 0.01953125;
  float unitLeafLength = 0.15625;
  float m_primUnit = 10.0;
  float m_secUnit = 1.25;
  float m_leafUnit = 0.15625;
  float m_voxUnit = 0.01953125;
};

#endif // LEAFNODE_H
