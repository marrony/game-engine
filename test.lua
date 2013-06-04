--print("Hello World from Lua")
--
--print_c("Hello World from C\n")
--
--print(my_sin(1))

--for k,v in pairs(_G) do
--	print(k, v)
--end

local count = 0
--while 1 do
--	count = count + 1;
--	print("Count of:", count);
--	coroutine.yield();
--end

Teste = {}

function Teste:teste()
	count = count + 1;
	
	if not self.test then
		self.test = 0
	end
	
	self.test = self.test + 1
	print("Count of:", count, self.test);
end
