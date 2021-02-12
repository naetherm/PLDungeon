/*********************************************************\
 *  File: WindowMenu.h                                   *
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


#ifndef __BRIDGE_WINDOW_MENU_H__
#define __BRIDGE_WINDOW_MENU_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include "Gui/WindowBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGui {
	class Font;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Window that displays the main menu
*/
class WindowMenu : public WindowBase {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
		// Signals
		pl_signal_1_def(SignalCommand,	int)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Command ID's
		*/
		enum {
			COMMAND_NONE = -1,
			COMMAND_WALKMODE,
			COMMAND_FREEMODE,
			COMMAND_GHOSTMODE,
			COMMAND_MOVIE,
			COMMAND_MAKINGOF,
			COMMAND_RESOLUTION,
			COMMAND_HELP,
			COMMAND_EXIT
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Parent widget, can be a null pointer
		*/
		WindowMenu(Widget *pParent = nullptr);


	//[-------------------------------------------------------]
	//[ Protected virtual PLGui::Widget functions             ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnDraw(PLGui::Graphics &cGraphics) override;
		virtual void OnMouseMove(const PLMath::Vector2i &vPos) override;
		virtual void OnMouseLeave() override;
		virtual void OnMouseButtonDown(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;


	//[-------------------------------------------------------]
	//[ Protected virtual WindowBase functions                ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnSetBlend(bool bBlend) override;
		virtual void OnBlend(float fBlend) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~WindowMenu();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLGui::Font			*m_pFontTitle;		/**< Title font, always valid! */
		PLGui::Font			*m_pFontText;		/**< Text font, always valid! */
		PLGraphics::Color4	 m_cColorTitle;		/**< Title color */
		PLGraphics::Color4	 m_cColorText;		/**< Text color */
		PLGraphics::Color4	 m_cColorSelected;	/**< Selected color */
		int					 m_nSelected;		/**< Currently selected menu item */
		PLCore::uint32		 m_nTitleHeight;	/**< Height of title text */
		PLCore::uint32	 	 m_nTextHeight;		/**< Height of text */


};


#endif // __BRIDGE_WINDOW_MENU_H__
