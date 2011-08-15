-- -> The global variable "this" points to the C++ RTTI application class instance invoking the script


--[-------------------------------------------------------]
--[ Tell Lua were to find the required files              ]
--[-------------------------------------------------------]
package.path = package.path .. ';../Data/Scripts/Lua/?.lua'	-- The path used by "require" to search for a Lua loader


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
