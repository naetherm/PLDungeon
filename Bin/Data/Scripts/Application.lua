--[-------------------------------------------------------]
--[ Includes                                              ]
--[-------------------------------------------------------]
require("Data/Scripts/ShowText")
require("Data/Scripts/DepthFog")


--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Function is called by C++ after a scene has been loaded
function PostLoad()
	-- Configurate the scene renderer
	ConfigureSceneRenderer()

	-- Set the fog density
	SetDepthFogDensity(0.6)

	-- Adds the dancing skeleton to the scene
	AddDancingSkeleton()
end

--@brief
--  Configurates the scene renderer
function ConfigureSceneRenderer()
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
--  Add the dancing skeleton to the scene
function AddDancingSkeleton()
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

--@brief
--  Update function called by C++
function Update()
end
