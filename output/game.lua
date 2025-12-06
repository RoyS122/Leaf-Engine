local Engine = Engine  -- get the global table engine

local room, err = Engine.create_room()
if room then 
   Engine.log("test de success")
    print("test de success")
end 
if not room then
       Engine.log("test d'erreur")

    print("Failed:", err)
end

local camera, err = room:createCamera(0, 0, 400, 300)

Engine.log("test cam cam x: " .. camera.x)

local enn_test, err = room:createGameObject(300, 10)
local player, err = room:createGameObject(1, 10)

if player:setSprite("./sprite_player_temp.png", 16, 16, 1, 1, 0) then
    Engine.log("test d'erreur setSprite")
end
if enn_test:setSprite("./sprite_player_temp.png", 16, 16, 1, 1, 0) then
    Engine.log("test d'erreur setSprite")
end
player:setStep(function(self)
    if not Engine.check_collision(enn_test, player) then
        self.x = self.x + 1
        -- camera.x = self.x - camera.w / 2
        -- Engine.log("test cam cam x: " .. camera.x)
    end 
end)

room:setStep(function(self)
    Engine.log("Test room step")
end)

