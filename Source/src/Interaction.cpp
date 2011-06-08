/*********************************************************\
 *  File: Interaction.cpp                                *
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
#include <PLCore/Tools/Localization.h>
#include <PLGeneral/Tools/Timing.h>
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Base/Keys.h>
#include <PLGui/Widgets/Widget.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLPhysics/Body.h>
#include <PLPhysics/SceneNodeModifiers/SNMPhysicsBodyBox.h>
#include <PLPhysics/SceneNodes/SNPhysicsMouseInteraction.h>
#include "Application.h"
#include "Gui/IngameGui.h"
#include "Camcorder.h"
#include "MakingOf.h"
#include "Interaction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLMath;
using namespace PLGui;
using namespace PLScene;
using namespace PLPhysics;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Interaction::Interaction(Application &cApplication) :
	EventHandlerMouseMove				(&Interaction::NotifyMouseMove,					this),
	EventHandlerMouseButtonDown			(&Interaction::NotifyMouseButtonDown,			this),
	EventHandlerMouseButtonUp			(&Interaction::NotifyMouseButtonUp,				this),
	EventHandlerKeyDown					(&Interaction::NotifyKeyDown,					this),
	EventHandlerMoviePlaybackFinished	(&Interaction::NotifyMoviePlaybackFinished,		this),
	EventHandlerMakingOfPlaybackFinished(&Interaction::NotifyMakingOfPlaybackFinished,	this),
	m_pApplication(&cApplication),
	m_nMode(UnknownMode),
	m_nModeBackup(UnknownMode),
	m_bLeftMouseButtonDown(false),
	m_pIngameGui(new IngameGui(*this)),
	m_pCamcorder(new Camcorder(*this)),
	m_pMakingOf(new MakingOf(*this)),
	m_fMousePickingPullAnimation(0.0f),
	m_fGUIBackgroundBlur(0.0f)
{
	// Connect the camcorder event handler
	m_pCamcorder->EventPlaybackFinished.Connect(&EventHandlerMoviePlaybackFinished);

	// Connect the making of event handler
	m_pMakingOf->SignalPlaybackFinished.Connect(&EventHandlerMakingOfPlaybackFinished);

	// Get the scene container
	SceneContainer *pScene = m_pApplication->GetScene();
	if (pScene) {
		// Get references to important scene nodes - because they move through the scene, they may
		// change their scene container and therefore change their name....

		// Walk camera scene node handler
		m_cWalkCameraSceneNodeHandler.SetElement(pScene->GetByName("Container.kanal3.WalkCamera"));

		// Free camera scene node handler
		m_cFreeCameraSceneNodeHandler.SetElement(pScene->GetByName("Container.kanal3.FreeCamera"));

		// Ghost camera scene node handler
		m_cGhostCameraSceneNodeHandler.SetElement(pScene->GetByName("Container.kanal3.GhostCamera"));

		// Making of camera scene node handler
		m_cMakingOfCameraSceneNodeHandler.SetElement(pScene->GetByName("Container.WineCellar.MakingOfCamera"));
	}

	// The offical release should always start with the movie mode
	#ifdef INTERNALRELEASE
		SetMode(MovieMode, false);
		// [TEST]
	//	SetMode(MakingOfMode);
	//	SetMode(WalkMode);
	#else
		// Set movie but, but do not show the mode changed text
		SetMode(MovieMode, false);

		// Show an initial help text for 10 seconds
		if (!m_pApplication->IsExpertMode())
			m_pApplication->ShowText(PLT("Press ESC to open menu"), 10.0f);
	#endif

	// Get the main window of the application
	Widget *pWidget = m_pApplication->GetMainWindow();
	if (pWidget) {
		// Connect event handler
		pWidget->GetContentWidget()->SignalMouseMove.	   Connect(&EventHandlerMouseMove);
		pWidget->GetContentWidget()->SignalMouseButtonDown.Connect(&EventHandlerMouseButtonDown);
		pWidget->GetContentWidget()->SignalMouseButtonUp.  Connect(&EventHandlerMouseButtonUp);
		pWidget->SignalKeyDown.Connect(&EventHandlerKeyDown);
		// [TODO] Linux: Currently we need to listen to the content widget key signals as well ("focus follows mouse"-topic)
		if (pWidget->GetContentWidget() != pWidget)
			pWidget->GetContentWidget()->SignalKeyDown.Connect(&EventHandlerKeyDown);
	}

	// By default, the mouse cursor is visible
	SetMouseVisible(true);
}

/**
*  @brief
*    Destructor
*/
Interaction::~Interaction()
{
	delete m_pIngameGui;
	delete m_pCamcorder;
	delete m_pMakingOf;
}

/**
*  @brief
*    Returns the owner application
*/
Application &Interaction::GetApplication() const
{
	return *m_pApplication;
}

/**
*  @brief
*    Returns the current mode
*/
Interaction::EMode Interaction::GetMode() const
{
	return m_nMode;
}

/**
*  @brief
*    Sets the current mode
*/
void Interaction::SetMode(EMode nMode, bool bText)
{
	// Mode change?
	if (m_nMode != nMode) {
		// Was camcorder playback active?
		if (m_pCamcorder->IsPlaying()) {
			// Stop the playback
			m_pCamcorder->StopPlayback();
		}

		// Was making of playback active?
		if (m_pMakingOf->IsPlaying()) {
			// Stop the playback
			m_pMakingOf->StopPlayback();
		}

		// Set new mode
		m_nMode = nMode;

		// Get new camera scene node and a short text
		String sModeChangedText;
		SceneNode *pCameraNewSceneNode = nullptr;
		switch (m_nMode) {
			case WalkMode:
				sModeChangedText = PLT("Walk mode");
				pCameraNewSceneNode = m_cWalkCameraSceneNodeHandler.GetElement();
				break;

			case FreeMode:
				sModeChangedText = PLT("Free mode");
				pCameraNewSceneNode = m_cFreeCameraSceneNodeHandler.GetElement();
				break;

			case GhostMode:
				sModeChangedText = PLT("Ghost mode");
				pCameraNewSceneNode = m_cGhostCameraSceneNodeHandler.GetElement();
				break;

			case MovieMode:
				sModeChangedText = PLT("Movie mode");

				// Use the ghost camera for the movie
				pCameraNewSceneNode = m_cGhostCameraSceneNodeHandler.GetElement();
				break;

			case MakingOfMode:
				sModeChangedText = PLT("Making of mode");
				pCameraNewSceneNode = m_cMakingOfCameraSceneNodeHandler.GetElement();
				break;
		}

		// Show mode changed text?
		if (bText && !m_pApplication->IsExpertMode())
			m_pApplication->ShowText(sModeChangedText);

		// Set camera...
		if (pCameraNewSceneNode) {
			// If not movie or making of mode...
			if (m_nMode != MovieMode && m_nMode != MakingOfMode) {
				// Get the currently set camera scene node
				SceneNode *pCurrentCameraSceneNode = reinterpret_cast<SceneNode*>(m_pApplication->GetCamera());

				// Place the new camera into the same scene container as the old camera
				if (pCurrentCameraSceneNode->GetContainer())
					pCameraNewSceneNode->SetContainer(*pCurrentCameraSceneNode->GetContainer());

				// Give the new camera the position and rotation of the old camera
				pCameraNewSceneNode->GetTransform().SetPosition(pCurrentCameraSceneNode->GetTransform().GetPosition());
				pCameraNewSceneNode->GetTransform().SetRotation(pCurrentCameraSceneNode->GetTransform().GetRotation());
			}

			// Set the new application camera
			m_pApplication->SetCamera(reinterpret_cast<SNCamera*>(pCameraNewSceneNode));

			{ // If there's a physics body attached to the camera, reset physics properties to avoid or at least reduct uggly jumps when changing the camera...
				// Get the PL physics body scene node modifier of the given scene node
				const SNMPhysicsBody *pModifier = static_cast<SNMPhysicsBody*>(pCameraNewSceneNode->GetModifier("PLPhysics::SNMPhysicsBody"));
				if (pModifier) {
					// Get the PL physics body this modifier is using
					Body *pBody = pModifier->GetBody();
					if (pBody) {
						// Reset the force, torque, linear and angular velocity
						pBody->SetForce(Vector3::Zero);
						pBody->SetTorque(Vector3::Zero);
						pBody->SetLinearVelocity(Vector3::Zero);
						pBody->SetAngularVelocity(Vector3::Zero);
					}
				}
			}

			// Start the movie?
			if (m_nMode == MovieMode) {
				// Start the playback
				#ifdef INTERNALRELEASE
					// Just a short movie for the internal release - else we would have to wait to long to test the demo
					m_pCamcorder->StartPlayback("ShortMovie");
				#else
					m_pCamcorder->StartPlayback("Movie");
				#endif
			} else {
				// Start the making of?
				if (m_nMode == MakingOfMode) {
					// Start the playback
					m_pMakingOf->StartPlayback();
				}
			}
		}
	}
}

/**
*  @brief
*    Updates the interaction component
*/
void Interaction::Update()
{
	// Update the ingame GUI
	m_pIngameGui->Update();

	// Update the camcorder
	m_pCamcorder->Update();

	// Update the making of
	m_pMakingOf->Update();

	// Get the currently set camera scene node
	SceneNode *pCameraSceneNode = reinterpret_cast<SceneNode*>(m_pApplication->GetCamera());
	if (pCameraSceneNode) {
		// Deactivate the camera if the GUI is currently shown
		pCameraSceneNode->SetActive(!m_pIngameGui->IsGuiShown());
	}

	// Update the mouse picking pull animation
	UpdateMousePickingPullAnimation();

	// Update the GUI background blur
	UpdateGUIBackgroundBlur();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Updates the mouse picking pull animation
*/
void Interaction::UpdateMousePickingPullAnimation()
{
	// Get the current time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Update the mouse picking pull animation
	m_fMousePickingPullAnimation += fTimeDiff*5;

	// Get the scene
	SceneContainer *pSceneContainer = m_pApplication->GetScene();
	if (pSceneContainer) {
		// Get the "PLPhysics::SNPhysicsMouseInteraction" instance
		SceneNode *pSceneNode = pSceneContainer->GetByName("Container.SNPhysicsMouseInteraction");
		if (pSceneNode && pSceneNode->IsInstanceOf("PLPhysics::SNPhysicsMouseInteraction")) {
			SNPhysicsMouseInteraction *pSNPhysicsMouseInteraction = static_cast<SNPhysicsMouseInteraction*>(pSceneNode);

			// Get the currently set camera scene node
			SceneNode *pCameraSceneNode = reinterpret_cast<SceneNode*>(m_pApplication->GetCamera());
			if (pCameraSceneNode) {
				// Is picking currently performed?
				Vector2i vMousePos;
				if (pSNPhysicsMouseInteraction->IsPicking(&vMousePos)) {
					// Update "PLPostProcessEffects::SNMPostProcessPull" modifier
					SceneNodeModifier *pSceneNodeModifier = pCameraSceneNode->GetModifier("PLPostProcessEffects::SNMPostProcessPull");
					if (!pSceneNodeModifier)
						pSceneNodeModifier = pCameraSceneNode->AddModifier("PLPostProcessEffects::SNMPostProcessPull");
					if (pSceneNodeModifier) {
						// Get the main window of the application
						Widget *pWidget = m_pApplication->GetMainWindow();
						if (pWidget && pWidget->GetContentWidget()) {
							pSceneNodeModifier->SetAttribute("WarpPoint",	  String::Format("%d %d", vMousePos.x, pWidget->GetContentWidget()->GetSize().y-vMousePos.y));
							pSceneNodeModifier->SetAttribute("WarpScale",	  -5.0f  + Math::Sin(m_fMousePickingPullAnimation)*Math::Cos(m_fMousePickingPullAnimation/4)*10.0f);
							pSceneNodeModifier->SetAttribute("WarpDimension", 150.0f + Math::Cos(m_fMousePickingPullAnimation)*Math::Sin(m_fMousePickingPullAnimation/6)*60.0f);
						}
					}
				} else {
					// Remove "PLPostProcessEffects::SNMPostProcessPull" modifier
					pCameraSceneNode->RemoveModifier("PLPostProcessEffects::SNMPostProcessPull");
				}
			}
		}
	}
}

/**
*  @brief
*    Updates the the GUI background blur
*/
void Interaction::UpdateGUIBackgroundBlur()
{
	// Get the current time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Update the the GUI background blur depending on whether or not the ingame GUI is currently visible
	if (m_pIngameGui->IsGuiShown()) {
		m_fGUIBackgroundBlur += fTimeDiff;
		if (m_fGUIBackgroundBlur > 1.0f)
			m_fGUIBackgroundBlur = 1.0f;
	} else {
		m_fGUIBackgroundBlur -= fTimeDiff;
		if (m_fGUIBackgroundBlur < 0.0f)
			m_fGUIBackgroundBlur = 0.0f;
	}

	// Get the currently set camera scene node
	SceneNode *pCameraSceneNode = reinterpret_cast<SceneNode*>(m_pApplication->GetCamera());
	if (pCameraSceneNode) {
		// Is there currently GUI background blur?
		if (m_fGUIBackgroundBlur) {
			// Update "PLPostProcessEffects::SNMPostProcessBlur" modifier
			SceneNodeModifier *pSceneNodeModifier = pCameraSceneNode->GetModifier("PLPostProcessEffects::SNMPostProcessBlur");
			if (!pSceneNodeModifier)
				pSceneNodeModifier = pCameraSceneNode->AddModifier("PLPostProcessEffects::SNMPostProcessBlur");
			if (pSceneNodeModifier) {
				const float fFactor = static_cast<float>(Math::Sin(m_fGUIBackgroundBlur*Math::PiHalf));
				pSceneNodeModifier->SetAttribute("EffectWeight", fFactor);
				pSceneNodeModifier->SetAttribute("BloomScale",   0.8f + (1.0f-fFactor)*3.0f);
				pSceneNodeModifier->SetAttribute("Strength",     String::Format("%f %f", 1.0f+fFactor*3, 1.0f+fFactor*4));
			}
		} else {
			// Remove "PLPostProcessEffects::SNMPostProcessBlur" modifier
			pCameraSceneNode->RemoveModifier("PLPostProcessEffects::SNMPostProcessBlur");
		}
	}
}

/**
*  @brief
*    Set mouse cursor visibility
*/
void Interaction::SetMouseVisible(bool bVisible)
{
	// Do not hide the mouse cursor if the GUI is currently visible!
	if (m_pIngameGui->IsGuiShown())
		bVisible = true;

	// Set mouse cursor visibility
	Gui::GetSystemGui()->SetMouseVisible(bVisible);

	// Get the main window of the application
	Widget *pWidget = m_pApplication->GetMainWindow();
	if (pWidget) {
		// Trap mouse inside the content widget when the mouse cursor is invisible
		pWidget->GetContentWidget()->SetTrapMouse(!bVisible);
	}
}

/**
*  @brief
*    Called when the mouse is moved
*/
void Interaction::NotifyMouseMove(const Vector2i &vPos)
{
	if (m_bLeftMouseButtonDown) {
		// Hide the mouse cursor - we don't want to have one during look around
		SetMouseVisible(false);
	}
}

/**
*  @brief
*    Called when a mouse button is pressed
*/
void Interaction::NotifyMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	// Left mouse button
	if (nButton == 0)
		m_bLeftMouseButtonDown = true;
}

/**
*  @brief
*    Called when a mouse button is released
*/
void Interaction::NotifyMouseButtonUp(uint32 nButton, const Vector2i &vPos)
{
	// Left mouse button
	if (nButton == 0) {
		m_bLeftMouseButtonDown = false;

		// Show the mouse cursor
		SetMouseVisible(true);
	}
}

/**
*  @brief
*    Called when a key is pressed down
*/
void Interaction::NotifyKeyDown(uint32 nKey, uint32 nModifiers)
{
	switch (nKey) {
		case PLGUIKEY_ESCAPE:
			if (m_pIngameGui->IsGuiShown()) {
				m_pIngameGui->Hide();
			} else {
				// Toggle menu visibility
				m_pIngameGui->ShowMenu(!m_pIngameGui->IsMenuShown());

				// Show the mouse cursor?
				if (m_pIngameGui->IsMenuShown())
					SetMouseVisible(true);
			}
			break;

		case PLGUIKEY_1:
			SetMode(WalkMode);
			break;

		case PLGUIKEY_2:
			SetMode(FreeMode);
			break;

		case PLGUIKEY_3:
			SetMode(GhostMode);
			break;

		case PLGUIKEY_4:
			SetMode(MovieMode);
			break;

		case PLGUIKEY_5:
			SetMode(MakingOfMode);
			break;

		case PLGUIKEY_F12:
			// Make a screenshot from the current render target
			m_pApplication->GetScreenshotTool().SaveScreenshot(m_pApplication->GetScreenshotTool().GetScreenshotFilename());
			break;
	}

	#ifdef INTERNALRELEASE
		// If not movie or making of mode...
		if (m_nMode != MovieMode && m_nMode != MakingOfMode) {
			switch (nKey) {
				case PLGUIKEY_R:
					// Toggle camcorder recording
					if (m_pCamcorder->IsRecording())
						m_pCamcorder->StopRecord();
					else
						m_pCamcorder->StartRecord("Test");
					break;

				case PLGUIKEY_P:
					// Toggle camcorder playback
					if (m_pCamcorder->IsPlaying()) {
						// Stop the playback
						m_pCamcorder->StopPlayback();

						// Restore previously set mode
						SetMode(m_nModeBackup);
					} else {
						// Backup the current set mode
						m_nModeBackup = m_nMode;

						// Set to ghost mode
						SetMode(GhostMode);

						// Start the playback
						m_pCamcorder->StartPlayback("Test");
					}
					break;
			}
		}
	#endif
}

/**
*  @brief
*    Called when the camcorder playback has been finished
*/
void Interaction::NotifyMoviePlaybackFinished()
{
	// Change into the making of mode
	SetMode(MakingOfMode);
}

/**
*  @brief
*    Called when the making playback has been finished
*/
void Interaction::NotifyMakingOfPlaybackFinished()
{
	// Change into the free mode or start the movie again
	SetMode(m_pApplication->IsRepeatMode() ? MovieMode : FreeMode);
}
