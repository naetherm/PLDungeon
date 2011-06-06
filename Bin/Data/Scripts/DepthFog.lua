--[-------------------------------------------------------]
--[ Global functions                                      ]
--[-------------------------------------------------------]
--@brief
--  Sets the depthfog density
--
--@param[in] density
--  Depth fog density, <= 0 means there's no fog
function SetDepthFogDensity(density)
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
