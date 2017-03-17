#include "include/LeafNode.h"

LeafNode::LeafNode()
{
  m_VoxelMap.fill(0x00);
}
LeafNode::LeafNode(glm::vec3 _origin) : m_origin(_origin)
{
  idx = (int)(_origin[0]/m_leafUnit);
  idy = (int)(_origin[1]/m_leafUnit);
  idz = (int)(_origin[2]/m_leafUnit);
  m_VoxelMap.fill(0x00);
}

LeafNode::LeafNode(LeafNode const &l)
{
  m_VoxelMap=l.m_VoxelMap;
  m_VoxelData=l.m_VoxelData;
  idx=l.idx;
  idy=l.idy;
  idz=l.idz;
  m_origin=l.getOrigin();
}

bool LeafNode::isVoxel(glm::vec3 _position)
{
  glm::vec3 xyz = _position-m_origin;
  xyz = floor(xyz/unitVoxelLength);
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

void LeafNode::shiftOrigin(int _x, int _y, int _z)
{
  idx+=_x;
  idy+=_y;
  idz+=_z;
  m_origin+=glm::vec3(_x,_y,_z)*m_leafUnit;
}

void LeafNode::moveX(const int &_shift)
{
  std::array<char,64> oldMap = m_VoxelMap;
  for(int i = 0; i<8; ++i)
  {
    for(int j = 0; j<8; ++j)
    {
      if((i-_shift) >= 0 && (i-_shift) < 8)
      {
        m_VoxelMap[i*8 + j] = oldMap[(i-_shift)*8 + j];
      }
      else
      {
        m_VoxelMap[i*8 + j] = 0x00;
      }
    }
  }
}

void LeafNode::moveY(const int &_shift)
{
  std::array<char,64> oldMap = m_VoxelMap;
  for(int i = 0; i<8; ++i)
  {
    for(int j = 0; j<8; ++j)
    {
      if((j-_shift) >= 0 && (j-_shift) < 8)
      {
        m_VoxelMap[i*8 + j] = oldMap[i*8 + (j-_shift)];
      }
      else
      {
        m_VoxelMap[i*8 + j] = 0x00;
      }
    }
  }
}

void LeafNode::moveZ(const int &_shift)
{
  for(int i = 0; i<64; ++i)
  {
    if(_shift>0) m_VoxelMap[i] = m_VoxelMap[i]>>_shift;
    else m_VoxelMap[i] = m_VoxelMap[i]<<_shift;
  }
}

LeafNode LeafNode::operator+(LeafNode const &_l) const
{
  LeafNode retLeaf = LeafNode(m_origin);
  for(int i = 0; i<64; ++i)
  {
    retLeaf.m_VoxelMap[i] = m_VoxelMap[i] | _l.m_VoxelMap[i];
  }

  int index1 =0;
  int index2 =0;
  bool done = false;

  while(!done)
  {
    if(m_VoxelData[index1].index<_l.m_VoxelData[index2].index)
    {
      retLeaf.m_VoxelData.push_back(m_VoxelData[index1]);
      index1++;
    }
    else if(m_VoxelData[index1].index>_l.m_VoxelData[index2].index)
    {
      retLeaf.m_VoxelData.push_back(_l.m_VoxelData[index2]);
      index2++;
    }
    else
    {
      retLeaf.m_VoxelData.push_back(m_VoxelData[index1]);
      index2++;
      index1++;
    }
    if(index2>=_l.m_VoxelData.size())
    {
      for(int i=index1; i<m_VoxelData.size(); ++i)
      {
        retLeaf.m_VoxelData.push_back(m_VoxelData[i]);
      }
      done=true;
    }
    else if(index1>=m_VoxelData.size())
    {
      for(int i=index2; i<_l.m_VoxelData.size(); ++i)
      {
        retLeaf.m_VoxelData.push_back(_l.m_VoxelData[i]);
      }
      done=true;
    }
  }
  return retLeaf;
}
