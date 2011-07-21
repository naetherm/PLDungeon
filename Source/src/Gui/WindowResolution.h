/*********************************************************\
 *  File: WindowResolution.h                             *
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


#ifndef __BRIDGE_WINDOW_RESOLUTION_H__
#define __BRIDGE_WINDOW_RESOLUTION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Gui/WindowBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Color4;
}
namespace PLGui {
	class Font;
	class Slider;
}
namespace PLRenderer {
	struct DisplayMode;
}
class Application;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Window that displays options for choosing the resolution
*/
class WindowResolution : public WindowBase {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, WindowResolution, "", WindowBase, "Window that displays options for choosing the resolution")
		// Signals
		pl_signal_2(SignalResolutionChanged,	const PLRenderer::DisplayMode*,	bool,	"Resolution changed",	"")
		// Slots
		pl_slot_1(OnChangeValue,	int,	"Called when slider value has changed, current slider value as first parameter",	"")
	pl_class_end


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
		WindowResolution(Application *pApplication, Widget *pParent = nullptr);

		/**
		*  @brief
		*    Get currently selected display mode
		*
		*  @return
		*    Resolution
		*/
		const PLRenderer::DisplayMode *GetDisplayMode() const;


	//[-------------------------------------------------------]
	//[ Protected virtual PLGui::Widget functions             ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnDraw(PLGui::Graphics &cGraphics);
		virtual void OnMouseMove(const PLMath::Vector2i &vPos);
		virtual void OnMouseLeave();
		virtual void OnMouseButtonDown(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);


	//[-------------------------------------------------------]
	//[ Protected virtual WindowBase functions                ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnSetBlend(bool bBlend);
		virtual void OnBlend(float fBlend);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~WindowResolution();

		/**
		*  @brief
		*    Called when slider value has changed
		*
		*  @param[in] nValue
		*    Current slider value
		*/
		void OnChangeValue(int nValue);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Child widgets and data
		Application									 *m_pApplication;			/**< Pointer to application */
		PLGui::Slider								 *m_pSlider;				/**< Slider for adjusting the brightness */
		PLGui::Font									 *m_pFont;					/**< Text font, always valid! */
		PLCore::List<const PLRenderer::DisplayMode*>  m_lstDisplayModes;		/**< Available display modes */
		PLCore::List<PLCore::String>				  m_lstDisplayModeNames;	/**< Available display modes as strings */
		PLGraphics::Color4							  m_cColorText;				/**< Text color */
		PLGraphics::Color4							  m_cColorSelected;			/**< Selected text color */
		PLCore::uint32								  m_nWidthButton1;			/**< Width of button "Fullscreen" */
		PLCore::uint32								  m_nWidthButton2;			/**< Width of button "Change Mode" */
		int											  m_nSelectedButton;		/**< Selected button */


};


#endif // __BRIDGE_WINDOW_RESOLUTION_H__
