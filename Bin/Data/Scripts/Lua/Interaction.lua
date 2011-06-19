--[-------------------------------------------------------]
--[ Includes                                              ]
--[-------------------------------------------------------]
require("Data/Scripts/Lua/GUI")	-- GUI script component class


--[-------------------------------------------------------]
--[ Classes                                               ]
--[-------------------------------------------------------]
--@brief
--  Interaction script component class
Interaction = {


	-- The default constructor - In Lua a static method
	new = function()


		--[-------------------------------------------------------]
		--[ Private class attributes                              ]
		--[-------------------------------------------------------]
		local this					= {}		-- A private class attribute -> Emulates the C++ "this"-pointer by using a Lua table
		local gui					= GUI.new()	-- An instance of the GUI script component class
		local oldFilmPostProcess 	= 0			-- Old film post process effect factor (0 = not visible, 1 = fully visible)


		--[-------------------------------------------------------]
		--[ Private class methods                                 ]
		--[-------------------------------------------------------]
		--@brief
		--  Updates the old film post process effect
		local function UpdateOldFilm()
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


		--[-------------------------------------------------------]
		--[ Public class methods                                  ]
		--[-------------------------------------------------------]
		--@brief
		--  Updates the interaction script component
		function this.Update()
			-- Update the old film post process effect
			UpdateOldFilm()

			-- Update the instance of the GUI script component class
			gui:Update()

			-- Update the interaction application component
			local interaction = PL:GetApplication():GetInteraction()
			if interaction ~= nil then
				interaction:Update()
			end
		end


		-- Return the created class instance
		return this
	end


}
