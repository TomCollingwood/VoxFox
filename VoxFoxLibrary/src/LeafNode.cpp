#include "include/LeafNode.h"

LeafNode::LeafNode()
{
  m_VoxelMap = (char*)malloc(64*sizeof(char));
  memset(m_VoxelMap, 0, 64*sizeof(char));
}
LeafNode::LeafNode(glm::vec3 _origin) : m_origin(_origin) {
  m_VoxelMap = (char*)malloc(64*sizeof(char));
  memset(m_VoxelMap, 0, 64*sizeof(char));
}

bool LeafNode::isVoxel(glm::vec3 _position)
{
  glm::vec3 xyz = _position-m_origin;
  xyz = floor(xyz/unitVoxelLength);
  // this goes negative?? shiii man!!!!!!!
  if(xyz.x<0 || xyz.x>7 || xyz.y<0|| xyz.y>7 || xyz.z<0|| xyz.z>7) return false;
  return m_VoxelMap[((int)xyz.x)*8 + (int)xyz.y] & (unsigned int) 1<<(int)xyz.z;
}

//bool LeafNode::addVoxel(glm::vec3 _position, Voxel _voxel)
//{
//  glm::vec3 xyz = _position-m_origin;
//  if(xyz.x>=0 && xyz.x<unitLeafLength && xyz.y>=0 && xyz.y<unitLeafLength && xyz.z>=0 && xyz.z<unitLeafLength )
//  {
//    Voxel insertVoxel = Voxel(_voxel);
//    xyz = floor(xyz/unitVoxelLength);
//    insertVoxel.index = (int) (xyz.x*8*8 + xyz.y*8 + xyz.z);

//    if(!(m_VoxelMap[((int)xyz.x)*8 + (int)xyz.y] & (unsigned int) 1<<(int)xyz.z))
//    {
//      bool found = false;
//      for(std::vector<Voxel>::iterator it = m_VoxelData.begin(); it != m_VoxelData.end(); ++it)
//      {
//        if(it->index>insertVoxel.index)
//        {
//          m_VoxelData.insert(it,_voxel);
//          found = true;
//          break;
//        }
//        if(!found)
//        {
//           m_VoxelData.push_back(_voxel);
//        }
//      }
//      m_VoxelMap[((int)xyz.x)*8 + (int)xyz.y] |= (unsigned int) 1<<(int)xyz.z;
//    }
//    else
//    {
//      m_VoxelData.push_back(_voxel);
//    }
//    return true;
//  }
//  else
//  {
//    return false;
//  }
//}

//old non Voxel
bool LeafNode::addVoxel(glm::vec3 _position, Voxel _voxel)
{
  glm::vec3 xyz = _position-m_origin;
  if(xyz.x>=0 && xyz.x<unitLeafLength && xyz.y>=0 && xyz.y<unitLeafLength && xyz.z>=0 && xyz.z<unitLeafLength )
  {
    xyz = floor(xyz/unitVoxelLength);
    m_VoxelMap[((int)xyz.x)*8 + (int)xyz.y] |= (unsigned int) 1<<(int)xyz.z;

    return true;
  }
  else
  {
    return false;
  }
}

void LeafNode::draw(std::vector<float> * _vertexes, glm::vec3 _DOF)
{
  for(int i = 0; i<64; ++i)
  {
    for(int j = 0; j<8 ; ++j)
    {
      if(m_VoxelMap[i] & (1<<j))
      {
        float _x = (i/8)*unitVoxelLength +m_origin[0];
        float _y = (i-(floor(i/8)*8))*unitVoxelLength+m_origin[1];
        float _z = j*unitVoxelLength+m_origin[2];
        float _u =  unitVoxelLength;//*10;

        glm::vec3 n = _DOF;

        float incre = _u/5;

//        for(int k = 0; k<5; ++k)
//        {
//          glm::vec3 one = glm::vec3(_x,_y,k*_u+_z);
//          glm::vec3 two = glm::vec3(_x+_u,_y,(n.x*_x + n.y*_y + n.z*_z - n.x*(_x+_u) - n.y*+y)/n.z);
//          glm::vec3 thr = glm::vec3(_x+_u,_y+_u,(n.x*_x + n.y*_y + n.z*_z - n.x*(_x+_u) - n.y*+y)/n.z);
//        }

        /*
        float x1 = 0.4;
        float z1 = 0.4;
        float y1 = (n.x*_x + n.y*_y + n.z*_z - n.x*x1 - n.z*z1)/n.y;

        float x2 = 0.2;
        float z2 = 0.4;
        float y2 = (n.x*_x + n.y*_y + n.z*_z - n.x*x2 - n.z*z2)/n.y;

        _vertexes->push_back(_x);
        _vertexes->push_back(_y);
        _vertexes->push_back(_z);
        _vertexes->push_back(x1);
        _vertexes->push_back(y1);
        _vertexes->push_back(z1);
        _vertexes->push_back(x2);
        _vertexes->push_back(y2);
        _vertexes->push_back(z2);
        */


        // BACK FACE
        if(!(j>0 && m_VoxelMap[i] & (1<<(j-1))))
        {
          // 1
          _vertexes->push_back(_x);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z);

          // 2
          _vertexes->push_back(_x);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z);
        }
        // FRONT FACE
        if(!(j<7 && m_VoxelMap[i] & (1<<(j+1))))
        {
          // 3
          _vertexes->push_back(_x);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z+_u);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z+_u);

          _vertexes->push_back(_x);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z+_u);
          // 4
          _vertexes->push_back(_x);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z+_u);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z+_u);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z+_u);
        }
        // LEFT FACE
        if(!(i>7 && m_VoxelMap[i-8] & (1<<j)))
        {
          // 5
          _vertexes->push_back(_x);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z+_u);
          // 6
          _vertexes->push_back(_x);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z+_u);

          _vertexes->push_back(_x);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z+_u);
        }
        // RIGHT FACE
        if(!(i<=56 && m_VoxelMap[i+8] & (1<<j)))
        {
          // 7
          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z+_u);
          // 8
          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y);
          _vertexes->push_back(_z+_u);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z);

          _vertexes->push_back(_x+_u);
          _vertexes->push_back(_y+_u);
          _vertexes->push_back(_z+_u);
        }

        if(!(i%8<7 && m_VoxelMap[i+1] & (1<<j)))
        {
        // 9
        _vertexes->push_back(_x);
        _vertexes->push_back(_y+_u);
        _vertexes->push_back(_z);

        _vertexes->push_back(_x);
        _vertexes->push_back(_y+_u);
        _vertexes->push_back(_z+_u);

        _vertexes->push_back(_x+_u);
        _vertexes->push_back(_y+_u);
        _vertexes->push_back(_z+_u);
        // 10
        _vertexes->push_back(_x);
        _vertexes->push_back(_y+_u);
        _vertexes->push_back(_z);

        _vertexes->push_back(_x+_u);
        _vertexes->push_back(_y+_u);
        _vertexes->push_back(_z+_u);

        _vertexes->push_back(_x+_u);
        _vertexes->push_back(_y+_u);
        _vertexes->push_back(_z);
        }
        if(!(i%8>0 && m_VoxelMap[i-1] & (1<<j)))
        {
        // 11
        _vertexes->push_back(_x);
        _vertexes->push_back(_y);
        _vertexes->push_back(_z);

        _vertexes->push_back(_x+_u);
        _vertexes->push_back(_y);
        _vertexes->push_back(_z);

        _vertexes->push_back(_x+_u);
        _vertexes->push_back(_y);
        _vertexes->push_back(_z+_u);
        // 12
        _vertexes->push_back(_x);
        _vertexes->push_back(_y);
        _vertexes->push_back(_z);

        _vertexes->push_back(_x+_u);
        _vertexes->push_back(_y);
        _vertexes->push_back(_z+_u);

        _vertexes->push_back(_x);
        _vertexes->push_back(_y);
        _vertexes->push_back(_z+_u);
        }
        // */
      }
    }
  }
}

