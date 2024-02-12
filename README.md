# gb
A gb emulator in the works. Though not really working on it anymore :(

Using SFML for display purposes. Only on Windows for now, should be able to use the vs project file to build (not sure haven't tried yet but there's not much to try).
Uses static SFML linking but might need to add in `openal32.dll` into your executable directory (can get from SFML2.5 download).

Currently has basic support for non-colour system and basic MBCs. Includes basic debugging windows for disassembly viewing, CPU state, PPU state, and memory map.

Todo:
* Implement timing accurate implementation of PPU which uses the pixel fetcher instead of rendering entire line at the beginning of mode 3 (games like Prehistorik Man rely on this)
* Add more MBC support
* Stop state
* Save files/Battery support
* Configurable window (i.e. turning off debugging windows and dynamically resizing main window with or without)
* Audio
* Corner cases (STAT IRQ quirks, interesting memory access patterns, etc.) Essentially certain aspects of the system that aren't necessary for the majority of games to run
* gbc support (long-term)

![Image of Emulator](https://i.imgur.com/uKTaymE.png)

![Another Image of Emulator](https://i.imgur.com/VkR8seN.png)
