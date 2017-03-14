#include "include/SecondaryNode.h"

SecondaryNode::SecondaryNode()
{

}

SecondaryNode::SecondaryNode(glm::vec3 _origin) : m_origin(_origin)
{
  idx = (int)(_origin[0]/m_secUnit);
  idy = (int)(_origin[1]/m_secUnit);
  idz = (int)(_origin[2]/m_secUnit);
}

bool SecondaryNode::isVoxel(glm::vec3 const &_position, LeafNode ** _leafAccessor)
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

bool SecondaryNode::isLeaf(glm::vec3 _position, LeafNode ** _leaf)
{
  for (int i =0; i<m_leafChildren.size(); ++i) // access by reference to avoid copying
  {
    if(_position[0]<m_leafChildren[i]->getOrigin()[0] || _position[0]>=m_leafChildren[i]->getOrigin()[0]+unitChildLength) continue;
    if(_position[1]<m_leafChildren[i]->getOrigin()[1] || _position[1]>=m_leafChildren[i]->getOrigin()[1]+unitChildLength) continue;
    if(_position[2]<m_leafChildren[i]->getOrigin()[2] || _position[2]>=m_leafChildren[i]->getOrigin()[2]+unitChildLength) continue;
    *_leaf = m_leafChildren[i];
    return true;
  }
  return false;
}

void SecondaryNode::add(SecondaryNode *_s)
{
  for(auto & i : m_leafChildren)
  {
    for(auto & j : _s->m_leafChildren)
    {
      if(i->idx==j->idx && i->idy==j->idy && i->idz==j->idz)
      {
        i->add(j);
      }
    }
  }
}
