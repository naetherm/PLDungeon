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
#include <PLGeneral/Tools/Timing.h>
#include <PLCore/Tools/Localization.h>
#include <PLScene/Compositing/SceneRenderer.h>
#include <PLScene/Scene/SPScene.h>
#include "Application.h"
#include "Interaction.h"
#include "MakingOf.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLRenderer;
using namespace PLScene;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MakingOf::MakingOf(Interaction &cInteraction) :
	m_pInteraction(&cInteraction),
	m_bPlaying(false),
	m_fTime(0.0f),
	m_nState(0),
	m_fStateTimeout(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
MakingOf::~MakingOf()
{
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
void MakingOf::StartPlayback()
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
					SceneRendererPass *pSceneRendererPass = pSceneRenderer->Get(i);

					// Backup the current scene renderer pass settings
					SSettings sSettings;
					sSettings.sName   = pSceneRendererPass->GetName();
					sSettings.sValues = pSceneRendererPass->ToString();
					m_lstInitialSceneRendererPassSettings.Add(sSettings);
				}
			}
		}
	}

	// Start at playback time 0 and state 0
	m_fTime			= 0.0f;
	m_nState		= 0;
	m_fStateTimeout = 5.0f;	// Wait 5 seconds, then go into the next state

	// Playback is now enabled
	m_bPlaying = true;
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
	// Playback?
	if (m_bPlaying) {
		// Get the current time difference
		float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

		#ifdef INTERNALRELEASE
			// Just half the timeout for the internal release - else we would have to wait to long to test the demo
			fTimeDiff *= 2.0f;
		#endif

		// Update the total playback time
		m_fTime += fTimeDiff;

		// Update the state timeout
		m_fStateTimeout -= fTimeDiff;
		if (m_fStateTimeout <= 0.0f) {
			// Next, please!
			m_nState++;

			// Set the initial state settings
			SetInitialSettings();

			// Get the application instance
			Application &cApplication = m_pInteraction->GetApplication();

			// Perform the state changes
			String sText;	// Text to show
			switch (m_nState) {
				// Making of start - should already be through if we're in here the first time...
				case 0:
					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;
					break;

				// Scene nodes - icons
				case 1:
					// Set the state text
					sText = PLT("A scene consists of nodes");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 2.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DebugSceneNodeIcons", "Flags", "");
					break;

				// Scene nodes - names
				case 2:
					// Set the state text
					sText = PLT("A scene consists of nodes");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DebugSceneNodeIcons", "Flags", "");
					cApplication.GetSceneRendererTool().SetPassAttribute("DebugSceneNodeNames", "Flags", "");
					break;

				// Wireframes
				case 3:
					// Set the state text
					sText = PLT("A node can represent a mesh");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 8.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DebugWireframes", "Flags", "");
					break;

				// Deferred rendering - Albedo
				case 4:
					// Set the state text
					sText = PLT("Deferred rendering - Albedo");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Flags", "");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowAlbedo");
					break;

				// Deferred rendering - Ambient occlusion
				case 5:
					// Set the state text
					sText = PLT("Deferred rendering - Ambient occlusion");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Flags", "");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowAmbientOcclusion");
					break;

				// Deferred rendering - Normals
				case 6:
					// Set the state text
					sText = PLT("Deferred rendering - View space normals");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Flags", "");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowNormals");
					break;

				// Deferred rendering - Depth
				case 7:
					// Set the state text
					sText = PLT("Deferred rendering - Depth");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Flags", "");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowDepth");
					break;

				// Deferred rendering - Specular color
				case 8:
					// Set the state text
					sText = PLT("Deferred rendering - Specular color");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Flags", "");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowSpecularColor");
					break;

				// Deferred rendering - Specular exponent
				case 9:
					// Set the state text
					sText = PLT("Deferred rendering - Specular exponent");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Flags", "");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowSpecularExponent");
					break;

				// Deferred rendering - Self illumination
				case 10:
					// Set the state text
					sText = PLT("Deferred rendering - Self illumination");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Flags", "");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowSelfIllumination");
					break;

				// Deferred rendering - Glow
				case 11:
					// Set the state text
					sText = PLT("Deferred rendering - Glow");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Flags", "");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowGlow");
					break;

				// Ambient
				case 12:
					// Set the state text
					sText = PLT("Ambient");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredSPSSAO",			"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGlow",			"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredLighting",		"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGodRays",			"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDOF",				"Flags", "Inactive");
					break;

				// Ambient + SSAO
				case 13:
					// Set the state text
					sText = PLT("Ambient + SSAO");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGlow",			"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredLighting",		"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGodRays",			"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDOF",				"Flags", "Inactive");
					break;

				// Ambient + SSAO + Lighting
				case 14:
					// Set the state text
					sText = PLT("Ambient + SSAO + Lighting");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGlow",			"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredLighting",		"Flags", "NoShadow");
					cApplication.GetSceneRendererTool().SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGodRays",			"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDOF",				"Flags", "Inactive");
					break;

				// Ambient + SSAO + Lighting + Shadows
				case 15:
					// Set the state text
					sText = PLT("Ambient + SSAO + Lighting + Shadows");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGlow",			"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGodRays",			"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDOF",				"Flags", "Inactive");
					break;

				// Ambient + SSAO + Lighting + Shadows + Glow
				case 16:
					// Set the state text
					sText = PLT("Ambient + SSAO + Lighting + Shadows + Glow");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive");
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDOF",				"Flags", "Inactive");
					break;

				// Ambient + SSAO + Lighting + Shadows + Glow + Fog
				case 17:
					// Set the state text
					sText = PLT("Ambient + SSAO + Lighting + Shadows + Glow + Fog");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredDOF",	"Flags", "Inactive");
					break;

				// Ambient + SSAO + Lighting + Shadows + Glow + Fog + DOF
				case 18:
					// Set the state text
					sText = PLT("Ambient + SSAO + Lighting + Shadows + Glow + Fog + DOF");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;
					break;

				// Texture maps: No maps
				case 19:
					// Set the state text
					sText = PLT("Texture maps: No maps");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBuffer", "Flags", "NoDiffuseMap|NoSpecularMap|NoNormalMap|NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap");
					break;

				// Texture maps: Diffuse
				case 20:
					// Set the state text
					sText = PLT("Texture maps: Diffuse");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBuffer", "Flags", "NoSpecularMap|NoNormalMap|NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap");
					break;

				// Texture maps: Diffuse + Specular
				case 21:
					// Set the state text
					sText = PLT("Texture maps: Diffuse + Specular");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBuffer", "Flags", "NoNormalMap|NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap");
					break;

				// Texture maps: Diffuse + Specular + Normal
				case 22:
					// Set the state text
					sText = PLT("Texture maps: Diffuse + Specular + Normal");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap");
					break;

				// Texture maps: Diffuse + Specular + Normal + Height
				case 23:
					// Set the state text
					sText = PLT("Texture maps: Diffuse + Specular + Normal + Height");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap");
					break;

				// Texture maps: Diffuse + Specular + Normal + Height + Emissive
				case 24:
					// Set the state text
					sText = PLT("Texture maps: Diffuse + Specular + Normal + Height + Emissive");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoAmbientOcclusionMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap");
					break;

				// Texture maps: Diffuse + Specular + Normal + Height + Emissive + Reflection
				case 25:
					// Set the state text
					sText = PLT("Texture maps: Diffuse + Specular + Normal + Height + Emissive + Reflection");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoAmbientOcclusionMap|NoLightMap");
					break;

				// HDR: off
				case 26:
					// Set the state text
					sText = PLT("HDR: Off");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("EndHDR", "Flags", "Inactive");
					break;

				// HDR: Reinhard tone mapping
				case 27:
					// Set the state text
					sText = PLT("HDR: Reinhard tone mapping");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("EndHDR", "Flags", "NoAutomaticAverageLuminance|NoLightAdaptation|NoBloom");
					break;

				// HDR: Reinhard tone mapping + Automatic light adaptation
				case 28:
					// Set the state text
					sText = PLT("HDR: Reinhard tone mapping + Automatic light adaptation");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;

					// Set state settings
					cApplication.GetSceneRendererTool().SetPassAttribute("EndHDR", "Flags", "NoBloom");
					break;

				// HDR: Reinhard tone mapping + Automatic light adaptation + Bloom
				case 29:
					// Set the state text
					sText = PLT("HDR: Reinhard tone mapping + Automatic light adaptation + Bloom");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 5.0f;
					break;

				// Making of end 1 
				case 30:
					// Set the state text
					sText = PLT("That's all folks - visit us at www.pixellight.org");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 10.0f;
					break;

				// Making of end 2
				case 31:
					// Set the state text
					sText = PLT("Have fun with the dungeon exploration! (space mouse supported)");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 7.0f;
					break;

				// Making of end 3
				case 32:
					// Set the state text
					sText = PLT("Use the keys 1-5 to change the demo mode");

					// Wait a few seconds, then go into the next state
					m_fStateTimeout = 6.0f;
					break;

				// Making of finished
				default:
					// Stop the playback
					StopPlayback();

					// Emit the playback finished event
					EventPlaybackFinished.Emit();
					break;
			}

			// Show the text
			if (sText.GetLength())
				m_pInteraction->GetApplication().ShowText(sText, m_fStateTimeout + 1.0f);	// +1 second so the text don't get invisible when showing twice
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
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
				SceneRendererPass *pSceneRendererPass = pSceneRenderer->Get(sSettings.sName);
				if (pSceneRendererPass) {
					// Set values
					pSceneRendererPass->FromString(sSettings.sValues);
				}
			}
		}
	}
}
