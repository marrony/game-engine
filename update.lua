function update()
--	local axis = {
--		{0, 0, 1},
--		{0, 1, 0},
--		{1, 0, 0},	
--	}

	for i = 1, model_count() do
		local node = get_node(i)
		rotate_node(node, i%3, 0.01);
	end
end