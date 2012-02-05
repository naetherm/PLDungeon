--/*********************************************************\
-- *  File: StringTools.lua                                *
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
--[ String tool function                                  ]
--[-------------------------------------------------------]
--@brief
--  Splits a string by using the given delimiter
--
--@param[in] delimiter
--  Delimiter used to split the string
--
--@return
--  The splitted parts of the string as Lua table
--
--@remarks
--  Usage example:
--    local parts = string.split("a b c", " ")	-- "parts" is now the same as "local parts = {a, b, c}"
function string:split(delimiter)
	local result = { }
	local from = 1
	local delim_from, delim_to = string.find(self, delimiter, from)
	while delim_from do
		table.insert(result, string.sub(self, from, delim_from - 1))
		from = delim_to + 1
		delim_from, delim_to = string.find(self, delimiter, from)
	end
	table.insert(result, string.sub(self, from))
	return result
end
