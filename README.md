# VoxFox
A Voxel Data Structure for C++ using NGL and GLM. It is compatible with OpenGL.

Initial design based on SimpleNGL by Jon Macey. The two shader files are written by Richard Southern. Model objs are not my own.

[Click here for the Youtube video showing the demo](https://www.youtube.com/watch?v=HZUeZx_pb_I)

See my initial ideas here:
[https://drive.google.com/file/d/0B3Bu_KdSBGhibENXZksyVjFCeUk/view?usp=sharing](https://drive.google.com/file/d/0B3Bu_KdSBGhibENXZksyVjFCeUk/view?usp=sharing)

## Folders ##
VoxFoxLibrary contains the library. The library has a class called Examples.h which the demo uses to render examples.

The demo project is in the demo folder. Change the int at around line 150 to select an example to render.

Tests contains the various tests for each class.

## Requirements ##
QtCreator is required to make the project. Requires NGL (normally placed in Home folder) and GLM. C++11 is also used. Google Test is used for the tests.


## WHAT DOES IT DO ##

## Create a Tree ##

simply write:
```
VoxFoxTree myTree;
```
You can use copy constructor, assignment and pointers with it like so.
```
VoxFoxTree * myTree = new VoxFoxTree();
(*myTree) = otherTree;
(*myTree) = VoxFoxTree(otherTree);
```

## Import Objs ##
Can import textured objs as well as objs with no texture.

importObj(...) takes an ngl::Obj pointer, ngl::Image pointer and a float to give size.

importTexturedObj(..) takes a glm::vec3 for the color instead of texture.
```
ngl::Image * myTex = new ngl::Image("../images/myTexture.jpg");
ngl::Obj * myObj = new ngl::Obj("../models/myObj.obj");
VoxFoxTree myTree;
myTree.importTexturedObj(myObj,myTex,2.0f);
```
There are an optional two flags which specify whether to include normals and/not interpolate normals.
Default these are set to true. Get that shiny look.

## Set Operators and Translate ##

You can use set operators on your trees!
```
// Union
myTree |= otherTree;
myTree = otherTree | otherTree2;
// Intersection
myTree += otherTree;
myTree = otherTree + otherTree2;
// Subtraction
myTree -= otherTree;
myTree = otherTree - otherTree2;
// Intersection gives us back only the voxel data from the LHS that intersects.
// Translate
myTree.translate(glm::vec3(1.0,0.4f,0.0f);
```

## Shapes ##
At the moment I have three workable shapes.
```
myTree.createCylinder(
                        glm::vec3(0.0f,0.0f,0.0f), // position of centre
                        glm::vec3(1.0f,0.0f,0.0f), // direction of cylinder (only x y z)
                        1.0f, //radius
                        3.0f, //height
                        glm::vec3(1.0f,0.0f,0.0f) // color (this case is red)
);

myTree.createSphere(
                      glm::vec3(3.0f,0.0f,0.0f), // position of centre
                      2.0f, // radius
                      glm::vec3(1.0f,0.0f,0.0f) // color
);

myTree.createBox(
                    glm::vec3(-1.0f,-1.0f,-1.0f), // minimum coordinate of box
                    glm::vec3(1.0f,1.0f,1.0f), //maxmimum coordinate of box
                    glm::vec3(1.0f,0.0f,0.5f) // color
);
```

## Rendering ! ##
Call
```
calculatePolys();
```
This calculates the normals, texture coordinates, vertex coordinates and colors. To get them simply call:

  - getVertexes()
  - getNormals()
  - getTextureCoords()
  - getColors()

These give you a vector of floats corresponding to each coordinate. So for example the vertexes would have a step of three floats (x y z) and texture coords a step of two (u v).

You pass these into the shader of your choosing yourself. However there is a demo in demos/ that will do this for you.

## Demo ##
In demos/NoGUI Simply go to where the Examples::runExample(..) is located in src/NGLScene.cpp and do whatever voxel stuff there and set (*myRoot) to it.

Or better yet simply change the integer on the runExample(..) function to see the examples !

See VoxFoxLibrary/html/index.html for more information on all the functions.
