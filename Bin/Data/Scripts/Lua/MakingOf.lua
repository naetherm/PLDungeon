--[-------------------------------------------------------]
--[ Classes                                               ]
--[-------------------------------------------------------]
--@brief
--  Making of script component class
MakingOf = {


	--@brief
	--  The default constructor - In Lua a static method
	--
	--@param[in] cppApplication
	--  C++ RTTI application class instance
	--@param[in] luaApplication
	--  Lua script application class instance
	--@param[in] onMakingOfPlaybackFinishedFunction
	--  Slot function is called by Lua when the making of playback has been finished
	new = function(cppApplication, luaApplication, onMakingOfPlaybackFinishedFunction)


		--[-------------------------------------------------------]
		--[ Private class attributes                              ]
		--[-------------------------------------------------------]
		local this								= {}	-- A private class attribute -> Emulates the C++ "this"-pointer by using a Lua table
		local _playing							= false	-- Is playback currently enabled?
		local _sceneRendererPassSettingsBackup	= {} 	-- Backup of all scene renderer pass settings, <key>=scene renderer pass name, <value>=Values of all scene renderer pass attributes
		local _timeScale						= 1		-- Time scale to speed up or slow down the playback
		local _state							= -1	-- Current state (changes over playback time, -1=invalid state)
		local _stateTimeout						= 0		-- State timeout until next state (in seconds)


		--[-------------------------------------------------------]
		--[ Private class methods                                 ]
		--[-------------------------------------------------------]
		--@brief
		--  Backups all scene renderer settings
		local function BackupSceneRendererSettings()
			-- Clear the previous scene renderer backup
			_sceneRendererPassSettingsBackup = {};

			-- Loop through all scene renderer passes
			for i=0,cppApplication:GetSceneRendererTool():GetNumOfPasses() do
				local pass = cppApplication:GetSceneRendererTool():GetPassByIndex(i)
				if pass ~= nil then
					_sceneRendererPassSettingsBackup[pass.Name] = pass:ToString()
				end
			end
		end

		--@brief
		--  Restores all scene renderer settings
		local function RestoreSceneRendererSettings()
			-- Loop through the scene renderer passes backup
			for key, value in pairs(_sceneRendererPassSettingsBackup) do
				local pass = cppApplication:GetSceneRendererTool():GetPassByName(key)
				if pass ~= nil then
					pass:FromString(value)
				end
			end
		end

		--@brief
		--  Makes the provided state to the currently used one
		--
		--@param[in] newState
		--  New state
		--
		--@return
		--  State text or nil
		local function SetState(newState)
			-- Restore all scene renderer settings
			RestoreSceneRendererSettings()

			-- Set the new state
			_state = newState

			-- Get scene renderer tool
			local sceneRendererTool = cppApplication:GetSceneRendererTool()

			-- Lua does not support switch/case statements, so we just use a Lua table (using if/else in here would be somewhat extreme)
			local action = {
				-- Making of start - should already be through if we're in here the first time...
				[0] = function()
					_stateTimeout = 5	-- Wait a few seconds, then go into the next state
				end,

				-- Scene nodes - icons
				[1] = function()
					sceneRendererTool:SetPassAttribute("DebugSceneNodeIcons", "Flags", "")
					_stateTimeout = 2					-- Wait a few seconds, then go into the next state
					return "A scene consists of nodes"	-- Return the state text
				end,

				-- Scene nodes - names
				[2] = function()
					sceneRendererTool:SetPassAttribute("DebugSceneNodeIcons", "Flags", "")
					sceneRendererTool:SetPassAttribute("DebugSceneNodeNames", "Flags", "")
					_stateTimeout = 5					-- Wait a few seconds, then go into the next state
					return "A scene consists of nodes"	-- Return the state text
				end,

				-- Wireframes
				[3] = function()
					sceneRendererTool:SetPassAttribute("DebugWireframes", "Flags", "")
					_stateTimeout = 8						-- Wait a few seconds, then go into the next state
					return "A node can represent a mesh"	-- Return the state text
				end,

				-- Deferred rendering - Albedo
				[4] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowAlbedo")
					_stateTimeout = 5						-- Wait a few seconds, then go into the next state
					return "Deferred rendering - Albedo"	-- Return the state text
				end,

				-- Deferred rendering - Ambient occlusion
				[5] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowAmbientOcclusion")
					_stateTimeout = 5								-- Wait a few seconds, then go into the next state
					return "Deferred rendering - Ambient occlusion"	-- Return the state text
				end,

				-- Deferred rendering - View space normals
				[6] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowNormals")
					_stateTimeout = 5									-- Wait a few seconds, then go into the next state
					return "Deferred rendering - View space normals"	-- Return the state text
				end,

				-- Deferred rendering - Depth
				[7] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowDepth")
					_stateTimeout = 5					-- Wait a few seconds, then go into the next state
					return "Deferred rendering - Depth"	-- Return the state text
				end,

				-- Deferred rendering - Specular color
				[8] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowSpecularColor")
					_stateTimeout = 5								-- Wait a few seconds, then go into the next state
					return "Deferred rendering - Specular color"	-- Return the state text
				end,

				-- Deferred rendering - Specular exponent
				[9] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowSpecularExponent")
					_stateTimeout = 5								-- Wait a few seconds, then go into the next state
					return "Deferred rendering - Specular exponent"	-- Return the state text
				end,

				-- Deferred rendering - Self illumination
				[10] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowSelfIllumination")
					_stateTimeout = 5								-- Wait a few seconds, then go into the next state
					return "Deferred rendering - Self illumination"	-- Return the state text
				end,

				-- Deferred rendering - Glow
				[11] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
					sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowGlow")
					_stateTimeout = 5					-- Wait a few seconds, then go into the next state
					return "Deferred rendering - Glow"	-- Return the state text
				end,

				-- Ambient
				[12] = function()
					sceneRendererTool:SetPassAttribute("DeferredSPSSAO",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredGlow",			"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredLighting",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredGodRays",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDOF",			"Flags", "Inactive")
					_stateTimeout = 5	-- Wait a few seconds, then go into the next state
					return "Ambient"	-- Return the state text
				end,

				-- Ambient + SSAO
				[13] = function()
					sceneRendererTool:SetPassAttribute("DeferredGlow",			"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredLighting",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredGodRays",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDOF",			"Flags", "Inactive")
					_stateTimeout = 5		-- Wait a few seconds, then go into the next state
					return "Ambient + SSAO"	-- Return the state text
				end,

				-- Ambient + SSAO + Lighting
				[14] = function()
					sceneRendererTool:SetPassAttribute("DeferredGlow",			"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredLighting",		"Flags", "NoShadow")
					sceneRendererTool:SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredGodRays",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDOF",			"Flags", "Inactive")
					_stateTimeout = 5					-- Wait a few seconds, then go into the next state
					return "Ambient + SSAO + Lighting"	-- Return the state text
				end,

				-- Ambient + SSAO + Lighting + Shadows
				[15] = function()
					sceneRendererTool:SetPassAttribute("DeferredGlow",			"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredGodRays",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDOF",			"Flags", "Inactive")
					_stateTimeout = 5								-- Wait a few seconds, then go into the next state
					return "Ambient + SSAO + Lighting + Shadows"	-- Return the state text
				end,

				-- Ambient + SSAO + Lighting + Shadows + Glow
				[16] = function()
					sceneRendererTool:SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive")
					sceneRendererTool:SetPassAttribute("DeferredDOF",			"Flags", "Inactive")
					_stateTimeout = 5									-- Wait a few seconds, then go into the next state
					return "Ambient + SSAO + Lighting + Shadows + Glow"	-- Return the state text
				end,

				-- Ambient + SSAO + Lighting + Shadows + Glow + Fog
				[17] = function()
					sceneRendererTool:SetPassAttribute("DeferredDOF", "Flags", "Inactive")
					_stateTimeout = 5											-- Wait a few seconds, then go into the next state
					return "Ambient + SSAO + Lighting + Shadows + Glow + Fog"	-- Return the state text
				end,

				-- Ambient + SSAO + Lighting + Shadows + Glow + Fog + DOF
				[18] = function()
					-- No set state settings... this is the default setting
					_stateTimeout = 5												-- Wait a few seconds, then go into the next state
					return "Ambient + SSAO + Lighting + Shadows + Glow + Fog + DOF"	-- Return the state text
				end,

				-- Texture maps: No maps
				[19] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDiffuseMap|NoSpecularMap|NoNormalMap|NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
					_stateTimeout = 5				-- Wait a few seconds, then go into the next state
					return "Texture maps: No maps"	-- Return the state text
				end,

				-- Texture maps: Diffuse
				[20] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoSpecularMap|NoNormalMap|NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
					_stateTimeout = 5				-- Wait a few seconds, then go into the next state
					return "Texture maps: Diffuse"	-- Return the state text
				end,

				-- Texture maps: Diffuse + Specular
				[21] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoNormalMap|NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
					_stateTimeout = 5							-- Wait a few seconds, then go into the next state
					return "Texture maps: Diffuse + Specular"	-- Return the state text
				end,

				-- Texture maps: Diffuse + Specular + Normal
				[22] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
					_stateTimeout = 5									-- Wait a few seconds, then go into the next state
					return "Texture maps: Diffuse + Specular + Normal"	-- Return the state text
				end,

				-- Texture maps: Diffuse + Specular + Normal + Height
				[23] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
					_stateTimeout = 5											-- Wait a few seconds, then go into the next state
					return "Texture maps: Diffuse + Specular + Normal + Height"	-- Return the state text
				end,

				-- Texture maps: Diffuse + Specular + Normal + Height + Emissive
				[24] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoAmbientOcclusionMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
					_stateTimeout = 5														-- Wait a few seconds, then go into the next state
					return "Texture maps: Diffuse + Specular + Normal + Height + Emissive"	-- Return the state text
				end,

				-- Texture maps: Diffuse + Specular + Normal + Height + Emissive + Reflection
				[25] = function()
					sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoAmbientOcclusionMap|NoLightMap")
					_stateTimeout = 5																	-- Wait a few seconds, then go into the next state
					return "Texture maps: Diffuse + Specular + Normal + Height + Emissive + Reflection"	-- Return the state text
				end,

				-- HDR: off
				[26] = function()
					sceneRendererTool:SetPassAttribute("EndHDR", "Flags", "Inactive")
					_stateTimeout = 5	-- Wait a few seconds, then go into the next state
					return "HDR: off"	-- Return the state text
				end,

				-- HDR: Reinhard tone mapping
				[27] = function()
					sceneRendererTool:SetPassAttribute("EndHDR", "Flags", "NoAutomaticAverageLuminance|NoLightAdaptation|NoBloom")
					_stateTimeout = 5					-- Wait a few seconds, then go into the next state
					return "HDR: Reinhard tone mapping"	-- Return the state text
				end,

				-- HDR: Reinhard tone mapping + Automatic light adaptation
				[28] = function()
					sceneRendererTool:SetPassAttribute("EndHDR", "Flags", "NoBloom")
					_stateTimeout = 5													-- Wait a few seconds, then go into the next state
					return "HDR: Reinhard tone mapping + Automatic light adaptation"	-- Return the state text
				end,

				-- HDR: Reinhard tone mapping + Automatic light adaptation + Bloom
				[29] = function()
					-- No set state settings... this is the default setting
					_stateTimeout = 5															-- Wait a few seconds, then go into the next state
					return "HDR: Reinhard tone mapping + Automatic light adaptation + Bloom"	-- Return the state text
				end,

				-- Making of end 1 
				[30] = function()
					_stateTimeout = 10											-- Wait a few seconds, then go into the next state
					return "That's all folks - visit us at www.pixellight.org"	-- Return the state text
				end,

				-- Making of end 2
				[31] = function()
					_stateTimeout = 7														-- Wait a few seconds, then go into the next state
					return "Have fun with the dungeon exploration! (space mouse supported)"	-- Return the state text
				end,

				-- Making of end 3
				[32] = function()
					_stateTimeout = 6									-- Wait a few seconds, then go into the next state
					return "Use the keys 1-5 to change the demo mode"	-- Return the state text
				end,

				-- Making of finished
				[33] = function()
					-- Emit the playback finished signal (... ok ok, it's just a simple Lua function...)
					onMakingOfPlaybackFinishedFunction()
				end,
			}

			-- Execute the "kind of" switch/case statement
			return action[_state]()
		end


		--[-------------------------------------------------------]
		--[ Public class methods                                  ]
		--[-------------------------------------------------------]
		-- @brief
		--  Starts/restarts the playback
		function this.StartPlayback()
			-- Stop the previous playback
			this:StopPlayback()

			-- Playback is now enabled
			_playing		= true
			_state			= -1
			_stateTimeout	= 0

			-- Backup all scene renderer settings
			BackupSceneRendererSettings()
		end

		--@brief
		--  Returns whether or not playback is currently active
		--
		--@return
		--  'true' if playback is currently active, else 'false'
		function this.IsPlaying()
			return _playing
		end

		--@brief
		--  Stops the playback
		function this.StopPlayback()
			-- Is playback currently enabled?
			if _playing then
				-- Restore all scene renderer settings
				RestoreSceneRendererSettings()

				-- Playback is no longer enabled
				_playing = false
			end
		end

		--@brief
		--  Updates the making of script component
		function this.Update()
			-- Is playback currently enabled?
			if _playing then
				-- Get the scaled time difference
				local timeDiff = PL.Timing.GetTimeDifference()*_timeScale

				-- Update the state timeout
				_stateTimeout = _stateTimeout - timeDiff
				if _stateTimeout <= 0 then
					-- Set the next state
					local text = SetState(_state + 1)

					-- Show the text
					if text ~= nil then
						luaApplication.ShowText(text, _stateTimeout + 1)	-- +1 second so the text don't get invisible when showing twice
					end
				end
			end
		end


		--[-------------------------------------------------------]
		--[ Public class constructor implementation               ]
		--[-------------------------------------------------------]
		if cppApplication:IsInternalRelease() then
			-- Reduce the timeout for the internal release - else we would have to wait to long to test the demo
			_timeScale = 20
		end


		-- Return the created class instance
		return this
	end


}
