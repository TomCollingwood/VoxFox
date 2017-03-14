#include "include/PrimaryNode.h"

PrimaryNode::PrimaryNode()
{

}

PrimaryNode::PrimaryNode(glm::vec3 _origin) : m_origin(_origin)
{
  idx = (int)(_origin[0]/m_primUnit);
  idy = (int)(_origin[1]/m_primUnit);
  idz = (int)(_origin[2]/m_primUnit);
}

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

void PrimaryNode::addVoxel(glm::vec3 _position, Voxel _voxel, SecondaryNode ** _secAccessor, LeafNode ** _leafAccessor)
{
  bool found = false;
  for (auto &sec : m_secChildren) // access by reference to avoid copying
      {
        if(_position[0]<sec->getOrigin()[0] || _position[0]>=sec->getOrigin()[0]+unitChildLength) continue;
        if(_position[1]<sec->getOrigin()[1] || _position[1]>=sec->getOrigin()[1]+unitChildLength) continue;
        if(_position[2]<sec->getOrigin()[2] || _position[2]>=sec->getOrigin()[2]+unitChildLength) continue;
        sec->addVoxel(_position,_voxel,_leafAccessor);
        (*_secAccessor)=sec;
        found = true;
        break;
      }
  if(!found)
  {
    glm::vec3 newOrigin = floor(_position/unitChildLength)*unitChildLength;
    m_secChildren.push_back(new SecondaryNode(newOrigin));
    m_secChildren.back()->addVoxel(_position,_voxel,_leafAccessor);
  }
}

bool PrimaryNode::isLeaf(glm::vec3 _position, LeafNode ** _leaf)
{
  for (int i =0 ; i< m_secChildren.size(); ++i) // access by reference to avoid copying
  {
    if(_position[0]<m_secChildren[i]->getOrigin()[0] || _position[0]>=m_secChildren[i]->getOrigin()[0]+unitChildLength) continue;
    if(_position[1]<m_secChildren[i]->getOrigin()[1] || _position[1]>=m_secChildren[i]->getOrigin()[1]+unitChildLength) continue;
    if(_position[2]<m_secChildren[i]->getOrigin()[2] || _position[2]>=m_secChildren[i]->getOrigin()[2]+unitChildLength) continue;
    return m_secChildren[i]->isLeaf(_position,_leaf);
  }
  return false;
}

bool PrimaryNode::isSecondary(glm::vec3 _position, SecondaryNode ** _secondary)
{
  for (int i =0 ; i< m_secChildren.size(); ++i) // access by reference to avoid copying
  {
    if(_position[0]<m_secChildren[i]->getOrigin()[0] || _position[0]>=m_secChildren[i]->getOrigin()[0]+unitChildLength) continue;
    if(_position[1]<m_secChildren[i]->getOrigin()[1] || _position[1]>=m_secChildren[i]->getOrigin()[1]+unitChildLength) continue;
    if(_position[2]<m_secChildren[i]->getOrigin()[2] || _position[2]>=m_secChildren[i]->getOrigin()[2]+unitChildLength) continue;
    *_secondary=m_secChildren[i];
    return true;
  }
  return false;
}


void PrimaryNode::add(PrimaryNode *_p)
{
  for(auto & i : m_secChildren)
  {
    for(auto & j : _p->m_secChildren)
    {
      if(i->idx==j->idx && i->idy==j->idy && i->idz==j->idz)
      {
        i->add(j);
      }
    }
  }
}

