--[-------------------------------------------------------]
--[ Global variables                                      ]
--[-------------------------------------------------------]
oldFilmPostProcess = 0	-- Old film post process effect factor


--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Updates the old film post process effect
function UpdateOldFilm()
	-- Get the currently set camera scene node
	local cameraSceneNode = PL.GetApplication():GetCamera()
	if cameraSceneNode ~= nil and cameraSceneNode:GetContainer() ~= nil then
		-- Get the current time difference
		local timeDiff = PL.Timing.GetTimeDifference()/4

		-- Update the the old film post process effect factor
		local containerName = cameraSceneNode:GetContainer().Name
		if containerName == "kanal2" or containerName == "kanal4" then
			oldFilmPostProcess = oldFilmPostProcess + timeDiff
			if oldFilmPostProcess > 1 then
				oldFilmPostProcess = 1
			end
		else
			oldFilmPostProcess = oldFilmPostProcess - timeDiff
			if oldFilmPostProcess < 0 then
				oldFilmPostProcess = 0
			end
		end

		-- Is there currently a old film post process effect?
		if oldFilmPostProcess > 0 then
			-- Update "PLPostProcessEffects::SNMPostProcessOldFilm" modifier
			local sceneNodeModifier = cameraSceneNode:GetModifier("PLPostProcessEffects::SNMPostProcessOldFilm")
			if sceneNodeModifier == nil then
				sceneNodeModifier = cameraSceneNode:AddModifier("PLPostProcessEffects::SNMPostProcessOldFilm")
			end
			if sceneNodeModifier ~= nil then
				sceneNodeModifier.EffectWeight = oldFilmPostProcess
			end
		else
			-- Remove "PLPostProcessEffects::SNMPostProcessOldFilm" modifier
			cameraSceneNode:RemoveModifier("PLPostProcessEffects::SNMPostProcessOldFilm")
		end
	end
end
