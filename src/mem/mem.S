// zero out a region of memory - 64bit aligned, will zero out at least one 64bit section
.global mzero
mzero:
	str xzr, [x0], #8
    sub w1, w1, #1
    cbnz w1, mzero
    ret