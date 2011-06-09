/*********************************************************\
 *  File: WindowMenu.cpp                                 *
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
#include "Gui/WindowMenu.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLGraphics;
using namespace PLMath;
using namespace PLGui;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(WindowMenu)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WindowMenu::WindowMenu(Widget *pParent) : WindowBase(pParent),
	m_pFontTitle(new Font(*GetGui())),
	m_pFontText(new Font(*GetGui())),
	m_cColorTitle(1.0f, 1.0f, 1.0f, 1.0f),
	m_cColorText(0.7f, 0.7f, 0.7f, 1.0f),
	m_cColorSelected(1.0f, 1.0f, 1.0f, 1.0f),
	m_nSelected(COMMAND_NONE),
	m_nTitleHeight(0),
	m_nTextHeight(0)
{
	// Load fonts from file
	m_pFontTitle->LoadFromFile("Data/Fonts/arialbd.ttf", 18);
	m_pFontText->LoadFromFile("Data/Fonts/arial.ttf", 16);

	// Setup window
	SetBackgroundColor(Color4(0.0f, 0.0, 0.0f, 0.6f));
}


//[-------------------------------------------------------]
//[ Protected virtual Window functions                    ]
//[-------------------------------------------------------]
void WindowMenu::OnDraw(Graphics &cGraphics)
{
	// Draw widget background
	DrawBackground(cGraphics);

	// Get line height
	m_nTitleHeight = static_cast<uint32>(cGraphics.GetTextHeight(*m_pFontTitle, "Text") * 1.3f);
	m_nTextHeight  = static_cast<uint32>(cGraphics.GetTextHeight(*m_pFontText,  "Text") * 1.3f);
	uint32 nY = 10;

	// Draw menu
		// "Menu"
	cGraphics.DrawText(*m_pFontTitle, m_cColorTitle, Color4::Transparent, Vector2i(30, nY), "Menu"); nY += m_nTitleHeight;
	nY += m_nTitleHeight;
		// "Walk mode", "Free mode", "Ghost mode", "Movie", "Making of"
	cGraphics.DrawText(*m_pFontText,  (m_nSelected == COMMAND_WALKMODE   ? m_cColorSelected : m_cColorText),  Color4::Transparent, Vector2i(30, nY), "Walk mode"); nY += m_nTextHeight;
	cGraphics.DrawText(*m_pFontText,  (m_nSelected == COMMAND_FREEMODE   ? m_cColorSelected : m_cColorText),  Color4::Transparent, Vector2i(30, nY), "Free mode"); nY += m_nTextHeight;
	cGraphics.DrawText(*m_pFontText,  (m_nSelected == COMMAND_GHOSTMODE  ? m_cColorSelected : m_cColorText),  Color4::Transparent, Vector2i(30, nY), "Ghost mode"); nY += m_nTextHeight;
	cGraphics.DrawText(*m_pFontText,  (m_nSelected == COMMAND_MOVIE      ? m_cColorSelected : m_cColorText),  Color4::Transparent, Vector2i(30, nY), "Movie"); nY += m_nTextHeight;
	cGraphics.DrawText(*m_pFontText,  (m_nSelected == COMMAND_MAKINGOF 	 ? m_cColorSelected : m_cColorText),  Color4::Transparent, Vector2i(30, nY), "Making of"); nY += m_nTextHeight;
	nY += m_nTitleHeight;
		// "Resolution", "Help", "Exit"
	// [TODO] Linux: Currently, resolution and fullscreen mode change are not ready for productive use - so just don't offer this option within the GUI
	#ifdef WIN32
		cGraphics.DrawText(*m_pFontText,  (m_nSelected == COMMAND_RESOLUTION ? m_cColorSelected : m_cColorText),  Color4::Transparent, Vector2i(30, nY), "Resolution"); nY += m_nTextHeight;
	#endif
	cGraphics.DrawText(*m_pFontText,  (m_nSelected == COMMAND_HELP		 ? m_cColorSelected : m_cColorText),  Color4::Transparent, Vector2i(30, nY), "About"); nY += m_nTextHeight;
	cGraphics.DrawText(*m_pFontText,  (m_nSelected == COMMAND_EXIT		 ? m_cColorSelected : m_cColorText),  Color4::Transparent, Vector2i(30, nY), "Exit"); nY += m_nTextHeight;
	nY += m_nTitleHeight;
	SetSize(Vector2i(GetSize().x, nY));
}

void WindowMenu::OnMouseMove(const Vector2i &vPos)
{
	// Calculate selected menu item
	m_nSelected = COMMAND_NONE;
	int nY = 10;
	nY += m_nTitleHeight;
	nY += m_nTitleHeight;
	if (vPos.y >= nY && vPos.y < nY + static_cast<int>(m_nTextHeight))	m_nSelected = COMMAND_WALKMODE;
	nY += m_nTextHeight;
	if (vPos.y >= nY && vPos.y < nY + static_cast<int>(m_nTextHeight))	m_nSelected = COMMAND_FREEMODE;
	nY += m_nTextHeight;
	if (vPos.y >= nY && vPos.y < nY + static_cast<int>(m_nTextHeight))	m_nSelected = COMMAND_GHOSTMODE;
	nY += m_nTextHeight;
	if (vPos.y >= nY && vPos.y < nY + static_cast<int>(m_nTextHeight))	m_nSelected = COMMAND_MOVIE;
	nY += m_nTextHeight;
	if (vPos.y >= nY && vPos.y < nY + static_cast<int>(m_nTextHeight))	m_nSelected = COMMAND_MAKINGOF;
	nY += m_nTextHeight;
	nY += m_nTextHeight;
	// [TODO] Linux: Currently, resolution and fullscreen mode change are not ready for productive use - so just don't offer this option within the GUI
	#ifdef WIN32
		if (vPos.y >= nY && vPos.y < nY + static_cast<int>(m_nTextHeight))	m_nSelected = COMMAND_RESOLUTION;
		nY += m_nTitleHeight;
	#endif
	if (vPos.y >= nY && vPos.y < nY + static_cast<int>(m_nTextHeight))	m_nSelected = COMMAND_HELP;
	nY += m_nTextHeight;
	if (vPos.y >= nY && vPos.y < nY + static_cast<int>(m_nTextHeight))	m_nSelected = COMMAND_EXIT;

	// Blend out after 10 seconds
	SetTimeout(10.0f);
}

void WindowMenu::OnMouseLeave()
{
	// Deselect menu item
	m_nSelected = COMMAND_NONE;

	// Blend out after 10 seconds
	SetTimeout(10.0f);
}

void WindowMenu::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	// Send command of currently selected menu item
	if (nButton == 0 && m_nSelected > COMMAND_NONE)
		SignalCommand(m_nSelected);

	// Blend out after 10 seconds
	SetTimeout(10.0f);
}


//[-------------------------------------------------------]
//[ Protected virtual WindowBase functions                ]
//[-------------------------------------------------------]
void WindowMenu::OnSetBlend(bool bBlend)
{
	// Blend out after 10 seconds
	if (bBlend)
		SetTimeout(10.0f);
}

void WindowMenu::OnBlend(float fBlend)
{
	// Update position and transparency
	SetPos(Vector2i(static_cast<int>((fBlend-1.0f)*GetSize().x), GetPos().y));
	SetTransparency(AlphaTransparency, Color4(0.0f, 0.0f, 0.0f, fBlend));
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
WindowMenu::~WindowMenu()
{
	delete m_pFontTitle;
	delete m_pFontText;
}
