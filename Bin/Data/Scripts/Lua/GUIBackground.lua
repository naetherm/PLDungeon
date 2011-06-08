--[-------------------------------------------------------]
--[ Global variables                                      ]
--[-------------------------------------------------------]
GUIBackgroundBlur = 0	-- GUI background blur (0 = no blur, 1 full blur)


--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Updates the the GUI background blur
function UpdateGUIBackgroundBlur()
	-- Get the current time difference
	local timeDiff = PL.Timing.GetTimeDifference()

	-- Get the interaction component instance
	local interaction = PL.GetApplication():GetInteraction()
	if interaction ~= nil then
		-- Update the the GUI background blur depending on whether or not the ingame GUI is currently visible
		if interaction:GetIngameGui():IsGuiShown() then
			GUIBackgroundBlur = GUIBackgroundBlur + timeDiff
			if GUIBackgroundBlur > 1 then
				GUIBackgroundBlur = 1
			end
		else
			GUIBackgroundBlur = GUIBackgroundBlur - timeDiff
			if GUIBackgroundBlur < 0 then
				GUIBackgroundBlur = 0
			end
		end
	end

	-- Get the currently set camera scene node
	local cameraSceneNode = PL.GetApplication():GetCamera()
	if cameraSceneNode ~= nil then
		-- Is there currently GUI background blur?
		if GUIBackgroundBlur > 0 then
			-- Update "PLPostProcessEffects::SNMPostProcessBlur" modifier
			local sceneNodeModifier = cameraSceneNode:GetModifier("PLPostProcessEffects::SNMPostProcessBlur")
			if sceneNodeModifier == nil then
				sceneNodeModifier = cameraSceneNode:AddModifier("PLPostProcessEffects::SNMPostProcessBlur")
			end
			if sceneNodeModifier ~= nil then
				local factor = math.sin(GUIBackgroundBlur*math.pi/2)
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
