#ifndef PRIMARYNODE_H
#define PRIMARYNODE_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "LeafNode.h"
#include "SecondaryNode.h"
#include "DataStructs.h"

class RootNode;

class PrimaryNode
{
public:
  PrimaryNode();
  PrimaryNode(glm::vec3 _origin, PrimaryNodeIndex _index) : m_index(_index), m_origin(_origin) {}
  void addVoxel(glm::vec3 _position, Voxel _voxel, SecondaryNodeIndex &_secAccessor, LeafNodeIndex &_leafAccessor);
  void draw(std::vector<float> * _vertexes, glm::vec3 _DOF);
  bool isVoxel(glm::vec3 _position, SecondaryNodeIndex &_secAccessor, LeafNodeIndex &_leafAccessor);
  glm::vec3 getOrigin() {return m_origin; }
  RootNode * m_parent;
  bool full;
  std::vector<SecondaryNode> m_secChildren;
  PrimaryNodeIndex getIndex() { return m_index; }
private:
  PrimaryNodeIndex m_index = PrimaryNodeIndex();
  float unitChildLength = 1.25;
  glm::vec3 m_origin;


};

#endif // PRIMARYNODE_H
