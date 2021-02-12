/*********************************************************\
 *  File: Application.cpp                                *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Class.h>
#include <PLCore/Script/Script.h>
#include <PLCore/System/System.h>
#include <PLCore/Tools/Timing.h>
#include <PLCore/Tools/Localization.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLRenderer/Material/ParameterManager.h>
#include <PLScene/Compositing/SceneRenderer.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneNodeModifier.h>
#include <PLEngine/Compositing/Console/SNConsoleBase.h>
#include <PLEngine/Controller/SNPhysicsMouseInteraction.h>
#include "Application.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
using namespace PLEngine;
using namespace PLPhysics;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
	pl_class_metadata(Application, "", PLEngine::ScriptApplication, "Application class")
		// Constructors
		pl_constructor_1_metadata(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
		// Methods
		pl_method_0_metadata(IsExpertMode,						pl_ret_type(bool),	"Returns whether or not the application runs within the expert mode. Returns 'true' if the application runs within the expert mode, else 'false' (no additional help texts).",															"")
		pl_method_0_metadata(IsRepeatMode,						pl_ret_type(bool),	"Returns whether or not the application runs within the repeat mode. Returns 'true' if the application runs within the repeat mode (\"movie -> making of -> movie\" instead of \"movie -> making of -> interactive\"), else 'false'.",	"")
		pl_method_0_metadata(IsInternalRelease,					pl_ret_type(bool),	"Returns whether or not this is an internal release. Returns 'true' if this is an internal release, else 'false'.",																														"")
		pl_method_0_metadata(UpdateMousePickingPullAnimation,	pl_ret_type(void),	"Updates the mouse picking pull animation",																																																"")
		// Signals
		pl_signal_2_metadata(SignalSetMode,	PLCore::uint32,	bool,	"Signal indicating that a new interaction mode has been chosen, mode index as first parameter(0 = Walk mode, 1 = Free mode, 2 = Ghost mode, 3 = Movie mode, 4 = Making of mode), 'true' as second parameter to show mode changed text",	"")
	pl_class_metadata_end(Application)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application::Application(Frontend &cFrontend) : ScriptApplication(cFrontend, "Data/Scripts/Lua/Main.lua", "Dungeon", PLT("PixelLight dungeon demo"), System::GetInstance()->GetDataDirName("PixelLight")),
	m_fMousePickingPullAnimation(0.0f)
{
	// The demo is published as a simple archive, so, put the log and configuration files in the same directory the executable is
	// in - as a result, the user only has to remove this directory and the demo is completly gone from the system :D
	SetMultiUser(false);

	// This application accepts all the standard parameters that are defined in the application
	// base class (such as --help etc.). The last parameter however is the filename to load, so add that.
	m_cCommandLine.AddFlag("Expert", "-e", "--expert", "Expert mode, no additional help texts", false);
	m_cCommandLine.AddFlag("Repeat", "-r", "--repeat", "If movie and making of is finished, start the movie again instead of switching to �nteractive mode", false);
}

/**
*  @brief
*    Destructor
*/
Application::~Application()
{
}

/**
*  @brief
*    Returns whether or not the application runs within the expert mode
*/
bool Application::IsExpertMode() const
{
	// Check 'Expert' commando line flag
	return m_cCommandLine.IsValueSet("Expert");
}

/**
*  @brief
*    Returns whether or not the application runs within the repeat mode
*/
bool Application::IsRepeatMode() const
{
	// Check 'Repeat' commando line flag
	return m_cCommandLine.IsValueSet("Repeat");
}

/**
*  @brief
*    Returns whether or not this is an internal release
*/
bool Application::IsInternalRelease() const
{
	#ifdef INTERNALRELEASE
		return true;
	#else
		return false;
	#endif
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Updates the mouse picking pull animation
*/
void Application::UpdateMousePickingPullAnimation()
{
	// Get the current time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Update the mouse picking pull animation
	m_fMousePickingPullAnimation += fTimeDiff*5;

	// Get the scene
	SceneContainer *pSceneContainer = GetScene();
	if (pSceneContainer) {
		// Get the "PLPhysics::SNPhysicsMouseInteraction" instance
		SceneNode *pSceneNode = pSceneContainer->GetByName("Container.SNPhysicsMouseInteraction");
		if (pSceneNode && pSceneNode->IsInstanceOf("PLEngine::SNPhysicsMouseInteraction")) {
			SNPhysicsMouseInteraction *pSNPhysicsMouseInteraction = static_cast<SNPhysicsMouseInteraction*>(pSceneNode);

			// Get the currently set camera scene node
			SceneNode *pCameraSceneNode = reinterpret_cast<SceneNode*>(GetCamera());
			if (pCameraSceneNode) {
				// Is picking currently performed?
				Vector2i vMousePos;
				if (pSNPhysicsMouseInteraction->IsPicking(&vMousePos)) {
					// Update "PLPostProcessEffects::SNMPostProcessPull" modifier
					SceneNodeModifier *pSceneNodeModifier = pCameraSceneNode->GetModifier("PLPostProcessEffects::SNMPostProcessPull");
					if (!pSceneNodeModifier)
						pSceneNodeModifier = pCameraSceneNode->AddModifier("PLPostProcessEffects::SNMPostProcessPull");
					if (pSceneNodeModifier) {
						pSceneNodeModifier->SetAttribute("WarpPoint",	  String::Format("%d %d", vMousePos.x, GetFrontend().GetHeight()-vMousePos.y));
						pSceneNodeModifier->SetAttribute("WarpScale",	  -5.0f  + Math::Sin(m_fMousePickingPullAnimation)*Math::Cos(m_fMousePickingPullAnimation/4)*10.0f);
						pSceneNodeModifier->SetAttribute("WarpDimension", 150.0f + Math::Cos(m_fMousePickingPullAnimation)*Math::Sin(m_fMousePickingPullAnimation/6)*60.0f);
					}
				} else {
					// Remove "PLPostProcessEffects::SNMPostProcessPull" modifier
					pCameraSceneNode->RemoveModifier("PLPostProcessEffects::SNMPostProcessPull");
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::CoreApplication functions   ]
//[-------------------------------------------------------]
void Application::OnInit()
{
	// Call base implementation
	ScriptApplication::OnInit();

	// Enable/disable edit mode
	SetEditModeEnabled(GetConfig().GetVar("DungeonConfig", "EditModeEnabled").GetBool());
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneApplication functions ]
//[-------------------------------------------------------]
void Application::OnCreateRootScene()
{
	// Get the scene context
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		// First, create the scene root container which holds the scene container with our 'concrete' scene within it
		SceneContainer *pRootContainer = pSceneContext->GetRoot() ? static_cast<SceneContainer*>(pSceneContext->GetRoot()->Create("PLSound::SCSound", "RootScene", "SoundAPI=\"" + GetConfig().GetVar("DungeonConfig", "SoundAPI") + '"')) : nullptr;
		if (!pRootContainer)
			pRootContainer = pSceneContext->GetRoot() ? static_cast<SceneContainer*>(pSceneContext->GetRoot()->Create("PLScene::SceneContainer", "RootScene")) : nullptr;
		if (pRootContainer) {
			// Protect this important container!
			pRootContainer->SetProtected(true);

			// Create a scene container with our 'concrete scene'
			SceneNode *pSceneContainerNode = pRootContainer->Create("PLScene::SceneContainer", "Scene");
			if (pSceneContainerNode && pSceneContainerNode->IsInstanceOf("PLScene::SceneContainer")) {
				SceneContainer *pSceneContainer = static_cast<SceneContainer*>(pSceneContainerNode);

				// Protect this important container!
				pSceneContainer->SetProtected(true);

				// Create the 'concrete scene'
				OnCreateScene(*pSceneContainer);
			}

			// Create scene node for engine information
			SceneNode *pSceneNode = pRootContainer->Create("PLEngine::SNEngineInformation");
			if (pSceneNode)
				pSceneNode->SetActive(m_bEditModeEnabled);

			// Create console scene node - using the console command 'timescale <value>' we
			// can change the scene time (slowdown or accelerate)
			pSceneNode = pRootContainer->Create("PLEngine::SNConsole");
			if (pSceneNode && pSceneNode->GetClass()->IsDerivedFrom("PLEngine::SNConsoleBase")) {
				SNConsoleBase *pConsole = static_cast<SNConsoleBase*>(pSceneNode);

				// Register default commands
				pConsole->RegisterCommand(0,	"quit",			"",	"",	Functor<void, ConsoleCommand &>(&EngineApplication::ConsoleCommandQuit, this));
				pConsole->RegisterCommand(0,	"exit",			"",	"",	Functor<void, ConsoleCommand &>(&EngineApplication::ConsoleCommandQuit, this));
				pConsole->RegisterCommand(0,	"bye",			"",	"",	Functor<void, ConsoleCommand &>(&EngineApplication::ConsoleCommandQuit, this));
				pConsole->RegisterCommand(0,	"logout",		"",	"",	Functor<void, ConsoleCommand &>(&EngineApplication::ConsoleCommandQuit, this));

				// Set active state
				pConsole->SetActive(m_bEditModeEnabled);
			}
		}

		// Set the root scene
		SetRootScene(pRootContainer);
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLEngine::EngineApplication functions  ]
//[-------------------------------------------------------]
bool Application::LoadScene(const String &sFilename)
{
	// Call base implementation
	const bool bResult = ScriptApplication::LoadScene(sFilename);

	// Get the renderer context
	RendererContext *pRendererContext = GetRendererContext();
	if (pRendererContext) {
		// Give the "DoorGlow" material an animated emissive map for a more impressive god rays effect and enhance the diffuse color for more glow
		Material *pMaterial = pRendererContext->GetMaterialManager().GetByName("Data\\Materials\\Dungeon\\DoorGlow.mat");
		if (pMaterial) {
			// Set an animated emissive map
			pMaterial->GetParameterManager().SetParameterString("EmissiveMap", "Data/Textures/Caust.tani");

			// Enhance the diffuse color for more glow
			pMaterial->GetParameterManager().SetParameter3f("DiffuseColor", 4.0f, 4.0f, 4.0f);
		}

		// Let the "ILB_texPak01_decal002" (mos) material glow
		pMaterial = pRendererContext->GetMaterialManager().GetByName("Data\\Materials\\Dungeon\\ILB_texPak01_decal002.mat");
		if (pMaterial) {
			// Set emissive map color
			pMaterial->GetParameterManager().SetParameter3f("EmissiveMapColor", 2.0f, 2.0f, 2.0f);

			// Set glow
			pMaterial->GetParameterManager().SetParameter1f("Glow", 4.0f);
		}
	}

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Private virtual PLEngine::EngineApplication functions ]
//[-------------------------------------------------------]
void Application::OnCreateScene(SceneContainer &cContainer)
{
	// Set scene container flags
	cContainer.SetFlags(SceneNode::NoCulling);

	// Setup scene surface painter
	SurfacePainter *pPainter = GetPainter();
	if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
		SPScene *pSPScene = static_cast<SPScene*>(pPainter);
		pSPScene->SetRootContainer(cContainer.GetContainer());
		pSPScene->SetSceneContainer(&cContainer);
	}

	// Set scene container
	SetScene(&cContainer);
}
