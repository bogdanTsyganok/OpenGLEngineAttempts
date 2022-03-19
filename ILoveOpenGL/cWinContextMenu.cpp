
#include "cWinContextMenu.h"

#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

// Note: we *don't* need to (or should) #include Windows.h
// Instead, we ask GFLW to expose the Win32 calls. 
//      #include <Windows.h>

#include <iostream>

// The global.h only has a forward declaration, so we need to include the actual header
#include "cMesh.h"

cWinContextMenu::cWinContextMenu()
{

}

void cWinContextMenu::showMenu(GLFWwindow* window, int x, int y)
{
    // Windows communication is mostly through "messages" that are passed around.
    // Things (mouse, packet, whatever) "send" messages to a queue, a "message pump"
    //  does some possible adjustments to them (for languages, power settings, whatever), 
    //  then they are sent to the message handler, which is a giant switch statement that
    //  the user writes. Seriously, that's usually what it is. Sometimes it's hidden in a 
    //  web of object oriented lies and nonsense to pretend like it's not just doing 
    //  whatever a giant switch statement does, but I digress. 
    // 
    // Anywhoo, since there's a lot of pre-defined messages, you have to be careful that
    //  your messages aren't overlapping.
    // Oh, I should mention that "messages" are just ints. Yeah. 
    // 
    // To help with this, each application can start their messages at WM_USER 
    //  (0x0400 or 1024). 
    // Note that this is specific to the application and to the window, actually.
    // Like different programs can have the same messages, and different 
    //  windows within that application can also have the same messages.
    // This is because the messages are sent to 
    const UINT_PTR ID_CLOSE = WM_USER + 1;
    const UINT_PTR ID_PLAY_MARYLN_MANSON = WM_USER + 2;
    const UINT_PTR ID_USE_THE_FORCE_LUKE = WM_USER + 3;
    const UINT_PTR ID_MAKE_EVERYTHING_WIREFRAME = WM_USER + 99;
    const UINT_PTR ID_MAKE_EVERYTHING_SOLID = WM_USER + 100;
    const UINT_PTR ID_SELECTED_LIGHT_POINT = WM_USER + 101;
    const UINT_PTR ID_SELECTED_LIGHT_SPOT = WM_USER + 102;


    HMENU hPopupMenu = CreatePopupMenu();
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_MAKE_EVERYTHING_WIREFRAME, (LPCWSTR)L"Make Everything wireframe!");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_MAKE_EVERYTHING_SOLID, (LPCWSTR)L"Make Everything solid!");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_CLOSE, (LPCWSTR)L"Exit");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_PLAY_MARYLN_MANSON, (LPCWSTR)L"Play");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_USE_THE_FORCE_LUKE, (LPCWSTR)L"Use the force, Luke!");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_SELECTED_LIGHT_POINT, (LPCWSTR)L"Make selected light a point");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_SELECTED_LIGHT_SPOT, (LPCWSTR)L"Make selected light a spot");

    HWND hWnd = glfwGetWin32Window(window);

    SetForegroundWindow(hWnd);
    // Note: not actually a "bool", it's an int
    // Yeah, talk to Microsoft, not me.
    // Actually, don't do that because even a "bool" isn't actually a bool, it's also an int.
    // True story.

    // TOP and BOTTOM align is how the menu pops up, either above or below the mouse
    // RETURN CMD says to NOT send the choice to the window, but instead return it just here.
    BOOL choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, x, y, 0, hWnd, NULL);

    // Returns 0 if no choice made
   
    // Now process the choice we made with the mouse click
    switch ( choice )
    {
    case ID_MAKE_EVERYTHING_WIREFRAME:
        {//ID_MAKE_EVERYTHING_WIREFRAME
            for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
            {
                ::g_vec_pMeshes[index]->bIsWireframe = true;
            }
        }//ID_MAKE_EVERYTHING_WIREFRAME
        break;
    case ID_MAKE_EVERYTHING_SOLID:
        {//ID_MAKE_EVERYTHING_WIREFRAME
            for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
            {
                ::g_vec_pMeshes[index]->bIsWireframe = false;
            }
        }//ID_MAKE_EVERYTHING_WIREFRAME
        break;
    case ID_CLOSE:
        std::cout << "Picked \"Exit\" (ID_CLOSE)" << std::endl;

        break;
    case ID_PLAY_MARYLN_MANSON:
        std::cout << "Picked \"Play\" (ID_PLAY_MARYLN_MANSON)" << std::endl;
        break;

    case ID_USE_THE_FORCE_LUKE:
        // Note the use of the scope as you can't declare variables within the scope of a case.
        // Sorry, you just can't. So why doesn't the C++ committee fix this? Who knows?
        {
            //            int indexLuke = findObjectByFriendlyName("Luke");
            cMesh* pLuke = ::g_findObjectByFriendlyName("Luke Skywalker");
        
            if ( pLuke )        // or: if ( pLuke != NULL )
            {
                pLuke->wholeObjectDiffuseRGBA.r = gGetRandBetween<float>(0.0f, 1.0f);
                pLuke->wholeObjectDiffuseRGBA.g = gGetRandBetween<float>(0.0f, 1.0f);
                pLuke->wholeObjectDiffuseRGBA.b = gGetRandBetween<float>(0.0f, 1.0f);
            }
        }
        break;
    case ID_SELECTED_LIGHT_POINT:
        ::g_pTheLights->theLights[::g_selectedLight].param1.x = 0.0f;
        std::cout << "Light[" << ::g_selectedLight << "] is a point" << std::endl;
        break;
    case ID_SELECTED_LIGHT_SPOT:
        ::g_pTheLights->theLights[::g_selectedLight].param1.x = 1.0f;
        std::cout << "Light[" << ::g_selectedLight << "] is a spot" << std::endl;
        break;

    case 0:
        std::cout << "Didn't make a choice (hit <esc>?)" << std::endl;
        break;

    default:
        std::cout << "Unknown value returned from menu" << std::endl;
    }

    return;
}


#endif


void ShowWindowsContextMenu(GLFWwindow* window, int button, int action, int mods)
{
#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

    cWinContextMenu thePopUpMenu;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);

    int menux = xpos + (int)mx;
    int menuy = ypos + (int)my;

    // Note: this is a "blocking" call... 
    std::cout << "Showing menu...";
    thePopUpMenu.showMenu(window, menux, menuy);
    std::cout << "done." << std::endl;

#endif	
    return;
}