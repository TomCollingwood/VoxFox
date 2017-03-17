#include "include/PrimaryNode.h"

PrimaryNode::PrimaryNode()
{

}

PrimaryNode::~PrimaryNode()
{
  for(auto & s : m_secChildren)
  {
    delete s;
  }
  m_secChildren.clear();
}

PrimaryNode::PrimaryNode(glm::vec3 _origin) : m_origin(_origin)
{
  idx = (int)(_origin[0]/m_primUnit);
  idy = (int)(_origin[1]/m_primUnit);
  idz = (int)(_origin[2]/m_primUnit);
}

PrimaryNode::PrimaryNode(const PrimaryNode &p)
{
  for(auto &i : p.m_secChildren)
  {
    m_secChildren.push_back(new SecondaryNode(*i));
  }
  idx = p.idx;
  idy = p.idy;
  idz = p.idz;
  m_origin = p.getOrigin();
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

void PrimaryNode::addVoxel(glm::vec3 _position, Voxel _voxel, SecondaryNode ** o_secAccessor, LeafNode ** o_leafAccessor)
{
  bool found = false;
  for (auto &sec : m_secChildren) // access by reference to avoid copying
      {
        if(_position[0]<sec->getOrigin()[0] || _position[0]>=sec->getOrigin()[0]+unitChildLength) continue;
        if(_position[1]<sec->getOrigin()[1] || _position[1]>=sec->getOrigin()[1]+unitChildLength) continue;
        if(_position[2]<sec->getOrigin()[2] || _position[2]>=sec->getOrigin()[2]+unitChildLength) continue;
        sec->addVoxel(_position,_voxel,o_leafAccessor);
        (*o_secAccessor)=sec;
        found = true;
        break;
      }
  if(!found)
  {
    glm::vec3 newOrigin = floor(_position/unitChildLength)*unitChildLength;
    m_secChildren.push_back(new SecondaryNode(newOrigin));
    m_secChildren.back()->addVoxel(_position,_voxel,o_leafAccessor);
    (*o_secAccessor)=m_secChildren.back();
  }
}

bool PrimaryNode::isLeaf(glm::vec3 _position, SecondaryNode ** _sec, LeafNode ** _leaf)
{
  for (int i =0 ; i< m_secChildren.size(); ++i) // access by reference to avoid copying
  {
    if(_position[0]<m_secChildren[i]->getOrigin()[0] || _position[0]>=m_secChildren[i]->getOrigin()[0]+unitChildLength) continue;
    if(_position[1]<m_secChildren[i]->getOrigin()[1] || _position[1]>=m_secChildren[i]->getOrigin()[1]+unitChildLength) continue;
    if(_position[2]<m_secChildren[i]->getOrigin()[2] || _position[2]>=m_secChildren[i]->getOrigin()[2]+unitChildLength) continue;
    *_sec = m_secChildren[i];
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

void PrimaryNode::shiftOrigin(int _x, int _y, int _z)
{
  idx+=_x;
  idy+=_y;
  idz+=_z;
  m_origin+=glm::vec3(_x,_y,_z)*m_primUnit;
}

PrimaryNode PrimaryNode::operator +(PrimaryNode const &_p) const
{
  PrimaryNode retPrim = PrimaryNode(m_origin);
  std::vector<bool> jfound;
  for(int i =0 ; i<m_secChildren.size(); ++i)
  {
    jfound.push_back(false);
  }

  for(const auto & i : _p.m_secChildren)
  {
    bool found = false;
    for(int j =0; j<m_secChildren.size(); ++j)
    {
      if(i->idx==m_secChildren[j]->idx && i->idy==m_secChildren[j]->idy && i->idz==m_secChildren[j]->idz)
      {
        retPrim.m_secChildren.push_back(new SecondaryNode(*m_secChildren[j]+*i));
        jfound[j]=true;
        found = true;
        break;
      }
    }
    if(!found)
    {
      retPrim.m_secChildren.push_back(new SecondaryNode(*i));
    }
  }

  for(int i =0 ; i<jfound.size(); ++i)
  {
    if(!jfound[i]) retPrim.m_secChildren.push_back(new SecondaryNode(*m_secChildren[i]));
  }

  return retPrim;
}

