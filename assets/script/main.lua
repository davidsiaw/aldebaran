-- script

local gameState = {}
gameState.next = "PART_001"


input.assign(input.A, 4)

scene.clear()

function PART_001()
	kagamin = game.newImage("kagamin.jpg")
	dialog = game.newDialog({"hello lol", "", "", ""})
	scene.display(kagamin, 0)
	scene.display(dialog, 4)
	script.waitfor(dialog)
	gameState.next = "PART_002"
end

function PART_002()
	dialog = game.newDialog({"I am kagamin", "", "", ""})
	scene.display(dialog, 4)
	script.waitfor(dialog)
	gameState.next = "PART_003"
end

function PART_003()
	dialog = game.newDialog({"how are you", "", "", ""})
	scene.display(dialog, 4)
	script.waitfor(dialog)
end

while gameState.next ~= nil do
	local next = gameState.next
	gameState.next = nil
	_G[next]()
end
