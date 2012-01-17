/*********************************************************\
 *  File: Application.h                                  *
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


#ifndef __DUNGEON_APPLICATION_H__
#define __DUNGEON_APPLICATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLEngine/Application/ScriptApplication.h>


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
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
		// Methods
		pl_method_0(IsExpertMode,						pl_ret_type(bool),	"Returns whether or not the application runs within the expert mode. Returns 'true' if the application runs within the expert mode, else 'false' (no additional help texts).",															"")
		pl_method_0(IsRepeatMode,						pl_ret_type(bool),	"Returns whether or not the application runs within the repeat mode. Returns 'true' if the application runs within the repeat mode (\"movie -> making of -> movie\" instead of \"movie -> making of -> interactive\"), else 'false'.",	"")
		pl_method_0(IsInternalRelease,					pl_ret_type(bool),	"Returns whether or not this is an internal release. Returns 'true' if this is an internal release, else 'false'.",																														"")
		pl_method_0(UpdateMousePickingPullAnimation,	pl_ret_type(void),	"Updates the mouse picking pull animation",																																																"")
		// Signals
		pl_signal_2(SignalSetMode,	PLCore::uint32,	bool,	"Signal indicating that a new interaction mode has been chosen, mode index as first parameter(0 = Walk mode, 1 = Free mode, 2 = Ghost mode, 3 = Movie mode, 4 = Making of mode), 'true' as second parameter to show mode changed text",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFrontend
		*    Frontend this application instance is running in
		*/
		Application(PLCore::Frontend &cFrontend);

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


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Updates the mouse picking pull animation
		*/
		void UpdateMousePickingPullAnimation();


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::CoreApplication functions   ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnInit() override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneApplication functions ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnCreateRootScene() override;


	//[-------------------------------------------------------]
	//[ Public virtual PLEngine::EngineApplication functions  ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadScene(const PLCore::String &sFilename) override;


	//[-------------------------------------------------------]
	//[ Private virtual PLEngine::EngineApplication functions ]
	//[-------------------------------------------------------]
	private:
		virtual void OnCreateScene(PLScene::SceneContainer &cContainer) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float m_fMousePickingPullAnimation;	/**< Mouse picking pull animation */


};


#endif // __DUNGEON_APPLICATION_H__