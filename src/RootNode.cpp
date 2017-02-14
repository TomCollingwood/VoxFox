#include "include/RootNode.h"

RootNode::RootNode()
{

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
}
