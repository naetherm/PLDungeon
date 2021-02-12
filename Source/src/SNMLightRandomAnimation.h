/*********************************************************\
 *  File: SNMLightRandomAnimation.h                      *
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


#ifndef __DUNGEON_LIGHTRANDOMANIMATION_H__
#define __DUNGEON_LIGHTRANDOMANIMATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color3.h>
#include <PLScene/Scene/SceneNodeModifier.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene node modifier class for a random light color animation
*
*  @remarks
*    Animates the color of the light scene node over time.
*/
class SNMLightRandomAnimation : public PLScene::SceneNodeModifier {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Flags (PLScene::SceneNodeModifier flags extension)
		*/
		enum EFlags {
			Multiply = 1<<2,	/**< Multiply the color to the fix color, if not set, the color is added to the fix color */
			NR       = 1<<3,	/**< Do NOT manipulate the red color component */
			NG       = 1<<4,	/**< Do NOT manipulate the green color componen */
			NB       = 1<<5		/**< Do NOT manipulate the blue color component */
		};
		pl_flag(EFlags)
			//pl_enum_base(SceneNodeModifier::EFlags)
			pl_enum_value(Multiply,	"Multiply the color to the fix color, if not set, the color is added to the fix color")
			pl_enum_value(NR,		"Do NOT manipulate the red color component")
			pl_enum_value(NG,		"Do NOT manipulate the green color componen")
			pl_enum_value(NB,		"Do NOT manipulate the blue color component")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
		// Attributes
		pl_attribute_directvalue(Speed,		float,					2.0f,									ReadWrite)
		pl_attribute_directvalue(Radius,	float,					0.5f,									ReadWrite)
		pl_attribute_directvalue(FixColor,	PLGraphics::Color3,		PLGraphics::Color3(0.5f, 0.5f, 0.5f),	ReadWrite)
		pl_attribute_directvalue(Color,		PLGraphics::Color3,		PLGraphics::Color3(1.0f, 1.0f, 1.0f),	ReadWrite)
			// Overwritten PLScene::SceneNodeModifier attributes
		pl_attribute_getset(SNMLightRandomAnimation, Flags,		PLCore::uint32,	0,										ReadWrite)
		// Slots
		pl_slot_0_def(SNMLightRandomAnimation, OnUpdate)
	pl_class_def_end


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
		SNMLightRandomAnimation(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNMLightRandomAnimation();


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions]
	//[-------------------------------------------------------]
	protected:
		virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float m_fCurrentIntensity;		/**< Current intensity */
		float m_fDestinationIntensity;	/**< Destination intensity */


};


#endif // __DUNGEON_LIGHTRANDOMANIMATION_H__
