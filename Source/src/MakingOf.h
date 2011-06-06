/*********************************************************\
 *  File: MakingOf.h                                     *
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


#ifndef __DUNGEON_MAKINGOF_H__
#define __DUNGEON_MAKINGOF_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLScript {
	class Script;
}
class Interaction;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Making of interaction component
*/
class MakingOf : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, MakingOf, "", PLCore::Object, "Making of interaction component")
		pl_signal_0(SignalPlaybackFinished, "Playback has been finished", "")
		pl_method_0(SetInitialSettings, void, "Sets the initial state settings", "")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sets the initial state settings
		*/
		void SetInitialSettings();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cInteraction
		*    Owner interaction
		*/
		MakingOf(Interaction &cInteraction);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MakingOf();

		/**
		*  @brief
		*    Returns the owner interaction
		*
		*  @return
		*    The owner interaction
		*/
		Interaction &GetInteraction() const;

		/**
		*  @brief
		*    Starts the playback
		*
		*  @param[in] sScriptFilename
		*    Filename of the script to use
		*/
		void StartPlayback(const PLGeneral::String &sScriptFilename = "Data/Scripts/MakingOf.lua");

		/**
		*  @brief
		*    Returns whether or not playback is currently active
		*/
		bool IsPlaying() const;

		/**
		*  @brief
		*    Stops the playback
		*/
		void StopPlayback();

		/**
		*  @brief
		*    Updates the making of component
		*/
		void Update();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Interaction			*m_pInteraction;	/**< Owner interaction, always valid! */
		bool				 m_bPlaying;		/**< Is playback currently enabled? */
		PLScript::Script	*m_pScript;			/**< The used script, can be a null pointer */
		class SSettings {
			public:
				PLGeneral::String sName;	// Scene renderer pass name
				PLGeneral::String sValues;	// Values as string
				bool operator ==(const SSettings &sSettings) { return false; }	// No implementation required
		};
		PLGeneral::Array<SSettings>	 m_lstInitialSceneRendererPassSettings;	/**< Initial scene renderer pass settings */


};


#endif // __DUNGEON_MAKINGOF_H__
