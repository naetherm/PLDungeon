/*********************************************************\
 *  File: WindowBase.h                                   *
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


#ifndef __BRIDGE_WINDOW_BASE_H__
#define __BRIDGE_WINDOW_BASE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGui/Widgets/Widget.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Base class for windows in the Bridge Viewer
*/
class WindowBase : public PLGui::Widget {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, WindowBase, "", PLGui::Widget, "Base class for windows in the Bridge Viewer")
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
		WindowBase(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Get blend status
		*
		*  @return
		*    'true' if window is blended in, else 'false'
		*/
		bool GetBlend() const;

		/**
		*  @brief
		*    Set blend status
		*
		*  @param[in] bBlend
		*    'true' to blend the window in, 'false' to blend it out
		*/
		void SetBlend(bool bBlend);

		/**
		*  @brief
		*    Set timeout
		*
		*  @param[in] fTimeout
		*    Time after which the window will be blend out
		*/
		void SetTimeout(float fTimeout);

		/**
		*  @brief
		*    Update blend status
		*
		*  @param[in] fTime
		*    Time past in milliseconds
		*/
		void Update(float fTime);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~WindowBase();


	//[-------------------------------------------------------]
	//[ Protected virtual WindowBase functions                ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    React on blend in/out command
		*
		*  @param[in] bBlend
		*    'true' to blend the window in, 'false' to blend it out
		*/
		virtual void OnSetBlend(bool bBlend);

		/**
		*  @brief
		*    Update blend status
		*
		*  @param[in] fBlend
		*    Current blend status (0.0 .. 1.0)
		*/
		virtual void OnBlend(float fBlend);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool	m_bBlend;	/**< Blend status (true=blend in, false=blend out) */
		float	m_fBlend;	/**< Current blend status (0.0 - 1.0) */
		float	m_fTimeout;	/**< Current timeout value */


};


#endif // __BRIDGE_WINDOW_BASE_H__
