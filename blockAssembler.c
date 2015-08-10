/*
Author - Abel Paul Babu
Roll Number - MT2014501

Code for - 

	Reads the block data
	Assembles them back to BMP
	Using the following header data

**Header**
 type: 0x4d42 
 fileSize: 786570 
 reserved1: 0 
 offset: 138 


**InfoHeader**
 size: 124 
 width: 512 
 height: 512 
 planes: 1 
 bits: 24 
 compression: 0 
 imagesize: 786432 
 xResolution: 0 
 yResolution: 0 
 coloursUsed: 0 
 importantColours: 0 
	

reference -
	 http://paulbourke.net/dataformats/bmp/
	 http://www.ue.eti.pg.gda.pl/fpgalab/zadania.spartan3/zad_vga_struktura_pliku_bmp_en.html

*/


	