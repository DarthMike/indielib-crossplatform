/*****************************************************************************************
 * File: IND_Sequence.h
 * Desc: Sequence object
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTAmNearClippingPlaneBILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef _IND_SEQUENCE_
#define _IND_SEQUENCE_

#include <vector>
#include "IND_Timer.h"
#include "Defines.h"


// --------------------------------------------------------------------------------
//										IND_Sequence
// --------------------------------------------------------------------------------

/**
 Contains a set of IND_Frame objects, contained in IND_Animation. It's mainly used internally by IND_Animation objects.
 */
class IND_Sequence {
private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Structures ------

	struct structFrameTime {
		int _pos;                           // Position in vector frames
		int _time;                          // Frame duration
		structFrameTime() :_pos(0), _time(150) {
		}
	};
	typedef struct structFrameTime FRAME_TIME;

	// Sequence (list of frames)
	struct structSequence {
		IND_Timer _timer;                   // Sequence timer
		int i;                              // Pointer to actual frame
		int _width;                         // With of the wider frame of the sequence
		int _height;                        // Height of the wider frame of the sequence
		int _numFrames;                     // Number of frames of the sequence
		char *_name;                        // Sequence name
		vector <FRAME_TIME *> *_listFrames; // List of frames with their times
		bool _isActive;                     // Flage
		structSequence() {
            _name = new char [1024];
			i = 0;
			_isActive = 0;
			_width = _height = _numFrames =  0;
			_listFrames = new vector <FRAME_TIME *>; // TODO: MFK: valgrind states that this line generates garbage in basic tutorial 11
		}
        
        ~structSequence() {
            DISPOSEARRAY(_name);
            
            // ----- Free frames in each sequence -----
            for (int m = 0; m < _numFrames; m++) {
                // Free all the pointers to FRAME_TIME
                vector <IND_Sequence::FRAME_TIME *>::iterator mVectorFrameTimeIter;
                for (mVectorFrameTimeIter  = _listFrames->begin();
                     mVectorFrameTimeIter  != _listFrames->end();
                     mVectorFrameTimeIter++) {
                    // Free frame of each sequence
                    DISPOSE(*mVectorFrameTimeIter);
                }
            }
            DISPOSE(_listFrames);
        }
	};
	typedef struct structSequence SEQUENCE;

	SEQUENCE _sequence;

	// ----- Private sets ------

	void                    setActualFramePos(int pPos)              {
		_sequence.i = pPos;
	}
	void                    setHighWidth(int pWidth)            {
		_sequence._width = pWidth;
	}
	void                    setHighHeight(int pHeight)           {
		_sequence._height = pHeight;
	}
	void                    setNumFrames(int pNumFrames)        {
		_sequence._numFrames = pNumFrames;
	}
	void                    setName(const char *pName)           {
        strcpy(_sequence._name,pName);
	}
	void                    setPosInVectorFrame(int pPos, int pNewPos) {
		(*_sequence._listFrames) [pPos]->_pos = pNewPos;
	}
	void                    setTimeInVectorFrame(int pPos, int pTime)   {
		(*_sequence._listFrames) [pPos]->_time = pTime;
	}

	void                    setIsActive(bool pAct)             {
		_sequence._isActive = pAct;
	}

	// ----- Private gets ------

	IND_Timer               *getSequenceTimer()                      {
		return &_sequence._timer;
	}
	int                     getActualFramePos()                      {
		return _sequence.i;
	}
	int                     getHighWidth()                      {
		return _sequence._width;
	}
	int                     getHighHeight()                      {
		return _sequence._height;
	}

	bool                    getIsActive()                      {
		return _sequence._isActive;
	}

	int                     getNumFrames()                      {
		return _sequence._numFrames;
	}
	const char* const getName()                      {
		return _sequence._name;
	}
	vector <FRAME_TIME *>    *getListFrames()                      {
		return _sequence._listFrames;
	}

	int                     getActualFramePosInVec()                      {
		return (*getListFrames()) [getActualFramePos()]->_pos;
	}
	int                     getActualFrameTime()                      {
		return (*getListFrames()) [getActualFramePos()]->_time;
	}


	// ----- Friends -----

	friend class IND_Animation;
	friend class IND_AnimationManager;
    /** @endcond */
};

#endif // _IND_FRAME_
