#include "include/PrimaryNode.h"

PrimaryNode::PrimaryNode()
{

}

PrimaryNode::PrimaryNode(glm::vec3 _origin) : m_origin(_origin)
{}

void PrimaryNode::addVoxel(glm::vec3 _position)
{
  bool found = false;
  for (auto &sec : m_secChildren) // access by reference to avoid copying
      {
        if(_position[0]<sec->getOrigin()[0] || _position[0]>sec->getOrigin()[0]+unitChildLength) continue;
        if(_position[0]<sec->getOrigin()[1] || _position[1]>sec->getOrigin()[1]+unitChildLength) continue;
        if(_position[0]<sec->getOrigin()[2] || _position[2]>sec->getOrigin()[2]+unitChildLength) continue;
        sec->addVoxel(_position);
        found = true;
        break;
      }
  if(!found)
  {
    glm::vec3 newOrigin = floor(_position/unitChildLength)*unitChildLength;
    m_secChildren.push_back(new SecondaryNode(newOrigin));
    m_secChildren.back()->addVoxel(_position);
  }
}

void PrimaryNode::draw(std::vector<float> * _vertexes)
{
  for (auto &sec : m_secChildren) // access by reference to avoid copying
  {
    sec->draw(_vertexes);
  }
}

