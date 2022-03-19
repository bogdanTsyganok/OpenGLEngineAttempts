#include "cAnimationControl.h"
#include "cCmd_MoveRelative.h"
#include "cCmd_MoveWaypoint.h"
#include "cCmd_Follow.h"
#include "cCmd_Rotation.h"
#include "cCmd_LocationTrigger.h"
#include "globalThings.h"
#include <fstream>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <iostream>

bool SetValue(rapidjson::Value& val, float& toSet)
{
	if (val.IsFloat())
	{
		toSet = val.GetFloat();
		return true;
	}
	return false;
}

bool SetValue(rapidjson::Value& val, std::string& toSet)
{
	if (val.IsString())
	{
		toSet = val.GetString();
		return true;
	}
	return false;
}

bool SetValue(rapidjson::Value& val, int& toSet)
{
	if (val.IsInt())
	{
		toSet = val.GetInt();
		return true;
	}
	return false;
}

bool GetGroup(rapidjson::Value& val, cCommandGroup* groupOut, cAnimationControl* pAnimationControl);

iCommand* getCommand(rapidjson::Value& val, cAnimationControl* pAnimationControl);

iCommand* getCommand(rapidjson::Value& val, cAnimationControl* pAnimationControl)
{
	std::string input;
	iCommand* parCmd=NULL;
	SetValue(val["CmdName"], input);
	if (input == "MoveCmd")
	{
		std::string object;
		glm::vec4 floatData;
		SetValue(val["ObjectName"], object);
		SetValue(val["X"], floatData.x);
		SetValue(val["Y"], floatData.y);
		SetValue(val["Z"], floatData.z);
		SetValue(val["Time"], floatData.w);
		sMessage moveMsg;
		void* mesh = ::g_findObjectByFriendlyName(object);
		if (mesh == NULL)
		{
			std::cout << "Cannot find object: " << object << std::endl;
			return NULL;
		}
		moveMsg.voidPointers.push_back(mesh);
		moveMsg.vecData.push_back(floatData);
		parCmd = new cCmd_MoveRelative();
		parCmd->Initialize(moveMsg);
	}
	else if (input == "RotateCmd")
	{
		std::string object;
		glm::vec4 floatData;
		SetValue(val["ObjectName"], object);
		SetValue(val["X"], floatData.x);
		SetValue(val["Y"], floatData.y);
		SetValue(val["Z"], floatData.z);
		SetValue(val["Time"], floatData.w);
		sMessage moveMsg;
		void* mesh = ::g_findObjectByFriendlyName(object);
		if (mesh == NULL)
		{
			std::cout << "Cannot find object: " << object << std::endl;
			return NULL;
		}
		moveMsg.voidPointers.push_back(mesh);
		moveMsg.vecData.push_back(floatData);
		parCmd = new cCmd_Rotate();
		parCmd->Initialize(moveMsg);
	}
	else if (input == "Waypoint")
	{
		if (val["Moves"].IsArray())
		{
			rapidjson::GenericArray<false, rapidjson::Value> moveArray = val["Moves"].GetArray();

			std::string object;
			SetValue(val["ObjectName"], object);
			sMessage moveMsg;
			void* mesh = ::g_findObjectByFriendlyName(object);
			if (mesh == NULL)
			{
				std::cout << "Cannot find object: " << object << std::endl;
				return NULL;
			}
			for (int i = 0; i < moveArray.Size(); i++)
			{
				glm::vec4 floatData;
				SetValue(moveArray[i]["X"], floatData.x);
				SetValue(moveArray[i]["Y"], floatData.y);
				SetValue(moveArray[i]["Z"], floatData.z);
				SetValue(moveArray[i]["Time"], floatData.w);
				moveMsg.vecData.push_back(floatData);
			}
			moveMsg.voidPointers.push_back(mesh);
			parCmd = new cCmd_MoveWaypoint();
			parCmd->Initialize(moveMsg);
		}
		else
		{
			std::cout << "No waypoints" << std::endl;
		}
	}
	else if (input == "Trigger")
	{
		std::string object;
		glm::vec4 floatData;
		SetValue(val["ObjectName"], object);
		SetValue(val["X"], floatData.x);
		SetValue(val["Y"], floatData.y);
		SetValue(val["Z"], floatData.z);
		SetValue(val["Radius"], floatData.w);
		sMessage triggerMsg;
		void* mesh = ::g_findObjectByFriendlyName(object);
		if (mesh == NULL)
		{
			std::cout << "Cannot find object: " << object << std::endl;
			return NULL;
		}
		iCommand* cmd = getCommand(val["Command"], pAnimationControl);
		if (cmd == NULL)
		{
			std::cout << "Cannot find command: " << object << std::endl;
			return NULL;
		}
		triggerMsg.voidPointers.push_back(cmd);
		triggerMsg.vecData.push_back(floatData);
		triggerMsg.voidPointers.push_back(mesh);
		parCmd = new cCmd_LocationTrigger();
		parCmd->Initialize(triggerMsg);

	}
	else if (input == "Follow")
	{
		std::string object, targt;
		glm::vec4 floatData;
		SetValue(val["ObjectName"], object);
		SetValue(val["TargetName"], targt);
		SetValue(val["X"], floatData.x);
		SetValue(val["Y"], floatData.y);
		SetValue(val["Z"], floatData.z);
		SetValue(val["MaxSpeed"], floatData.w);
		sMessage followMsg;
		void* mesh = ::g_findObjectByFriendlyName(object);
		if (mesh == NULL)
		{
			std::cout << "Cannot find object: " << object << std::endl;
			return NULL;
		}

		followMsg.voidPointers.push_back(mesh);

		mesh = ::g_findObjectByFriendlyName(targt);
		if (mesh == NULL)
		{
			std::cout << "Cannot find object: " << targt << std::endl;
			return NULL;
		}
		followMsg.voidPointers.push_back(mesh);

		followMsg.vecData.push_back(floatData);
		parCmd = new cCmd_Follow();
		parCmd->Initialize(followMsg);

	}
	else if (input == "CmdGroup")
	{
		parCmd = pAnimationControl->pCreateCommandGroup();
		if(!GetGroup(val, (cCommandGroup*)parCmd, pAnimationControl))
			parCmd = NULL;
	}
	return parCmd;
}

bool GetGroup(rapidjson::Value& val, cCommandGroup* groupOut, cAnimationControl* pAnimationControl)
{
	if (val.IsObject())
	{
		if (val["SerialCommands"].IsArray())
		{
			rapidjson::GenericArray<false, rapidjson::Value> commandArray = val["SerialCommands"].GetArray();
			
			for (int i = 0; i < commandArray.Size(); i++)
			{
				iCommand* cmd = getCommand(commandArray[i], pAnimationControl);
				if (cmd != NULL)
					groupOut->AddCommandSerial(cmd);
				else
				{
					std::cout << "Error loading commands" << std::endl;
				}
			}
		}
		if (val["ParallelCommands"].IsArray())
		{

			rapidjson::GenericArray<false, rapidjson::Value> commandArray = val["ParallelCommands"].GetArray();

			for (int i = 0; i < commandArray.Size(); i++)
			{
				iCommand* cmd = getCommand(commandArray[i], pAnimationControl);
				if (cmd != NULL)
					groupOut->AddCommandParallel(cmd);
				else
				{
					std::cout << "Error loading commands" << std::endl;
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

void LoadTheAnimationCommands(cAnimationControl* pAnimationControl)
{

	//    ____        _               _          _                 _   _             
	//   | __ )  __ _(_)___  ___     / \   _ __ (_)_ __ ___   __ _| |_(_) ___  _ __  
	//   |  _ \ / _` | / __|/ __|   / _ \ | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \ 
	//   | |_) | (_| | \__ \ (__   / ___ \| | | | | | | | | | (_| | |_| | (_) | | | |
	//   |____/ \__,_|_|___/\___| /_/   \_\_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|
	//                                


	FILE* fp = 0;
	fopen_s(&fp, "animationtext.json", "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	if (d.IsArray())
	{
		
	}

	cCommandGroup* pCutScene_1 = pAnimationControl->pCreateCommandGroup();
	GetGroup(d, pCutScene_1, pAnimationControl);
	fclose(fp);

	pAnimationControl->AddCommandGroup(pCutScene_1);
	// Add a permanent command

	//pAnimationControl->AddPermanentCommandGroup(permanentCommands);




	
	return;
}