--[-------------------------------------------------------]
--[ Includes                                              ]
--[-------------------------------------------------------]
require("Data/Scripts/Lua/ShowText")
require("Data/Scripts/Lua/Interaction")	-- Interaction script component class


--[-------------------------------------------------------]
--[ Classes                                               ]
--[-------------------------------------------------------]
--@brief
--  Application script component class
Application = {


	-- The default constructor - In Lua a static method
	new = function()


		--[-------------------------------------------------------]
		--[ Private class attributes                              ]
		--[-------------------------------------------------------]
		local this 			= {}				-- A private class attribute -> Emulates the C++ "this"-pointer by using a Lua table
		local interaction 	= Interaction.new()	-- An instance of the interaction script component class


		--[-------------------------------------------------------]
		--[ Private class methods                                 ]
		--[-------------------------------------------------------]
		--@brief
		--  Configurates the scene renderer
		local function ConfigureSceneRenderer()
			-- Get scene renderer tool
			local sceneRendererTool = PL.GetApplication():GetSceneRendererTool()

			-- More HDR bloom, please
			sceneRendererTool:SetPassAttribute("EndHDR", "BloomBrightThreshold", "0.2")
			sceneRendererTool:SetPassAttribute("EndHDR", "BloomDownscale", "4.0")
			sceneRendererTool:SetPassAttribute("EndHDR", "BloomFactor", "0.5")

			-- Use ambient occlusion also during lighting... this isn't physically correct, but within the dungeon it looks cool *g*
			sceneRendererTool:SetPassAttribute("DeferredLighting", "Flags", "NoShadowLOD")

			-- No one likes shy god rays, so increase them a bit *g*
			sceneRendererTool:SetPassAttribute("DeferredGodRays", "Density", "0.25")
			sceneRendererTool:SetPassAttribute("DeferredGodRays", "Decay", "0.92")
			sceneRendererTool:SetPassAttribute("DeferredGodRays", "Weight", "0.6")

			-- Tweak the HDR tone mapping settings a bit
			sceneRendererTool:SetPassAttribute("EndHDR", "Key", "0.4")
			sceneRendererTool:SetPassAttribute("EndHDR", "WhiteLevel", "100.0")
			sceneRendererTool:SetPassAttribute("EndHDR", "LuminanceConvert", "0.2125 0.2154 0.5721")	-- Give blue more importance... this isn't physically correct, but within the dungeon it looks cool *g*
		end

		--@brief
		--  Sets the depth fog density
		--
		--@param[in] density
		--  Depth fog density, <= 0 means there's no fog
		local function SetDepthFogDensity(density)
			-- Enable/disable the fog?
			if density > 0 then
				-- Enable the fog
				PL.GetApplication():GetSceneRendererTool():SetPassAttribute("DeferredDepthFog", "Flags", "")

				-- Set the fog density
				PL.GetApplication():GetSceneRendererTool():SetPassAttribute("DeferredDepthFog", "FogDensity", density)
			else
				-- Disable the fog
				PL.GetApplication():GetSceneRendererTool():SetPassAttribute("DeferredDepthFog", "Flags", "Inactive")
			end
		end

		--@brief
		--  Add the dancing skeleton to the scene
		local function AddDancingSkeleton()
			-- Get the scene container
			local sceneContainer = PL.GetApplication():GetScene()
			if sceneContainer ~= nil then
				-- Get the tavern scene container
				local tavernSceneContainer = sceneContainer:GetByName("Container.Tavern")
				if tavernSceneContainer ~= nil then
					-- Add the first dancing skeleton
					local dancingSkeletonSceneNode = tavernSceneContainer:Create("PLScene::SNMesh", "Bulck_DancingSkeleton", "Flags='CastShadow|ReceiveShadow' Position='18.0 -1.32 -2.0' Scale='0.75 0.75 0.75' Mesh='Data/Meshes/Bulck_DancingSkeleton.mesh' StaticMesh='0'")
					if dancingSkeletonSceneNode ~= nil then
						dancingSkeletonSceneNode:AddModifier("PLScene::SNMMeshAnimation", "Name='Skeleton animation'")
					end

					-- [TODO] Disabled because skinning currently is performed on the CPU (no multithreading, too) and this mesh has a lot of vertices...
					-- Add the second dancing skeleton
				--	dancingSkeletonSceneNode = tavernSceneContainer:Create("PLScene::SNMesh", "Doerholt_DancingSkeleton", "Flags='CastShadow|ReceiveShadow' Position='5.5 -1.38 1.0' Rotation='0 180 0' Scale='0.02 0.02 0.02' Mesh='Data/Meshes/Doerholt_DancingSkeleton.mesh' StaticMesh='0'")
				--	if dancingSkeletonSceneNode ~= then
				--		-- [TODO] "Skeleton animation_2" instead of "Skeleton animation" because the name was changed due name conflicts within the skeleton manager... not good, rethink this...
				--		dancingSkeletonSceneNode:AddModifier("PLScene::SNMMeshAnimation", "Name='Skeleton animation_2'")
				--	end
				end
			end
		end


		--[-------------------------------------------------------]
		--[ Public class methods                                  ]
		--[-------------------------------------------------------]
		--@brief
		--  Updates the application script component
		function this.Update()
			-- Update the instance of the interaction script component class
			interaction:Update()
		end

		--@brief
		--  Slot function is called by C++ after a new camera has been set
		function this.OnCameraSet()
			-- Make the current set camera to the listener of the sound manager *our ear*
			local rootScene = PL.GetApplication():GetRootScene()	-- The root scene is an instance of "PLSound::SCSound"
			if rootScene ~= nil then
				-- Get the currently set camera scene node
				local cameraSceneNode = PL.GetApplication():GetCamera()
				if cameraSceneNode ~= nil then
					rootScene.Listener = cameraSceneNode:GetAbsoluteName()	-- Use the current camera scene node as listener
				else
					rootScene.Listener = ""	-- No listener scene node set
				end
			end
		end

		--@brief
		--  Slot function is called by C++ after a scene has been loaded
		function this.OnSceneLoadingFinished()
			-- Configurate the scene renderer
			ConfigureSceneRenderer()

			-- Set the fog density
			SetDepthFogDensity(0.6)

			-- Adds the dancing skeleton to the scene
			AddDancingSkeleton()

			-- Show an initial help text for 10 seconds
			if not PL.GetApplication():IsExpertMode() then
				ShowText("Press ESC to open menu", 10.0)
			end
		end


		--[-------------------------------------------------------]
		--[ Public class constructor implementation               ]
		--[-------------------------------------------------------]
		-- Use the script function "OnCameraSet" as slot and connect it with the RTTI "SignalCameraSet"-signal of our RTTI application class instance
		PL.GetApplication().SignalCameraSet.Connect(this.OnCameraSet)

		-- Use the script function "OnSceneLoadingFinished" as slot and connect it with the RTTI "SignalSceneLoadingFinished"-signal of our RTTI application class instance
		PL.GetApplication().SignalSceneLoadingFinished.Connect(this.OnSceneLoadingFinished)


		-- Return the created class instance
		return this
	end


}
