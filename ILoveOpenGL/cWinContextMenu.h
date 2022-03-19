#ifndef _cWinContextMenu_HG_
#define _cWinContextMenu_HG_

#include "globalThings.h"

#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

class cWinContextMenu
{
public:
	cWinContextMenu();

	void showMenu(GLFWwindow* window, int x, int y);

};

#endif	// YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

#endif	// _cWinContextMenu_HG_

