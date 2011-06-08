/*********************************************************\
 *  File: Interaction.h                                  *
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


#ifndef __DUNGEON_INTERACTION_H__
#define __DUNGEON_INTERACTION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLScene/Scene/SceneNodeHandler.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2i;
}
class Application;
class IngameGui;
class Camcorder;
class MakingOf;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Interaction application component
*
*  @remarks
*    The main jobs are:
*    - Management of the modes (walk, free, ghost and so on)
*    - Hiding the mouse cursor on 'mouse look around'
*/
class Interaction : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, Interaction, "", PLCore::Object, "Interaction application component")
		// Methods
		pl_method_0(GetIngameGui,	IngameGui*,	"Returns the ingame GUI interaction component instance, never a null pointer",	"")
		// Slots
		pl_slot_1(NotifyMouseMove,					const PLMath::Vector2i&,								"Called when the mouse is moved, mouse position within the window as first parameter",																"")
		pl_slot_2(NotifyMouseButtonDown,			PLGeneral::uint32,			const PLMath::Vector2i&,	"Called when a mouse button is pressed, mouse button that is pressed as first parameter, mouse position within the window as second parameter",		"")
		pl_slot_2(NotifyMouseButtonUp,				PLGeneral::uint32,			const PLMath::Vector2i&,	"Called when a mouse button is released, mouse button that is released as first parameter, mouse position within the window as second parameter",	"")
		pl_slot_2(NotifyKeyDown,					PLGeneral::uint32,			PLGeneral::uint32,			"Called when a key is pressed down, pressed key as first parameter, modifier keys pressed as second parameter",										"")
		pl_slot_0(NotifyMoviePlaybackFinished,																"Called when the camcorder playback has been finished",																								"")
		pl_slot_0(NotifyMakingOfPlaybackFinished,															"Called when the making of playback has been finished",																								"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Mode
		*/
		enum EMode {
			WalkMode     = 0,	/**< Walk mode */
			FreeMode     = 1,	/**< Free mode */
			GhostMode    = 2,	/**< Ghost mode */
			MovieMode    = 3,	/**< Movie mode */
			MakingOfMode = 4,	/**< Making of mode */
			UnknownMode  = 5	/**< Unknown mode */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cApplication
		*    Owner application
		*/
		Interaction(Application &cApplication);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Interaction();

		/**
		*  @brief
		*    Returns the owner application
		*
		*  @return
		*    The owner application
		*/
		Application &GetApplication() const;

		/**
		*  @brief
		*    Returns the current mode
		*
		*  @return
		*    The current mode
		*/
		EMode GetMode() const;

		/**
		*  @brief
		*    Sets the current mode
		*
		*  @param[in] nMode
		*    The new mode
		*  @param[in] bText
		*    Show mode changed text?
		*/
		void SetMode(EMode nMode, bool bText = true);

		/**
		*  @brief
		*    Updates the interaction component
		*/
		void Update();

		/**
		*  @brief
		*    Returns the ingame GUI interaction component instance
		*
		*  @return
		*    The ingame GUI interaction component instance, never a null pointer
		*/
		IngameGui *GetIngameGui() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Updates the mouse picking pull animation
		*/
		void UpdateMousePickingPullAnimation();

		/**
		*  @brief
		*    Set mouse cursor visibility
		*
		*  @param[in] bVisible
		*    Shall the mouse cursor be visible?
		*/
		void SetMouseVisible(bool bVisible);

		/**
		*  @brief
		*    Called when the mouse is moved
		*
		*  @param[in] vPos
		*    Mouse position within the window
		*/
		void NotifyMouseMove(const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is pressed
		*
		*  @param[in] nButton
		*    Mouse button that is pressed
		*  @param[in] vPos
		*    Mouse position within the window
		*/
		void NotifyMouseButtonDown(PLGeneral::uint32 nButton, const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is released
		*
		*  @param[in] nButton
		*    Mouse button that is released
		*  @param[in] vPos
		*    Mouse position within the window
		*/
		void NotifyMouseButtonUp(PLGeneral::uint32 nButton, const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a key is pressed down
		*
		*  @param[in] nKey
		*    Pressed key
		*  @param[in] nModifiers
		*    Modifier keys pressed
		*/
		void NotifyKeyDown(PLGeneral::uint32 nKey, PLGeneral::uint32 nModifiers);

		/**
		*  @brief
		*    Called when the camcorder playback has been finished
		*/
		void NotifyMoviePlaybackFinished();

		/**
		*  @brief
		*    Called when the making of playback has been finished
		*/
		void NotifyMakingOfPlaybackFinished();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Application				  *m_pApplication;						/**< Owner application, always valid! */
		PLScene::SceneNodeHandler  m_cWalkCameraSceneNodeHandler;		/**< Walk camera scene node handler */
		PLScene::SceneNodeHandler  m_cFreeCameraSceneNodeHandler;		/**< Free camera scene node handler */
		PLScene::SceneNodeHandler  m_cGhostCameraSceneNodeHandler;		/**< Ghost camera scene node handler */
		PLScene::SceneNodeHandler  m_cMakingOfCameraSceneNodeHandler;	/**< Making of camera scene node handler */
		EMode					   m_nMode;								/**< Mode */
		EMode					   m_nModeBackup;						/**< Mode backup */
		bool					   m_bLeftMouseButtonDown;				/**< Is the left mouse button currently down? */
		IngameGui				  *m_pIngameGui;						/**< Ingame GUI interaction component instance, always valid! */
		Camcorder				  *m_pCamcorder;						/**< Camcorder interaction component instance, always valid! */
		MakingOf				  *m_pMakingOf;							/**< Making of interaction component instance, always valid! */
		float					   m_fMousePickingPullAnimation;		/**< Mouse picking pull animation */


};


#endif // __DUNGEON_INTERACTION_H__
