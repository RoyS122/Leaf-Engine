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

local player, err = Engine.create_gameobject(room, 10, 10)
if player:setSprite("./sprite_player_temp.png", 16, 16, 1, 1, 0) then
    Engine.log("test d'erreur setSprite")
end
player:setStep(function(self)
    Engine.log("Test " .. self.x)
    self.x = self.x + 1
end)
