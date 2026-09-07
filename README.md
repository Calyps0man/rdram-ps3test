PS3 PS2 RDRAM Test Tool  ver 0.1

An experimental diagnostic tool for testing the 32 MB of PS2 RDRAM in early backward-compatible PlayStation 3 consoles (CECHA/B). 

It is intended for consoles that fail to enter PS2 mode with an error such as: "failed to initialize memory: InitRDRAM returned -X". 

*You can also check some of the return codes through the link here (made by Kozarovv): https://www.psdevwiki.com/ps3/index.php?title=User_talk:Kozarovv&curid=9418&diff=78129&oldid=78114

The test runs before normal PS2 initialization and prints its results through the EEGS UART. Address space is tested using four patterns:

00000000<br>
FFFFFFFF<br>
AAAAAAAA<br>
55555555<br>

Each pattern is written across the memory and then read back in a separate pass. This helps detect stuck bits, address-related faults and memory locations that do not retain the written value. The ranges are logical 4 MB address windows across the 32 MB memory space. They are not physical data lanes.

On COK-001 motherboards, the channels should correspond to (Not 100% confirmed):

Channel A	- IC7002<br>
Channel B	- IC7003<br>

Example result:

testing Channel A<br>
Channel A fail rate=0.00%<br>
Channel A: OK<br>
testing Channel B<br>
Channel B fail rate=8.35%<br>
Channel B: FAIL<br>
failed to initialize memory: InitRDRAM returned -9<br>

A failed channel does not always mean that the RDRAM chip itself is defective. The failure may also be caused by bad solder joints, damaged traces, unstable power, missing signals or a fault in the memory controller.

Requirements:

-A backward-compatible PS3 with PS2 hardware (CECHA or CECHB models ONLY)<br>
-CFW or a suitable development environment<br>
-Devblind or similar tool to enable writing to dev folder<br>
-EEGS UART Adapter (same as for Syscon)<br>

Installation:

-Before doing anything, solder your UART adapter according to the pictures.<br>
-Run devblind and enable it.<br>
-Go to devblind folder through Irisman or any other file manager, locate ps2emu folder and replace ps2_emu.self file.<br>
-Optionally restart the system.<br>
-Open Serial terminal using program such as Putty, use 38400 baud.<br>
-Run a ps2 game and it should print if your memory is Ok or not. <br>

Credits:

Original RDRAM testing logic by krat0s - https://www.psx-place.com/resources/ps2-rdram-test-by-krat0s.899/

PS3 integration, channel/range diagnostics and UART output by Calyps0/Chatgpt.

Disclaimer:

This is an experimental repair and research tool. Use it at your own risk.
