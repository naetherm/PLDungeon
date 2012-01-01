/*********************************************************\
 *  File: WindowBase.cpp                                 *
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
#include "Gui/WindowBase.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGui;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(WindowBase)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WindowBase::WindowBase(Widget *pParent) : Widget(pParent),
	m_bBlend(false),
	m_fBlend(0.0f),
	m_fTimeout(0.0f)
{
}

/**
*  @brief
*    Get blend status
*/
bool WindowBase::GetBlend() const
{
	// Return blend status
	return m_bBlend;
}

/**
*  @brief
*    Set blend status
*/
void WindowBase::SetBlend(bool bBlend)
{
	// Set blend status
	m_bBlend = bBlend;

	// Call virtual function
	OnSetBlend(bBlend);
}

/**
*  @brief
*    Set timeout
*/
void WindowBase::SetTimeout(float fTimeout)
{
	// Set timeout
	m_fTimeout = fTimeout;
}

/**
*  @brief
*    Update blend status
*/
void WindowBase::Update(float fTime)
{
	// Blending
	if (m_bBlend && m_fBlend < 1.0f) {
		// Blend in
		m_fBlend += fTime;
		if (m_fBlend >= 1.0f)
			m_fBlend = 1.0f;
		OnBlend(m_fBlend);
	} else if (!m_bBlend && m_fBlend > 0.0f) {
		// Blend out
		m_fBlend -= fTime;
		if (m_fBlend <= 0.0f)
			m_fBlend = 0.0f;
		OnBlend(m_fBlend);
	}

	// Timeout
	if (m_fTimeout > 0.0f) {
		m_fTimeout -= fTime;
		if (m_fTimeout <= 0.0f && m_fBlend > 0.0f) {
			m_fTimeout = 0.0f;
			SetBlend(false);
		}
	}
}


//[-------------------------------------------------------]
//[ Protected virtual WindowBase functions                ]
//[-------------------------------------------------------]
/**
*  @brief
*    React on blend in/out command
*/
void WindowBase::OnSetBlend(bool bBlend)
{
	// To be implemented in derived classes
}

/**
*  @brief
*    Update blend status
*/
void WindowBase::OnBlend(float fBlend)
{
	// To be implemented in derived classes
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
WindowBase::~WindowBase()
{
}
