/*********************************************************\
 *  File: Camcorder.h                                    *
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


#ifndef __DUNGEON_CAMCORDER_H__
#define __DUNGEON_CAMCORDER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLScene/Scene/SceneNodeHandler.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Application;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Camcorder interaction component
*
*  @note
*    - Do not perform playback if the current camera is using physics...
*/
class Camcorder : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, Camcorder, "", PLCore::Object, "Camcorder interaction component")
		// Methods
		pl_method_1(StartRecord,	pl_ret_type(void),	const PLGeneral::String&,	"Starts the record, record name as first parameter (if empty string, no recording can be started). The currently used application camera will be recorded. If playback is currently enabled, the playback will be stopped at once.",	"")
		pl_method_0(IsRecording,	pl_ret_type(bool),								"Returns whether or not recording is currently active. Returns 'true' if recording is currently active, else 'false'.",																													"")
		pl_method_0(StopRecord,		pl_ret_type(void),								"Stops the record",																																																						"")
		pl_method_1(StartPlayback,	pl_ret_type(void),	const PLGeneral::String&,	"Starts the playback, record name as first parameter (must be valid). The currently used application camera will be feed with the recorded data. If recording is currently enabled, the recording will be stopped at once.",			"")
		pl_method_0(IsPlaying,		pl_ret_type(bool),								"Returns whether or not playback is currently active. Returns 'true' if playback is currently active, else 'false'.",																													"")
		pl_method_0(StopPlayback,	pl_ret_type(void),								"Stops the playback",																																																					"")
		pl_method_0(Update,			pl_ret_type(void),								"Updates the camcorder component",																																																		"")
		// Signals
		pl_signal_0(SignalPlaybackFinished,	"Playback has been finished",	"")
	pl_class_end


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
		Camcorder(Application &cApplication);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Camcorder();

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
		*    Starts the record
		*
		*  @param[in] sName
		*    Record name, if empty string, no recording can be started
		*
		*  @note
		*    - The currently used application camera will be recorded
		*    - If playback is currently enabled, the playback will be stopped at once
		*/
		void StartRecord(const PLGeneral::String &sName);

		/**
		*  @brief
		*    Returns whether or not recording is currently active
		*
		*  @return
		*    'true' if recording is currently active, else 'false'
		*/
		bool IsRecording() const;

		/**
		*  @brief
		*    Stops the record
		*/
		void StopRecord();

		/**
		*  @brief
		*    Starts the playback
		*
		*  @param[in] sName
		*    Record name, must be valid
		*
		*  @note
		*    - The currently used application camera will be feed with the recorded data
		*    - If recording is currently enabled, the recording will be stopped at once
		*/
		void StartPlayback(const PLGeneral::String &sName);

		/**
		*  @brief
		*    Returns whether or not playback is currently active
		*
		*  @return
		*    'true' if playback is currently active, else 'false'
		*/
		bool IsPlaying() const;

		/**
		*  @brief
		*    Stops the playback
		*/
		void StopPlayback();

		/**
		*  @brief
		*    Updates the camcorder component
		*/
		void Update();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Activates/deactivates certain scene node modifers of the given scene node
		*
		*  @param[in] cSceneNode
		*    Scene node
		*  @param[in] bActive
		*    New scene node modifier active state
		*/
		void SetupSceneNodeModifiers(PLScene::SceneNode &cSceneNode, bool bActive);

		/**
		*  @brief
		*    Called when one of the keyframe animations has been stopped
		*/
		void OnAnimationStop();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerAnimationStop;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Application				  *m_pApplication;					/**< Owner application, always valid! */
		bool					   m_bRecording;					/**< Is recording currently enabled? */
		bool					   m_bPlaying;						/**< Is playback currently enabled? */
		bool					   m_bPlaybackFinished;				/**< Playback finished? */
		PLGeneral::String		   m_sRecordName;					/**< Current record name */
		PLScene::SceneNodeHandler  m_cCameraSceneNodeHandler;		/**< Recorded camera scene node handler */
		PLGeneral::String		   m_sCameraStartSceneContainer;	/**< Camera start scene container */
		PLGeneral::String		   m_sCameraStartName;				/**< Camera start name */


};


#endif // __DUNGEON_CAMCORDER_H__
