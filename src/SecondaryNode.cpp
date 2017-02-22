#include "include/SecondaryNode.h"

SecondaryNode::SecondaryNode()
{

}

SecondaryNode::SecondaryNode(glm::vec3 _origin) : m_origin(_origin) {}

bool SecondaryNode::isVoxel(glm::vec3 _position)
{
  for (auto &leaf : m_leafChildren) // access by reference to avoid copying
      {
        if(_position[0]<leaf->getOrigin()[0] || _position[0]>=leaf->getOrigin()[0]+unitChildLength) continue;
        if(_position[1]<leaf->getOrigin()[1] || _position[1]>=leaf->getOrigin()[1]+unitChildLength) continue;
        if(_position[2]<leaf->getOrigin()[2] || _position[2]>=leaf->getOrigin()[2]+unitChildLength) continue;
        return leaf->isVoxel(_position);
      }
  return false;
}

void SecondaryNode::addVoxel(glm::vec3 _position, LeafNode * _leafAccessor)
{
  bool found = false;
  for (auto &leaf : m_leafChildren) // access by reference to avoid copying
      {
        if(_position[0]<leaf->getOrigin()[0] || _position[0]>=leaf->getOrigin()[0]+unitChildLength) continue;
        if(_position[1]<leaf->getOrigin()[1] || _position[1]>=leaf->getOrigin()[1]+unitChildLength) continue;
        if(_position[2]<leaf->getOrigin()[2] || _position[2]>=leaf->getOrigin()[2]+unitChildLength) continue;
        leaf->addVoxel(_position);
        _leafAccessor=leaf;
        found = true;
        break;
      }
  if(!found)
  {
    glm::vec3 newOrigin = floor(_position/unitChildLength)*unitChildLength;
    m_leafChildren.push_back(new LeafNode(newOrigin));
    m_leafChildren.back()->addVoxel(_position);
  }
}

void SecondaryNode::draw(std::vector<float> * _vertexes, glm::vec3 _DOF)
{
  for (auto &leaf : m_leafChildren) // access by reference to avoid copying
  {
    leaf->draw(_vertexes, _DOF);
  }
}
