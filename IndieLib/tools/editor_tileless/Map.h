/*****************************************************************************************
 * Desc: Map class, for loading and savin maps and creating, cloning and deleting nodes.
 *
 * gametuto.com - Javier López López (javilop.com)
 *
 *****************************************************************************************
 *
 * Creative Commons - Attribution 3.0 Unported
 * You are free:
 *	to Share — to copy, distribute and transmit the work
 *	to Remix — to adapt the work
 *
 * Under the following conditions:
 * Attribution. You must attribute the work in the manner specified by the author or licensor 
 * (but not in any way that suggests that they endorse you or your use of the work).
 *
 *****************************************************************************************/

#ifndef _MAP_
#define _MAP_

// ------ Includes -----

#include <vector>
#include "dependencies/tinyXml/tinyxml.h"
#include "Resources.h"
#include "Node.h"
#include "CIndieLib.h"
#include "Defines.h"
using namespace std; 


// --------------------------------------------------------------------------------
//								     	Map
// --------------------------------------------------------------------------------

class Map
{
public:

	Map												();	
	void						Free				();

	void						CreateNode			(int pX, int pY, int pZ, int pId, int pLayer, IND_Surface *pSurface);
	void						DeleteNode			(Node *pNode);
	void						CloneNode			(Node *pNode);
	bool						SaveMap				(char *pTilesetPath);
	char						*GetPathToTileset	();
	bool						LoadMap				(Resources *pResources);
	vector		<Node*>			*GetVectorNodes		()	{ return &mVectorNodes; }
	void						FreeMap				();

private:

	CIndieLib *mI;							// IndieLib pointer
	vector <Node*> mVectorNodes;				// Map nodes

	// ----- Private methods -----

	char		*OpenFileDialog				(char *pFilter, bool pAction, char *pTitle);
};

#endif // _MAP_