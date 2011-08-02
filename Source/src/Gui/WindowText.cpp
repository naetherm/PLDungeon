/*********************************************************\
 *  File: WindowText.cpp                                 *
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
#include <PLGui/Gui/Screen.h>
#include <PLGuiXmlText/XmlTextView.h>
#include "Gui/WindowText.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLMath;
using namespace PLGui;
using namespace PLGuiXmlText;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(WindowText)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WindowText::WindowText(Widget *pParent) : WindowBase(pParent),
	m_pXmlText(nullptr)
{
	// Create XmlText widget
	m_pXmlText = new XmlTextView(this);
	m_pXmlText->SetBackgroundColor(Color4::Transparent);
	m_pXmlText->SetPos(Vector2i::Zero);
	m_pXmlText->SetSize(GetSize());
	m_pXmlText->SetVisible(true);
}

/**
*  @brief
*    Load XML file
*/
void WindowText::Load(const String &sFilename)
{
	// Load XML
	m_pXmlText->Load(sFilename);

	// Adjust size of window according to size of XmlText content
	Vector2i vSize = m_pXmlText->GetContentSize();
	if (vSize.x > 0 && vSize.y > 0) {
		m_pXmlText->SetSize(vSize);
		SetSize(vSize);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual WindowBase functions                ]
//[-------------------------------------------------------]
void WindowText::OnBlend(float fBlend)
{
	// Get the default screen of the GUI
	Screen *pScreen = GetGui()->GetDefaultScreen();
	if (pScreen) {
		// Set position and blending
		SetPos(Vector2i(pScreen->GetSize().x - static_cast<int>(fBlend*GetSize().x), (pScreen->GetSize().y-GetSize().y)/2));
		SetTransparency(PLGui::AlphaTransparency, Color4(0.0f, 0.0f, 0.0f, fBlend));
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
WindowText::~WindowText()
{
}
