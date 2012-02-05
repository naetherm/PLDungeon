--/*********************************************************\
-- *  File: SNMPositionRandomAnimation.lua                 *
-- *
-- *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
-- *
-- *  This file is part of PixelLight.
-- *
-- *  PixelLight is free software: you can redistribute it and/or modify
-- *  it under the terms of the GNU Lesser General Public License as published by
-- *  the Free Software Foundation, either version 3 of the License, or
-- *  (at your option) any later version.
-- *
-- *  PixelLight is distributed in the hope that it will be useful,
-- *  but WITHOUT ANY WARRANTY; without even the implied warranty of
-- *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- *  GNU Lesser General Public License for more details.
-- *
-- *  You should have received a copy of the GNU Lesser General Public License
-- *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
--\*********************************************************/


-- This script is called by the scene "Scripts.scene"
-- -> The global variable "this" points to the C++ RTTI scene node modifier class instance invoking the script


--[-------------------------------------------------------]
--[ Tell Lua were to find the required files              ]
--[-------------------------------------------------------]
-- Set the path used by "require" to search for a Lua loader.
-- -> Lookout! There may be issues with the LUA_PATH environment variable, so, we add an absolute path.
-- -> Please note that we can't use "this" at this point in order to receive the application instance, because it has not been set, yet.
package.path = PL.GetApplication():GetBaseDirectory() .. 'Data/Scripts/Lua/?.lua;' .. package.path


--[-------------------------------------------------------]
--[ Includes                                              ]
--[-------------------------------------------------------]
require "StringTools"	-- String tools, required for "string.split()"


--[-------------------------------------------------------]
--[ Global public variables                               ]
--[-------------------------------------------------------]
PublicVariables = {
	Speed  = 0.01,	-- Animation speed
	Radius = 0.01,	-- Animation radius
}


--[-------------------------------------------------------]
--[ Global variables                                      ]
--[-------------------------------------------------------]
originalPosition	= {}	-- Original owner scene node position (Lua table with three entries for x, y, z)
currentPosition		= {}	-- Current local position
destinationPosition	= {}	-- Destination local position


--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Called by C++ when the script scene node modifier should initialize itself
function OnInit()
	-- Backup original owner scene node position (Lua table with three entries for x, y, z)
	originalPosition = string.split(this:GetSceneNode().Position, " ")

	-- Initialize local current and destination position
	currentPosition = { 0, 0, 0, }
	destinationPosition = { 0, 0, 0, }
end

--@brief
--  Update function called by C++
function OnUpdate()
	-- Update our timer
	local timeDifference = PL.Timing.GetTimeDifference()*PublicVariables.Speed

	-- Update the current local position
	for i=1,3 do
		if currentPosition[i] <= destinationPosition[i] then
			currentPosition[i] = currentPosition[i] + timeDifference
			if currentPosition[i] >= destinationPosition[i] then
				-- Clamp
				currentPosition[i] = destinationPosition[i]

				-- New destination
				destinationPosition[i] = (math.random()*2 - 1)*PublicVariables.Radius
			end
		else
			currentPosition[i] = currentPosition[i] - timeDifference
			if currentPosition[i] <= destinationPosition[i] then
				-- Clamp
				currentPosition[i] = destinationPosition[i]

				-- New destination
				destinationPosition[i] = (math.random()*2 - 1)*PublicVariables.Radius
			end
		end
	end

	-- Set current scene node position
	this:GetSceneNode().Position = string.format("%f %f %f", originalPosition[1] + currentPosition[1], originalPosition[2] + currentPosition[2], originalPosition[3] + currentPosition[3])
end
