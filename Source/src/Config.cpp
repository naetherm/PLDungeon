/*********************************************************\
 *  File: Config.cpp                                     *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Config.h"


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(DungeonConfigGroup)
pl_implement_class(DungeonConfig)


//[-------------------------------------------------------]
//[ Protected DungeonConfigGroup functions                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
DungeonConfigGroup::DungeonConfigGroup()
{
}

/**
*  @brief
*    Destructor
*/
DungeonConfigGroup::~DungeonConfigGroup()
{
}


//[-------------------------------------------------------]
//[ Private DungeonConfigGroup functions                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
DungeonConfigGroup::DungeonConfigGroup(const DungeonConfigGroup &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
DungeonConfigGroup &DungeonConfigGroup::operator =(const DungeonConfigGroup &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}




//[-------------------------------------------------------]
//[ Public DungeonConfig functions                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
DungeonConfig::DungeonConfig() :
	SoundAPI(this),
	EditModeEnabled(this)
{
}

/**
*  @brief
*    Destructor
*/
DungeonConfig::~DungeonConfig()
{
}


//[-------------------------------------------------------]
//[ Private DungeonConfig functions                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
DungeonConfig::DungeonConfig(const DungeonConfig &cSource) :
	SoundAPI(this),
	EditModeEnabled(this)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
DungeonConfig &DungeonConfig::operator =(const DungeonConfig &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}
