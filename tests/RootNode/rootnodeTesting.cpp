#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <RootNode.h>

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(VoxFoxRootNode,addVoxel1)
{
  RootNode test = RootNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  PrimaryNode * _testPrim = test.getChildren()->at(0);
  //std::cout<< _testPrim->getOrigin()[0] << "," << _testPrim->getOrigin()[1] << "," << _testPrim->getOrigin()[2] << " " << test.m_primUnit << "\n";

  EXPECT_TRUE(_testPrim->getOrigin()[0]<= 1.0 && _testPrim->getOrigin()[0]+test.m_primUnit>1.0 &&
      _testPrim->getOrigin()[1]<= 2.0 && _testPrim->getOrigin()[1]+test.m_primUnit>2.0 &&
      _testPrim->getOrigin()[2]<= 3.0 && _testPrim->getOrigin()[2]+test.m_primUnit>3.0);
}

TEST(VoxFoxRootNode,addVoxel2)
{
  RootNode test = RootNode();
  test.addVoxel(1.0,2.0,3.0);
  PrimaryNode * _testPrim = test.getChildren()->at(0);
  //std::cout<< _testPrim->getOrigin()[0] << "," << _testPrim->getOrigin()[1] << "," << _testPrim->getOrigin()[2] << " " << test.m_primUnit << "\n";

  EXPECT_TRUE(_testPrim->getOrigin()[0]<= 1.0 && _testPrim->getOrigin()[0]+test.m_primUnit>1.0 &&
      _testPrim->getOrigin()[1]<= 2.0 && _testPrim->getOrigin()[1]+test.m_primUnit>2.0 &&
      _testPrim->getOrigin()[2]<= 3.0 && _testPrim->getOrigin()[2]+test.m_primUnit>3.0);
}

TEST(VoxFoxRootNode,isVoxel)
{
  RootNode test = RootNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  EXPECT_TRUE(test.isVoxel(glm::vec3(1.0,2.0,3.0)));
}

TEST(VoxFoxRootNode,calculatePolys)
{
  RootNode test = RootNode();
  test.addVoxel(1.0,2.0,3.0);
  test.calculatePolys();
  std::vector<float> * testVerts = test.getVertexes();
//  int count2 = 0;
//  for(int count =0; count<testVerts->size(); ++count)
//  {
//    std::cout<<"testVerts->at("<<count<<")="<<testVerts->at(count)<<" && \n ";
//    ++count2;
//  }
  EXPECT_TRUE(testVerts->size()==108);
}
