#ifndef PRIMARYNODE_H
#define PRIMARYNODE_H

#include <iostream>
#include <vector>

#include "include/LeafNode.h"
#include "include/RootNode.h"
#include "include/SecondaryNode.h"

class PrimaryNode
{
public:
  PrimaryNode();
private:
  RootNode * m_parent;
  std::vector<SecondaryNode> m_secChildren;

};

#endif // PRIMARYNODE_H