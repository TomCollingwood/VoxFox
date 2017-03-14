/// \file PrimaryNode.h
/// \brief PrimaryNode.h  The highest node level that RootNode has.
///                       PrimaryNode contains the SecondaryNodes (up to 8^3)
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox

#ifndef PRIMARYNODE_H
#define PRIMARYNODE_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "LeafNode.h"
#include "SecondaryNode.h"
class RootNode;

class PrimaryNode
{
public:
  PrimaryNode();
  PrimaryNode(glm::vec3 _origin);
  void addVoxel(glm::vec3 _position, Voxel _voxel, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor);
  bool isVoxel(glm::vec3 _position, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor);
  glm::vec3 getOrigin() {return m_origin; }
  RootNode * m_parent;
  bool full;
  std::vector<SecondaryNode *> m_secChildren;
  void add(PrimaryNode *_p);
  bool isLeaf(glm::vec3 _position, LeafNode ** _leaf);
  bool isSecondary(glm::vec3 _position, SecondaryNode ** _secondary);
  int idx, idy, idz;
private:
  float unitChildLength = 1.25;
  const float m_primUnit = 10.0;
  const float m_secUnit = 1.25;
  const float m_leafUnit = 0.15625;
  const float m_voxUnit = 0.01953125;
  glm::vec3 m_origin;




};

#endif // PRIMARYNODE_H
