#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <RootNode.h>

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(VoxFoxRootNode,addVoxel)
{
  RootNode test = RootNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  PrimaryNode * _testPrim = test.getChildren()->at(0);

  EXPECT_TRUE(_testPrim->getOrigin()[0]<= 1.0 && _testPrim->getOrigin()[0]+test.m_primUnit>1.0 &&
      _testPrim->getOrigin()[1]<= 2.0 && _testPrim->getOrigin()[1]+test.m_primUnit>2.0 &&
      _testPrim->getOrigin()[2]<= 3.0 && _testPrim->getOrigin()[2]+test.m_primUnit>3.0);
}

TEST(VoxFoxRootNode,addVoxel2)
{
  RootNode test = RootNode();
  test.addVoxel(glm::vec3(10.0,0.0,0.0),Voxel());
  test.addVoxel(glm::vec3(20.0,0.0,0.0),Voxel());
  test.addVoxel(glm::vec3(30.0,0.0,0.0),Voxel());

  EXPECT_TRUE(test.getChildren()->size()==3);
}

TEST(VoxFoxRootNode,isVoxel)
{
  RootNode test = RootNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  EXPECT_TRUE(test.isVoxel(glm::vec3(1.0,2.0,3.0)));
}

TEST(VoxFoxRootNode,isVoxel2)
{
  RootNode test = RootNode();
  EXPECT_FALSE(test.isVoxel(glm::vec3(0.0,2.0,1.0)));
}

TEST(VoxFoxRootNode,isPrimary)
{
  RootNode test = RootNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  EXPECT_TRUE(test.isPrimary(glm::vec3(1.0,2.0,3.0)));
}

TEST(VoxFoxRootNode,isSecondary)
{
  RootNode test = RootNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  EXPECT_TRUE(test.isSecondary(glm::vec3(1.0,2.0,3.0)));
}

TEST(VoxFoxRootNode,isLeaf)
{
  RootNode test = RootNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  EXPECT_TRUE(test.isLeaf(glm::vec3(1.0,2.0,3.0)));
}

TEST(VoxFoxRootNode,calculatePolys)
{
  RootNode test = RootNode();
  test.addVoxel(glm::vec3(1.0,2.0,3.0),Voxel());
  test.calculatePolys();
  std::vector<float> testVerts = test.getVertexes();
  EXPECT_TRUE(testVerts.size()==108);
}
