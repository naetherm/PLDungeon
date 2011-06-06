--@brief
--  Returns the currently shown text
--
--@return
--  The currently shown text
function GetShownText()
	-- Get the root scene container
	local sceneContainer = PL.GetApplication():GetRootScene()
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
function ShowText(text, timeout)
	-- Get the root scene container
	local sceneContainer = PL.GetApplication():GetRootScene()
	if sceneContainer ~= nil then
		-- Get the text scene node, or create it right now
		local sceneNode = sceneContainer:GetByName("Text")
		if sceneNode == nil then
			-- Create the text scene node right now
			sceneNode = sceneContainer:Create("PLScene::SNText2D", "Text", "Position=\"0.5 0.95 0.0\" Scale=\"1.1 1.1 1.1\" Flags=\"No3DPosition\"")
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
