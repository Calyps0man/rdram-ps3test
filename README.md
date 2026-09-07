PS3 PS2 RDRAM Test Tool  ver 0.1

An experimental diagnostic tool for testing the 32 MB of PS2 RDRAM in early backward-compatible PlayStation 3 consoles (CECHA/B). It is intended for consoles that fail to enter PS2 mode with an error such as: "failed to initialize memory: InitRDRAM returned -X". 

*You can also check some of the return codes through the link here: https://www.psdevwiki.com/ps3/index.php?title=User_talk:Kozarovv&curid=9418&diff=78129&oldid=78114

The test runs before normal PS2 initialization and prints its results through the EEGS UART. It tests the complete 32 MB RDRAM address space is tested using four patterns:

00000000
FFFFFFFF
AAAAAAAA
55555555

Each pattern is written across the memory and then read back in a separate pass. This helps detect stuck bits, address-related faults and memory locations that do not retain the written value. The ranges are logical 4 MB address windows across the 32 MB memory space. They are not physical data lanes.

On COK-001 motherboards, the channels should correspond to (Not 100% confirmed)

Channel A	- IC7002
Channel B	- IC7003

Example result:

# testing Channel A
# Channel A fail rate=0.00%
# Channel A: OK
# testing Channel B
# Channel B fail rate=8.35%
# Channel B: FAIL
# RDRAM test finished
# failed to initialize memory: InitRDRAM returned -9

A failed channel does not always mean that the RDRAM chip itself is defective. The failure may also be caused by bad solder joints, damaged traces, unstable power, missing signals or a fault in the memory controller.

Requirements:

-A backward-compatible PS3 with PS2 hardware
-CFW or a suitable development environment
-devblind or similar tool to enable writing to dev folder
-EEGS Uart
-A compatible ps2_emu.self

Installation:

-Before doing anything, solder EEGS Uart according to the pictures.
-Run devblind and enable it.
-Go to devblind folder through Irisman or any other file manager, locate ps2emu folder and replace ps2_emu.self file.
-Optionally restart the system.
-Open Serial terminal using program such as Putty, use 38400 baud.
-Run a ps2 game and it should print if your memory is 

Credits:

Original RDRAM testing logic by krat0s.

PS3 integration, channel/range diagnostics and UART output by Calyps0/Chatgpt.

Disclaimer:

This is an experimental repair and research tool. Use it at your own risk.
