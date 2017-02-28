#include "include/PrimaryNode.h"

PrimaryNode::PrimaryNode()
{

}

PrimaryNode::PrimaryNode(glm::vec3 _origin) : m_origin(_origin)
{}

bool PrimaryNode::isVoxel(glm::vec3 _position, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor)
{
  for (auto &sec : m_secChildren) // access by reference to avoid copying
      {
        if(_position[0]<sec->getOrigin()[0] || _position[0]>=sec->getOrigin()[0]+unitChildLength) continue;
        if(_position[1]<sec->getOrigin()[1] || _position[1]>=sec->getOrigin()[1]+unitChildLength) continue;
        if(_position[2]<sec->getOrigin()[2] || _position[2]>=sec->getOrigin()[2]+unitChildLength) continue;
        (*_secAccessor)=sec;
        return sec->isVoxel(_position,_leafAccessor);
      }
  return false;
}

void PrimaryNode::addVoxel(glm::vec3 _position, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor)
{
  bool found = false;
  for (auto &sec : m_secChildren) // access by reference to avoid copying
      {
        if(_position[0]<sec->getOrigin()[0] || _position[0]>=sec->getOrigin()[0]+unitChildLength) continue;
        if(_position[1]<sec->getOrigin()[1] || _position[1]>=sec->getOrigin()[1]+unitChildLength) continue;
        if(_position[2]<sec->getOrigin()[2] || _position[2]>=sec->getOrigin()[2]+unitChildLength) continue;
        sec->addVoxel(_position,_leafAccessor);
        (*_secAccessor)=sec;
        found = true;
        break;
      }
  if(!found)
  {
    glm::vec3 newOrigin = floor(_position/unitChildLength)*unitChildLength;
    m_secChildren.push_back(new SecondaryNode(newOrigin));
    m_secChildren.back()->addVoxel(_position,_leafAccessor);
  }
}

void PrimaryNode::draw(std::vector<float> * _vertexes, glm::vec3 _DOF)
{
  for (auto &sec : m_secChildren) // access by reference to avoid copying
  {
    sec->draw(_vertexes, _DOF);
  }
}

