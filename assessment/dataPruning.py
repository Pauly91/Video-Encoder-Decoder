
fo = open("rleData", "r")
f1 = open("huffmanACtable","w")
var = True
for line in fo:
	if line.split() :
		if var:
			a,b = line.split()
			print a,b
		else:
			print a,b,len(line)
			print line
			f1.write(a + " " + b +  " " + str(len(line) - 1)+"\n")
			f1.write(line)
		var = not var
			

		