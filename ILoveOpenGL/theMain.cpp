
#include "GLCommon.h"
//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include "globalThings.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>      // "string stream" just like iostream and fstream
#include <vector>       // "smart array"
#include <fstream>      // C++ file I-O library (look like any other stream)
#include "cAnimationControl.h"


// Are in globalThings.h
//#include "cMesh.h"
//#include "cVAOManager.h"
//#include "cShaderManager.h"
//
//#include "cLightManager.h"
//#include "cLightHelper.h"

// Function signature for DrawObject()
void DrawObject(
    cMesh* pCurrentMesh, 
    glm::mat4 matModel,
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager);


void DrawDebugObjects(
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager);

//// Global things are here:
//glm::vec3 cameraEye = glm::vec3(0.0, 0.0, +4.0f);
//
//cVAOManager*    g_pVAOManager;
//cShaderManager*  g_pShaderManager;
//
//cLightManager gTheLights;
//cLightHelper gTheLightHelper;
//
//// List of objects to draw
//std::vector< cMesh* > g_vec_pMeshes;
////std::vector<cMesh> g_vecMeshes;
//
//cMesh* g_pDebugSphere = NULL;
//bool g_bShowDebugShere = true;
//
//
//unsigned int g_selectedObject = 0;
//unsigned int g_selectedLight = 0;
//
//
//// This will be printed in the title bar
//std::string g_TitleText = "";

float distanceToCamera(glm::vec3 pos)
{
    return glm::distance(::g_pFlyCamera->getEye(), pos);
}



void LoadTheAnimationCommands(cAnimationControl* pAnimationControl);

// This is used for the discard example on November 23rd
float g_blackHoleSize_for_discard_example = 0.0f;



GLint mvp_location = -1;
GLint matModel_Location = -1;
GLint matView_Location = -1;
GLint matProjection_Location = -1;
GLint matModelInverseTranspose_Location = -1;



void DrawEverything(float ratio, GLuint program, cMesh* pSkyBox, float objectNumber = 0.f);

int main(void)
{
    GLFWwindow* pWindow;

//    GLuint vertex_buffer = 0;     // ** NOW in VAO Manager **

//    GLuint vertex_shader;     // Now in the "Shader Manager"
//    GLuint fragment_shader;   // Now in the "Shader Manager"
    GLuint program = 0;     // 0 means "no shader program"

            // Because glGetAttribLocation() returns -1 on error
//    GLint vpos_location = -1;       // Because glGetAttribLocation() returns -1 on error
//    GLint vcol_location = -1;       // Because glGetAttribLocation() returns -1 on error

    glfwSetErrorCallback(GLFW_error_callback);

    if ( ! glfwInit() )
    {
        return -1;
        //exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    pWindow = glfwCreateWindow(1200, 640, "OpenGL is great!", NULL, NULL);

    if (!pWindow )
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(pWindow, GLFW_key_callback);
    // NEW: 
    glfwSetCursorEnterCallback( pWindow, GLFW_cursor_enter_callback );
    glfwSetCursorPosCallback( pWindow, GLFW_cursor_position_callback );
    glfwSetScrollCallback( pWindow, GLFW_scroll_callback );
    glfwSetMouseButtonCallback( pWindow, GLFW_mouse_button_callback );
    glfwSetWindowSizeCallback( pWindow, GLFW_window_size_callback );

    glfwMakeContextCurrent(pWindow);
    gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);


    // void glGetIntegerv(GLenum pname, GLint * data);
    // See how many active uniform variables (registers) I can have
    GLint max_uniform_location = 0;
    GLint* p_max_uniform_location = NULL;
    p_max_uniform_location = &max_uniform_location;
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

    // You'll actually see something like this:
//    GLint max_uniform_location = 0;
//    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max_uniform_location);

    std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max_uniform_location << std::endl;

    // Create global things
    ::g_StartUp(pWindow);

    ::g_pFlyCamera->setEye( glm::vec3(0.0f, 122.0f, 225.0f) );
//    ::g_pFlyCamera->setEye( glm::vec3(0.0f, 5000.0f, -3000.0f) );


    cShaderManager::cShader vertShader;
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";
        
    cShaderManager::cShader fragShader;
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        program = ::g_pShaderManager->getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
    }


    // Select the shader program we want to use
    // (Note we only have one shader program at this point)
    glUseProgram(program);


    // ************************************************
    // Now, I'm going to "load up" all the uniform locations
    // (This was to show how a map could be used)
    cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

    int theUniformIDLoc = -1;
    theUniformIDLoc = glGetUniformLocation(program, "matModel");
    pShaderProc->mapUniformName_to_UniformLocation["matModel"] = theUniformIDLoc;
    
    // Or...
    pShaderProc->mapUniformName_to_UniformLocation["matModel"] = glGetUniformLocation(program, "matModel");
    pShaderProc->mapUniformName_to_UniformLocation["matView"] = glGetUniformLocation(program, "matView");
    pShaderProc->mapUniformName_to_UniformLocation["matProjection"] = glGetUniformLocation(program, "matProjection");
    pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"] = glGetUniformLocation(program, "matModelInverseTranspose");

    pShaderProc->mapUniformName_to_UniformLocation["wholeObjectSpecularColour"] = glGetUniformLocation(program, "wholeObjectSpecularColour");
    // .. and so on...
    pShaderProc->mapUniformName_to_UniformLocation["theLights[0].position"] = glGetUniformLocation(program, "wholeObjectSpecularColour");
    // ... and so on..
    // ************************************************

    //GLint mvp_location = -1;
    mvp_location = glGetUniformLocation(program, "MVP");

    // Get "uniform locations" (aka the registers these are in)
    matModel_Location = glGetUniformLocation(program, "matModel");
//  t matView_Location = glGetUniformLocation(program, "matView");
    matProjection_Location = glGetUniformLocation(program, "matProjection");
    matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");


    cAnimationControl* animationController = new cAnimationControl();

//    ::g_pTheLights->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//    //... and so on...
////    	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
////	                // 0 = pointlight
////					// 1 = spot light
////					// 2 = directional light
////    ::g_pTheLights->theLights[0].param1.x = 1.0f;    // Spot light
//    ::g_pTheLights->theLights[0].param1.x = 0.0f;    // point light
//    // Direction RELATIVE TO THE LIGHT
//    // (-1 in y is straight down)
    ::g_pTheLights->theLights[0].position = glm::vec4(100.0f, 100.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].atten.y = 0.0001f;
    ::g_pTheLights->theLights[0].atten.z = 0.00001f;
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//    
    ::g_pTheLights->theLights[0].param1.x = 2.0f;    // directional
    ::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(0.9922f, 0.9843f, 0.9275f, 1.0f);
    ::g_pTheLights->theLights[0].param2.x = 1.f;
    //::g_pTheLights->theLights[0].diffuse *= 0.0001f;
    //::g_pTheLights->TurnOnLight(0);  // Or this!
    //gTheLights.theLights[0].param2.x = 1.0f;
    //::g_pTheLights->TurnOnLight(0);  // Or this!

    // Sunlight: https://encycolorpedia.com/fdfbd3#:~:text=The%20hexadecimal%20color%20code%20%23fdfbd3,%25%20saturation%20and%2091%25%20lightness.
    //The hexadecimal color code #fdfbd3 is a very light shade of yellow. In the RGB color model #fdfbd3 is comprised of 99.22% red, 98.43% green and 82.75% blue. 


    // Get the uniform locations of the light shader values
    ::g_pTheLights->SetUpUniformLocations(program);

    
    // Set up the debug sphere object
    ::g_pDebugSphere = new cMesh();
    ::g_pDebugSphere->meshName = "Sphere_xyz_n_rgba.ply";
    ::g_pDebugSphere->bIsWireframe = true;
    ::g_pDebugSphere->bUseObjectDebugColour = true;
    ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pDebugSphere->bDontLight = true;
    ::g_pDebugSphere->scale = 1.0f;
    ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
    // Give this a friendly name
    ::g_pDebugSphere->friendlyName = "Debug Sphere";
    // Then I could make a small function that searches for that name


    


    ::g_pVAOManager->setFilePath("assets/models/");

    std::vector<std::string> vecModelsToLoad;
    //vecModelsToLoad.push_back("Sphere_xyz_n_rgba.ply");
    vecModelsToLoad.push_back("vurdalak.ply");
    vecModelsToLoad.push_back("Table.ply");
    vecModelsToLoad.push_back("die.ply");
    vecModelsToLoad.push_back("glass.ply");
    vecModelsToLoad.push_back("Warrior.ply");
    //vecModelsToLoad.push_back("Lamps.ply");
    vecModelsToLoad.push_back("carpet.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_4div_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("Sphere_xyz_n_rgba.ply");
    vecModelsToLoad.push_back("Cylinder.ply");
    vecModelsToLoad.push_back("Quad_1_sided_aligned_on_XY_plane.ply");

    vecModelsToLoad.push_back("Isosphere_Smooth_Normals.ply");
    vecModelsToLoad.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");




    unsigned int totalVerticesLoaded = 0;
    unsigned int totalTrianglesLoaded = 0;
    for (std::vector<std::string>::iterator itModel = vecModelsToLoad.begin(); itModel != vecModelsToLoad.end(); itModel++)
    {
        sModelDrawInfo theModel;
        std::string modelName = *itModel;
        std::cout << "Loading " << modelName << "...";
        if ( !::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program) )
        {
            std::cout << "didn't work because: " << std::endl;
            std::cout << ::g_pVAOManager->getLastError(true) << std::endl;
        }
        else
        {
            std::cout << "OK." << std::endl;
            std::cout << "\t" << theModel.numberOfVertices << " vertices and " << theModel.numberOfTriangles << " triangles loaded." << std::endl;
            totalTrianglesLoaded += theModel.numberOfTriangles;
            totalVerticesLoaded += theModel.numberOfVertices;
        }

//        ::g_pVAOManager->LoadPLYModelFromFile(modelName, theModel);
//        ::g_pVAOManager->GenerateSphericalTextureCoords(theModel, false, 1.0f, false);
//        ::g_pVAOManager->LoadModelIntoVAO(theModel, program);

    }//for (std::vector<std::string>::iterator itModel


    std::cout << "Done loading models." << std::endl;
    std::cout << "Total vertices loaded = " << totalVerticesLoaded << std::endl;
    std::cout << "Total triangles loaded = " << totalTrianglesLoaded << std::endl;


    cMesh* pDie = new cMesh();
    pDie->meshName = "die.ply";
    pDie->positionXYZ = glm::vec3(50.f, 85.f, 320.f);
    pDie->textureNames[0] = "plastic.bmp";
    pDie->textureRatios[0] = 0.3f;
    pDie->textureNames[1] = "purple.bmp";
    pDie->textureRatios[1] = 0.7f;
    pDie->scale = 0.1f;

    pDie->alphaTransparency = 0.7f;

    ::g_vec_pMeshes.push_back(pDie);

    cMesh* pGlass = new cMesh();
    pGlass->meshName = "glass.ply";
    pGlass->positionXYZ = glm::vec3(25.f, 82.f, 360.f);
    pGlass->textureNames[0] = "glass.bmp";
    pGlass->textureRatios[0] = 1.f;
    pGlass->textureNames[2] = "noise.bmp";
    pGlass->textureRatios[2] = 0.f;
    pGlass->scale = 0.1f;

    pGlass->alphaTransparency = 0.4f;

    ::g_vec_pMeshes.push_back(pGlass);

    cMesh* pCyl = new cMesh();
    pCyl->meshName = "Cylinder.ply";
    pCyl->positionXYZ = glm::vec3(25.f, 90.f, 360.f);
    pCyl->bDontLight = true;
    pCyl->textureNames[0] = "water-ripple-texture.bmp";
    pCyl->textureRatios[0] = 0.5f;
    pCyl->textureNames[1] = "water-ripple-texture.bmp";
    pCyl->textureRatios[1] = 0.5f;
    pCyl->scale = 0.3f;
    pCyl->friendlyName = "water";
    pCyl->alphaTransparency = 0.4f;
    ::g_vec_pMeshes.push_back(pCyl);


    cMesh* pTable = new cMesh();
    pTable->meshName = "Table.ply";
    pTable->positionXYZ = glm::vec3(0.f, 0.f, 350.f);
    pTable->textureNames[0] = "Table_BaseColor.bmp";
    pTable->textureRatios[0] = 1.f;


    ::g_vec_pMeshes.push_back(pTable);

    cMesh* pMonster = new cMesh();
    pMonster->meshName = "vurdalak.ply";
    pMonster->positionXYZ = glm::vec3(0.f, 83.f, 320.f);
    pMonster->wholeObjectDiffuseRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
    pMonster->bUseWholeObjectDiffuseColour = true;
    pMonster->friendlyName = "Vurdalak";


    ::g_vec_pMeshes.push_back(pMonster);

    cMesh* pHero = new cMesh();
    pHero->meshName = "Warrior.ply";
    pHero->positionXYZ = glm::vec3(0.f, 82.f, 340.f);
    pHero->textureNames[0] = "glass.bmp";
    pHero->textureRatios[0] = 0.3f;
    pHero->textureNames[1] = "blood.bmp";
    pHero->textureRatios[1] = 0.7f;
    pHero->scale = 0.3f;
    pHero->orientationXYZ.x = glm::radians(-90.f);
    pHero->orientationXYZ.y = glm::radians(-90.f);
    pHero->friendlyName = "Hero";

    ::g_vec_pMeshes.push_back(pHero);

    cMesh* pQuad = new cMesh();
    pQuad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
    pQuad->positionXYZ = glm::vec3(0.f, 122.f, 340.f);
    pQuad->bDontLight = true;
    pQuad->scale = 40.f;
    pQuad->friendlyName = "FBO";
    pQuad->isFbo = true;

    ::g_vec_pMeshes.push_back(pQuad);

    //cMesh* pLamps = new cMesh();
    //pLamps->meshName = "Lamps.ply";
    //pLamps->positionXYZ = glm::vec3(-75.f, 82.f, 340.f);
    //pLamps->orientationXYZ.y = glm::radians(90.f);
    //pLamps->textureNames[0] = "plastic.bmp";
    //pLamps->textureRatios[0] = 1.f;
    //pLamps->scale = 1.f;


    //::g_vec_pMeshes.push_back(pLamps);

    //cMesh* pCover = new cMesh();
    //pCover->meshName = "carpet.ply";
    //pCover->positionXYZ = glm::vec3(0.f, 84.f, 340.f);
    //pCover->orientationXYZ.y = glm::radians(90.f);
    //pCover->textureNames[0] = "rag.bmp";
    //pCover->textureRatios[0] = 1.f;
    ////pCover->textureNames[3] = "noise.bmp";
    ////pCover->textureRatios[3] = 0.f;
    //pCover->friendlyID = 42069;
    //pCover->scale = 0.2f;


    //::g_vec_pMeshes.push_back(pCover);

    
    


    GLint glMaxCombinedTextureImageUnits = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &glMaxCombinedTextureImageUnits);
    std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = " << glMaxCombinedTextureImageUnits << std::endl;

    GLint glMaxVertexTextureImageUnits = 0;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &glMaxVertexTextureImageUnits);
    std::cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << glMaxVertexTextureImageUnits << std::endl;

    GLint glMaxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTextureSize);
    std::cout << "GL_MAX_TEXTURE_SIZE = " << glMaxTextureSize << std::endl;
   
    
    // Load the textures
    ::g_pTextureManager->SetBasePath("assets/textures");


//    ::g_pTextureManager->Create2DTextureFromBMPFile("2k_jupiter.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Table_BaseColor.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("vurdalak_Base_Color.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("vurdalak_Roughness.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("glass.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("blood.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("plastic.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("rag.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("discard.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("noise.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("frosted.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("purple.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("water-ripple-texture.bmp", true);

    // Add a skybox texture
    std::string errorTextString;

    ::g_pTextureManager->SetBasePath("assets/textures/cubemaps");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Space01",
        "SpaceBox_right1_posX.bmp",     /* posX_fileName */
        "SpaceBox_left2_negX.bmp",      /*negX_fileName */
        "SpaceBox_top3_posY.bmp",       /*posY_fileName*/
        "SpaceBox_bottom4_negY.bmp",    /*negY_fileName*/
        "SpaceBox_front5_posZ.bmp",     /*posZ_fileName*/
        "SpaceBox_back6_negZ.bmp",      /*negZ_fileName*/
        true, errorTextString))
    {
        std::cout << "Didn't load because: " << errorTextString << std::endl;
    }
    else
    {
        std::cout << "Loaded the sunny day cube texture OK" << std::endl;
    }    
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Indoors01",
        "posx.bmp",     /* posX_fileName */
        "negx.bmp",      /*negX_fileName */
        "negy.bmp",       /*posY_fileName*/
        "posy.bmp",    /*negY_fileName*/
        "posz.bmp",     /*posZ_fileName*/
        "negz.bmp",      /*negZ_fileName*/
        true, errorTextString))
    {
        std::cout << "Didn't load because: " << errorTextString << std::endl;
    }
    else
    {
        std::cout << "Loaded the Inddors cube texture OK" << std::endl;
    }    
    std::cout << "I've loaded these textures:" << std::endl;
    std::vector<std::string> vecTexturesLoaded;
    ::g_pTextureManager->GetLoadedTextureList(vecTexturesLoaded);

    for (std::vector<std::string>::iterator itTexName = vecTexturesLoaded.begin();
        itTexName != vecTexturesLoaded.end(); itTexName++)
    {
        std::cout << "\t" << *itTexName << std::endl;
    }


    // Create a skybox object (a sphere with inverted normals that moves with the camera eye)
    cMesh* pSkyBox = new cMesh();
    //pSkyBox->meshName = "Isosphere_Smooth_Normals.ply";
    //
    // We are using a sphere with INWARD facing normals. 
    // This is so we see the "back" of the sphere.
    // 
    pSkyBox->meshName = "Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply";
    //
    // 2 main ways we can do a skybox:
    //
    // - Make a sphere really big, so everything fits inside
    //   (be careful of the far clipping plane)
    // 
    // - Typical way is:
    //   - Turn off the depth test
    //   - Turn off the depth function (i.e. writing to the depth buffer)
    //   - Draw the skybox object (which can be really small, since it's not interacting with the depth buffer)
    //   - Once drawn:
    //      - turn on the depth function
    //      - turn on the depth test
    // 
//    pSkyBox->scale = 5'000'000.0f;
    //    
    // We are now turning off the depth check, so this can be quite small, 
    // just big enough to be beyond the near clipping plane.
    // (here I'm making it 10x the size of the near plane)
    pSkyBox->scale = ::g_pFlyCamera->nearPlane * 1000.0f;
    // 
    pSkyBox->positionXYZ = ::g_pFlyCamera->getEye();


//    ::g_vec_pMeshes.push_back(pDebugSphere);     // Debug sphere is #5

    const double MAX_DELTA_TIME = 0.1;  // 100 ms
    double previousTime = glfwGetTime();


    float offX = 0.f, offY = 0.f;

    // Create the FBO (Frame Buffer Object)
   // The texture we can render to
    ::g_pFBO = new cFBO();
    // Set this off screen texture buffer to some random size
    std::string FBOerrorString;
    //    if (::g_pFBO->init(1024, 1024, FBOerrorString))
    //    if (::g_pFBO->init( 8 * 1024, 8 * 1024, FBOerrorString))
    if (::g_pFBO->init(960, 960, FBOerrorString))
    {
        std::cout << "FBO is all set!" << std::endl;
    }
    else
    {
        std::cout << "FBO Error: " << FBOerrorString << std::endl;
    }

    float ratio;
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height);
    ratio = width / (float)height;
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //const GLint RENDER_PASS_0_ENTIRE_SCENE = 0;
    //const GLint RENDER_PASS_1_QUAD_ONLY = 1;   
    GLint renderPassNumber_LocID = glGetUniformLocation(program, "renderPassNumber");


    //Do an FBO
    GLuint FBO_ID = ::g_pFBO->ID;
    //glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBO->ID);

    GLuint TextureNumber = ::g_pFBO->colourTexture_0_ID;
    GLuint FBO_TextureId_01 = 12;	    // I picked 5 just because
    glActiveTexture(FBO_TextureId_01 + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureNumber);

    GLint texture_00_LocID = glGetUniformLocation(program, "fboTextureUnit1");
    glUniform1i(texture_00_LocID, FBO_TextureId_01);

    //LoadTheAnimationCommands(animationController);
    while (!glfwWindowShouldClose(pWindow))
    {
        float ratio;
        float FBORatio;
        int width, height;
        //glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
        //glm::mat4 matProjection;        // used to be "p";
        //glm::mat4 matView;              // used to be "v";

        double currentTime = glfwGetTime();
//        double deltaTime = previousTime - currentTime;        OOOPSS!
        double deltaTime = currentTime - previousTime;
        deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
        previousTime = currentTime;

        ::g_pFlyCamera->Update(deltaTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //GLint renderinfboID = glGetUniformLocation(program, "renderingInFBO");
        FBORatio = ::g_pFBO->width / (float)::g_pFBO->height;
        //// Set the viewport to the size of my offscreen texture (FBO)
        glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);

        glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBO->ID);
        // 

        //glUniform1f(renderinfboID, (float)GL_TRUE);
         DrawEverything(FBORatio, program, pSkyBox);
        //glUniform1f(renderinfboID, (float)GL_FALSE);
       
        //glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBO->ID);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //// Set the viewport to the size of my offscreen texture (FBO)
        //glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);
        //ratio = ::g_pFBO->width / (float)::g_pFBO->height; 
        // 
        // Turn on the depth buffer
        glEnable(GL_DEPTH);         // Turns on the depth buffer
        glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer


        glfwGetFramebufferSize(pWindow, &width, &height);
        ratio = width / (float)height;       

        glViewport(0, 0, width, height);
        

        // *******************************************************
        // Screen is cleared and we are ready to draw the scene...
        // *******************************************************

        // Update the title text
        glfwSetWindowTitle(pWindow, ::g_TitleText.c_str() );

        // Copy the light information into the shader to draw the scene
        ::g_pTheLights->CopyLightInfoToShader();     
       
        DrawEverything(ratio, program, pSkyBox);




        // "Present" what we've drawn.
        glfwSwapBuffers(pWindow);        // Show what we've drawn

        // Process any events that have happened
        glfwPollEvents();
        ::g_pFBO->clearBuffers(true, true);

        // Handle OUR keyboard, mouse stuff
        handleAsyncKeyboard(pWindow, deltaTime);
        handleAsyncMouse(pWindow, deltaTime);

    }//while (!glfwWindowShouldClose(window))

    // All done, so delete things...
    ::g_ShutDown(pWindow);

    delete animationController;

    glfwDestroyWindow(pWindow);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void DrawEverything(float ratio, GLuint program, cMesh* pSkyBox, float objectNumber)
{
    // Place the "debug sphere" at the same location as the selected light (again)
    ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
    // HACK: Debug sphere is 5th item added
//        ::g_vecMeshes[5].positionXYZ = gTheLights.theLights[0].position;

    glm::mat4 matProjection = glm::perspective(
        ::g_pFlyCamera->FOV,
        ratio,
        ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
        ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

    //matProjection = glm::perspective(
    //    0.6f,       // in degrees
    //    ratio,
    //    10.0f,       // Near plane (as large as possible)
    //    1'000'000.0f);   // Far plane (as small as possible)


    glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
    glm::vec3 cameraAt = ::g_pFlyCamera->getAtInWorldSpace();
    glm::vec3 cameraUp = ::g_pFlyCamera->getUpVector();


    glm::mat4 matView = glm::mat4(1.0f);
    matView = glm::lookAt(cameraEye,   // "eye"
        cameraAt,    // "at"
        cameraUp);

    //        // HACK: Black hole discard example
    //        ::g_blackHoleSize_for_discard_example += 1.0f;
    //        GLint blackHoleSize_LocID = glGetUniformLocation(program, "blackHoleSize");
    //        glUniform1f(blackHoleSize_LocID, ::g_blackHoleSize_for_discard_example);
    //        std::cout << ::g_blackHoleSize_for_discard_example << std::endl;


    //        glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(matView));

    cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

    glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
        1, GL_FALSE, glm::value_ptr(matView));


    glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));



    //        // Point the spotlight at the car
    //        glm::vec3 vecToCar = pTheCar->positionXYZ - glm::vec3(::g_pTheLights->theLights[0].position);
    //
    //        vecToCar = glm::normalize (vecToCar);
    //
    //        ::g_pTheLights->theLights[0].direction = glm::vec4 (vecToCar, 1.0f);


            // *********************************************************************
            //    ____  _            ____             
            //   / ___|| | ___   _  | __ )  _____  __ 
            //   \___ \| |/ / | | | |  _ \ / _ \ \/ / 
            //    ___) |   <| |_| | | |_) | (_) >  <  
            //   |____/|_|\_\\__, | |____/ \___/_/\_\ 
            //               |___/                    
            //
            // Since this is a space game (most of the screen is filled with "sky"), 
            //  I'll draw the skybox first
            // 
            // This is updated to the "proper" way to do a skybox. 
            // Here's how:
            // 1. Turn off the depth function AND depth test
            // 2. Draw the skybox - it doesn't matter how big it is, since it's the 1st thing being drawn.
            //    Since I'm NOT writing to the depth buffer, it's written to the colour buffer 
            //    (i.e. we can see it), but anything drawn later doesn't "know" that's happened, 
            //    so all the other objects draw "on top" of it (on the colour buffer)
            // 
    GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
    glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);
    GLint bUseHeightMap_LodID = glGetUniformLocation(program, "bUseHeightMap");
    glUniform1f(bUseHeightMap_LodID, (GLfloat)GL_FALSE);

    glDisable(GL_DEPTH_TEST);
    // Move the "skybox object" with the camera
    pSkyBox->positionXYZ = ::g_pFlyCamera->getEye();
    DrawObject(
        pSkyBox, glm::mat4(1.0f),
        matModel_Location, matModelInverseTranspose_Location,
        program, ::g_pVAOManager);

    glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);
    //
    glEnable(GL_DEPTH_TEST);
    // *********************************************************************



    // Ordering the objects by transparency
    // 1. Make two arrays or vectors
    // 2. Clear them
    // 3. Go through all your "things to draw", separating transparent from non-transparent
    //    (if the alpha < 1.0f, then it's "transparent" )
    // 3a. glDisable(GL_BLEND);
    // 4. Draw all the NON transparent things (in any order you want)
    // 5. Sort ONLY the transparent object from "far" to "close" for THIS frame
    //     glm::distance() to compare 
    //     Can be ONE PASS of the bubble sort
    // 5a. glEnable(GL_BLEND);
    // 6. Draw transparent objects from FAR to CLOSE



    // Set up the discard texture, etc.

    {

        for (int i = 0; i < ::g_vec_pMeshes.size() - 1; i++)
        {
            if (distanceToCamera(::g_vec_pMeshes[i]->positionXYZ) < distanceToCamera(::g_vec_pMeshes[i + 1]->positionXYZ))
            {
                cMesh* temp = ::g_vec_pMeshes[i];
                ::g_vec_pMeshes[i] = ::g_vec_pMeshes[i + 1];
                ::g_vec_pMeshes[i + 1] = temp;
            }
        }
    }

    // **********************************************************************
    // Draw the "scene" of all objects.
    // i.e. go through the vector and draw each one...
    // **********************************************************************
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        // So the code is a little easier...
        cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

        glm::mat4 matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

        DrawObject(pCurrentMesh,
            matModel,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);


    }//for (unsigned int index
    // Scene is drawn
    // **********************************************************************

    DrawDebugObjects(matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);

}


void DrawDebugObjects(
        GLint matModel_Location,
        GLint matModelInverseTranspose_Location,
        GLuint program,
        cVAOManager* pVAOManager)
{


    {   // Draw a sphere where the camera is looking.
        // Take the at - eye --> vector 
        glm::vec3 eye = ::g_pFlyCamera->getEye();
        glm::vec3 at = ::g_pFlyCamera->getAtInWorldSpace();

        glm::vec3 deltaDirection = at - eye;
        // Normalize to make this vector 1.0 units in length
        deltaDirection = glm::normalize(deltaDirection);

        float SphereDistanceFromCamera = 300.0f;
        glm::vec3 sphereLocation =
            eye + (deltaDirection * SphereDistanceFromCamera);

        // Draw the sphere

        ::g_pDebugSphere->positionXYZ = sphereLocation;
        ::g_pDebugSphere->scale = 5.0f;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

        DrawObject(::g_pDebugSphere,
            glm::mat4(1.0f),
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

    }//Draw a sphere where the camera is looking.



    if ( ::g_bShowDebugShere )
    {
        // Draw other things, like debug objects, UI, whatever
        glm::mat4 matModelDS = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

        // Draw a small white shere at the location of the light
        sLight& currentLight = ::g_pTheLights->theLights[::g_selectedLight];

        ::g_pDebugSphere->positionXYZ = currentLight.position;
        ::g_pDebugSphere->scale = 1.0f;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        DrawObject(::g_pDebugSphere,
                   matModelDS,
                   matModel_Location,
                   matModelInverseTranspose_Location,
                   program,
                   ::g_pVAOManager);

        // Save old debug sphere model name
        std::string oldDebugSphereModel = ::g_pDebugSphere->meshName;

        const float LOW_RES_SPHERE_DISTANCE = 50.0f;
        const std::string LOW_RES_SPHERE_MODEL = "ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply";
        const std::string HIGH_RES_SPHERE_MODEL = "ISO_Shphere_flat_4div_xyz_n_rgba_uv.ply";

        //float calcApproxDistFromAtten( 
        //      float targetLightLevel, 
        //      float accuracy, 
        //      float infiniteDistance, 
        //      float constAttenuation, 
        //      float linearAttenuation,  
        //      float quadraticAttenuation, 
        //	    unsigned int maxIterations = DEFAULTMAXITERATIONS /*= 50*/ );

                // How far away is 95% brightness?
        float distTo95Percent = ::g_pTheLights->lightHelper.calcApproxDistFromAtten(0.95f,    /* the target light level I want*/
                                                                          0.01f,    /*accuracy - how close to 0.25f*/
                                                                          10000.0f, /*infinity away*/
                                                                          currentLight.atten.x, /*const atten*/
                                                                          currentLight.atten.y, /*linear atten*/
                                                                          currentLight.atten.z, /*quadratic atten*/
                                                                          cLightHelper::DEFAULTMAXITERATIONS);
        // Draw a red sphere at 95%
        ::g_pDebugSphere->scale = distTo95Percent;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

        ::g_pDebugSphere->meshName = (::g_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);

        DrawObject(::g_pDebugSphere,
                   matModelDS,
                   matModel_Location,
                   matModelInverseTranspose_Location,
                   program,
                   ::g_pVAOManager);

        // How far away is 50% brightness?
        float distTo50Percent = ::g_pTheLights->lightHelper.calcApproxDistFromAtten(0.50f,    /* the target light level I want*/
                                                                          0.01f,    /*accuracy - how close to 0.25f*/
                                                                          10000.0f, /*infinity away*/
                                                                          currentLight.atten.x, /*const atten*/
                                                                          currentLight.atten.y, /*linear atten*/
                                                                          currentLight.atten.z, /*quadratic atten*/
                                                                          cLightHelper::DEFAULTMAXITERATIONS);
        // Draw a red sphere at 50%
        ::g_pDebugSphere->scale = distTo50Percent;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        ::g_pDebugSphere->meshName = (::g_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(::g_pDebugSphere,
                   matModelDS,
                   matModel_Location,
                   matModelInverseTranspose_Location,
                   program,
                   ::g_pVAOManager);

        // How far away is 25% brightness?
        float distTo25Percent = ::g_pTheLights->lightHelper.calcApproxDistFromAtten(0.25f,    /* the target light level I want*/
                                                                          0.01f,    /*accuracy - how close to 0.25f*/
                                                                          10000.0f, /*infinity away*/
                                                                          currentLight.atten.x, /*const atten*/
                                                                          currentLight.atten.y, /*linear atten*/
                                                                          currentLight.atten.z, /*quadratic atten*/
                                                                          cLightHelper::DEFAULTMAXITERATIONS);
        // Draw a red sphere at 25%
        ::g_pDebugSphere->scale = distTo25Percent;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        ::g_pDebugSphere->meshName = (::g_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(::g_pDebugSphere,
                   matModelDS,
                   matModel_Location,
                   matModelInverseTranspose_Location,
                   program,
                   ::g_pVAOManager);

        // How far away is 5% brightness?
        float distTo5Percent = ::g_pTheLights->lightHelper.calcApproxDistFromAtten(0.05f,    /* the target light level I want*/
                                                                         0.01f,    /*accuracy - how close to 0.25f*/
                                                                         10000.0f, /*infinity away*/
                                                                         currentLight.atten.x, /*const atten*/
                                                                         currentLight.atten.y, /*linear atten*/
                                                                         currentLight.atten.z, /*quadratic atten*/
                                                                         cLightHelper::DEFAULTMAXITERATIONS);
        // Draw a red sphere at 5%
        ::g_pDebugSphere->scale = distTo5Percent;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        ::g_pDebugSphere->meshName = (::g_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(::g_pDebugSphere,
                   matModelDS,
                   matModel_Location,
                   matModelInverseTranspose_Location,
                   program,
                   ::g_pVAOManager);


        ::g_pDebugSphere->meshName = oldDebugSphereModel;

    }//if ( ::g_bShowDebugShere )

    return;
}
