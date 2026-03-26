-- Auto-generated Project: TestProjectGame

-- Auto-generated Rooms: 1


-- Auto-generated Room: RooTest

local RooTest = Engine.create_room()

-- Auto-generated Instances: 1
local camera = Engine.create_camera(2.5, 2.4, 300, 300)
local ins_ObjPlayer_0 = Engine.create_gameobject(RooTest, 280, 125)
local playerstep = Engine.load_script("./Scripts/ins_ObjPlayer_0/step.lua")
camera = "";
ins_ObjPlayer_0:setStep(playerstep)
ins_ObjPlayer_0:setSprite("/home/roys/git/TestProjectGame/Build/Sprites/SprPlayer/spr_player-sheet.png", 160, 32, 5, 1, 2)
