#include "include/LeafNode.h"

LeafNode::LeafNode()
{

}
LeafNode::LeafNode(glm::vec3 _origin) : m_origin(_origin) {
}

void LeafNode::addVoxel(glm::vec3 _position)
{
  glm::vec3 xyz = _position-m_origin;
  xyz = floor(xyz/unitVoxelLength);
  m_VoxelData[((int)xyz.x)*8 + (int)xyz.y] = (unsigned int) 1<<(int)xyz.z;
}
