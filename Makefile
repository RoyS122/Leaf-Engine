# Compilateur
CC = gcc

# Dossiers
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OUTPUT_DIR = output

# Fichiers sources et headers
SOURCES = $(shell find $(SRC_DIR) -name '*.c')
HEADERS = $(shell find $(INCLUDE_DIR) -name '*.h')

# Génération des fichiers objets
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET = $(OUTPUT_DIR)/game.exe

# Flags de compilation (ajout de Lua)
CFLAGS = -I$(INCLUDE_DIR) \
         $(addprefix -I,$(shell find $(INCLUDE_DIR) -type d)) \
         -I/ucrt64/include \
         -I/mingw64/include/SDL2 \
         -Wall -Wextra

# Flags de liaison (ajout de Lua)
LDFLAGS = -L/ucrt64/lib \
          -L/mingw64/lib \
          -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image \
          -llua \
          -mwindows

# Cible principale
all: $(TARGET) copy_assets copy_lua_dll
	@echo "Compilation terminée. Exécutable dans $(OUTPUT_DIR)/"

# Lien final
$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilation des objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Copie des assets
copy_assets:
	@mkdir -p $(OUTPUT_DIR)/assets
	@cp -r assets/* $(OUTPUT_DIR)/assets/ 2>/dev/null || true
	@echo "Assets copiés dans $(OUTPUT_DIR)/assets/"

# Copie de la DLL Lua (IMPORTANT pour l'exécution)
copy_lua_dll:
	@cp /ucrt64/bin/lua54.dll $(OUTPUT_DIR)/ 2>/dev/null || \
	 cp /ucrt64/bin/lua5.4.dll $(OUTPUT_DIR)/ 2>/dev/null || \
	 cp /ucrt64/bin/lua.dll $(OUTPUT_DIR)/ 2>/dev/null || \
	 echo "Attention: DLL Lua non trouvée, vérifiez manuellement"
	@echo "DLL Lua copiée dans $(OUTPUT_DIR)/"

# Nettoyage
clean:
	@rm -rf $(BUILD_DIR) $(OUTPUT_DIR)
	@echo "Nettoyage terminé."

# Installation des dépendances (ajout de Lua)
install-deps:
	pacman -S --needed \
		mingw-w64-ucrt-x86_64-lua \
		mingw-w64-x86_64-SDL2 \
		mingw-w64-x86_64-SDL2_ttf \
		mingw-w64-x86_64-SDL2_image

# Compression de l'exécutable (optionnel)
compress:
	upx --best $(TARGET)
	@echo "Exécutable compressé avec UPX."

.PHONY: all clean copy_assets copy_lua_dll install-deps compress