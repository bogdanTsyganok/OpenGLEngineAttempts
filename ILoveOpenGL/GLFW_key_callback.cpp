#include "globalThings.h"

#include <sstream>
#include <iostream>


// Turns off the: warning C4005: 'APIENTRY': macro redefinition
#pragma warning( disable: 4005)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
void UseMessageBox(void)
{

    // If you want to do some Win32 calls:
    // Note the "L" which is "wide" or Unicode encoding
    //MessageBox(NULL, L"Moved Camera", L"FYI:", MB_OK);
    int response = MessageBox(NULL, L"Moved Camera", L"FYI:", MB_YESNOCANCEL | MB_ICONWARNING);
    switch (response)
    {
    case IDYES:
        std::cout << "You chose YES" << std::endl;
        break;
    case IDNO:
        std::cout << "You chose NO" << std::endl;
        break;
    case IDCANCEL:
        std::cout << "You chose CANCEL" << std::endl;
        break;
    }

    return;
}

/*static*/ void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }


    // From the help review session.
    // Move the camera to a location and look somewhere
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        // Move the eye
        ::g_pFlyCamera->setEye(glm::vec3(8631.0f, -1487.0f, 13010.0f));

        // "look at" something.
        // Since we are using the fly camera, we can't just set the eye.
        //::g_pFlyCamera->setAt(glm::vec3(0.0f, 0.0f, 0.0f));

        ::g_pFlyCamera->Yaw_LeftRight(-80.0f);
        ::g_pFlyCamera->Pitch_UpDown(-10.0f);


        UseMessageBox();
    }

    float cameraSpeed = 1.0f;
    float objectMovementSpeed = 1.0f;
    float lightMovementSpeed = 1.0f;

    bool bShiftDown = false;
    bool bControlDown = false;
    bool bAltDown = false;

    //    // Shift down?
    //    if ( mods == GLFW_MOD_SHIFT )       // 0x0001   0000 0001
    //    {
    //        // ONLY shift is down
    //    }
    //    // Control down?
    //    if ( mods == GLFW_MOD_CONTROL  )    // 0x0002   0000 0010
    //    // Alt down?
    //    if ( mods == GLFW_MOD_ALT   )       // 0x0004   0000 0100

        //   0000 0111 
        // & 0000 0001
        // -----------
        //   0000 0001 --> Same as the shift mask

        // Use bitwise mask to filter out just the shift
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Shift is down and maybe other things, too
        bShiftDown = true;
    }
    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Shift is down and maybe other things, too
        bControlDown = true;
    }
    if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
    {
        // Shift is down and maybe other things, too
        bAltDown = true;
    }



//   // If JUST the shift is down, move the "selected" object
//   if ( bShiftDown && (!bControlDown) && (!bAltDown) )
//   {
//       //if (key == GLFW_KEY_A)  {   g_vecMeshes[::g_selectedObject].positionXYZ.x -= objectMovementSpeed;     } // Go left
//       if ( key == GLFW_KEY_A ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.x -= objectMovementSpeed; } // Go left
//       if ( key == GLFW_KEY_D ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.x += objectMovementSpeed; } // Go right
//       if ( key == GLFW_KEY_W ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.z += objectMovementSpeed; }// Go forward 
//       if ( key == GLFW_KEY_S ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.z -= objectMovementSpeed; }// Go backwards
//       if ( key == GLFW_KEY_Q ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.y -= objectMovementSpeed; }// Go "Down"
//       if ( key == GLFW_KEY_E ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.y += objectMovementSpeed; }// Go "Up"
//
//       std::stringstream strTitle;
//       // std::cout << 
//       strTitle << "::g_vec_pMeshes[" << ::g_selectedObject << "].positionXYZ : "
//           << ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.x << ", "
//           << ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.y << ", "
//           << ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.z;// << std::endl;
//
//       ::g_TitleText = strTitle.str();
//
//       // TODO: Add some controls to change the "selcted object"
//       // i.e. change the ::g_selectedObject value
//
//
//   }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        for (std::vector<cMesh*>::iterator it = ::g_vec_pMeshes.begin(); it != ::g_vec_pMeshes.end(); it++)
        {
            (*it)->bIsWireframe = !(*it)->bIsWireframe;
        }
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        if(::g_pTheLights->theLights[0].param2.x > 0.f)
            ::g_pTheLights->TurnOffLight(0);
        else
            ::g_pTheLights->TurnOnLight(0);
    }
   // If JUST the ALT is down, move the "selected" light
   if ( (!bShiftDown) && (!bControlDown) && bAltDown )
   {
//       if ( key == GLFW_KEY_A ) { ::g_pTheLights->theLights[::g_selectedLight].position.x -= lightMovementSpeed; } // Go left
//       if ( key == GLFW_KEY_D ) { ::g_pTheLights->theLights[::g_selectedLight].position.x += lightMovementSpeed; } // Go right
//       if ( key == GLFW_KEY_W ) { ::g_pTheLights->theLights[::g_selectedLight].position.z += lightMovementSpeed; }// Go forward 
//       if ( key == GLFW_KEY_S ) { ::g_pTheLights->theLights[::g_selectedLight].position.z -= lightMovementSpeed; }// Go backwards
//       if ( key == GLFW_KEY_Q ) { ::g_pTheLights->theLights[::g_selectedLight].position.y -= lightMovementSpeed; }// Go "Down"
//       if ( key == GLFW_KEY_E ) { ::g_pTheLights->theLights[::g_selectedLight].position.y += lightMovementSpeed; }// Go "Up"
//
//       // constant attenuation
//       if ( key == GLFW_KEY_1 )
//       {
//           ::g_pTheLights->theLights[::g_selectedLight].atten.x *= 0.99f; // -1% less
//       }
//       if ( key == GLFW_KEY_2 )
//       {
//           // Is it at (or below) zero?
//           if ( ::g_pTheLights->theLights[::g_selectedLight].atten.x <= 0.0f )
//           {
//               // Set it to some really small initial attenuation
//               ::g_pTheLights->theLights[::g_selectedLight].atten.x = 0.001f;
//           }
//           ::g_pTheLights->theLights[::g_selectedLight].atten.x *= 1.01f; // +1% more
//       }
//       // linear attenuation
//       if ( key == GLFW_KEY_3 )
//       {
//           ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 0.99f; // -1% less
//       }
//       if ( key == GLFW_KEY_4 )
//       {
//           // Is it at (or below) zero?
//           if ( ::g_pTheLights->theLights[::g_selectedLight].atten.y <= 0.0f )
//           {
//               // Set it to some really small initial attenuation
//               ::g_pTheLights->theLights[::g_selectedLight].atten.y = 0.001f;
//           }
//           ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 1.01f; // +1% more
//       }
//       // quardatic attenuation
//       if ( key == GLFW_KEY_5 )
//       {
//           ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 0.99f; // -1% less
//       }
//       if ( key == GLFW_KEY_6 )
//       {
//           // Is it at (or below) zero?
//           if ( ::g_pTheLights->theLights[::g_selectedLight].atten.z <= 0.0f )
//           {
//               // Set it to some really small initial attenuation
//               ::g_pTheLights->theLights[::g_selectedLight].atten.z = 0.001f;
//           }
//           ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 1.01f; // +1% more
//       }

        if ( key == GLFW_KEY_PAGE_UP )
        {
            ::g_bShowDebugShere = true;
        }
        if ( key == GLFW_KEY_PAGE_DOWN )
        {
            ::g_bShowDebugShere = false;
        }


        if ( key == GLFW_KEY_KP_ADD && action == GLFW_PRESS )
        {
            ::g_selectedLight++;
            if ( ::g_selectedLight >= cLightManager::NUMBER_OF_LIGHTS )
            {
                ::g_selectedLight = cLightManager::NUMBER_OF_LIGHTS - 1;
            }
        }
        if ( key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS )
        {
            ::g_selectedLight--;
            if ( ::g_selectedLight >= cLightManager::NUMBER_OF_LIGHTS )
            {
                ::g_selectedLight = 0;
            }
        }


        if ( key == GLFW_KEY_KP_MULTIPLY )
        {
            // Turn light on
            ::g_pTheLights->theLights[::g_selectedLight].param2.x = 1.0f;
        }
        if ( key == GLFW_KEY_KP_DIVIDE )
        {
            // Turn light off
            ::g_pTheLights->theLights[::g_selectedLight].param2.x = 0.0f;
        }



        std::stringstream strTitle;
        // std::cout << 
        strTitle << "Light # " << ::g_selectedLight << " positionXYZ : "
            << ::g_pTheLights->theLights[::g_selectedLight].position.x << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.y << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.z << "  "
            << "attenuation (C, L, Q): "
            << ::g_pTheLights->theLights[::g_selectedLight].atten.x << ", "        // Const
            << ::g_pTheLights->theLights[::g_selectedLight].atten.y << ", "        // Linear
            << ::g_pTheLights->theLights[::g_selectedLight].atten.z << "  "        // Quadratic
            << (::g_pTheLights->theLights[::g_selectedLight].param2.x > 0.0f ? " is on" : " is off");
        //<< std::endl;
        ::g_TitleText = strTitle.str();

 

    }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

    return;
}
