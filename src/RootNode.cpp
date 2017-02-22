#include "include/RootNode.h"

RootNode::RootNode()
{
  m_vertexes = new std::vector<float>(0);
}

std::vector<float> * RootNode::getVertexes()
{
  return m_vertexes;
}

int RootNode::getSize()
{
  return m_primChildren.size();
}

float RootNode::getFloat(int i)
{
  return m_vertexes->at(i);
}

int RootNode::getVertexSize()
{
  return m_vertexes->size();
}

void RootNode::printVertexes()
{
  for (std::vector<float>::const_iterator i = m_vertexes->begin(); i != m_vertexes->end(); ++i)
      std::cout << *i << ' ';
}

bool RootNode::isVoxel(glm::vec3 _position)
{

    for (auto &prim : m_primChildren) // access by reference to avoid copying
        {
          if(_position[0]<prim->getOrigin()[0] || _position[0]>=prim->getOrigin()[0]+unitChildLength) continue;
          if(_position[1]<prim->getOrigin()[1] || _position[1]>=prim->getOrigin()[1]+unitChildLength) continue;
          if(_position[2]<prim->getOrigin()[2] || _position[2]>=prim->getOrigin()[2]+unitChildLength) continue;
          return prim->isVoxel(_position);
        }
    return false;
}

void RootNode::addVoxel(glm::vec3 _position)
{
  bool found = false;
  for (auto &prim : m_primChildren) // access by reference to avoid copying
      {
        if(_position[0]<prim->getOrigin()[0] || _position[0]>=prim->getOrigin()[0]+unitChildLength) continue;
        if(_position[1]<prim->getOrigin()[1] || _position[1]>=prim->getOrigin()[1]+unitChildLength) continue;
        if(_position[2]<prim->getOrigin()[2] || _position[2]>=prim->getOrigin()[2]+unitChildLength) continue;
        m_primAccessor=prim;
        prim->addVoxel(_position,m_secAccessor,m_leafAccessor);
        found = true;
        break;
      }
  if(!found)
  {
    glm::vec3 newOrigin = floor(_position/unitChildLength)*unitChildLength;
    m_primChildren.push_back(new PrimaryNode(newOrigin));
    m_primChildren.back()->addVoxel(_position,m_secAccessor,m_leafAccessor);
  }
  numberOfVoxels++;
}

void RootNode::draw(ngl::Mat4 MV)
{
  m_vertexes->clear();

  glm::vec3 DOF = glm::vec3(-MV.m_02,-MV.m_12,-MV.m_22);

//  DOF[0] = static_cast<float>(MV.m_02); // x
//  DOF[1] = static_cast<float>(MV.m_12); // y
//  DOF[2] = static_cast<float>(MV.m_22); // z

  for (auto &prim : m_primChildren) // access by reference to avoid copying
  {
    //prim->draw(m_vertexes, DOF);
    for(auto &sec : prim->m_secChildren)
    {
      for(auto &leaf : sec->m_leafChildren)
      {
        for(int i = 0; i<64; ++i)
        {
          for(int j = 0; j<8 ; ++j)
          {
            if(leaf->m_VoxelData[i] & (1<<j))
            {
              float _x = (i/8)*unitVoxelLength +leaf->m_origin[0];
              float _y = (i-(floor(i/8)*8))*unitVoxelLength+leaf->m_origin[1];
              float _z = j*unitVoxelLength+leaf->m_origin[2];
              float _u =  unitVoxelLength;//*10;


              glm::vec3 n = DOF;

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

              m_vertexes->push_back(_x);
              m_vertexes->push_back(_y);
              m_vertexes->push_back(_z);
              m_vertexes->push_back(x1);
              m_vertexes->push_back(y1);
              m_vertexes->push_back(z1);
              m_vertexes->push_back(x2);
              m_vertexes->push_back(y2);
              m_vertexes->push_back(z2);
              */


              // BACK FACE
              if(!isVoxel(glm::vec3(_x,_y,_z-unitVoxelLength)) && !(j>0 && leaf->m_VoxelData[i] & (1<<(j-1))))
              {
                // 1
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z);

                // 2
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z);

              }
              // FRONT FACE
              if(!isVoxel(glm::vec3(_x,_y,_z+unitVoxelLength)) && !(j<7 && leaf->m_VoxelData[i] & (1<<(j+1))))
              {
                // 3
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);

                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z+_u);
                // 4
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z+_u);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z+_u);
              }
              // LEFT FACE
              if(!isVoxel(glm::vec3(_x-unitVoxelLength,_y,_z)) && !(i>7 && leaf->m_VoxelData[i-8] & (1<<j)))
              {
                // 5
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);
                // 6
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);

                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z+_u);
              }
              // RIGHT FACE
              if(!isVoxel(glm::vec3(_x+unitVoxelLength,_y,_z)) && !(i<=56 && leaf->m_VoxelData[i+8] & (1<<j)))
              {
                // 7
                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);
                // 8
                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z+_u);
              }

              if(!isVoxel(glm::vec3(_x,_y+unitVoxelLength,_z)) && !(i%8<7 && leaf->m_VoxelData[i+1] & (1<<j)))
              {
              // 9
              m_vertexes->push_back(_x);
              m_vertexes->push_back(_y+_u);
              m_vertexes->push_back(_z);

              m_vertexes->push_back(_x);
              m_vertexes->push_back(_y+_u);
              m_vertexes->push_back(_z+_u);

              m_vertexes->push_back(_x+_u);
              m_vertexes->push_back(_y+_u);
              m_vertexes->push_back(_z+_u);
              // 10
              m_vertexes->push_back(_x);
              m_vertexes->push_back(_y+_u);
              m_vertexes->push_back(_z);

              m_vertexes->push_back(_x+_u);
              m_vertexes->push_back(_y+_u);
              m_vertexes->push_back(_z+_u);

              m_vertexes->push_back(_x+_u);
              m_vertexes->push_back(_y+_u);
              m_vertexes->push_back(_z);
              }
              if(!isVoxel(glm::vec3(_x,_y-unitVoxelLength,_z)) && !(i%8>0 && leaf->m_VoxelData[i-1] & (1<<j)))
              {
              // 11
              m_vertexes->push_back(_x);
              m_vertexes->push_back(_y);
              m_vertexes->push_back(_z);

              m_vertexes->push_back(_x+_u);
              m_vertexes->push_back(_y);
              m_vertexes->push_back(_z);

              m_vertexes->push_back(_x+_u);
              m_vertexes->push_back(_y);
              m_vertexes->push_back(_z+_u);
              // 12
              m_vertexes->push_back(_x);
              m_vertexes->push_back(_y);
              m_vertexes->push_back(_z);

              m_vertexes->push_back(_x+_u);
              m_vertexes->push_back(_y);
              m_vertexes->push_back(_z+_u);

              m_vertexes->push_back(_x);
              m_vertexes->push_back(_y);
              m_vertexes->push_back(_z+_u);
              }
              // */
            }
          }
        }
      }
    }
  }

  printf("%d",m_vertexes->size()/3);

}

void RootNode::createSphere(glm::vec3 _position, int _radius)
{
  for(int x = -_radius; x<_radius; ++x)
  {
    for(int y = -_radius; y<_radius; ++y)
      {
      for(int z = -_radius; z<_radius; ++z)
        {
          glm::vec3 pos = glm::vec3(x,y,z);
          pos*=unitVoxelLength;
          pos+=_position;
          glm::vec3 vect = pos-_position;
          if(glm::length(vect) < _radius*unitVoxelLength)
          {
            addVoxel(pos);
          }
        }
      }
  }
}

void RootNode::createTorus(glm::vec3 _position, glm::vec2 _t)
{
  int _radius = 50;
  for(int x = -_radius; x<_radius; ++x)
  {
    for(int y = -_radius; y<_radius; ++y)
      {
      for(int z = -_radius; z<_radius; ++z)
        {
          glm::vec3 pos = glm::vec3(x,y,z);
          pos*=unitVoxelLength;
          pos+=_position;

          glm::vec2 q = glm::vec2(glm::length(glm::vec2(pos.x,pos.z))-_t.x,pos.y);
          if(glm::length(q) < _t.y)
          {
            addVoxel(pos);
          }
        }
      }
  }
}

//void createRoundBox( glm::vec3 p, glm::vec3 b, float r )
//{
//  return length(max(abs(p)-b,0.0))-r;

//  for(int x = -_radius; x<_radius; ++x)
//  {
//    for(int y = -_radius; y<_radius; ++y)
//      {
//      for(int z = -_radius; z<_radius; ++z)
//        {
//          glm::vec3 pos = glm::vec3(x,y,z);
//          pos*=unitVoxelLength;
//          pos+=_position;

//          glm::vec2 q = glm::vec2(glm::length(glm::vec2(pos.x,pos.z))-_t.x,pos.y);
//          if(glm::length(q) < _t.y)
//          {
//            addVoxel(pos);
//          }
//        }
//      }
//  }
//}
