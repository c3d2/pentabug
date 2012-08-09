rate = 8000000 / (3 * 64)
offset = 50

print "const uint16_t freq_table[] = {"

for i in range(100):
	n = int(440.0 / rate * 2 ** ((i - offset) / 12.0) * 2 ** 16 + 0.5)
	if n < 0x10000 / 2:
		print "/*0x%02x*/\t%d," % (i, n)
print "};"
