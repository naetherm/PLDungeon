--[-------------------------------------------------------]
--[ Classes                                               ]
--[-------------------------------------------------------]
--@brief
--  GUI script component class
GUI = {


	-- The default constructor - In Lua a static method
	new = function()


		--[-------------------------------------------------------]
		--[ Private class attributes                              ]
		--[-------------------------------------------------------]
		local this 				= {}	-- A private class attribute -> Emulates the C++ "this"-pointer by using a Lua table
		local backgroundBlur 	= 0		-- GUI background blur (0 = no blur, 1 = full blur)


		--[-------------------------------------------------------]
		--[ Private class methods                                 ]
		--[-------------------------------------------------------]
		--@brief
		--  Updates the the GUI background blur
		local function UpdateBackgroundBlur()
			-- Get the current time difference
			local timeDiff = PL.Timing.GetTimeDifference()

			-- Get the interaction component instance
			local interaction = PL.GetApplication():GetInteraction()
			if interaction ~= nil then
				-- Update the the GUI background blur depending on whether or not the ingame GUI is currently visible
				if interaction:GetIngameGui():IsGuiShown() then
					backgroundBlur = backgroundBlur + timeDiff
					if backgroundBlur > 1 then
						backgroundBlur = 1
					end
				else
					backgroundBlur = backgroundBlur - timeDiff
					if backgroundBlur < 0 then
						backgroundBlur = 0
					end
				end
			end

			-- Get the currently set camera scene node
			local cameraSceneNode = PL.GetApplication():GetCamera()
			if cameraSceneNode ~= nil then
				-- Is there currently GUI background blur?
				if backgroundBlur > 0 then
					-- Update "PLPostProcessEffects::SNMPostProcessBlur" modifier
					local sceneNodeModifier = cameraSceneNode:GetModifier("PLPostProcessEffects::SNMPostProcessBlur")
					if sceneNodeModifier == nil then
						sceneNodeModifier = cameraSceneNode:AddModifier("PLPostProcessEffects::SNMPostProcessBlur")
					end
					if sceneNodeModifier ~= nil then
						local factor = math.sin(backgroundBlur*math.pi/2)
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
