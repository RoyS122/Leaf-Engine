---@meta
-- Cette ligne dit au Language Server : "C'est un fichier de définitions, ne l'exécute pas"

---@class Engine
---@field camera Camera
Engine = {}

---Affiche un message dans la console du moteur Leaf.
---@param message any Le message à logger
function Engine.log(message) end

---Charge un script et prépare son environnement local (include).
---@param script_path string Chemin vers le fichier .lua
---@return function # La fonction d'update (mon_update)
function Engine.load_script(script_path) end

---@class GameObject
---@field x number Position horizontale
---@field y number Position verticale
local GameObject = {}

---Définit la fonction appelée à chaque frame pour cet objet.
---@param func fun(self: GameObject) La fonction de step
function GameObject:setStep(func) end

---@class Camera
---@field x number
---@field y number
---@field zoom number
