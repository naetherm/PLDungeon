/*********************************************************\
 *  File: Config.h                                       *
 *
 *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


#ifndef __DUNGEON_CONFIG_H__
#define __DUNGEON_CONFIG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Config/Config.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Dungeon configuration 'collection' class
*
*  @remarks
*    ALL dungeon configuration classes are derived from this class.
*    Use cConfig.Save("Dungeon.cfg", "DungeonConfigGroup") to save
*    ONLY all dungeon configuration classes.
*/
class DungeonConfigGroup : public PLCore::ConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		DungeonConfigGroup();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DungeonConfigGroup();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		DungeonConfigGroup(const DungeonConfigGroup &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		DungeonConfigGroup &operator =(const DungeonConfigGroup &cSource);


};

/**
*  @brief
*    Dungeon configuration class
*/
class DungeonConfig : public DungeonConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
		// Attributes
		pl_attribute_directvalue(SoundAPI,			PLCore::String,	"PLSoundOpenAL::SoundManager",	ReadWrite)
	#ifdef INTERNALRELEASE
		pl_attribute_directvalue(EditModeEnabled,	bool,			true,							ReadWrite)
	#else
		pl_attribute_directvalue(EditModeEnabled,	bool,			false,							ReadWrite)
	#endif
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DungeonConfig();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DungeonConfig();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		DungeonConfig(const DungeonConfig &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		DungeonConfig &operator =(const DungeonConfig &cSource);


};


#endif // __DUNGEON_CONFIG_H__
