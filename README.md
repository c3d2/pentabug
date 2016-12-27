pentabug
========

**If you experience difficulties with your bug, please check the [troubleshooting documentation](https://github.com/c3d2/pentabug/blob/master/troubleshooting.md).**

The pentabug is a small microcontroller soldering project. It's especially suited for young hackers and beginners of all ages. Assembly requires no prior soldering skills and can be accomplished - with a little help for the tricky parts - by children old enough to savely hold a soldering iron (thats about 7 years and up).

At various events, e.g. [Datenspuren 2012 and 2013](http://datenspuren.de "Datenspuren 2012 and 2013"), Chaos Communication Congress 2012-2015, and many more, several hundred kits have been successfully assembled with just a handfull of bugs needing 'special treatment' for e.g. a controller mounted the wrong way. Have a look at our [slides in german](/c3d2/pentabug/raw/master/doc/201210_slides_datenspuren.pdf "slides form the Datenspuren 2012 talk") from the accompanying [presentation](http://www.datenspuren.de/2012/fahrplan/events/5074.de.html "pentabug talk announcement") or watch [this video](http://www.youtube.com/watch?v=HTBhXqNR6LY "Dresdner Piratencast Folge 14 - Datenspuren") of Frank and his sons beeing interviewed at our workshop table or the [NDR report](https://www.ndr.de/fernsehen/sendungen/hamburg_journal/Kleine-Hacker-am-Werk,hamj45298.html) covering the 32c3 Junghackertag.

A complete walktrough of the assembly process is given in ["Pentabug - The Movie"](http://www.c3d2.de/news/video-pentabug-bauen.html "Pentabug - The Movie"), courtesy of [C3D2 - Chaos Computer Club Dresden](http://c3d2.de "C3D2 - Chaos Computer Club Dresden"). It would be amazing if Scott Manley could do an english version ;-). I would even visit SF to make that happen. 

![Assembled V2 PCB with extension boards](https://github.com/c3d2/pentabug/raw/master/img/assembled_v2_728x483.jpg)  
Img. 1: A completely assembled V2 PCB with extension boards.

![Slightly Advanced Prototype Hardware](https://github.com/c3d2/pentabug/raw/master/img/prototype.jpg)  
Img. 2: A slightly advanced (but early) prototype version of the bug with a single layer pcb.

![Top Layer of Version 1.0 Board](https://github.com/c3d2/pentabug/raw/master/img/board_top.png)  
Img. 3: The top layer view of the Version 1.0 production board (two layer pcb).

![Version 2.1 Schematics (Thumbnail)](https://github.com/c3d2/pentabug/raw/master/img/schematics_v2_1_thumb.png)  
Img. 4: Version 2.1 schematics ([high resolution image](https://github.com/c3d2/pentabug/raw/master/img/schematics_v2_1.png "high resolution image of schematics")).

[Bill of Materials for Version 1.0](https://github.com/c3d2/pentabug/blob/master/bom.md "bill of materials") - CAVE: This is outdated for the V2.0 and V2.1 versions.

AVR GCC Toolchain
==================
 * install binutils-avr, gcc-avr, avr-libc, avrdude
 <pre><code>sudo apt-get install binutils-avr gcc-avr avr-libc avrdude</code></pre>
