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
local originalPosition		= { 0, 0, 0 }	-- Original owner scene node position (Lua table with three entries for x, y, z)
local currentPosition		= { 0, 0, 0 }	-- Current local position
local destinationPosition	= { 0, 0, 0 }	-- Destination local position


--[-------------------------------------------------------]
--[ Performance optimization using fast local variables   ]
--[-------------------------------------------------------]
local math_random					= math.random
local string_format					= string.format
local PL_Timing_GetTimeDifference	= PL.Timing.GetTimeDifference
local sceneNode						= nil	-- Owner scene node (will not change)


--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Called by C++ when the script scene node modifier should initialize itself
function OnInit()
	-- Get the owner scene node (will not change)
	sceneNode = this:GetSceneNode()

	-- Backup original owner scene node position (Lua table with three entries for x, y, z)
	originalPosition = string.split(sceneNode.Position, " ")

	-- Initialize local current and destination position
	currentPosition = { 0, 0, 0, }
	destinationPosition = { 0, 0, 0, }
end

--@brief
--  Update function called by C++
function OnUpdate()
	-- Update our timer
	local timeDifference = PL_Timing_GetTimeDifference()*PublicVariables.Speed

	-- Update the current local position
	for i=1,3 do
		-- Get current values
		local currentValue = currentPosition[i]
		local destinationValue = destinationPosition[i]

		-- Update value
		if currentValue <= destinationValue then
			currentValue = currentValue + timeDifference
			if currentValue >= destinationValue then
				-- Clamp
				currentValue = destinationValue

				-- New destination
				destinationPosition[i] = (math_random()*2 - 1)*PublicVariables.Radius
			end
		else
			currentValue = currentValue - timeDifference
			if currentValue <= destinationValue then
				-- Clamp
				currentValue = destinationValue

				-- New destination
				destinationPosition[i] = (math_random()*2 - 1)*PublicVariables.Radius
			end
		end

		-- Update current position
		currentPosition[i] = currentValue
	end

	-- Set current scene node position
	sceneNode.Position = string_format("%f %f %f", originalPosition[1] + currentPosition[1], originalPosition[2] + currentPosition[2], originalPosition[3] + currentPosition[3])
end
