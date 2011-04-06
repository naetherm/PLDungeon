/*********************************************************\
 *  File: IngameGui.h                                    *
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


#ifndef __DUNGEON_INGAMEGUI_H__
#define __DUNGEON_INGAMEGUI_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGui {
	class Gui;
}
namespace PLRenderer {
	struct DisplayMode;
}
class Interaction;
class WindowMenu;
class WindowText;
class WindowResolution;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ingame GUI interaction component
*/
class IngameGui {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cInteraction
		*    Owner interaction
		*/
		IngameGui(Interaction &cInteraction);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~IngameGui();

		/**
		*  @brief
		*    Returns the owner interaction
		*
		*  @return
		*    The owner interaction
		*/
		Interaction &GetInteraction() const;

		/**
		*  @brief
		*    Updates the ingame GUI component
		*/
		void Update();

		/**
		*  @brief
		*    Hides all GUI elements
		*/
		void Hide();

		/**
		*  @brief
		*    Returns whether or not at least one GUI element is currently shown
		*
		*  @return
		*    'true' if currently at least one GUI element is shown, else 'false'
		*/
		bool IsGuiShown() const;

		/**
		*  @brief
		*    Returns whether or not the menu is currently shown
		*
		*  @return
		*    'true' if the menu is currently shown, else 'false'
		*/
		bool IsMenuShown() const;

		/**
		*  @brief
		*    Shows/hides the menu
		*
		*  @param[in] bShow
		*    'true' to show the menu, else 'faöse'
		*/
		void ShowMenu(bool bShow = true);

		/**
		*  @brief
		*    Display a help text window
		*
		*  @param[in] sName
		*    Name of help text ("name.xml" is loaded, "" to hide the help text)
		*/
		void ShowHelpText(const PLGeneral::String &sGroup);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when a menu item has been selected
		*
		*  @param[in] nCommand
		*    Selected menu item
		*/
		void OnMenu(int nCommand);

		/**
		*  @brief
		*    Called when the resolution slider was changed
		*
		*  @param[in] pMode
		*    New display mode
		*  @param[in] bFullscreen
		*    New fullscreen mode
		*/
		void OnResolution(const PLRenderer::DisplayMode *pMode, bool bFullscreen);

		/**
		*  @brief
		*    Called when an ingame window has got the focus
		*/
		void OnFocus();

		/**
		*  @brief
		*    Called when a mouse button was pressed
		*
		*  @param[in] nButton
		*    Mouse button
		*  @param[in] vPos
		*    Mouse position
		*/
		void OnMouseDown(PLGeneral::uint32 nButton, const PLMath::Vector2i &vPos);


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<int>											EventHandlerMenu;
		PLCore::EventHandler<const PLRenderer::DisplayMode*, bool>			EventHandlerResolution;
		PLCore::EventHandler<>												EventHandlerFocus;
		PLCore::EventHandler<PLGeneral::uint32, const PLMath::Vector2i &>	EventHandlerMouseDown;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Interaction			*m_pInteraction;	/**< Owner interaction, always valid! */
		PLGui::Gui			*m_pIngameGui;		/**< Ingame GUI */
		PLGui::Widget		*m_pFocusWindow;	/**< Window that holds the focus */
		WindowMenu			*m_pMenu;			/**< Main menu */
		WindowText			*m_pText;			/**< Text window */
		WindowResolution	*m_pResolution;		/**< Resolution options */


};


#endif // __DUNGEON_INGAMEGUI_H__
