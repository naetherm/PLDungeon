--/*********************************************************\
-- *  File: Main.lua                                       *
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


-- -> The global variable "this" points to the C++ RTTI application class instance invoking the script


--[-------------------------------------------------------]
--[ Tell Lua were to find the required files              ]
--[-------------------------------------------------------]
-- Set the path used by "require" to search for a Lua loader.
-- -> Lookout! There may be issues with the LUA_PATH environment variable, so, we add an absolute path.
-- -> Please note that we can't use "this" at this point in order to receive the application instance, because it has not been set, yet.
package.path = PL.GetApplication():GetScriptDirectory() .. '?.lua;' .. package.path


--[-------------------------------------------------------]
--[ Includes                                              ]
--[-------------------------------------------------------]
require "Application"	-- Application script component class


--[-------------------------------------------------------]
--[ Global variables                                      ]
--[-------------------------------------------------------]
SceneFilename	= "Data/Scenes/Dungeon.scene"	-- Filename of the scene to use, the C++ application framework will load it (this filename will be ignored if the program was started with a scene filename within the command line parameters)
application		= nil							-- An instance of the application script component class


--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Called by C++ when the application should initialize itself
function OnInit()
	-- Create an instance of the application script component class
	application = Application.new(this)
end

--@brief
--  Update function called by C++
function OnUpdate()
	-- Update the instance of the application script component class
	if application ~= nil then
		application.Update()
	end
end
