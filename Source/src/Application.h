/*********************************************************\
 *  File: Application.h                                  *
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


#ifndef __DUNGEON_APPLICATION_H__
#define __DUNGEON_APPLICATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLEngine/Tools/Camcorder.h>
#include <PLEngine/Application/ScriptApplication.h>
#include "Gui/IngameGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLScene {
	class SceneRendererPass;
}
class Interaction;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Application class
*/
class Application : public PLEngine::ScriptApplication {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, Application, "", PLEngine::ScriptApplication, "Application class")
		// Methods
		pl_method_0(IsExpertMode,						pl_ret_type(bool),					"Returns whether or not the application runs within the expert mode. Returns 'true' if the application runs within the expert mode, else 'false' (no additional help texts).",															"")
		pl_method_0(IsRepeatMode,						pl_ret_type(bool),					"Returns whether or not the application runs within the repeat mode. Returns 'true' if the application runs within the repeat mode (\"movie -> making of -> movie\" instead of \"movie -> making of -> interactive\"), else 'false'.",	"")
		pl_method_0(IsInternalRelease,					pl_ret_type(bool),					"Returns whether or not this is an internal release. Returns 'true' if this is an internal release, else 'false'.",																														"")
		pl_method_0(GetIngameGui,						pl_ret_type(IngameGui&),			"Returns the ingame GUI interaction component instance",																																												"")
		pl_method_0(GetCamcorder,						pl_ret_type(PLEngine::Camcorder&),	"Returns the camcorder component instance",																																																"")
		pl_method_0(UpdateMousePickingPullAnimation,	pl_ret_type(void),					"Updates the mouse picking pull animation",																																																"")
		// Signals
		pl_signal_2(SignalSetMode,	PLGeneral::uint32,	bool,	"Signal indicating that a new interaction mode has been chosen, mode index as first parameter(0 = Walk mode, 1 = Free mode, 2 = Ghost mode, 3 = Movie mode, 4 = Making of mode), 'true' as second parameter to show mode changed text",	"")
		// Slots
		pl_slot_1(OnLoadProgress,	float,	"Called on load progress, load progress (0.0-1.0) as first parameter",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		Application();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Application();

		/**
		*  @brief
		*    Returns whether or not the application runs within the expert mode
		*
		*  @return
		*    'true' if the application runs within the expert mode, else 'false' (no additional help texts)
		*/
		bool IsExpertMode() const;

		/**
		*  @brief
		*    Returns whether or not the application runs within the repeat mode
		*
		*  @return
		*    'true' if the application runs within the repeat mode ("movie -> making of -> movie" instead of "movie -> making of -> interactive"), else 'false'
		*/
		bool IsRepeatMode() const;

		/**
		*  @brief
		*    Returns whether or not this is an internal release
		*
		*  @return
		*    'true' if this is an internal release, else 'false'
		*/
		bool IsInternalRelease() const;

		/**
		*  @brief
		*    Returns the ingame GUI interaction component instance
		*
		*  @return
		*    The ingame GUI interaction component instance, never a null pointer
		*/
		IngameGui &GetIngameGui() const;

		/**
		*  @brief
		*    Returns the camcorder component instance
		*
		*  @return
		*    The camcorder component instance
		*/
		PLEngine::Camcorder &GetCamcorder() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called on load progress
		*
		*  @param[in] fLoadProgress
		*    Load progress (0.0-1.0)
		*/
		void OnLoadProgress(float fLoadProgress);

		/**
		*  @brief
		*    Updates the mouse picking pull animation
		*/
		void UpdateMousePickingPullAnimation();


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::ConsoleApplication functions ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnInitLog();
		virtual void OnInit();
		virtual void OnDeInit();


	//[-------------------------------------------------------]
	//[ Protected virtual PLEngine::SceneApplication functions ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnCreateRootScene();


	//[-------------------------------------------------------]
	//[ Public virtual PLEngine::BasicSceneApplication functions ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadScene(PLGeneral::String sFilename);


	//[-------------------------------------------------------]
	//[ Private virtual PLEngine::BasicSceneApplication functions ]
	//[-------------------------------------------------------]
	private:
		virtual void OnCreateScene(PLScene::SceneContainer &cContainer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IngameGui			*m_pIngameGui;					/**< Ingame GUI interaction component instance, always valid! */
		PLEngine::Camcorder	*m_pCamcorder;					/**< Camcorder interaction component instance, always valid! */
		float				 m_fLoadProgress;				/**< Load progress */
		float				 m_fMousePickingPullAnimation;	/**< Mouse picking pull animation */


};


#endif // __DUNGEON_APPLICATION_H__