--[-------------------------------------------------------]
--[ Classes                                               ]
--[-------------------------------------------------------]
--@brief
--  PLGui script helper class (don't forget to use "PLGui.Init()")
PLGui = {


	--[-------------------------------------------------------]
	--[ Public definitions                                    ]
	--[-------------------------------------------------------]
	--@brief
	--   Keys
	Key = {
		ESCAPE,	-- PLGUIKEY_ESCAPE
		ONE,	-- PLGUIKEY_1
		TWO,	-- PLGUIKEY_2
		THREE,	-- PLGUIKEY_3
		FOUR,	-- PLGUIKEY_4
		FIVE,	-- PLGUIKEY_5
		F12,	-- PLGUIKEY_F12
		R,		-- PLGUIKEY_R
		P,		-- PLGUIKEY_P
	},


	--@brief
	--  Initializes the PLGui script helper class
	Init = function()
		-- Platform dependent key definitions
		if PL.System.IsWindows() then
			PLGui.Key = {
				ESCAPE	= 0x1B,	-- PLGUIKEY_ESCAPE
				ONE		= 0x31,	-- PLGUIKEY_1
				TWO		= 0x32,	-- PLGUIKEY_2
				THREE	= 0x33,	-- PLGUIKEY_3
				FOUR	= 0x34,	-- PLGUIKEY_4
				FIVE	= 0x35,	-- PLGUIKEY_5
				F12		= 0x7B,	-- PLGUIKEY_F12
				R		= 0x52,	-- PLGUIKEY_R
				P		= 0x50,	-- PLGUIKEY_P
			}
		elseif PL.System.IsLinux() then
			PLGui.Key = {
				ESCAPE	= 0xFF1B,	-- PLGUIKEY_ESCAPE
				ONE		= 0x0031,	-- PLGUIKEY_1
				TWO		= 0x0032,	-- PLGUIKEY_2
				THREE	= 0x0033,	-- PLGUIKEY_3
				FOUR	= 0x0034,	-- PLGUIKEY_4
				FIVE	= 0x0035,	-- PLGUIKEY_5
				F12		= 0xFFC9,	-- PLGUIKEY_F12
				R		= 0x0052,	-- PLGUIKEY_R
				P		= 0x0050,	-- PLGUIKEY_P
			}
		end
	end


}
