#include "GLCommon.h" 
#include "globalThings.h"

#include <sstream>
#include <iostream>

#include "cWinContextMenu.h"



// Handle async IO (keyboard, joystick, mouse, etc.)


// This is so the "fly camera" won't pay attention to the mouse if it's 
// not directly over the window. 
bool g_MouseIsInsideWindow = false;



void handleAsyncKeyboard(GLFWwindow* pWindow, double deltaTime)
{
    float cameraMoveSpeed = ::g_pFlyCamera->movementSpeed;

    float objectMovementSpeed = 1.f;
    float lightMovementSpeed = 1.0f;

    if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )
    {
        // Use "fly" camera (keyboard for movement, mouse for aim)
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS )
        {
            ::g_pFlyCamera->MoveForward_Z(+cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS )	// "backwards"
        {
            ::g_pFlyCamera->MoveForward_Z(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS )	// "left"
        {
            ::g_pFlyCamera->MoveLeftRight_X(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS )	// "right"
        {
            ::g_pFlyCamera->MoveLeftRight_X(+cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS )	// "up"
        {
            ::g_pFlyCamera->MoveUpDown_Y(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS )	// "down"
        {
            ::g_pFlyCamera->MoveUpDown_Y(+cameraMoveSpeed);
        }

        std::stringstream strTitle;
        // std::cout << 
        glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        strTitle << "Camera: "
            << cameraEye.x << ", "
            << cameraEye.y << ", "
            << cameraEye.z; //<< std::endl;

        ::g_TitleText = strTitle.str();

    }//if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )


//    // Basic camera controls (if NONE of the control keys are pressed)
//    if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )
//    {
//        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS ) { ::g_cameraEye.x -= cameraMoveSpeed; } // Go left
//        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS ) { ::g_cameraEye.x += cameraMoveSpeed; } // Go right
//        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS ) { ::g_cameraEye.z += cameraMoveSpeed; }// Go forward 
//        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS ) { ::g_cameraEye.z -= cameraMoveSpeed; }// Go backwards
//        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS ) { ::g_cameraEye.y -= cameraMoveSpeed; }// Go "Down"
//        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS ) { ::g_cameraEye.y += cameraMoveSpeed; }// Go "Up"
//
//        std::stringstream strTitle;
//        // std::cout << 
//        strTitle << "Camera: "
//            << ::g_cameraEye.x << ", "
//            << ::g_cameraEye.y << ", "
//            << ::g_cameraEye.z; //<< std::endl;
//
//        ::g_TitleText = strTitle.str();
//
//    }//if ( areAllModsUp(window) )...



    // If JUST the shift is down, move the "selected" object
    if ( cGFLWKeyboardModifiers::isModifierDown(pWindow, true, false, false ) )
    {
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.x -= objectMovementSpeed; } // Go left
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.x += objectMovementSpeed; } // Go right
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.z += objectMovementSpeed; }// Go forward 
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.z -= objectMovementSpeed; }// Go backwards
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.y -= objectMovementSpeed; }// Go "Down"
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.y += objectMovementSpeed; }// Go "Up"


        if (glfwGetKey(pWindow, GLFW_KEY_KP_ADD) == GLFW_PRESS) { if (::g_selectedObject < ::g_vec_pMeshes.size() - 1) { ::g_selectedObject++; } }// Go "Up"
        if (glfwGetKey(pWindow, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) { if (::g_selectedObject > 0) { ::g_selectedObject--; } }// Go "Up"

        std::stringstream strTitle;
        // std::cout << 
        strTitle << "::g_vec_pMeshes[" << ::g_selectedObject << "].positionXYZ : "
            << ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.x << ", "
            << ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.y << ", "
            << ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.z;// << std::endl;

        ::g_TitleText = strTitle.str();

        // TODO: Add some controls to change the "selected object"
        // i.e. change the ::g_selectedObject value


    }//if ( cGFLWKeyboardModifiers::...


    // If JUST the ALT is down, move the "selected" light
    if ( cGFLWKeyboardModifiers::isModifierDown(pWindow, false, false, true) )
    {
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.x -= lightMovementSpeed; } // Go left
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.x += lightMovementSpeed; } // Go right
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.z += lightMovementSpeed; }// Go forward 
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.z -= lightMovementSpeed; }// Go backwards
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.y -= lightMovementSpeed; }// Go "Down"
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.y += lightMovementSpeed; }// Go "Up"

        // constant attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS )
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.x *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::g_pTheLights->theLights[::g_selectedLight].atten.x <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::g_pTheLights->theLights[::g_selectedLight].atten.x = 0.001f;
            }
            ::g_pTheLights->theLights[::g_selectedLight].atten.x *= 1.01f; // +1% more
        }
        // linear attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_3) == GLFW_PRESS )
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_4) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::g_pTheLights->theLights[::g_selectedLight].atten.y <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::g_pTheLights->theLights[::g_selectedLight].atten.y = 0.001f;
            }
            ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 1.01f; // +1% more
        }
        // quadratic attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_5) == GLFW_PRESS )
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_6) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::g_pTheLights->theLights[::g_selectedLight].atten.z <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::g_pTheLights->theLights[::g_selectedLight].atten.z = 0.001f;
            }
            ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 1.01f; // +1% more
        }

        if ( glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS ) { ::g_pTheLights->theLights[0].param1.y -= 0.5f; }   // Inner
        if ( glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS ) { ::g_pTheLights->theLights[0].param1.y += 0.5f; }   // Inner
        if ( glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS ) { ::g_pTheLights->theLights[0].param1.z -= 0.5f; }   // Outer
        if ( glfwGetKey(pWindow, GLFW_KEY_P) == GLFW_PRESS ) { ::g_pTheLights->theLights[0].param1.z += 0.5f; }   // Outer


        // For the texture height map displacement example:
        const float TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED = 0.005f;
        const float TERRAIN_MAP_DISPLACEMENT_ROTATION_SPEED = 0.025f;

        if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)     { ::g_heightMapUVOffsetRotation.x -= TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED; }
        if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS)   { ::g_heightMapUVOffsetRotation.x += TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED; }
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS)   { ::g_heightMapUVOffsetRotation.y -= TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED; }
        if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)  { ::g_heightMapUVOffsetRotation.y += TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED; }
        // Rotation with the num-pad "+" and "-"
        if (glfwGetKey(pWindow, GLFW_KEY_KP_ADD) == GLFW_PRESS)  { ::g_heightMapUVOffsetRotation.z += TERRAIN_MAP_DISPLACEMENT_ROTATION_SPEED; }
        if (glfwGetKey(pWindow, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)  { ::g_heightMapUVOffsetRotation.z -= TERRAIN_MAP_DISPLACEMENT_ROTATION_SPEED; }


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


    }//if ( cGFLWKeyboardModifiers::...

    return;
}


// We call these every frame
void handleAsyncMouse(GLFWwindow* window, double deltaTime)
{

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    ::g_pFlyCamera->setMouseXY(x, y);

    const float MOUSE_SENSITIVITY = 2.0f;


    // Mouse left (primary?) button pressed? 
    // AND the mouse is inside the window...
    if ( (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        && ::g_MouseIsInsideWindow )
    {
        // Mouse button is down so turn the camera
        ::g_pFlyCamera->Yaw_LeftRight( ::g_pFlyCamera->getDeltaMouseX() * MOUSE_SENSITIVITY, deltaTime );

        ::g_pFlyCamera->Pitch_UpDown( -::g_pFlyCamera->getDeltaMouseY() * MOUSE_SENSITIVITY, deltaTime );

    }

    // Adjust the mouse speed
    if ( ::g_MouseIsInsideWindow )
    {
        const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

        // Adjust the movement speed based on the wheel position
        ::g_pFlyCamera->movementSpeed -= ( ::g_pFlyCamera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY );

        // Clear the mouse wheel delta (or it will increase constantly)
        ::g_pFlyCamera->clearMouseWheelValue();


        if ( ::g_pFlyCamera->movementSpeed <= 0.0f )
        {
            ::g_pFlyCamera->movementSpeed = 0.0f;
        }
    }


    return;
}

void GLFW_cursor_enter_callback(GLFWwindow* window, int entered)
{
    if ( entered )
    {
        std::cout << "Mouse cursor is over the window" << std::endl;
        ::g_MouseIsInsideWindow = true;
    }
    else
    {
        std::cout << "Mouse cursor is no longer over the window" << std::endl;
        ::g_MouseIsInsideWindow = false;
    }
    return;
}

// Called when the mouse scroll wheel is moved
void GLFW_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float mouseScrollWheelSensitivity = 0.1f;

    ::g_pFlyCamera->setMouseWheelDelta(yoffset * mouseScrollWheelSensitivity);

    return;
}

void GLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG
    // Right button is pop-up
    if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
    {
        ShowWindowsContextMenu(window, button, action, mods);
    }
#endif

    return;
}


void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    return;
}


