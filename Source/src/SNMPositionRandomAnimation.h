/*********************************************************\
 *  File: SNMPositionRandomAnimation.h                   *
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


#ifndef __DUNGEON_POSITIONRANDOMANIMATION_H__
#define __DUNGEON_POSITIONRANDOMANIMATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLMath/Vector3.h>
#include <PLScene/Scene/SceneNodeModifiers/SNMTransform.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Random position animation scene node modifier class
*
*  @remarks
*    Animates the position of the scene nodes over time.
*/
class SNMPositionRandomAnimation : public PLScene::SNMTransform {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, SNMPositionRandomAnimation, "", PLScene::SNMTransform, "Random position animation scene node modifier class")
		// Attributes
		pl_attribute(Speed,			float,				1.0f,								ReadWrite,	DirectValue,	"Animation speed",													"")
		pl_attribute(Radius,		float,				10.0f,								ReadWrite,	DirectValue,	"Animation radius",													"")
		pl_attribute(FixPosition,	PLMath::Vector3,	PLMath::Vector3(0.0f, 0.0f, 0.0f),	ReadWrite,	DirectValue,	"Position, if default, the initial scene node position is used",	"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		SNMPositionRandomAnimation(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNMPositionRandomAnimation();


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions]
	//[-------------------------------------------------------]
	protected:
		virtual void OnActivate(bool bActivate);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void NotifyUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3 m_vCurrentPosition;		/**< Current local position */
		PLMath::Vector3 m_vDestinationPosition;	/**< Destination local position */


};


#endif // __DUNGEON_POSITIONRANDOMANIMATION_H__
