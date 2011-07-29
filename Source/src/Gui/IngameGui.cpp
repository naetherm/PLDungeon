/*********************************************************\
 *  File: IngameGui.cpp                                  *
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
#include <PLCore/Tools/Timing.h>
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Screen.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLEngine/Gui/RenderWindow.h>
#include <PLEngine/Compositing/Gui/SNGui.h>
#include "Application.h"
#include "Gui/WindowMenu.h"
#include "Gui/WindowText.h"
#include "Gui/WindowResolution.h"
#include "Gui/IngameGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLGui;
using namespace PLRenderer;
using namespace PLScene;
using namespace PLEngine;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(IngameGui)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
IngameGui::IngameGui(Application &cApplication) :
	SlotOnMenu(this),
	SlotOnResolution(this),
	SlotOnFocus(this),
	SlotOnMouseDown(this),
	m_pApplication(&cApplication),
	m_pIngameGui(nullptr),
	m_pMenu(nullptr),
	m_pText(nullptr),
	m_pResolution(nullptr)
{
	// Get scene container
	SceneContainer *pSceneContainer = m_pApplication->GetRootScene();
	if (pSceneContainer) {
		// Create 'ingame'-GUI scene node
		SNGui *pGuiSceneNode = static_cast<SNGui*>(pSceneContainer->Create("PLEngine::SNGui", "GUI"));
		if (pGuiSceneNode) {
			m_pIngameGui = pGuiSceneNode->GetGui();
			if (m_pIngameGui) {
				// Setup ingame GUI
				m_pIngameGui->SetMouseVisible(false);

				// Create a window that holds the focus
				m_pFocusWindow = new Window(m_pIngameGui->GetRootWidget());
				m_pFocusWindow->SetVisible(false);
				m_pFocusWindow->SetFocus();
				m_pFocusWindow->SignalLooseFocus.Connect(SlotOnFocus);

				// Create menu window
				m_pMenu = new WindowMenu(m_pIngameGui->GetRootWidget());
				m_pMenu->SetPos(Vector2i(-200, 99));
				m_pMenu->SetSize(Vector2i(160, 570));
				m_pMenu->SetVisible(true);
				m_pMenu->SetBlend(false);

				// Create XmlText window
				m_pText = new WindowText(m_pIngameGui->GetRootWidget());
				m_pText->SetBackgroundColor(Color4::Transparent);
				m_pText->SetPos(Vector2i(1030, 100));
				m_pText->SetSize(Vector2i(825, 570));
				m_pText->SetVisible(true);

				// Create resolution window
				m_pResolution = new WindowResolution(m_pApplication, m_pIngameGui->GetRootWidget());
				m_pResolution->SetPos(Vector2i(220, 345));
				m_pResolution->SetSize(Vector2i(m_pIngameGui->GetDefaultScreen()->GetSize().x - 240, 100));
				m_pResolution->SetVisible(false);

				// Connect signals
				m_pMenu->SignalCommand.Connect(SlotOnMenu);
				m_pResolution->SignalResolutionChanged.Connect(SlotOnResolution);
			}
		}
	}

	// Get the main window
	Widget *pWidget = m_pApplication->GetMainWindow();
	if (pWidget) {
		// We're working within the inner part of the window
		Widget *pContentWidget = pWidget->GetContentWidget();

		// Connect event handler
		pContentWidget->SignalMouseButtonDown.Connect(SlotOnMouseDown);
	}
}

/**
*  @brief
*    Destructor
*/
IngameGui::~IngameGui()
{
	// Get scene container
	SceneContainer *pSceneContainer = m_pApplication->GetRootScene();
	if (pSceneContainer) {
		// Get 'ingame'-GUI scene node
		SceneNode *pGuiSceneNode = pSceneContainer->GetByName("GUI");
		if (pGuiSceneNode)
			pGuiSceneNode->Delete(true);
	}
}

/**
*  @brief
*    Returns the owner application
*/
Application &IngameGui::GetApplication() const
{
	return *m_pApplication;
}

/**
*  @brief
*    Updates the ingame GUI component
*/
void IngameGui::Update()
{
	// Get time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference()*2;

	// Update the GUI elements
	if (m_pMenu)
		m_pMenu->Update(fTimeDiff);
	if (m_pText)
		m_pText->Update(fTimeDiff);
	if (m_pResolution)
		m_pResolution->Update(fTimeDiff);
}

/**
*  @brief
*    Hides all GUI elements
*/
void IngameGui::Hide()
{
	if (m_pMenu)
		m_pMenu->SetBlend(false);
	if (m_pText)
		m_pText->SetBlend(false);
	if (m_pResolution)
		m_pResolution->SetBlend(false);
}

/**
*  @brief
*    Returns whether or not at least one GUI element is currently shown
*/
bool IngameGui::IsGuiShown() const
{
	return ((m_pMenu && m_pMenu->GetBlend()) || (m_pText && m_pText->GetBlend()) || (m_pResolution && m_pResolution->GetBlend()));
}

/**
*  @brief
*    Returns whether or not the menu is currently shown
*/
bool IngameGui::IsMenuShown() const
{
	return (m_pMenu ? m_pMenu->GetBlend() : false);
}

/**
*  @brief
*    Shows/hides the menu
*/
void IngameGui::ShowMenu(bool bShow)
{
	if (m_pMenu)
		m_pMenu->SetBlend(bShow);
}

/**
*  @brief
*    Display a help text window
*/
void IngameGui::ShowHelpText(const String &sName)
{
	// Show or hide window?
	if (sName.GetLength()) {
		// Hide windows
		m_pResolution->SetBlend(false);

		// Load help text
		m_pText->Load("Data/HelpTexts/" + sName + ".xml");

		// Show help text
		m_pText->SetBlend(true);
	} else {
		// Hide help text
		m_pText->SetBlend(false);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a menu item has been selected
*/
void IngameGui::OnMenu(int nCommand)
{
	switch (nCommand) {
		case WindowMenu::COMMAND_WALKMODE:
			// Set walk mode
			m_pApplication->SignalSetMode(0, true);	// 0 = Walk mode

			// Hide the GUI
			Hide();
			break;

		case WindowMenu::COMMAND_FREEMODE:
			// Set free mode
			m_pApplication->SignalSetMode(1, true);	// 1 = Free mode

			// Hide the GUI
			Hide();
			break;

		case WindowMenu::COMMAND_GHOSTMODE:
			// Set ghost mode
			m_pApplication->SignalSetMode(2, true);	// 2 = Ghost mode

			// Hide the GUI
			Hide();
			break;

		case WindowMenu::COMMAND_MOVIE:
			// Set movie mode
			m_pApplication->SignalSetMode(3, true);	// 3 = Movie mode

			// Hide the GUI
			Hide();
			break;

		case WindowMenu::COMMAND_MAKINGOF:
			// Set making of mode
			m_pApplication->SignalSetMode(4, true);	// 4 = Making of mode

			// Hide the GUI
			Hide();
			break;

		case WindowMenu::COMMAND_RESOLUTION:
			// Adjust resolution
			m_pResolution->SetBlend(!m_pResolution->GetBlend());
			break;

		case WindowMenu::COMMAND_HELP:
			// Show help text
			ShowHelpText("Info");
			break;

		case WindowMenu::COMMAND_EXIT:
			// Exit application
			m_pApplication->Exit(0);
			break;
	}
}

/**
*  @brief
*    Called when the resolution slider was changed
*/
void IngameGui::OnResolution(const DisplayMode *pMode, bool bFullscreen)
{
	// Get main frame
	RenderWindow *pFrame = static_cast<RenderWindow*>(m_pApplication->GetMainWindow());
	if (pFrame) {
		// Fullscreen mode change?
		if (pFrame->IsFullscreen() != bFullscreen) {
			// Set new fullscreen mode state, changes other display mode settings on the fly, too
			pFrame->SetFullscreen(bFullscreen);
		} else {
			// Set new display mode
			pFrame->SetDisplayMode(*pMode);
		}

		{ // Update the configuration
			// Write fullscreen state back to the configuration
			m_pApplication->GetConfig().SetVar("PLEngine::RendererConfig", "Fullscreen", bFullscreen);

			// Write down display mode information
			if (pMode) {
				m_pApplication->GetConfig().SetVar("PLEngine::RendererConfig", "DisplayWidth",     pMode->vSize.x);
				m_pApplication->GetConfig().SetVar("PLEngine::RendererConfig", "DisplayHeight",    pMode->vSize.y);
				m_pApplication->GetConfig().SetVar("PLEngine::RendererConfig", "DisplayColorBits", pMode->nColorBits);
				m_pApplication->GetConfig().SetVar("PLEngine::RendererConfig", "DisplayFrequency", pMode->nFrequency);
			}
		}
	}
}

/**
*  @brief
*    Called when an ingame window has got the focus
*/
void IngameGui::OnFocus()
{
	// Reset focus to our dummy
	m_pFocusWindow->SetFocus();
}

/**
*  @brief
*    Called when a mouse button was pressed
*/
void IngameGui::OnMouseDown(uint32 nButton, const Vector2i &vPos)
{
	// Left mouse button
	if (nButton == 0) {
		// Close help text
		if (m_pText->GetBlend())
			ShowHelpText("");
	}
}
