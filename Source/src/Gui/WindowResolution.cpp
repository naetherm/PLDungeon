/*********************************************************\
 *  File: WindowResolution.cpp                           *
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
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Resources/Font.h>
#include <PLGui/Gui/Resources/Graphics.h>
#include <PLGui/Widgets/Controls/Slider.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Renderer.h>
// #include <PLEngine/Gui/RenderWindow.h>// [TODO]
#include "Application.h"
#include "Gui/WindowResolution.h"


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
pl_implement_class(WindowResolution)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WindowResolution::WindowResolution(Application *pApplication, Widget *pParent) : WindowBase(pParent),
	SlotOnChangeValue(this),
	m_pApplication(pApplication),
	m_pSlider(nullptr),
	m_pFont(new Font(*GetGui())),
	m_cColorText(0.7f, 0.7f, 0.7f, 1.0f),
	m_cColorSelected(1.0f, 1.0f, 1.0f, 1.0f),
	m_nWidthButton1(0),
	m_nWidthButton2(0),
	m_nSelectedButton(-1)
{
	// Load font from file
	m_pFont->LoadFromFile("Data/Fonts/arialbd.ttf", 14);

	// Set background
	SetBackgroundColor(Color4(0.0f, 0.0f, 0.0f, 0.6f));

	// Create slider
	m_pSlider = new Slider(this);
	m_pSlider->SetBackgroundColor(Color4::Transparent);
	m_pSlider->SetOrientation(Horizontal);
	m_pSlider->SetPos(Vector2i(10, 35));
	m_pSlider->SetSize(Vector2i(764, 10));
	m_pSlider->SignalChangeValue.Connect(SlotOnChangeValue);

	// Create display mode list
	if (m_pApplication->GetRendererContext()) {
		Renderer &cRenderer = m_pApplication->GetRendererContext()->GetRenderer();

		// [TODO] PLGui
		// Get the display settings
//		const int nDesktopBitsPerPixel		  = GetGui()->SystemGetDesktopSettings(PLGui::Gui::SettingBitsPerPixel);
//		const int nDesktopVerticalRefreshRate = GetGui()->SystemGetDesktopSettings(PLGui::Gui::SettingVerticalRefreshRate);
		const int nDesktopBitsPerPixel		  = 32;
		const int nDesktopVerticalRefreshRate = 60;

		// Loop through all display modes
		const uint32 nNumOfDisplayModes = cRenderer.GetNumOfDisplayModes();
		for (uint32 i=0; i<nNumOfDisplayModes; i++) {
			// Get display mode
			const DisplayMode *pDisplayMode = cRenderer.GetDisplayMode(i);

			// Filter out unwanted display modes
			if (pDisplayMode->vSize.x >= 640 && pDisplayMode->vSize.y >= 480 &&
				pDisplayMode->nColorBits == static_cast<uint32>(nDesktopBitsPerPixel) && pDisplayMode->nFrequency == static_cast<uint32>(nDesktopVerticalRefreshRate)) {
				// Get display mode string
				String sName;
				sName += static_cast<int>(pDisplayMode->vSize.x);
				sName += 'x';
				sName += static_cast<int>(pDisplayMode->vSize.y);

				// Sort display modes
				int nIndex = m_lstDisplayModes.GetNumOfElements()-1;
				if (nIndex > -1) {
					while (nIndex > 0 && m_lstDisplayModes[nIndex]->vSize.x > pDisplayMode->vSize.x)
						nIndex--;
				}

				// Add the display mode to the list
				m_lstDisplayModes.AddAtIndex(pDisplayMode, nIndex+1);
				m_lstDisplayModeNames.AddAtIndex(sName, nIndex+1);
			}
		}

		// Set slider size
		m_pSlider->SetMinValue(0);
		m_pSlider->SetMaxValue(m_lstDisplayModes.GetNumOfElements()-1);
		m_pSlider->SetStepSize(1);
		m_pSlider->SetStepSizeFast(2);
		m_pSlider->SetVisible(true);
	}
}

/**
*  @brief
*    Get currently selected display mode
*/
const DisplayMode *WindowResolution::GetDisplayMode() const
{
	// Return selected display mode
	int nIndex = m_pSlider->GetValue();
	return m_lstDisplayModes[nIndex];
}


//[-------------------------------------------------------]
//[ Protected virtual PLGui::Widget functions             ]
//[-------------------------------------------------------]
void WindowResolution::OnDraw(Graphics &cGraphics)
{
	// Draw widget background
	DrawBackground(cGraphics);

	// Display currently selected resolution
	cGraphics.DrawText(*m_pFont, Color4::White, Color4::Transparent, Vector2i(10, 10), "Resolution");
	int nIndex = m_pSlider->GetValue();
	String sName = m_lstDisplayModeNames[nIndex];
	uint32 nWidth = cGraphics.GetTextWidth(*m_pFont, sName);
	cGraphics.DrawText(*m_pFont, Color4::White, Color4::Transparent, Vector2i(GetSize().x - 20 - nWidth, 10), sName);

	// Display button
	String sChangeMode = "Change resolution";
	m_nWidthButton1 = cGraphics.GetTextWidth(*m_pFont, sChangeMode);
	uint32 nTextHeight = cGraphics.GetTextHeight(*m_pFont, sChangeMode);
	cGraphics.DrawText(*m_pFont, (m_nSelectedButton == 1 ? m_cColorSelected : m_cColorText), Color4::Transparent, Vector2i(GetSize().x/4-m_nWidthButton1/2, 60+nTextHeight/2), sChangeMode);

	// [TODO] PLFrontend update
	bool bFullscreen = false;
	/*
	// Get current fullscreen mode
	Widget *pWidget = m_pApplication->GetMainWindow();
	if (pWidget && pWidget->IsInstanceOf("PLEngine::RenderWindow"))
		bFullscreen = static_cast<RenderWindow*>(pWidget)->IsFullscreen();
	*/

	// Display fullscreen mode
	String sFullscreen = (bFullscreen ? "Switch to window" : "Switch to fullscreen");
	m_nWidthButton2 = cGraphics.GetTextWidth(*m_pFont, sFullscreen);
	nTextHeight = cGraphics.GetTextHeight(*m_pFont, sFullscreen);
	cGraphics.DrawText(*m_pFont, (m_nSelectedButton == 2 ? m_cColorSelected : m_cColorText), Color4::Transparent, Vector2i(GetSize().x/4*3-m_nWidthButton2/2, 60+nTextHeight/2), sFullscreen);

	// Set window height
	SetSize(Vector2i(GetSize().x, 60 + 2*cGraphics.GetTextHeight(*m_pFont, sFullscreen)));
}

void WindowResolution::OnMouseMove(const Vector2i &vPos)
{
	// Determine current selection
	if (vPos.y >= 60 && vPos.x >= static_cast<int>(GetSize().x/4-m_nWidthButton1/2) && vPos.x <= static_cast<int>(GetSize().x/4+m_nWidthButton1/2))
		m_nSelectedButton = 1;
	else if (vPos.y >= 60 && vPos.x >= static_cast<int>(GetSize().x/4*3-m_nWidthButton2/2) && vPos.x <= static_cast<int>(GetSize().x/4*3+m_nWidthButton2/2))
		m_nSelectedButton = 2;
	else
		m_nSelectedButton = -1;

	// Blend out after 10 seconds
	SetTimeout(10.0f);
}

void WindowResolution::OnMouseLeave()
{
	// No button selected
	m_nSelectedButton = -1;

	// Blend out after 10 seconds
	SetTimeout(10.0f);
}

void WindowResolution::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	if (nButton == 0 && m_nSelectedButton > 0) {
		// Get selected display mode
		int nIndex = m_pSlider->GetValue();
		const DisplayMode *pDisplayMode = m_lstDisplayModes[nIndex];

		// Change mode
		if (pDisplayMode) {
			// Get current fullscreen mode
			bool bFullscreen = false;
			// [TODO] PLFrontend update
			/*
			Widget *pWidget = m_pApplication->GetMainWindow();
			if (pWidget && pWidget->IsInstanceOf("PLEngine::RenderWindow"))
				bFullscreen = static_cast<RenderWindow*>(pWidget)->IsFullscreen();
			*/

			// Call ResolutionChanged-signal
			SignalResolutionChanged(pDisplayMode, (m_nSelectedButton == 2 ? !bFullscreen : bFullscreen));
		}
	}

	// Blend out after 10 seconds
	SetTimeout(10.0f);
}


//[-------------------------------------------------------]
//[ Protected virtual WindowBase functions                ]
//[-------------------------------------------------------]
void WindowResolution::OnSetBlend(bool bBlend)
{
	// Blend?
	if (bBlend) {
		// Blend out after 10 seconds
		SetTimeout(10.0f);

		// Get current display mode
		PLCore::Config &cConfig = m_pApplication->GetConfig();
		const uint32 nCurrentDisplayWidth  = cConfig.GetVarInt("PLEngine::RendererConfig", "DisplayWidth");
		const uint32 nCurrentDisplayHeight = cConfig.GetVarInt("PLEngine::RendererConfig", "DisplayHeight");

		// Get the index of the currently selected display mode
		uint32 nIndex = 0;
		ConstIterator<const DisplayMode*> cIterator = m_lstDisplayModes.GetConstIterator();
		int nSelected = -1; // By default, nothing is selected
		while (cIterator.HasNext() && nSelected < 0) {
			// Get the current display mode
			const DisplayMode *pDisplayMode = cIterator.Next();

			// Match found?
			if (pDisplayMode->vSize.x == static_cast<int>(nCurrentDisplayWidth) &&
				pDisplayMode->vSize.y == static_cast<int>(nCurrentDisplayHeight)) {
				// Backup the index
				nSelected = nIndex;
			} else {
				// Update the index
				nIndex++;
			}
		}

		// Set correct slider position
		m_pSlider->SetValue(nSelected);
	}
}

void WindowResolution::OnBlend(float fBlend)
{
	// Set blending
	SetTransparency(AlphaTransparency, Color4(0.0f, 0.0f, 0.0f, fBlend));
	SetVisible(fBlend > 0.0f);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
WindowResolution::~WindowResolution()
{
	delete m_pFont;
}

/**
*  @brief
*    Called when slider value has changed
*/
void WindowResolution::OnChangeValue(int nValue)
{
	// Blend out after 10 seconds
	SetTimeout(10.0f);
}
