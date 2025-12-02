local Engine = Engine  -- récupère la table globale que tu as enregistrée en C



local room, err = Engine.create_room()
if room then 
   Engine.log("test de success")
    print("test de success")
end 
if not room then
       Engine.log("test d'erreur")

    print("Failed:", err)
end

local player, err = room:createGameObject(10, 10)
if player:setSprite("./sprite_player_temp.png", 16, 16, 1, 1, 0) then
    Engine.log("test d'erreur setSprite")
end

player:setStep(function(self)
    
    self.x = self.x + 1
end)

room:setStep(function(self)
    Engine.log("Test room step")
end)