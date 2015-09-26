import os
import matplotlib.pyplot as pyplot
# for i in range(0,256):
# 	os.system("./quantizer Lenna.bmp "+str(i))
# 	os.system("./psnr Lenna.bmp quantized.bmp")

# line = open("data1","r")
# data = []
# for l in line:
# 	data.append(l[0:len(l)-1])

line1 = open("data","r")
data1 = []
for l in line1:
	data1.append(l[0:len(l)-1])

bits = [i for i in range(0,256)]

pyplot.plot(bits,data,'-b', label='Yr')
#pyplot.plot(bits,data1,'-r', label='Cr Br')
pyplot.xlabel("Mask")
pyplot.ylabel("PSNR")
pyplot.grid(True)
pyplot.title('PSNR vs Bit Mask')
pyplot.legend(loc='upper left')
pyplot.savefig("psnr_comparison.png")
pyplot.show()


