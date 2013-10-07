-- script

local gameState = {}

input.assign(input.A, 4)

scene.clear()

i = scene.image("kagamin.jpg", 0)
aaa = scene.dialog({"hello lol", "", "", ""}, 4)

script.waitfor(aaa)
