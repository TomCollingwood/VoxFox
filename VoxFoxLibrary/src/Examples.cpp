#include "Examples.h"

int Examples::runExample(int _whichExample, VoxFoxTree * _toRunOn, std::string _RelativePathToLibrary)
{
  ngl::Obj * dwarfmesh = new ngl::Obj(_RelativePathToLibrary+"/models/dwarf.obj");
  ngl::Obj * deermesh = new ngl::Obj(_RelativePathToLibrary+"/models/deer.obj");

  ngl::Image * dwarftex = new ngl::Image(_RelativePathToLibrary+"/images/dwarf.jpg");
  ngl::Image * deertex = new ngl::Image(_RelativePathToLibrary+"/images/deer.jpg");

  ngl::Image * foxtex= new ngl::Image(_RelativePathToLibrary+"/images/foxemoji.png");
  ngl::Image * titletex= new ngl::Image(_RelativePathToLibrary+"/images/title.png");
  ngl::Image * labelstex = new ngl::Image(_RelativePathToLibrary+"/images/labels.png");

  ngl::Image * easteregg = new ngl::Image(_RelativePathToLibrary+"/images/computers.png");

  // EXAMPLE 0: The Title ------------------------------------------------------------
  //    Number of Voxels:               56,346
  //    Number of Polygons              691,368
  // ---------------------------------------------------------------------------------
  if(_whichExample==0)
  {
    std::cout<<"// EXAMPLE 0: The Title ------------------------------------------------------------"<<std::endl;

    VoxFoxTree theTitleExample;
    glm::vec3 currentPos = glm::vec3(0.0f,1.0f,0.0f);
    theTitleExample.drawFlatImage(currentPos,titletex,3.5f);
    theTitleExample.drawFlatImage(currentPos+glm::vec3(0.0f,-1.0f,0.8f),foxtex,1.0f);
    // Zoom out for a scary surprise
    theTitleExample.drawFlatImage(currentPos+glm::vec3(0.0f,0.0f,5.8f),easteregg,6.0f);
    (*_toRunOn)=theTitleExample;
  }
  // EXAMPLE 1: Gnome Sphere Binary Operations----------------------------------------
  // Number of Voxels:               446,487
  // Number of Polygons              1,095,228
  // ---------------------------------------------------------------------------------
  else if(_whichExample==1)
  {
    std::cout<<"// EXAMPLE 1: Gnome Sphere Binary Operations----------------------------------------"<<std::endl;

    VoxFoxTree gnomeSphereExample;
    float labelsize = 10.0f;
    gnomeSphereExample.drawFlatImage(glm::vec3(0.0f,0.0f,0.0f),labelstex,labelsize);

    float depthh = 1.0f;
    VoxFoxTree dwarf, sphered, dwarfunion, dwarfminus, dwarfintersect;
    dwarf.importTexturedObj(dwarfmesh,dwarftex,2.0f);
    sphered.createSphere(glm::vec3(0.0f,1.5f,0.0f),0.7f,glm::vec3(1.0f,0.0f,0.0f));

    glm::vec3 dwarfstart = glm::vec3(-0.85f*labelsize/2.0f,0.3f*labelsize/2.0f,depthh);

    dwarfunion = sphered | dwarf;
    dwarfunion.translate(dwarfstart+glm::vec3(4.0f,0.0f,0.0f));
    dwarfminus = dwarf - sphered;
    dwarfminus.translate(dwarfstart+glm::vec3(6.0f,0.0f,0.0f));
    dwarfintersect = dwarf + sphered;
    dwarfintersect.translate(dwarfstart+glm::vec3(8.0f,0.0f,0.0f));

    dwarf.translate(dwarfstart);
    sphered.translate(dwarfstart+glm::vec3(2.0f,0.0f,0.0f));

    gnomeSphereExample |= dwarf | sphered | dwarfunion | dwarfminus | dwarfintersect ;
    gnomeSphereExample.translate(glm::vec3(4.5f,-3.0f,0.0f));
    (*_toRunOn)=gnomeSphereExample;
  }
  // EXAMPLE 2: The CSG TREE ---------------------------------------------------------
  // Number of Voxels:               186,601
  // Number of Polygons              333,384
  // ---------------------------------------------------------------------------------
  else if(_whichExample==2)
  {
    std::cout<<"// EXAMPLE 2: The CSG TREE ---------------------------------------------------------"<<std::endl;

    VoxFoxTree theCSGTree;// = title;
    float labelsize = 10.0f;
    float depthh = 1.0f;

    theCSGTree.drawFlatImage(glm::vec3(0.0f,0.0f,0.0f),labelstex,labelsize);

    float scale = 0.7f;
    glm::vec3 ourcol = glm::vec3(0.0f,0.7,1.0f);
    VoxFoxTree sphere, cube, cylinder1, cylinder2, cylinder3, CSGfinal, CSG2cylinder, CSG3cylinder, CSGcubesphere;

    cylinder1.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),0.2f*scale,1.1f*scale,ourcol);
    cylinder2.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),0.2f*scale,1.1f*scale,ourcol);
    cylinder3.createCylinder(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),0.2f*scale,1.1f*scale,ourcol);

    sphere.createSphere(glm::vec3(0.0f,0.0f,0.0f),0.6f*scale,ourcol);
    cube.createBox(glm::vec3(-0.5f,-0.5f,-0.5f)*scale,glm::vec3(0.5f,0.5,0.5f)*scale,ourcol);

    CSG2cylinder = cylinder3 | cylinder2;
    CSG3cylinder = cylinder1 | cylinder2 | cylinder3;
    CSGcubesphere = cube + sphere;

    CSGfinal = ((cube + sphere) - (cylinder1 | cylinder2 | cylinder3)) ;

    cube.translate(glm::vec3(((21.0f-256.0f)/512.0f)*labelsize,((256.0f-411.0f)/512.0f)*labelsize,depthh));
    sphere.translate(glm::vec3(((112.0f-256.0f)/512.0f)*labelsize,((256.0f-411.0f)/512.0f)*labelsize,depthh));
    cylinder1.translate(glm::vec3(((153.0f-256.0f)/512.0f)*labelsize,((256.0f-411.0f)/512.0f)*labelsize,depthh));
    cylinder2.translate(glm::vec3(((292.0f-256.0f)/512.0f)*labelsize,((256.0f-507.0f)/512.0f)*labelsize,depthh));
    cylinder3.translate(glm::vec3(((196.0f-256.0f)/512.0f)*labelsize,((256.0f-507.0f)/512.0f)*labelsize,depthh));
    CSG2cylinder.translate(glm::vec3(((230.0f-256.0f)/512.0f)*labelsize,((256.0f-411.0f)/512.0f)*labelsize,depthh));
    CSG3cylinder.translate(glm::vec3(((181.0f-256.0f)/512.0f)*labelsize,((256.0f-317.0f)/512.0f)*labelsize,depthh));
    CSGcubesphere.translate(glm::vec3(((66.0f-256.0f)/512.0f)*labelsize,((256.0f-317.0f)/512.0f)*labelsize,depthh));
    CSGfinal.translate(glm::vec3(((123.0f-256.0f)/512.0f)*labelsize,((256.0f-234.0f)/512.0f)*labelsize,depthh));

    theCSGTree |= cube | sphere | cylinder1 | cylinder2 | cylinder3 | CSG2cylinder | CSG3cylinder | CSGcubesphere | CSGfinal;
    theCSGTree.translate(glm::vec3(2.5f,2.0f,0.0f));
    (*_toRunOn)=theCSGTree;
  }
  // EXAMPLE 3: The Cascade ---------------------------------------------------------
  // Number of Voxels:               236,164
  // Number of Polygons              2,973,744
  // ---------------------------------------------------------------------------------
  else if(_whichExample==3)
  {
    std::cout<<"// EXAMPLE 3: The Cascade ---------------------------------------------------------"<<std::endl;

    VoxFoxTree theCascade;

    // These guys change how they're shaded each iteration.

    VoxFoxTree deercascade;
    for(int i =0; i<19; ++i)
    {
      bool normals = i%3;
      bool interpo = i%4;
      deercascade.importTexturedObj(deermesh,deertex,2.0f-(i*0.1f),normals,!interpo);
      deercascade.translate(glm::vec3((2.0f-(i*0.1f))*1.0f,0.0f,0.0f));
    }

    VoxFoxTree gnomecascade;
    for(int i =1; i<20; ++i)
    {
      bool normals = i%3;
      bool interpo = i%4;
      gnomecascade.importTexturedObj(dwarfmesh,dwarftex,2.0f-(i*0.1f),normals,!interpo);
      gnomecascade.translate(glm::vec3(-(2.0f-(i*0.1f))*0.7,0.0f,0.0f));
    }

    theCascade = gnomecascade | deercascade;
    (*_toRunOn)=theCascade;
  }

  // EXAMPLE 4: The Horned Deer Gnome ------------------------------------------------
  // Number of Voxels:               110,384
  // Number of Polygons              1,334,952
  // ---------------------------------------------------------------------------------
  else if(_whichExample==4)
  {
    std::cout<<"// EXAMPLE 4: The Horned Deer Gnome ------------------------------------------------"<<std::endl;

    VoxFoxTree theHornedDeerGnome;

    float gnomedeerscale =1.0f;
    for(int i=1; i<20; ++i)
    {
      bool normals = i%3;
      bool interpo = i%4;
      gnomedeerscale=i*0.05f;
      VoxFoxTree gnomehead, sphere1, deer;
      gnomehead.importTexturedObj(dwarfmesh,dwarftex,1.6f*gnomedeerscale,normals,!interpo);
      sphere1.createSphere(glm::vec3(0.0f,1.55f,0.0f)*gnomedeerscale,0.7f*gnomedeerscale,glm::vec3(1.0f,0.0f,0.0f));
      gnomehead += sphere1;
      deer.importTexturedObj(deermesh,deertex,2.0f*gnomedeerscale,normals,!interpo);
      gnomehead.translate(glm::vec3(-0.65f,0.4f,0.2f)*gnomedeerscale);
      deer |= gnomehead; // imagine placing a gnome head on a deer
      VoxFoxTree horns;
      horns.createCylinder(glm::vec3(-0.75f,2.0f,0.3f)*gnomedeerscale,glm::vec3(0.0f,1.0f,0.0f),0.05f*gnomedeerscale,0.8f*gnomedeerscale,glm::vec3(1.0f,0.0f,0.0f));
      horns.createCylinder(glm::vec3(-0.42f,2.0f,0.3f)*gnomedeerscale,glm::vec3(0.0f,1.0f,0.0f),0.05f*gnomedeerscale,0.8f*gnomedeerscale,glm::vec3(1.0f,0.0f,0.0f));
      theHornedDeerGnome |= deer | horns;
      theHornedDeerGnome.translate(glm::vec3(2.0f*gnomedeerscale*1.1f,0.0f,0.0f));
    }
    (*_toRunOn)=theHornedDeerGnome;
  }

  // EXAMPLE 5: The Gnome ------------------------------------------------------------
  // Number of Voxels:               23,143
  // Number of Polygons              298,152
  // ---------------------------------------------------------------------------------
  else if(_whichExample==5)
  {
    std::cout<<"// EXAMPLE 5: The Gnome ------------------------------------------------------------"<<std::endl;

    VoxFoxTree theGnome;
    theGnome.importTexturedObj(dwarfmesh,dwarftex,2.0f);
    (*_toRunOn)=theGnome;

  }
  // EXAMPLE 6: The RGB Gnomes ------------------------------------------------------
  // Number of Voxels:               43,502
  // Number of Polygons              555,252
  // ---------------------------------------------------------------------------------
  else if(_whichExample==6)
  {
    std::cout<<"// EXAMPLE 6: The RGB Gnomes ------------------------------------------------------"<<std::endl;

    VoxFoxTree theRGBGnomes;
    theRGBGnomes.importObj(dwarfmesh,glm::vec3(1.0f,0.3f,0.0f),1.5f);
    theRGBGnomes.translate(glm::vec3(1.0f,0.0f,0.0f));
    theRGBGnomes.importObj(dwarfmesh,glm::vec3(0.3f,1.0f,0.0f),2.0f);
    theRGBGnomes.translate(glm::vec3(0.85f,0.0f,0.0f));
    theRGBGnomes.importObj(dwarfmesh,glm::vec3(0.0f,0.3f,1.0f),1.1f);
    theRGBGnomes.translate(glm::vec3(-0.85f,0.0f,0.0f));
    (*_toRunOn)=theRGBGnomes;

  }
  // EXAMPLE 7: Idiotically Massive Gnome --------------------------------------------
  // Number of Voxels:               562,591
  // Number of Polygons              7,464,780
  // ---------------------------------------------------------------------------------

  else if(_whichExample==7)
  {
    std::cout<<"// EXAMPLE 7: Idiotically Massive Gnome --------------------------------------------"<<std::endl;

    VoxFoxTree theGnome;
    theGnome.importTexturedObj(dwarfmesh,dwarftex,10.0f);
    (*_toRunOn)=theGnome;

  }
  // EXAMPLE 8: Idiotically Massive Deer --------------------------------------------
  // Number of Voxels:               305,035
  // Number of Polygons              4,006,668
  // ---------------------------------------------------------------------------------
  else if(_whichExample==8)
  {
    std::cout<<"// EXAMPLE 8: Idiotically Massive Deer --------------------------------------------"<<std::endl;
    VoxFoxTree theDeer;
    theDeer.importTexturedObj(deermesh,deertex,10.0f);
    (*_toRunOn)=theDeer;
  }

  // EXAMPLE 9: Three Teapots -------------------------------------------------------
  // Number of Voxels:               120,712
  // Number of Polygons              1,595,316
  // ---------------------------------------------------------------------------------
  else if(_whichExample==9)
  {
    std::cout<<"// EXAMPLE 9: Three Teapots -------------------------------------------------------"<<std::endl;

    ngl::Obj * teapot = new ngl::Obj(_RelativePathToLibrary+"/models/teapot.obj");
    (*_toRunOn).importObj(teapot,glm::vec3(1.0f,0.3f,0.0f),0.3f);
    (*_toRunOn).translate(glm::vec3(1.0f,0.0f,0.0f));
    (*_toRunOn).importObj(teapot,glm::vec3(0.3f,1.0f,0.0f),0.6f);
    (*_toRunOn).translate(glm::vec3(2.2f,0.0f,0.0f));
    (*_toRunOn).importObj(teapot,glm::vec3(0.0f,0.3f,1.0f),2.5f);
  }
}
