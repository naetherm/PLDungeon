/*********************************************************\
 *  File: MakingOf.cpp                                   *
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
#include <PLScript/Script.h>
#include <PLScript/FuncScriptPtr.h>
#include <PLScript/ScriptManager.h>
#include <PLScene/Compositing/SceneRenderer.h>
#include <PLScene/Scene/SPScene.h>
#include "Application.h"
#include "Interaction.h"
#include "MakingOf.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLCore;
using namespace PLScript;
using namespace PLRenderer;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(MakingOf)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the initial state settings
*/
void MakingOf::SetInitialSettings()
{
	// Get the painter
	SurfacePainter *pPainter = m_pInteraction->GetApplication().GetPainter();
	if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
		// Get the default scene renderer
		SceneRenderer *pSceneRenderer = static_cast<SPScene*>(pPainter)->GetDefaultSceneRenderer();
		if (pSceneRenderer) {
			// Loop through all scene renderer passes
			for (uint32 i=0; i<m_lstInitialSceneRendererPassSettings.GetNumOfElements(); i++) {
				// Get the settings
				const SSettings &sSettings = m_lstInitialSceneRendererPassSettings[i];

				// Get the scene renderer pass
				SceneRendererPass *pSceneRendererPass = pSceneRenderer->GetByName(sSettings.sName);
				if (pSceneRendererPass) {
					// Set values
					pSceneRendererPass->FromString(sSettings.sValues);
				}
			}
		}
	}
}

/**
*  @brief
*    Shows a text
*/
void MakingOf::ShowText(String sText, float fTimeout)
{
	m_pInteraction->GetApplication().ShowText(sText, fTimeout);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MakingOf::MakingOf(Interaction &cInteraction) :
	SignalPlaybackFinished(this),
	m_pInteraction(&cInteraction),
	m_bPlaying(false),
	m_pScript(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
MakingOf::~MakingOf()
{
	// Stop the playback
	StopPlayback();

	// Destroy the used script
	if (m_pScript) {
		delete m_pScript;
		m_pScript = nullptr;
	}
}

/**
*  @brief
*    Returns the owner interaction
*/
Interaction &MakingOf::GetInteraction() const
{
	return *m_pInteraction;
}

/**
*  @brief
*    Starts the playback
*/
void MakingOf::StartPlayback(const String &sScriptFilename)
{
	// Stop the previous playback
	StopPlayback();

	{ // Backup the initial scene renderer settings
		m_lstInitialSceneRendererPassSettings.Clear();

		// Get the painter
		SurfacePainter *pPainter = m_pInteraction->GetApplication().GetPainter();
		if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
			// Get the default scene renderer
			SceneRenderer *pSceneRenderer = static_cast<SPScene*>(pPainter)->GetDefaultSceneRenderer();
			if (pSceneRenderer) {
				// Loop through all scene renderer passes
				for (uint32 i=0; i<pSceneRenderer->GetNumOfElements(); i++) {
					// Get the scene renderer pass
					SceneRendererPass *pSceneRendererPass = pSceneRenderer->GetByIndex(i);

					// Backup the current scene renderer pass settings
					SSettings sSettings;
					sSettings.sName   = pSceneRendererPass->GetName();
					sSettings.sValues = pSceneRendererPass->ToString();
					m_lstInitialSceneRendererPassSettings.Add(sSettings);
				}
			}
		}
	}

	// Playback is now enabled
	m_bPlaying = true;

	// Destroy the used script
	if (m_pScript) {
		delete m_pScript;
		m_pScript = nullptr;
	}

	// Create the script instance
	m_pScript = ScriptManager::GetInstance()->CreateFromFile(sScriptFilename);
	if (m_pScript) {
		// Add the global variable "this" to the script so that it's able to access "this" RTTI class instance
		m_pScript->SetGlobalVariable("this", Var<Object*>(this));

		#ifdef INTERNALRELEASE
			// Just half the timeout for the internal release - else we would have to wait to long to test the demo
			m_pScript->SetGlobalVariable("timeScale", Var<float>(2.0f));
		#endif
	}
}

/**
*  @brief
*    Returns whether or not playback is currently active
*/
bool MakingOf::IsPlaying() const
{
	return m_bPlaying;
}

/**
*  @brief
*    Stops the playback
*/
void MakingOf::StopPlayback()
{
	// Is playback currently enabled?
	if (m_bPlaying) {
		// Set the initial state settings
		SetInitialSettings();

		// Playback is no longer enabled
		m_bPlaying = false;
	}
}

/**
*  @brief
*    Updates the making of component
*/
void MakingOf::Update()
{
	// Playback a script?
	if (m_bPlaying && m_pScript) {
		// Call the update script function
		FuncScriptPtr<void>(m_pScript, "Update").Call(Params<void>());
	}
}
