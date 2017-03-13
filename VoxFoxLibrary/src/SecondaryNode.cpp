#include "include/SecondaryNode.h"

SecondaryNode::SecondaryNode()
{

}

SecondaryNode::SecondaryNode(glm::vec3 _origin) : m_origin(_origin) {}

bool SecondaryNode::isVoxel(glm::vec3 _position, LeafNode ** _leafAccessor)
{
  for (auto &leaf : m_leafChildren) // access by reference to avoid copying
      {
        if(_position[0]<leaf->getOrigin()[0] || _position[0]>=leaf->getOrigin()[0]+unitChildLength) continue;
        if(_position[1]<leaf->getOrigin()[1] || _position[1]>=leaf->getOrigin()[1]+unitChildLength) continue;
        if(_position[2]<leaf->getOrigin()[2] || _position[2]>=leaf->getOrigin()[2]+unitChildLength) continue;
        (*_leafAccessor)=leaf;
        return leaf->isVoxel(_position);
      }
  return false;
}

bool SecondaryNode::addVoxel(glm::vec3 _position, Voxel _voxel, LeafNode ** _leafAccessor)
{
  for (auto &leaf : m_leafChildren) // access by reference to avoid copying
      {
        if(_position[0]<leaf->getOrigin()[0] || _position[0]>=leaf->getOrigin()[0]+unitChildLength) continue;
        if(_position[1]<leaf->getOrigin()[1] || _position[1]>=leaf->getOrigin()[1]+unitChildLength) continue;
        if(_position[2]<leaf->getOrigin()[2] || _position[2]>=leaf->getOrigin()[2]+unitChildLength) continue;

        //if(glm::all(glm::lessThan(_position,leaf->getOrigin)) && glm::all(glm::greaterThanEqual(_position,)))

        leaf->addVoxel(_position,_voxel);
        (*_leafAccessor)=leaf;
        return true;
      }

  glm::vec3 newOrigin = floor(_position/unitChildLength)*unitChildLength;

  if(_position[0]>=m_origin[0] && _position[0]<m_origin[0]+unitSecondaryLength &&
    _position[1]>=m_origin[1] && _position[1]<m_origin[1]+unitSecondaryLength &&
    _position[2]>=m_origin[2] && _position[2]<m_origin[2]+unitSecondaryLength)
{
    m_leafChildren.push_back(new LeafNode(newOrigin));
    m_leafChildren.back()->addVoxel(_position,_voxel);
    return true;
  }
  else
  {
    return false;
  }
}

void SecondaryNode::add(SecondaryNode const& _s)
{
  for(auto & i : m_leafChildren)
  {
    for(auto & j : _s.m_leafChildren)
    {
      if(j->getOrigin()==i->getOrigin())
      {
        i->add(*j);
      }
    }
  }
}
