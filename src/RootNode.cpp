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

void RootNode::addVoxel(glm::vec3 _position)
{
  bool found = false;
  for (auto &prim : m_primChildren) // access by reference to avoid copying
      {
        if(_position[0]<prim->getOrigin()[0] || _position[0]>prim->getOrigin()[0]+unitChildLength) continue;
        if(_position[0]<prim->getOrigin()[1] || _position[1]>prim->getOrigin()[1]+unitChildLength) continue;
        if(_position[0]<prim->getOrigin()[2] || _position[2]>prim->getOrigin()[2]+unitChildLength) continue;
        prim->addVoxel(_position);
        found = true;
        break;
      }
  if(!found)
  {
    glm::vec3 newOrigin = floor(_position/unitChildLength)*unitChildLength;
    m_primChildren.push_back(new PrimaryNode(newOrigin));
    m_primChildren.back()->addVoxel(_position);
  }
  numberOfVoxels++;
}

void RootNode::draw()
{
  for (auto &prim : m_primChildren) // access by reference to avoid copying
  {
    prim->draw(m_vertexes);
  }
  

}



