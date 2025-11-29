local Engine = Engine  -- récupère la table globale que tu as enregistrée en C



local success, err = Engine.create_room()
if success then 
   Engine.log("test de success")
    print("test de success")
end 
if not success then
       Engine.log("test d'erreur")

    print("Failed:", err)
end
