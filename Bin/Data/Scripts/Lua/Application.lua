--/*********************************************************\
-- *  File: Application.lua                                *
-- *
-- *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
-- *
-- *  This file is part of PixelLight.
-- *
-- *  PixelLight is free software: you can redistribute it and/or modify
-- *  it under the terms of the GNU Lesser General Public License as published by
-- *  the Free Software Foundation, either version 3 of the License, or
-- *  (at your option) any later version.
-- *
-- *  PixelLight is distributed in the hope that it will be useful,
-- *  but WITHOUT ANY WARRANTY; without even the implied warranty of
-- *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- *  GNU Lesser General Public License for more details.
-- *
-- *  You should have received a copy of the GNU Lesser General Public License
-- *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
--\*********************************************************/


--[-------------------------------------------------------]
--[ Includes                                              ]
--[-------------------------------------------------------]
require "Interaction"	-- Interaction script component class


--[-------------------------------------------------------]
--[ Classes                                               ]
--[-------------------------------------------------------]
--@brief
--  Application script component class
Application = {


	--@brief
	--  The default constructor - In Lua a static method
	--
	--@param[in] cppApplication
	--  C++ RTTI application class instance
	new = function(cppApplication)


		--[-------------------------------------------------------]
		--[ Private class attributes                              ]
		--[-------------------------------------------------------]
		local this         = {}		-- A private class attribute -> Emulates the C++ "this"-pointer by using a Lua table
		local cppCamcorder = nil	-- Camcorder instance (C++ PixelLight RTTI class)
		local cppIngameGui = nil	-- Ingame GUI instance (C++ PixelLight RTTI class)


		--[-------------------------------------------------------]
		--[ Public class methods                                  ]
		--[-------------------------------------------------------]
		--@brief
		--  Returns whether or not this is an internal release
		--
		--@return
		--  'true' if this is an internal release, else 'false'
		function this.IsInternalRelease()
			-- The "IsInternalRelease()"-method is implemented within the dungeon executable
			if cppApplication.IsInternalRelease ~= nil then
				return cppApplication:IsInternalRelease()
			else
				return false
			end
		end

		--@brief
		--  Returns the camcorder instance, or nil in case there's no instance
		--
		--@return
		--  Camcorder instance, or nil in case there's no instance
		function this.GetCamcorder()
			-- Create the camcorder instance right now?
			if cppCamcorder == nil then
				cppCamcorder = PL.ClassManager.CreateByConstructor("PLEngine::Camcorder", "ParameterConstructor", "Param0=\"" .. tostring(cppApplication) .. "\"")
			end

			-- Return the camcorder instance
			return cppCamcorder
		end

		--@brief
		--  Returns the ingame GUI instance, or nil in case there's no instance
		--
		--@return
		--  Ingame GUI instance, or nil in case there's no instance
		function this.GetIngameGui()
			-- Create the ingame GUI instance right now?
			if cppIngameGui == nil then
				cppIngameGui = PL.ClassManager.CreateByConstructor("IngameGui", "ParameterConstructor", "Param0=\"" .. tostring(cppApplication) .. "\"")
			end

			-- Return the Ingame GUI instance
			return cppIngameGui
		end


		--[-------------------------------------------------------]
		--[ Private class attributes                              ]
		--[-------------------------------------------------------]
		local _interaction 	= Interaction.new(cppApplication, this)	-- An instance of the interaction script component class
		local _loadProgress = 0										-- Current load progress (0.0-1.0)


		--[-------------------------------------------------------]
		--[ Private class methods                                 ]
		--[-------------------------------------------------------]
		--@brief
		--  Configurates the scene renderer
		local function ConfigureSceneRenderer()
			-- Get scene renderer tool
			local sceneRendererTool = cppApplication:GetSceneRendererTool()

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
				cppApplication:GetSceneRendererTool():SetPassAttribute("DeferredDepthFog", "Flags", "")

				-- Set the fog density
				cppApplication:GetSceneRendererTool():SetPassAttribute("DeferredDepthFog", "FogDensity", density)
			else
				-- Disable the fog
				cppApplication:GetSceneRendererTool():SetPassAttribute("DeferredDepthFog", "Flags", "Inactive")
			end
		end

		--@brief
		--  Add the dancing skeleton to the scene
		local function AddDancingSkeleton()
			-- Get the scene container
			local sceneContainer = cppApplication:GetScene()
			if sceneContainer ~= nil then
				-- Get the tavern scene container
				local tavernSceneContainer = sceneContainer:GetByName("Container.Tavern")
				if tavernSceneContainer ~= nil then
					-- Add the first dancing skeleton
					local dancingSkeletonSceneNode = tavernSceneContainer:Create("PLScene::SNMesh", "Bulck_DancingSkeleton", "Flags='CastShadow|ReceiveShadow' Position='18.0 -1.32 -2.0' Scale='0.75 0.75 0.75' Mesh='Data/Meshes/Bulck_DancingSkeleton.mesh' StaticMesh='0'")
					if dancingSkeletonSceneNode ~= nil then
						dancingSkeletonSceneNode:AddModifier("PLScene::SNMMeshAnimation", "Name='Skeleton animation'")
					end
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
			_interaction.Update()

			-- Get the ingame GUI component
			local ingameGui = this.GetIngameGui()
			if ingameGui ~= nil then
				-- Get the currently set camera scene node
				local cameraSceneNode = cppApplication:GetCamera()
				if cameraSceneNode ~= nil then
					-- Deactivate the camera if the GUI is currently shown
					cameraSceneNode:SetActive(not ingameGui:IsGuiShown())
				end

				-- Update the ingame GUI
				ingameGui:Update()
			end

			-- Update the camcorder
			local camcorder = this.GetCamcorder()
			if camcorder ~= nil then
				camcorder:Update()
			end

			-- Update the mouse picking pull animation (implemented in the dungeon executable)
			if cppApplication.UpdateMousePickingPullAnimation ~= nil then
				cppApplication:UpdateMousePickingPullAnimation()
			end
		end

		--@brief
		--  Slot function is called by C++ after a new camera has been set
		function this.OnCameraSet()
			-- Make the current set camera to the listener of the sound manager *our ear*
			local rootScene = cppApplication:GetRootScene()	-- The root scene is an instance of "PLSound::SCSound"
			if rootScene ~= nil then
				-- Get the currently set camera scene node
				local cameraSceneNode = cppApplication:GetCamera()
				if cameraSceneNode ~= nil then
					rootScene.Listener = cameraSceneNode:GetAbsoluteName()	-- Use the current camera scene node as listener
				else
					rootScene.Listener = ""	-- No listener scene node set
				end
			end
		end

		--@brief
		--  Called on load progress
		--
		--@param[in] progress
		--  Load progress (0.0-1.0)
		function this.OnLoadProgress(progress)
			-- Time for an update? (we don't want to redraw & ping the frontend each time a single tiny scene node was loaded *performance*)
			if (progress - _loadProgress) >= 0.01 then
				_loadProgress = progress;

				-- Redraw & ping the frontend
				cppApplication:GetFrontend():RedrawAndPing()
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
			if not cppApplication:IsExpertMode() then
				this.ShowText("Press ESC to open menu", 10.0)
			end
		end

		--@brief
		--  Returns the currently shown text
		--
		--@return
		--  The currently shown text
		function this.GetShownText()
			-- Get the root scene container
			local sceneContainer = cppApplication:GetRootScene()
			if sceneContainer ~= nil then
				-- Get the text scene node
				local sceneNode = sceneContainer:GetByName("Text")
				if sceneNode ~= nil and sceneNode:IsActive() then
					-- Return the text
					return sceneNode.Text
				end
			end

			-- Just return an empty string
			return ""
		end

		--@brief
		--  Shows a text
		--
		--@param[in] text
		--  Text to show
		--@param[in] timeout
		--  Timeout (in seconds)
		function this.ShowText(text, timeout)
			-- Get the root scene container
			local sceneContainer = cppApplication:GetRootScene()
			if sceneContainer ~= nil then
				-- Get the text scene node, or create it right now
				local sceneNode = sceneContainer:GetByName("Text")
				if sceneNode == nil then
					-- Create the text scene node right now
					sceneNode = sceneContainer:Create("PLScene::SNText2D", "Text", "Position='0.5 0.95 0.0' Scale='1.1 1.1 1.1' Flags='No3DPosition'")
				end
				if sceneNode ~= nil then
					-- Set the text
					sceneNode.Text = text

					-- Make the scene node active (in case SNMDeactivationOnTimeout already deactivated it)
					sceneNode:SetActive(true)

					-- Get the timeout scene node modifier, or create it right now
					local sceneNodeModifier = sceneNode:GetModifier("PLScene::SNMDeactivationOnTimeout")
					if sceneNodeModifier == nil then
						-- Create the scene node modifier right now
						sceneNodeModifier = sceneNode:AddModifier("PLScene::SNMDeactivationOnTimeout")
					end
					if sceneNodeModifier ~= nil then
						-- Set the timeout
						sceneNodeModifier.Timeout = timeout
					end
				end
			end
		end


		--[-------------------------------------------------------]
		--[ Public class constructor implementation               ]
		--[-------------------------------------------------------]
		-- Use the script function "OnCameraSet" as slot and connect it with the RTTI "SignalCameraSet"-signal of our RTTI application class instance
		cppApplication.SignalCameraSet.Connect(this.OnCameraSet)

		-- Use the script function "OnSceneLoadingFinished" as slot and connect it with the RTTI "SignalSceneLoadingFinished"-signal of our RTTI application class instance
		cppApplication.SignalSceneLoadingFinished.Connect(this.OnSceneLoadingFinished)

		-- Use the script function "OnLoadProgress" as slot and connect it with the RTTI "SignalLoadProgress"-signal of our RTTI scene container class instance
		cppApplication:GetScene().SignalLoadProgress.Connect(this.OnLoadProgress)


		-- Return the created class instance
		return this
	end


}
