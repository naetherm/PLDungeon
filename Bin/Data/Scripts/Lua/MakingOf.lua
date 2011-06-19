--[-------------------------------------------------------]
--[ Includes                                              ]
--[-------------------------------------------------------]
require("Data/Scripts/Lua/ShowText")


--[-------------------------------------------------------]
--[ Global variables                                      ]
--[-------------------------------------------------------]
sceneRendererPassSettingsBackup	= {} 	-- Backup of all scene renderer pass settings, <key>=scene renderer pass name, <value>=Values of all scene renderer pass attributes
timeScale						= 1		-- Time scale to speed up or slow down the playback
playbackTime					= 0		-- Total playback time (in seconds)
state							= -1	-- Current state (changes over playback time, -1=invalid state)
stateTimeout					= 0		-- State timeout until next state (in seconds)


--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Backups all scene renderer settings
function BackupSceneRendererSettings()
	-- Clear the previous scene renderer backup
	sceneRendererPassSettingsBackup = {};

	-- Loop through all scene renderer passes
	for i=0,PL.GetApplication():GetSceneRendererTool():GetNumOfPasses() do
		local pass = PL.GetApplication():GetSceneRendererTool():GetPassByIndex(i)
		if pass ~= nil then
			sceneRendererPassSettingsBackup[pass.Name] = pass:ToString()
		end
	end
end

--@brief
--  Restores all scene renderer settings
function RestoreSceneRendererSettings()
	-- Loop through the scene renderer passes backup
	for key, value in pairs(sceneRendererPassSettingsBackup) do
		local pass = PL.GetApplication():GetSceneRendererTool():GetPassByName(key)
		if pass ~= nil then
			pass:FromString(value)
		end
	end
end

--@brief
--  Called by C++ when the script should initialize itself
function OnStartPlayback()
	-- Backup all scene renderer settings
	BackupSceneRendererSettings()
end

--@brief
--  Called by C++ when the script should update itself
function OnUpdate()
	-- Get the scaled time difference
	local timeDiff = PL.Timing.GetTimeDifference()*timeScale

	-- Update the total playback time
	playbackTime = playbackTime + timeDiff

	-- Update the state timeout
	stateTimeout = stateTimeout - timeDiff
	if stateTimeout <= 0 then
		-- Set the next state
		local text = SetState(state + 1)

		-- Show the text
		if text ~= nil then
			ShowText(text, stateTimeout + 1)	-- +1 second so the text don't get invisible when showing twice
		end
	end
end

--@brief
--  Called by C++ when the script should stop itself
function OnStopPlayback()
	-- Restore all scene renderer settings
	RestoreSceneRendererSettings()
end

--@brief
--  Makes the provided state to the currently used one
--
--@param[in] newState
--  New state
--
--@return
--  State text or nil
function SetState(newState)
	-- Restore all scene renderer settings
	RestoreSceneRendererSettings()

	-- Set the new state
	state = newState

	-- Get scene renderer tool
	local sceneRendererTool = PL.GetApplication():GetSceneRendererTool()

	-- Lua does not support switch/case statements, so we just use a Lua table (using if/else in here would be somewhat extreme)
	action = {
		-- Making of start - should already be through if we're in here the first time...
		[0] = function()
			stateTimeout = 5	-- Wait a few seconds, then go into the next state
		end,

		-- Scene nodes - icons
		[1] = function()
			sceneRendererTool:SetPassAttribute("DebugSceneNodeIcons", "Flags", "")
			stateTimeout = 2					-- Wait a few seconds, then go into the next state
			return "A scene consists of nodes"	-- Return the state text
		end,

		-- Scene nodes - names
		[2] = function()
			sceneRendererTool:SetPassAttribute("DebugSceneNodeIcons", "Flags", "")
			sceneRendererTool:SetPassAttribute("DebugSceneNodeNames", "Flags", "")
			stateTimeout = 5					-- Wait a few seconds, then go into the next state
			return "A scene consists of nodes"	-- Return the state text
		end,

		-- Wireframes
		[3] = function()
			sceneRendererTool:SetPassAttribute("DebugWireframes", "Flags", "")
			stateTimeout = 8						-- Wait a few seconds, then go into the next state
			return "A node can represent a mesh"	-- Return the state text
		end,

		-- Deferred rendering - Albedo
		[4] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowAlbedo")
			stateTimeout = 5						-- Wait a few seconds, then go into the next state
			return "Deferred rendering - Albedo"	-- Return the state text
		end,

		-- Deferred rendering - Ambient occlusion
		[5] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowAmbientOcclusion")
			stateTimeout = 5								-- Wait a few seconds, then go into the next state
			return "Deferred rendering - Ambient occlusion"	-- Return the state text
		end,

		-- Deferred rendering - View space normals
		[6] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowNormals")
			stateTimeout = 5									-- Wait a few seconds, then go into the next state
			return "Deferred rendering - View space normals"	-- Return the state text
		end,

		-- Deferred rendering - Depth
		[7] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowDepth")
			stateTimeout = 5					-- Wait a few seconds, then go into the next state
			return "Deferred rendering - Depth"	-- Return the state text
		end,

		-- Deferred rendering - Specular color
		[8] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowSpecularColor")
			stateTimeout = 5								-- Wait a few seconds, then go into the next state
			return "Deferred rendering - Specular color"	-- Return the state text
		end,

		-- Deferred rendering - Specular exponent
		[9] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowSpecularExponent")
			stateTimeout = 5								-- Wait a few seconds, then go into the next state
			return "Deferred rendering - Specular exponent"	-- Return the state text
		end,

		-- Deferred rendering - Self illumination
		[10] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowSelfIllumination")
			stateTimeout = 5								-- Wait a few seconds, then go into the next state
			return "Deferred rendering - Self illumination"	-- Return the state text
		end,

		-- Deferred rendering - Glow
		[11] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Flags", "")
			sceneRendererTool:SetPassAttribute("DeferredGBufferDebug", "Mode",  "ShowGlow")
			stateTimeout = 5					-- Wait a few seconds, then go into the next state
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
			stateTimeout = 5	-- Wait a few seconds, then go into the next state
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
			stateTimeout = 5		-- Wait a few seconds, then go into the next state
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
			stateTimeout = 5					-- Wait a few seconds, then go into the next state
			return "Ambient + SSAO + Lighting"	-- Return the state text
		end,

		-- Ambient + SSAO + Lighting + Shadows
		[15] = function()
			sceneRendererTool:SetPassAttribute("DeferredGlow",			"Flags", "Inactive")
			sceneRendererTool:SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive")
			sceneRendererTool:SetPassAttribute("DeferredGodRays",		"Flags", "Inactive")
			sceneRendererTool:SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive")
			sceneRendererTool:SetPassAttribute("DeferredDOF",			"Flags", "Inactive")
			stateTimeout = 5								-- Wait a few seconds, then go into the next state
			return "Ambient + SSAO + Lighting + Shadows"	-- Return the state text
		end,

		-- Ambient + SSAO + Lighting + Shadows + Glow
		[16] = function()
			sceneRendererTool:SetPassAttribute("ForwardVolumetricFog",	"Flags", "Inactive")
			sceneRendererTool:SetPassAttribute("DeferredDepthFog",		"Flags", "Inactive")
			sceneRendererTool:SetPassAttribute("DeferredDOF",			"Flags", "Inactive")
			stateTimeout = 5									-- Wait a few seconds, then go into the next state
			return "Ambient + SSAO + Lighting + Shadows + Glow"	-- Return the state text
		end,

		-- Ambient + SSAO + Lighting + Shadows + Glow + Fog
		[17] = function()
			sceneRendererTool:SetPassAttribute("DeferredDOF", "Flags", "Inactive")
			stateTimeout = 5											-- Wait a few seconds, then go into the next state
			return "Ambient + SSAO + Lighting + Shadows + Glow + Fog"	-- Return the state text
		end,

		-- Ambient + SSAO + Lighting + Shadows + Glow + Fog + DOF
		[18] = function()
			-- No set state settings... this is the default setting
			stateTimeout = 5												-- Wait a few seconds, then go into the next state
			return "Ambient + SSAO + Lighting + Shadows + Glow + Fog + DOF"	-- Return the state text
		end,

		-- Texture maps: No maps
		[19] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDiffuseMap|NoSpecularMap|NoNormalMap|NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
			stateTimeout = 5				-- Wait a few seconds, then go into the next state
			return "Texture maps: No maps"	-- Return the state text
		end,

		-- Texture maps: Diffuse
		[20] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoSpecularMap|NoNormalMap|NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
			stateTimeout = 5				-- Wait a few seconds, then go into the next state
			return "Texture maps: Diffuse"	-- Return the state text
		end,

		-- Texture maps: Diffuse + Specular
		[21] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoNormalMap|NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
			stateTimeout = 5							-- Wait a few seconds, then go into the next state
			return "Texture maps: Diffuse + Specular"	-- Return the state text
		end,

		-- Texture maps: Diffuse + Specular + Normal
		[22] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoParallaxMapping|NoDisplacementMapping|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
			stateTimeout = 5									-- Wait a few seconds, then go into the next state
			return "Texture maps: Diffuse + Specular + Normal"	-- Return the state text
		end,

		-- Texture maps: Diffuse + Specular + Normal + Height
		[23] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoAmbientOcclusionMap|NoLightMap|NoEmissiveMap|NoGlow|NoGlowMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
			stateTimeout = 5											-- Wait a few seconds, then go into the next state
			return "Texture maps: Diffuse + Specular + Normal + Height"	-- Return the state text
		end,

		-- Texture maps: Diffuse + Specular + Normal + Height + Emissive
		[24] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoAmbientOcclusionMap|NoFresnelReflection|NoReflectivityMap|NoReflectionMap")
			stateTimeout = 5														-- Wait a few seconds, then go into the next state
			return "Texture maps: Diffuse + Specular + Normal + Height + Emissive"	-- Return the state text
		end,

		-- Texture maps: Diffuse + Specular + Normal + Height + Emissive + Reflection
		[25] = function()
			sceneRendererTool:SetPassAttribute("DeferredGBuffer", "Flags", "NoDetailNormalMap|NoAmbientOcclusionMap|NoLightMap")
			stateTimeout = 5																	-- Wait a few seconds, then go into the next state
			return "Texture maps: Diffuse + Specular + Normal + Height + Emissive + Reflection"	-- Return the state text
		end,

		-- HDR: off
		[26] = function()
			sceneRendererTool:SetPassAttribute("EndHDR", "Flags", "Inactive")
			stateTimeout = 5	-- Wait a few seconds, then go into the next state
			return "HDR: off"	-- Return the state text
		end,

		-- HDR: Reinhard tone mapping
		[27] = function()
			sceneRendererTool:SetPassAttribute("EndHDR", "Flags", "NoAutomaticAverageLuminance|NoLightAdaptation|NoBloom")
			stateTimeout = 5					-- Wait a few seconds, then go into the next state
			return "HDR: Reinhard tone mapping"	-- Return the state text
		end,

		-- HDR: Reinhard tone mapping + Automatic light adaptation
		[28] = function()
			sceneRendererTool:SetPassAttribute("EndHDR", "Flags", "NoBloom")
			stateTimeout = 5													-- Wait a few seconds, then go into the next state
			return "HDR: Reinhard tone mapping + Automatic light adaptation"	-- Return the state text
		end,

		-- HDR: Reinhard tone mapping + Automatic light adaptation + Bloom
		[29] = function()
			-- No set state settings... this is the default setting
			stateTimeout = 5															-- Wait a few seconds, then go into the next state
			return "HDR: Reinhard tone mapping + Automatic light adaptation + Bloom"	-- Return the state text
		end,

		-- Making of end 1 
		[30] = function()
			stateTimeout = 10											-- Wait a few seconds, then go into the next state
			return "That's all folks - visit us at www.pixellight.org"	-- Return the state text
		end,

		-- Making of end 2
		[31] = function()
			stateTimeout = 7														-- Wait a few seconds, then go into the next state
			return "Have fun with the dungeon exploration! (space mouse supported)"	-- Return the state text
		end,

		-- Making of end 3
		[32] = function()
			stateTimeout = 6									-- Wait a few seconds, then go into the next state
			return "Use the keys 1-5 to change the demo mode"	-- Return the state text
		end,

		-- Making of finished
		[33] = function()
			-- Emit the playback finished signal
			this:SignalPlaybackFinished()
		end,
	}

	-- Execute the "kind of" switch/case statement
	return action[state]()
end
