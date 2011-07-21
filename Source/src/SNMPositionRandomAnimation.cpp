/*********************************************************\
 *  File: SNMPositionRandomAnimation.cpp                 *
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
#include <PLScene/Scene/SceneNode.h>
#include <PLScene/Scene/SceneContext.h>
#include "SNMPositionRandomAnimation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPositionRandomAnimation)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPositionRandomAnimation::SNMPositionRandomAnimation(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	Speed(this),
	Radius(this),
	FixPosition(this),
	SlotOnUpdate(this)
{
	// Set initial fixed position
	FixPosition.Set(cSceneNode.GetTransform().GetPosition());
}

/**
*  @brief
*    Destructor
*/
SNMPositionRandomAnimation::~SNMPositionRandomAnimation()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMPositionRandomAnimation::OnActivate(bool bActivate)
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
void SNMPositionRandomAnimation::OnUpdate()
{
	// Update the current local position
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference()*Speed;
	for (uint32 i=0; i<3; i++) {
		if (m_vCurrentPosition.fV[i] <= m_vDestinationPosition.fV[i]) {
			m_vCurrentPosition.fV[i] += fTimeDiff;
			if (m_vCurrentPosition.fV[i] >= m_vDestinationPosition.fV[i]) {
				// Clamp
				m_vCurrentPosition.fV[i] = m_vDestinationPosition.fV[i];

				// New destination
				m_vDestinationPosition.fV[i] = Math::GetRandNegFloat()*Radius;
			}
		} else {
			m_vCurrentPosition.fV[i] -= fTimeDiff;
			if (m_vCurrentPosition.fV[i] <= m_vDestinationPosition.fV[i]) {
				// Clamp
				m_vCurrentPosition.fV[i] = m_vDestinationPosition.fV[i];

				// New destination
				m_vDestinationPosition.fV[i] = Math::GetRandNegFloat()*Radius;
			}
		}
	}

	// Set current scene node position
	GetSceneNode().GetTransform().SetPosition(FixPosition.Get() + m_vCurrentPosition);
}
