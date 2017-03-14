#include "include/LeafNode.h"

LeafNode::LeafNode()
{
  m_VoxelMap = (char*)malloc(64*sizeof(char));
  memset(m_VoxelMap, 0, 64*sizeof(char));
}
LeafNode::LeafNode(glm::vec3 _origin) : m_origin(_origin)
{
  m_VoxelMap = (char*)malloc(64*sizeof(char));
  memset(m_VoxelMap, 0, 64*sizeof(char));
  idx = (int)(_origin[0]/m_leafUnit);
  idy = (int)(_origin[1]/m_leafUnit);
  idz = (int)(_origin[2]/m_leafUnit);
}
LeafNode::~LeafNode()
{
  free(m_VoxelMap);
}

bool LeafNode::isVoxel(glm::vec3 _position)
{
  glm::vec3 xyz = _position-m_origin;
  xyz = floor(xyz/unitVoxelLength);
  // this goes negative?? shiii man!!!!!!!
  if(xyz.x<0 || xyz.x>7 || xyz.y<0|| xyz.y>7 || xyz.z<0|| xyz.z>7) return false;
  return m_VoxelMap[((int)xyz.x)*8 + (int)xyz.y] & (unsigned int) 1<<(int)xyz.z;
}

bool LeafNode::addVoxel(glm::vec3 _position, Voxel _voxel)
{
  glm::vec3 xyz = _position-m_origin;
  if(xyz.x>=0 && xyz.x<unitLeafLength && xyz.y>=0 && xyz.y<unitLeafLength && xyz.z>=0 && xyz.z<unitLeafLength )
  {
    Voxel insertVoxel = _voxel;
    xyz = floor(xyz/unitVoxelLength);
    insertVoxel.index = (int) (xyz.x*8*8 + xyz.y*8 + xyz.z);
    if(!(m_VoxelMap[((int)xyz.x)*8 + (int)xyz.y] & (unsigned int) 1<<(int)xyz.z))
    {
      int count = 0;
      if(m_VoxelData.size()>0)
      {
        while(m_VoxelData[count].index<insertVoxel.index)
        {
          ++count;
          if(count+1>m_VoxelData.size())  break;
        }      
      }
      m_VoxelData.insert(m_VoxelData.begin()+count,insertVoxel);
      m_VoxelMap[((int)xyz.x)*8 + (int)xyz.y] |= (unsigned int) 1<<(int)xyz.z;
    }
    // */
    return true;
  }
  else
  {
    return false;
  }
}

void LeafNode::add(LeafNode *_l)
{
  for(int i = 0; i<64; ++i)
  {
    m_VoxelMap[i] |= _l->m_VoxelMap[i];
  }

  int index1 =0;
  int index2 =0;
  std::vector<Voxel> newVoxelData;
  bool done = false;

  // mergesort might be back?

  while(!done)
  {
    if(m_VoxelData[index1].index<_l->m_VoxelData[index2].index)
    {
      newVoxelData.push_back(m_VoxelData[index1]);
      index1++;
    }
    else if(m_VoxelData[index1].index>_l->m_VoxelData[index2].index)
    {
      newVoxelData.push_back(m_VoxelData[index2]);
      index2++;
    }
    else
    {
      newVoxelData.push_back(m_VoxelData[index1]);
      index2++;
      index1++;
    }
    if(index2>=_l->m_VoxelData.size())
    {
      for(int i=index1; i<m_VoxelData.size(); ++i)
      {
        newVoxelData.push_back(m_VoxelData[i]);
      }
      done=true;
    }
    else if(index1>=m_VoxelData.size())
    {
      for(int i=index2; i<_l->m_VoxelData.size(); ++i)
      {
        newVoxelData.push_back(_l->m_VoxelData[i]);
      }
      done=true;
    }
  }
  m_VoxelData = newVoxelData;
}
