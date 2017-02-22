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
  if(m_leafAccessor->isVoxel(_position))
  {
          return true;
  }
  else if(m_secAccessor->isVoxel(_position))
  {
          return true;
  }
  else if(m_primAccessor->isVoxel(_position))
  {
          return true;
  }
  else
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
    prim->draw(m_vertexes, DOF);
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

