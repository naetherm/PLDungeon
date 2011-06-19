-- -> The global variable "this" points to the RTTI application class instance invoking the script


--[-------------------------------------------------------]
--[ Includes                                              ]
--[-------------------------------------------------------]
require("Data/Scripts/Lua/Application")	-- Application script component class


--[-------------------------------------------------------]
--[ Global variables                                      ]
--[-------------------------------------------------------]
application = Application.new()	-- An instance of the application script component class


--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Update function called by C++
function OnUpdate()
	-- Update the instance of the application script component class
	application:Update()
end
