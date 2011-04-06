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
#include <PLCore/Base/Event/Event.h>
#include <PLGeneral/String/String.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Interaction;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Making of interaction component
*/
class MakingOf {


	//[-------------------------------------------------------]
	//[ Events                                                ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<>	EventPlaybackFinished;	/**< Playback has been finished */


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
		*/
		void StartPlayback();

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
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Sets the initial state settings
		*/
		void SetInitialSettings();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Interaction							*m_pInteraction;						/**< Owner interaction, always valid! */
		bool								 m_bPlaying;							/**< Is playback currently enabled? */
		float								 m_fTime;								/**< Total playback time (in seconds) */
		PLGeneral::uint32					 m_nState;								/**< Current state (changes over playback time) */
		float								 m_fStateTimeout;						/**< State timeout until next state (in seconds) */
		class SSettings {
			public:
				PLGeneral::String sName;	// Scene renderer pass name
				PLGeneral::String sValues;	// Values as string
				bool operator ==(const SSettings &sSettings) { return false; }	// No implementation required
		};
		PLGeneral::Array<SSettings>	 m_lstInitialSceneRendererPassSettings;	/**< Initial scene renderer pass settings */


};


#endif // __DUNGEON_MAKINGOF_H__
