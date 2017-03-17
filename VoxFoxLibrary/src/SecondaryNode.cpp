#include "include/SecondaryNode.h"

SecondaryNode::SecondaryNode()
{

}

SecondaryNode::~SecondaryNode()
{
  for(auto & l : m_leafChildren)
  {
    delete l;
  }
  m_leafChildren.clear();
}

SecondaryNode::SecondaryNode(glm::vec3 _origin) : m_origin(_origin)
{
  idx = (int)(_origin[0]/m_secUnit);
  idy = (int)(_origin[1]/m_secUnit);
  idz = (int)(_origin[2]/m_secUnit);
}

SecondaryNode::SecondaryNode(const SecondaryNode &s)
{
  for(auto & i : s.m_leafChildren)
  {
    m_leafChildren.push_back(new LeafNode(*i));
  }
  m_origin = s.getOrigin();
  idx = s.idx;
  idy = s.idy;
  idz = s.idz;
}

SecondaryNode & SecondaryNode::operator=(SecondaryNode const &s)
{
  if(this!=&s)
  {
    // DELETE OLD DATA
    for(auto & l : m_leafChildren)
    {
      delete l;
    }
    m_leafChildren.clear();

    // ASSIGN NEW DATA
    for(auto & i : s.m_leafChildren)
    {
      m_leafChildren.push_back(new LeafNode(*i));
    }
    m_origin=s.getOrigin();
    idx = s.idx;
    idy = s.idy;
    idz = s.idz;
  }
  return *this;
}

bool SecondaryNode::isVoxel(glm::vec3 const &_position, LeafNode ** _leafAccessor)
{
  for (auto &leaf : m_leafChildren) // access by reference to avoid copying
      {
        if(_position[0]<leaf->getOrigin()[0] || _position[0]>=leaf->getOrigin()[0]+m_leafUnit) continue;
        if(_position[1]<leaf->getOrigin()[1] || _position[1]>=leaf->getOrigin()[1]+m_leafUnit) continue;
        if(_position[2]<leaf->getOrigin()[2] || _position[2]>=leaf->getOrigin()[2]+m_leafUnit) continue;
        (*_leafAccessor)=leaf;
        return leaf->isVoxel(_position);
      }
  return false;
}

bool SecondaryNode::addVoxel(glm::vec3 _position, Voxel _voxel, LeafNode ** o_leafAccessor)
{
  for (auto &leaf : m_leafChildren) // access by reference to avoid copying
      {
        if(_position[0]<leaf->getOrigin()[0] || _position[0]>=leaf->getOrigin()[0]+m_leafUnit) continue;
        if(_position[1]<leaf->getOrigin()[1] || _position[1]>=leaf->getOrigin()[1]+m_leafUnit) continue;
        if(_position[2]<leaf->getOrigin()[2] || _position[2]>=leaf->getOrigin()[2]+m_leafUnit) continue;

        //if(glm::all(glm::lessThan(_position,leaf->getOrigin)) && glm::all(glm::greaterThanEqual(_position,)))

        leaf->addVoxel(_position,_voxel);
        (*o_leafAccessor)=leaf;
        return true;
      }

  glm::vec3 newOrigin = floor(_position/m_leafUnit)*m_leafUnit;

  if(_position[0]>=m_origin[0] && _position[0]<m_origin[0]+m_secUnit &&
    _position[1]>=m_origin[1] && _position[1]<m_origin[1]+m_secUnit &&
    _position[2]>=m_origin[2] && _position[2]<m_origin[2]+m_secUnit)
  {
    m_leafChildren.push_back(new LeafNode(newOrigin));
    m_leafChildren.back()->addVoxel(_position,_voxel);
    (*o_leafAccessor)=m_leafChildren.back();
    return true;
  }
  else
  {
    return false;
  }
}

bool SecondaryNode::isLeaf(glm::vec3 _position, LeafNode ** o_leaf)
{

  for (int i =0; i<m_leafChildren.size(); ++i) // access by reference to avoid copying
  {
    if(_position[0]<m_leafChildren[i]->getOrigin()[0] || _position[0]>=m_leafChildren[i]->getOrigin()[0]+m_leafUnit) continue;
    if(_position[1]<m_leafChildren[i]->getOrigin()[1] || _position[1]>=m_leafChildren[i]->getOrigin()[1]+m_leafUnit) continue;
    if(_position[2]<m_leafChildren[i]->getOrigin()[2] || _position[2]>=m_leafChildren[i]->getOrigin()[2]+m_leafUnit) continue;
    *o_leaf = m_leafChildren[i];
    return true;
  }
  return false;
}

SecondaryNode SecondaryNode::operator +(SecondaryNode _s)
{
  SecondaryNode retSec = SecondaryNode(m_origin);
  std::vector<bool> jfound;
  for(int i =0 ; i<m_leafChildren.size(); ++i)
  {
    jfound.push_back(false);
  }
  for(const auto &i : _s.m_leafChildren)
  {
    bool found = false;
    for(int j =0; j<m_leafChildren.size(); ++j)
    {
      if(i->idx==m_leafChildren[j]->idx && i->idy==m_leafChildren[j]->idy && i->idz==m_leafChildren[j]->idz)
      {
        retSec.m_leafChildren.push_back(new LeafNode(*m_leafChildren[j]+*i));
        jfound[j]=true;
        found = true;
        break;
      }
    }
    if(!found)
    {
      retSec.m_leafChildren.push_back(new LeafNode(*i));
    }
  }

  for(int i =0 ; i<jfound.size(); ++i)
  {
    if(!jfound[i]) retSec.m_leafChildren.push_back(new LeafNode(*m_leafChildren[i]));
  }

  return retSec;
}
