--print("Hello World from Lua")
--
--print_c("Hello World from C\n")
--
--print(my_sin(1))

--for k,v in pairs(_G) do
--	print(k, v)
--end

local count = 0

Teste = {test = 0, name = "none"}

function Teste:new(obj)
	obj = obj or {}
	setmetatable(obj, self)
	self.__index = self
	return obj
end

function Teste:update()
	count = count + 1;
	self.test = self.test + 1
	print("Count of ", self.name, count, self.test)
end

DerivedTeste = Teste:new({derived_attr = "derived attribute"})

function DerivedTeste:update()
	Teste.update(self)
	print(self.name, self.derived_attr, self.test)
end
