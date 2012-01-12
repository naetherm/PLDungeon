--[-------------------------------------------------------]
--[ Classes                                               ]
--[-------------------------------------------------------]
--@brief
--  GUI script component class
GUI = {


	--@brief
	--  The default constructor - In Lua a static method
	--
	--@param[in] cppApplication
	--  C++ RTTI application class instance
	--@param[in] luaApplication
	--  Lua script application class instance
	new = function(cppApplication, luaApplication)


		--[-------------------------------------------------------]
		--[ Private class attributes                              ]
		--[-------------------------------------------------------]
		local this 				= {}	-- A private class attribute -> Emulates the C++ "this"-pointer by using a Lua table
		local _backgroundBlur 	= 0		-- GUI background blur (0 = no blur, 1 = full blur)


		--[-------------------------------------------------------]
		--[ Private class methods                                 ]
		--[-------------------------------------------------------]
		--@brief
		--  Updates the the GUI background blur
		local function UpdateBackgroundBlur()
			-- Get the current time difference
			local timeDiff = PL.Timing.GetTimeDifference()

			-- Update the the GUI background blur depending on whether or not the ingame GUI is currently visible
			local ingameGui = luaApplication.GetIngameGui()
			if ingameGui ~= nil and ingameGui:IsGuiShown() then
				_backgroundBlur = _backgroundBlur + timeDiff
				if _backgroundBlur > 1 then
					_backgroundBlur = 1
				end
			else
				_backgroundBlur = _backgroundBlur - timeDiff
				if _backgroundBlur < 0 then
					_backgroundBlur = 0
				end
			end

			-- Get the currently set camera scene node
			local cameraSceneNode = cppApplication:GetCamera()
			if cameraSceneNode ~= nil then
				-- Is there currently GUI background blur?
				if _backgroundBlur > 0 then
					-- Update "PLPostProcessEffects::SNMPostProcessBlur" modifier
					local sceneNodeModifier = cameraSceneNode:GetModifier("PLPostProcessEffects::SNMPostProcessBlur")
					if sceneNodeModifier == nil then
						sceneNodeModifier = cameraSceneNode:AddModifier("PLPostProcessEffects::SNMPostProcessBlur")
					end
					if sceneNodeModifier ~= nil then
						local factor = math.sin(_backgroundBlur*math.pi/2)
						sceneNodeModifier.EffectWeight = factor
						sceneNodeModifier.BloomScale   = 0.8 + (1 - factor)*3
						sceneNodeModifier.Strength     = string.format("%f %f", 1 + factor*3, 1 + factor*4)
					end
				else
					-- Remove "PLPostProcessEffects::SNMPostProcessBlur" modifier
					cameraSceneNode:RemoveModifier("PLPostProcessEffects::SNMPostProcessBlur")
				end
			end
		end


		--[-------------------------------------------------------]
		--[ Public class methods                                  ]
		--[-------------------------------------------------------]
		--@brief
		--  Updates the GUI script component
		function this.Update()
			-- Update the the GUI background blur
			UpdateBackgroundBlur()
		end


		-- Return the created class instance
		return this
	end


}
