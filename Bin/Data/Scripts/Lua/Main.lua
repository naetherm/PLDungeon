-- -> The global variable "this" points to the C++ RTTI application class instance invoking the script


--[-------------------------------------------------------]
--[ Includes                                              ]
--[-------------------------------------------------------]
require("Data/Scripts/Lua/Application")	-- Application script component class


--[-------------------------------------------------------]
--[ Global variables                                      ]
--[-------------------------------------------------------]
application = nil	-- An instance of the application script component class


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
