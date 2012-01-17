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
application	    = nil	-- An instance of the application script component class
InternalRelease = true	-- 'true' for internal release (we can use stuff helping us during development), else 'false' for a public release


--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Called by C++ when the application should initialize itself
function OnInit()
	-- Set base directory to make it possible to find the required resources
	-- -> This might not always be required, but in case this script was thrown into PLViewer this application has to know about the base directory
	-- -> When provided with a relative path, the following method will use the directory this script is in
	--    (e.g. a script filename of "C:/Programs/MyApplication/Main.lua" will result in "C:/Programs/MyApplication/")
	this:SetBaseDirectory("../../../")	-- We're in "Data/Scripts/Lua/", so, go 3 directories up

	-- Scan our project directory for compatible plugins and load them in
	-- -> This might not always be required, but in case this script was thrown into PLViewer this application has to know about the new plugins
	-- -> Base directory is "C:/Programs/MyApplication/", for x86 the plugins are in this case within "C:/Programs/MyApplication/x86/"
	PL.ClassManager.ScanPlugins(this:GetBaseDirectory() .. PL.System.GetPlatformArchitecture(), false, true)

	-- Create an instance of the application script component class
	application = Application.new(this)

	-- Load scene
	this:LoadScene("Data/Scenes/Dungeon.scene")
end

--@brief
--  Update function called by C++
function OnUpdate()
	-- Update the instance of the application script component class
	if application ~= nil then
		application.Update()
	end
end
