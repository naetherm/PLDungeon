-- -> The global variable "this" points to the C++ RTTI application class instance invoking the script


--[-------------------------------------------------------]
--[ Tell Lua were to find the required files              ]
--[-------------------------------------------------------]
-- Set the path used by "require" to search for a Lua loader.
-- -> Lookout! There may be issues with the LUA_PATH environment variable, so, we add an absolute path.
-- -> Please note that we can't use "this" at this point in order to receive the application instance, because it has not been set, yet.
-- -> Lua can't deal with something like 'file:\\C:\MyApplication', so, make it to 'C:\MyApplication'.
package.path = string.gsub(PL.GetApplication():GetApplicationContext():GetAppDirectory(), "file://", "") .. '/Data/Scripts/Lua/?.lua;' .. package.path


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
	application.Update()
end
