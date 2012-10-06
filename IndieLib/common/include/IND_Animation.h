/*****************************************************************************************
 * File: IND_Animation.h
 * Desc: Animation object
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef _IND_ANIMATION_
#define _IND_ANIMATION_

// ----- Includes -----
#include <vector>

// ----- Forward Declarations -----
class IND_Frame;
class IND_Sequence;
class IND_Image;
class IND_Frame;
class IND_Surface;

// --------------------------------------------------------------------------------
//									IND_Animation
// --------------------------------------------------------------------------------

/**
@defgroup IND_Animation IND_Animation
@ingroup Objects
Class for storing sequences of frames (animations). Click in ::IND_Animation to see all the methods of this class.
*/
/*@{*/

/**
IND_Animation objects are loaded trough the methods of IND_AnimationManager. In order to load
an animation it is necessary to create an animation script in xml format. The structure of the script is quite
easy to understand.

In each animation script you first define the frames (each of the sprites) that you will use for
the sequences that you want to create. Later, you create sequences, using references to these frames,
and specifying the time delay between frames.

So, you can set different sequences using the same frames.

The schema of one animation would be as follows:

\code
Animation
{
    Frames {Frame_1, Frame_2, Frame_3... Frame_n}

    Sequences
    {
        Sequence_1 (list of n frames)
        Sequence_2 (list of n frames)
        Sequence_3 (list of n frames)
        ...
        Sequence_n
    }
}
\endcode

The  @b tokens or <b>keywords</b> of a @b IndieLib animation file are:

<b><< Relative to frames >></b>

@arg <b><frames></b>                    Frame section
@arg <b><name></b>                      Name assigned to the frames (later it will be used in the sequences).
@arg <b><file></b>                      Path to the image file.
@arg <b><offset_x></b>                  X offset of the frame.    (Optional, 0 by default).
@arg <b><offset_y></b>                  Y offset of the frame.    (Optional, 0 by default).

<b><< Relative to sequences >></b>

@arg <b><sequences></b>             Indicates that the sequences are going to be defined in the section between brackets.
@arg <b><name></b>                  Name assigned to the sequence.
@arg <b><frame name></b>            Name of the frame that we are refering to. Here we have to use one of the names we assigned to the loaded frames.
@arg <b><time></b>                  Time, in milliseconds, that the frame will be displayed. (Optional, 150 by default).

@b Note: "//"  can be used before a phrase for writing comments.

\image html surfa1.jpg Animations example in IndieLib.
*/
class LIB_EXP IND_Animation {
public:

	IND_Animation() :
			_vectorFrames(NULL) 
	  {}
	// ----- Public gets ------

	// ----- Relative to lobal animation ------

	//! This function returns the animation script file name in a string of characters.
	char                    *getName()              {
		return _animation._name;
	}
	//! This function returns the total number of animation sequences.
	int                     getNumSequences()              {
		return _animation._sumSequences;
	}
	//! This function returns the total number of animation frames.
	unsigned long getNumTotalFrames()              {
		return _vectorFrames->size();
	}
	//! This function returns the pointer to the object ::IND_Image that contains the frame received or NULL in case that the object has not been loaded.
	IND_Image               *getImage(int pFrame);
	//! This function returns the pointer to the object ::IND_Surface that contains the frame received or NULL in case that the object has not been loaded.
	IND_Surface             *getSurface(int pFrame);

	// ----- Relative to a concrete sequence ------

	//! This function returns the maximum width of the sequence. It means, the width of the widest frame of the sequence.
	int                     getHighWidth(int pSequence);
	//! This function returns the maximum height of the sequence. It means, the height of the highest frame of the sequence.
	int                     getHighHeight(int pSequence);
	//! This function returns the number of frames which are referenced by the sequeence received as a parameter.
	int                     getNumFrames(int pSequence);
	//! This function returns the name, in a string of characters, of the sequence received as a paramater.
	char                    *getName(int pSequence);

	// NOT DOCUMENTED
	int                     getActualFramePos(int pSequence);
	int                     getActualFrameTime(int pSequence);
	int                     getActualOffsetX(int pSequence);
	int                     getActualOffsetY(int pSequence);
	IND_Surface             *getActualSurface(int pSequence);
	void                    setActualFramePos(int pSequence, int pPos);
	IND_Timer               *getSequenceTimer(int pSequence);

	bool                    getIsActive(int pSequence);
	void                    setIsActive(int pSequence, bool pAct);

	// ----- Public sets ------

	//! This function establishes a IND_Image object in a frame. It can be useful for modifying some frames of animations.Note: It is convenient to eliminate any IND_Image object that would be there before setting the actual one.
	void                    setImage(int pFrame, IND_Image *pNewImage);
	//! This function establishes a IND_Surface object in a frame. It can be useful for modifying some frames of animations.Note: It is convenient to eliminate any IND_Surface object that would be there before setting the actual one.
	void                    setSurface(int pFrame, IND_Surface *pNewSurface);

private:

	// ----- Structures ------

	vector <IND_Frame *> *_vectorFrames;    // Vector of frames

	// Animation (list of sequences)
	struct structAnimation {
		char *_name;                            // Animation name
		int _sumSequences;                      // Number of sequences
		vector <IND_Sequence *> *_listSequences; // Sequence list
		structAnimation() : 
			_name(NULL),
			_sumSequences(0),
			_listSequences(NULL) {}
		
	};
	typedef struct structAnimation ANIMATION;

	ANIMATION _animation;

	// ----- Private sets ------

	void                    setName(char *pName)               {
		_animation._name = pName;
	}
	void                    setNumSequences(int pNum)                  {
		_animation._sumSequences = pNum;
	}

	// ----- Private gets ------

	vector <IND_Sequence *>  *getListSequences()                          {
		return _animation._listSequences;
	}
	vector <IND_Frame *>     *getVectorFrames()                          {
		return _vectorFrames;
	}

	// ----- Friends -----

	friend class IND_AnimationManager;
	friend class DirectXRender;
	friend class OpenGLES_iOS_Render;
	friend class IND_Entity2dManager;
};
/*@}*/

#endif // _IND_ANIMATION_
