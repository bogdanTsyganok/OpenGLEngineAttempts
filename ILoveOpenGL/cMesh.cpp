#include "cMesh.h"


cMesh::cMesh()
{
	this->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	this->scale = 1.0f;

	this->bIsWireframe = false;
	this->bDisableDepthBufferCheck = false;

	this->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// White
	this->bUseObjectDebugColour = false;
	this->bDontLight = false;
	this->isFbo = false;

	// These are for colouring the ENTIRE object as one colour.
	// Later, these will be replaced:
	// * The diffuse will be replaced by textures
	// * The specular will be replaced by specular and gloss maps
	this->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// The "colour" of the object
	//
	// Specular HIGHLIGHT colour (usually the same as the light, or white)
	this->wholeObjectSpecularRGB = glm::vec3(1.0f, 1.0f, 1.0f);			
	// Specular highlight POWER (or shininess). Starts at 1.0 and goes to ? (like 100, 1000, 10000 is OK)
	this->wholeObjectShininess_SpecPower = 1.0f;	// 1.0 to ??

	// Default to 1.0f (all solid)
	this->alphaTransparency = 1.0f;

	// Clear all the textures
	for (unsigned int index = 0; index != cMesh::MAX_TEXTURES; index++)
	{
		this->textureNames[index] = "";
		this->textureRatios[index] = 0.0f;
	}
	this->textureNames[0] = "BrightColouredUVMap.bmp";
	this->textureRatios[0] = 1.0f;

	// Assign a unique ID
	this->m_UniqueID = cMesh::m_NextID;
	cMesh::m_NextID++;

}


void cMesh::clearTextureRatiosToZero(void)
{
	for (unsigned int index = 0; index != cMesh::MAX_TEXTURES; index++)
	{
		this->textureRatios[index] = 0.0f;
	}
	return;
}


unsigned int cMesh::getUniqueID(void)
{
	return this->m_UniqueID;
}

//static 
unsigned int cMesh::m_NextID = cMesh::INITIAL_UNIQUE_ID;

