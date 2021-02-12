/*********************************************************\
 *  File: SNMLightRandomAnimation.cpp                    *
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
#include <PLCore/Tools/Timing.h>
#include <PLScene/Scene/SNLight.h>
#include <PLScene/Scene/SceneContext.h>
#include "SNMLightRandomAnimation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMLightRandomAnimation, "", PLScene::SceneNodeModifier, "Scene node modifier class for a random light color animation")
	// Properties
	pl_properties
		pl_property("SceneNodeClass",	"PLScene::SNLight")
	pl_properties_end
	// Attributes
	pl_attribute_metadata(Speed,		float,					2.0f,									ReadWrite,	"Animation speed",	"")
	pl_attribute_metadata(Radius,	float,					0.5f,									ReadWrite,	"Animation radius",	"")
	pl_attribute_metadata(FixColor,	PLGraphics::Color3,		PLGraphics::Color3(0.5f, 0.5f, 0.5f),	ReadWrite,	"Fix color",		"")
	pl_attribute_metadata(Color,		PLGraphics::Color3,		PLGraphics::Color3(1.0f, 1.0f, 1.0f),	ReadWrite,	"Color to apply",	"")
		// Overwritten PLScene::SceneNodeModifier attributes
	pl_attribute_metadata(Flags,		pl_flag_type_def3(SNMLightRandomAnimation, EFlags),	0,										ReadWrite,			"Flags",			"")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	// Slots
	pl_slot_0_metadata(OnUpdate,	"Called when the scene node needs to be updated",	"")
pl_class_metadata_end(SNMLightRandomAnimation)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMLightRandomAnimation::SNMLightRandomAnimation(SceneNode &cSceneNode) : SceneNodeModifier(cSceneNode),
	Speed(this),
	Radius(this),
	FixColor(this),
	Color(this),
	Flags(this),
	SlotOnUpdate(this),
	m_fCurrentIntensity(1.0f),
	m_fDestinationIntensity(1.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMLightRandomAnimation::~SNMLightRandomAnimation()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMLightRandomAnimation::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNMLightRandomAnimation::OnUpdate()
{
	// Set current scene node scale
	SNLight &cLight = static_cast<SNLight&>(GetSceneNode());

	// Animate color
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference()*Speed;
	if (m_fCurrentIntensity <= m_fDestinationIntensity) {
		m_fCurrentIntensity += fTimeDiff;
		if (m_fCurrentIntensity >= m_fDestinationIntensity) {
			// Clamp
			m_fCurrentIntensity = m_fDestinationIntensity;

			// New destination
			m_fDestinationIntensity = Math::GetRandNegFloat()*Radius;
		}
	} else {
		m_fCurrentIntensity -= fTimeDiff;
		if (m_fCurrentIntensity <= m_fDestinationIntensity) {
			// Clamp
			m_fCurrentIntensity = m_fDestinationIntensity;

			// New destination
			m_fDestinationIntensity = Math::GetRandNegFloat()*Radius;
		}
	}

	// Set color
	Color3 cColor = cLight.Color.Get();
	Color3 cColorChanged = cColor*m_fCurrentIntensity;
	if (GetFlags() & Multiply) {
		// Red
		cColor.r = (GetFlags() & NR) ? FixColor.Get().r : FixColor.Get().r*cColorChanged.r;
		// Green
		cColor.g = (GetFlags() & NG) ? FixColor.Get().g : FixColor.Get().g*cColorChanged.g;
		// Blue
		cColor.b = (GetFlags() & NB) ? FixColor.Get().b : FixColor.Get().b*cColorChanged.b;
	} else {
		// Red
		cColor.r = (GetFlags() & NR) ? FixColor.Get().r : FixColor.Get().r+cColorChanged.r;
		// Green
		cColor.g = (GetFlags() & NG) ? FixColor.Get().g : FixColor.Get().g+cColorChanged.g;
		// Blue
		cColor.b = (GetFlags() & NB) ? FixColor.Get().b : FixColor.Get().b+cColorChanged.b;
	}

	// Clamp the color values between 0.0 and 1.0
	cColor.Saturate();

	// Finally, set the new color of the light
	cLight.Color.Set(cColor);
}
