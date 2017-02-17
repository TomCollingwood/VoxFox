#include "include/LeafNode.h"

LeafNode::LeafNode()
{
  memset(&m_VoxelData[0], 0, sizeof(m_VoxelData));
}
LeafNode::LeafNode(glm::vec3 _origin) : m_origin(_origin) {
}

void LeafNode::addVoxel(glm::vec3 _position)
{
  glm::vec3 xyz = _position-m_origin;
  xyz = floor(xyz/unitVoxelLength);
  m_VoxelData[((int)xyz.x)*8 + (int)xyz.y] = (unsigned int) 1<<(int)xyz.z;
}

void LeafNode::draw(std::vector<float> * _vertexes)
{
  for(int i = 0; i<64; ++i)
  {
    for(int j = 0; j<8 ; ++j)
    {
      if(m_VoxelData[i] & (1<<j))
      {
        _vertexes->push_back( (i/8)*unitVoxelLength +m_origin[0] );
        _vertexes->push_back((i-(floor(i/8)*8))*unitVoxelLength+m_origin[1]);
        _vertexes->push_back(j*unitVoxelLength+m_origin[2]);

        _vertexes->push_back( (i/8)*unitVoxelLength +m_origin[0] + unitVoxelLength);
        _vertexes->push_back((i-(floor(i/8)*8))*unitVoxelLength+m_origin[1]);
        _vertexes->push_back(j*unitVoxelLength+m_origin[2]);

        _vertexes->push_back( (i/8)*unitVoxelLength +m_origin[0] +unitVoxelLength);
        _vertexes->push_back((i-(floor(i/8)*8))*unitVoxelLength+m_origin[1] +unitVoxelLength);
        _vertexes->push_back(j*unitVoxelLength+m_origin[2]);
      }
    }
  }
}

